/*
 * $Id: main.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (c) 1990,1993 Regents of The University of Michigan.
 * All Rights Reserved.  See COPYRIGHT.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <sys/param.h>
#include <sys/uio.h>
#include <atalk/logger.h>
#include <sys/time.h>
#include <sys/socket.h>

#include <errno.h>

#include <atalk/adouble.h>

#include <netatalk/at.h>
#include <atalk/compat.h>
#include <atalk/dsi.h>
#include <atalk/atp.h>
#include <atalk/asp.h>
#include <atalk/afp.h>
#include <atalk/paths.h>
#include <atalk/util.h>
#include <atalk/server_child.h>
#include <atalk/server_ipc.h>

#include "globals.h"
#include "afp_config.h"
#include "status.h"
#include "fork.h"
#include "uam_auth.h"

#ifdef TRU64
#include <sys/security.h>
#include <prot.h>
#include <sia.h>

static int argc = 0;
static char **argv = NULL;
#endif /* TRU64 */

unsigned char	nologin = 0;

struct afp_options default_options;
static AFPConfig *configs;
static server_child *server_children;
static fd_set save_rfds;
static int    Ipc_fd = -1;

#ifdef TRU64
void afp_get_cmdline( int *ac, char ***av)
{
    *ac = argc;
    *av = argv;
}
#endif /* TRU64 */

static void afp_exit(const int i)
{
    server_unlock(default_options.pidfile);
    exit(i);
}

/* ------------------
   initialize fd set we are waiting for.
*/
static void set_fd(int ipc_fd)
{
    AFPConfig   *config;

    FD_ZERO(&save_rfds);
    for (config = configs; config; config = config->next) {
        if (config->fd < 0) /* for proxies */
            continue;
        FD_SET(config->fd, &save_rfds);
    }
    if (ipc_fd >= 0) {
        FD_SET(ipc_fd, &save_rfds);
    }
}
 
/* ------------------ */
static void afp_goaway(int sig)
{

#ifndef NO_DDP
    asp_kill(sig);
#endif /* ! NO_DDP */

    dsi_kill(sig);
    switch( sig ) {
    case SIGTERM :
        LOG(log_info, logtype_afpd, "shutting down on signal %d", sig );
        break;
    case SIGUSR1 :
    case SIGHUP :
        /* w/ a configuration file, we can force a re-read if we want */
        nologin++;
        auth_unload();
        if (sig == SIGHUP || ((nologin + 1) & 1)) {
            AFPConfig *config;

            LOG(log_info, logtype_afpd, "re-reading configuration file");
            for (config = configs; config; config = config->next)
                if (config->server_cleanup)
                    config->server_cleanup(config);

            /* configfree close atp socket used for DDP tickle, there's an issue
             * with atp tid.
            */
            configfree(configs, NULL);
            if (!(configs = configinit(&default_options))) {
                LOG(log_error, logtype_afpd, "config re-read: no servers configured");
                afp_exit(EXITERR_CONF);
            }
            set_fd(Ipc_fd);
        } else {
            LOG(log_info, logtype_afpd, "disallowing logins");
        }
        if (sig == SIGHUP) {
            nologin = 0;
        }
        break;
    default :
        LOG(log_error, logtype_afpd, "afp_goaway: bad signal" );
    }
    if ( sig == SIGTERM ) {
        AFPConfig *config;

        for (config = configs; config; config = config->next)
            if (config->server_cleanup)
                config->server_cleanup(config);

        afp_exit(0);
    }
    return;
}

static void child_handler()
{
    server_child_handler(server_children);
}

int main( ac, av )
int		ac;
char	**av;
{
    AFPConfig           *config;
    fd_set              rfds;
    void                *ipc;
    struct sigaction	sv;
    sigset_t            sigs;
    int                 ret;

#ifdef TRU64
    argc = ac;
    argv = av;
    set_auth_parameters( ac, av );
#endif /* TRU64 */

#ifdef DEBUG1
    fault_setup(NULL);
#endif
    afp_options_init(&default_options);
    if (!afp_options_parse(ac, av, &default_options))
        exit(EXITERR_CONF);

    /* Save the user's current umask for use with CNID (and maybe some 
     * other things, too). */
    default_options.save_mask = umask( default_options.umask );

    switch(server_lock("afpd", default_options.pidfile,
                       default_options.flags & OPTION_DEBUG)) {
    case -1: /* error */
        exit(EXITERR_SYS);
    case 0: /* child */
        break;
    default: /* server */
        exit(0);
    }

    /* Register CNID  */
    cnid_init();

    /* install child handler for asp and dsi. we do this before afp_goaway
     * as afp_goaway references stuff from here. 
     * XXX: this should really be setup after the initial connections. */
    if (!(server_children = server_child_alloc(default_options.connections,
                            CHILD_NFORKS))) {
        LOG(log_error, logtype_afpd, "main: server_child alloc: %s", strerror(errno) );
        afp_exit(EXITERR_SYS);
    }
    
#ifdef AFP3x
    /* linux at least up to 2.4.22 send a SIGXFZ for vfat fs,
       even if the file is open with O_LARGEFILE ! */
#ifdef SIGXFSZ
    signal(SIGXFSZ , SIG_IGN); 
#endif
#endif    
    
    memset(&sv, 0, sizeof(sv));
    sv.sa_handler = child_handler;
    sigemptyset( &sv.sa_mask );
    sigaddset(&sv.sa_mask, SIGALRM);
    sigaddset(&sv.sa_mask, SIGHUP);
    sigaddset(&sv.sa_mask, SIGTERM);
    sigaddset(&sv.sa_mask, SIGUSR1);
    
    sv.sa_flags = SA_RESTART;
    if ( sigaction( SIGCHLD, &sv, 0 ) < 0 ) {
        LOG(log_error, logtype_afpd, "main: sigaction: %s", strerror(errno) );
        afp_exit(EXITERR_SYS);
    }

    sv.sa_handler = afp_goaway;
    sigemptyset( &sv.sa_mask );
    sigaddset(&sv.sa_mask, SIGALRM);
    sigaddset(&sv.sa_mask, SIGTERM);
    sigaddset(&sv.sa_mask, SIGHUP);
    sigaddset(&sv.sa_mask, SIGCHLD);
    sv.sa_flags = SA_RESTART;
    if ( sigaction( SIGUSR1, &sv, 0 ) < 0 ) {
        LOG(log_error, logtype_afpd, "main: sigaction: %s", strerror(errno) );
        afp_exit(EXITERR_SYS);
    }

    sigemptyset( &sv.sa_mask );
    sigaddset(&sv.sa_mask, SIGALRM);
    sigaddset(&sv.sa_mask, SIGTERM);
    sigaddset(&sv.sa_mask, SIGUSR1);
    sigaddset(&sv.sa_mask, SIGCHLD);
    sv.sa_flags = SA_RESTART;
    if ( sigaction( SIGHUP, &sv, 0 ) < 0 ) {
        LOG(log_error, logtype_afpd, "main: sigaction: %s", strerror(errno) );
        afp_exit(EXITERR_SYS);
    }


    sigemptyset( &sv.sa_mask );
    sigaddset(&sv.sa_mask, SIGALRM);
    sigaddset(&sv.sa_mask, SIGHUP);
    sigaddset(&sv.sa_mask, SIGUSR1);
    sigaddset(&sv.sa_mask, SIGCHLD);
    sv.sa_flags = SA_RESTART;
    if ( sigaction( SIGTERM, &sv, 0 ) < 0 ) {
        LOG(log_error, logtype_afpd, "main: sigaction: %s", strerror(errno) );
        afp_exit(EXITERR_SYS);
    }

    /* afpd.conf: not in config file: lockfile, connections, configfile
     *            preference: command-line provides defaults.
     *                        config file over-writes defaults.
     *
     * we also need to make sure that killing afpd during startup
     * won't leave any lingering registered names around.
     */

    sigemptyset(&sigs);
    sigaddset(&sigs, SIGALRM);
    sigaddset(&sigs, SIGHUP);
    sigaddset(&sigs, SIGUSR1);
#if 0
    /* don't block SIGTERM */
    sigaddset(&sigs, SIGTERM);
#endif
    sigaddset(&sigs, SIGCHLD);

    sigprocmask(SIG_BLOCK, &sigs, NULL);
    if (!(configs = configinit(&default_options))) {
        LOG(log_error, logtype_afpd, "main: no servers configured: %s", strerror(errno));
        afp_exit(EXITERR_CONF);
    }
    sigprocmask(SIG_UNBLOCK, &sigs, NULL);

    /* watch atp, dsi sockets and ipc parent/child file descriptor. */
    if ((ipc = server_ipc_create())) {
        Ipc_fd = server_ipc_parent(ipc);
    }
    set_fd(Ipc_fd);

    /* wait for an appleshare connection. parent remains in the loop
     * while the children get handled by afp_over_{asp,dsi}.  this is
     * currently vulnerable to a denial-of-service attack if a
     * connection is made without an actual login attempt being made
     * afterwards. establishing timeouts for logins is a possible 
     * solution. */
    while (1) {
        rfds = save_rfds;
        sigprocmask(SIG_UNBLOCK, &sigs, NULL);
        ret = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
        sigprocmask(SIG_BLOCK, &sigs, NULL);
        if (ret < 0) {
            if (errno == EINTR)
                continue;
            LOG(log_error, logtype_afpd, "main: can't wait for input: %s", strerror(errno));
            break;
        }
        if (Ipc_fd >=0 && FD_ISSET(Ipc_fd, &rfds)) {
            server_ipc_read(server_children);
        }
        for (config = configs; config; config = config->next) {
            if (config->fd < 0)
                continue;
            if (FD_ISSET(config->fd, &rfds)) {
                config->server_start(config, configs, server_children);
            }
        }
    }

    return 0;
}
