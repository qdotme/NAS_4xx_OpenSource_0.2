/*
 * $Id: afp_options.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (c) 1997 Adrian Sun (asun@zoology.washington.edu)
 * Copyright (c) 1990,1993 Regents of The University of Michigan.
 * All Rights Reserved.  See COPYRIGHT.
 *
 * modified from main.c. this handles afp options.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>

/* STDC check */
#if STDC_HEADERS
#include <string.h>
#else /* STDC_HEADERS */
#ifndef HAVE_STRCHR
#define strchr index
#define strrchr index
#endif /* HAVE_STRCHR */
char *strchr (), *strrchr ();
#ifndef HAVE_MEMCPY
#define memcpy(d,s,n) bcopy ((s), (d), (n))
#define memmove(d,s,n) bcopy ((s), (d), (n))
#endif /* ! HAVE_MEMCPY */
#endif /* STDC_HEADERS */

#include <ctype.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <sys/param.h>
#include <sys/socket.h>
#include <atalk/logger.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif /* HAVE_NETDB_H */

#include <atalk/paths.h>
#include <atalk/util.h>
#include "globals.h"
#include "status.h"
#include "auth.h"

#include <atalk/compat.h>

#ifdef ADMIN_GRP
#include <grp.h>
#include <sys/types.h>
#endif /* ADMIN_GRP */

#ifndef MIN
#define MIN(a, b)  ((a) < (b) ? (a) : (b))
#endif /* MIN */

/* FIXME CNID */
char             Cnid_srv[MAXHOSTNAMELEN + 1] = "localhost";
int              Cnid_port = 4700;

#define OPTIONS "dn:f:s:uc:g:P:ptDS:TL:F:U:hIvVm:"
#define LENGTH 512

/* return an option. this uses an internal array, so it's necessary
 * to duplicate it if you want to hold it for long. this is probably
 * non-optimal. */
static char *getoption(char *buf, const char *option)
{
    static char string[LENGTH + 1];
    char *end;
    int len;

    if (option && (buf = strstr(buf, option)))
        buf = strpbrk(buf, " \t");

    while (buf && isspace(*buf))
        buf++;

    if (!buf)
        return NULL;

    /* search for any quoted stuff */
    if (*buf == '"' && (end = strchr(buf + 1, '"'))) {
        buf++;
        len = MIN(end - buf, LENGTH);
    } else if ((end = strpbrk(buf, " \t\n"))) /* option or eoln */
        len = MIN(end - buf, LENGTH);
    else
        len = MIN(strlen(buf), LENGTH);

    strncpy(string, buf, len);
    string[len] = '\0';
    return string;
}

/* get rid of any allocated afp_option buffers. */
void afp_options_free(struct afp_options *opt,
                      const struct afp_options *save)
{
    if (opt->defaultvol.name && (opt->defaultvol.name != save->defaultvol.name))
        free(opt->defaultvol.name);
    if (opt->defaultvol.full_name && (opt->defaultvol.full_name != save->defaultvol.full_name))
        free(opt->defaultvol.full_name);

    if (opt->systemvol.name && (opt->systemvol.name != save->systemvol.name))
        free(opt->systemvol.name);
    if (opt->systemvol.full_name && (opt->systemvol.full_name != save->systemvol.full_name))
        free(opt->systemvol.full_name);

    if (opt->uservol.name && (opt->uservol.name != save->uservol.name))
        free(opt->uservol.name);
    if (opt->uservol.full_name && (opt->uservol.full_name != save->uservol.full_name))
        free(opt->uservol.full_name);

    if (opt->loginmesg && (opt->loginmesg != save->loginmesg))
        free(opt->loginmesg);
    if (opt->guest && (opt->guest != save->guest))
        free(opt->guest);
    if (opt->server && (opt->server != save->server))
        free(opt->server);
    if (opt->ipaddr && (opt->ipaddr != save->ipaddr))
        free(opt->ipaddr);
    if (opt->fqdn && (opt->fqdn != save->fqdn))
        free(opt->fqdn);
    if (opt->uampath && (opt->uampath != save->uampath))
        free(opt->uampath);
    if (opt->uamlist && (opt->uamlist != save->uamlist))
        free(opt->uamlist);
    if (opt->passwdfile && (opt->passwdfile != save->passwdfile))
        free(opt->passwdfile);
    if (opt->signature && (opt->signature != save->signature))
	free(opt->signature);
    if (opt->k5service && (opt->k5service != save->k5service))
	free(opt->k5service);
    if (opt->k5realm && (opt->k5realm != save->k5realm))
	free(opt->k5realm);
    if (opt->k5keytab && (opt->k5keytab != save->k5keytab))
	free(opt->k5keytab);
    if (opt->unixcodepage && (opt->unixcodepage != save->unixcodepage))
	free(opt->unixcodepage);
    if (opt->maccodepage && (opt->maccodepage != save->maccodepage))
	free(opt->maccodepage);
}

/* initialize options */
void afp_options_init(struct afp_options *options)
{
    memset(options, 0, sizeof(struct afp_options));
    options->connections = 20;
    options->pidfile = _PATH_AFPDLOCK;
    options->defaultvol.name = _PATH_AFPDDEFVOL;
    options->systemvol.name = _PATH_AFPDSYSVOL;
    options->configfile = _PATH_AFPDCONF;
    options->uampath = _PATH_AFPDUAMPATH;
    options->uamlist = "uams_clrtxt.so,uams_dhx.so";
    options->guest = "nobody";
    options->loginmesg = "";
    options->transports = AFPTRANS_ALL;
    options->passwdfile = _PATH_AFPDPWFILE;
    options->tickleval = 30;
    options->timeout = 4;
    options->sleep = 10* 120; /* 10 h in 30 seconds tick */
    options->server_notif = 1;
    options->authprintdir = NULL;
    options->signature = "host";
    options->umask = 0;
#ifdef ADMIN_GRP
    options->admingid = 0;
#endif /* ADMIN_GRP */
    options->k5service = NULL;
    options->k5realm = NULL;
    options->k5keytab = NULL;
    options->unixcharset = CH_UNIX;
    options->unixcodepage = "LOCALE";
    options->maccharset = CH_MAC;
    options->maccodepage = "MAC_ROMAN";
}

/* parse an afpd.conf line. i'm doing it this way because it's
 * easy. it is, however, massively hokey. sample afpd.conf:
 * server:AFPServer@zone -loginmesg "blah blah blah" -nodsi 
 * "private machine"@zone2 -noguest -port 11012
 * server2 -nocleartxt -nodsi
 *
 * NOTE: this ignores unknown options 
 */
int afp_options_parseline(char *buf, struct afp_options *options)
{
    char *c, *opt;

    /* handle server */
    if (*buf != '-' && (c = getoption(buf, NULL)) && (opt = strdup(c)))
        options->server = opt;

    /* parse toggles */
    if (strstr(buf, " -nodebug"))
        options->flags &= ~OPTION_DEBUG;
#ifdef USE_SRVLOC
    if (strstr(buf, " -noslp"))
        options->flags |= OPTION_NOSLP;
#endif /* USE_SRVLOC */

    if (strstr(buf, " -nouservolfirst"))
        options->flags &= ~OPTION_USERVOLFIRST;
    if (strstr(buf, " -uservolfirst"))
        options->flags |= OPTION_USERVOLFIRST;
    if (strstr(buf, " -nouservol"))
        options->flags |= OPTION_NOUSERVOL;
    if (strstr(buf, " -uservol"))
        options->flags &= ~OPTION_NOUSERVOL;
    if (strstr(buf, " -proxy"))
        options->flags |= OPTION_PROXY;
    if (strstr(buf, " -noicon"))
        options->flags &= ~OPTION_CUSTOMICON;
    if (strstr(buf, " -icon"))
        options->flags |= OPTION_CUSTOMICON;
    if (strstr(buf, " -advertise_ssh"))
        options->flags |= OPTION_ANNOUNCESSH;

    /* passwd bits */
    if (strstr(buf, " -nosavepassword"))
        options->passwdbits |= PASSWD_NOSAVE;
    if (strstr(buf, " -savepassword"))
        options->passwdbits &= ~PASSWD_NOSAVE;
    if (strstr(buf, " -nosetpassword"))
        options->passwdbits &= ~PASSWD_SET;
    if (strstr(buf, " -setpassword"))
        options->passwdbits |= PASSWD_SET;

    /* transports */
    if (strstr(buf, " -transall"))
        options->transports = AFPTRANS_ALL;
    if (strstr(buf, " -notransall"))
        options->transports = AFPTRANS_NONE;
    if (strstr(buf, " -tcp"))
        options->transports |= AFPTRANS_TCP;
    if (strstr(buf, " -notcp"))
        options->transports &= ~AFPTRANS_TCP;
    if (strstr(buf, " -ddp"))
        options->transports |= AFPTRANS_DDP;
    if (strstr(buf, " -noddp"))
        options->transports &= ~AFPTRANS_DDP;
    if (strstr(buf, "-client_polling"))
        options->server_notif = 0;

    /* figure out options w/ values. currently, this will ignore the setting
     * if memory is lacking. */
    if ((c = getoption(buf, "-defaultvol")) && (opt = strdup(c)))
        options->defaultvol.name = opt;
    if ((c = getoption(buf, "-systemvol")) && (opt = strdup(c)))
        options->systemvol.name = opt;
    if ((c = getoption(buf, "-loginmesg")) && (opt = strdup(c)))
        options->loginmesg = opt;
    if ((c = getoption(buf, "-guestname")) && (opt = strdup(c)))
        options->guest = opt;
    if ((c = getoption(buf, "-passwdfile")) && (opt = strdup(c)))
        options->passwdfile = opt;
    if ((c = getoption(buf, "-passwdminlen")))
        options->passwdminlen = MIN(1, atoi(c));
    if ((c = getoption(buf, "-loginmaxfail")))
        options->loginmaxfail = atoi(c);
    if ((c = getoption(buf, "-tickleval"))) {
        options->tickleval = atoi(c);
        if (options->tickleval <= 0) {
            options->tickleval = 30;
        }
    }
    if ((c = getoption(buf, "-timeout"))) {
        options->timeout = atoi(c);
        if (options->timeout <= 0) {
            options->timeout = 4;
        }
    }

    if ((c = getoption(buf, "-sleep"))) {
        options->sleep = atoi(c) *120;
        if (options->sleep <= 4) {
            options->sleep = 4;
        }
    }

    if ((c = getoption(buf, "-server_quantum")))
        options->server_quantum = strtoul(c, NULL, 0);

#ifndef DISABLE_LOGGER
    /* -setuplogtype <syslog|filelog> <logtype> <loglevel> <filename>*/
    /* -[no]setuplog <logtype> <loglevel> [<filename>]*/
    if ((c = getoption(buf, "-setuplog")))
    {
      char *ptr, *logsource, *logtype, *loglevel, *filename;

      LOG(log_debug6, logtype_afpd, "setting up logtype, c is %s", c);
      ptr = c;
      
      /* 
      logsource = ptr = c;
      if (ptr)
      {
        ptr = strpbrk(ptr, " \t");
        if (ptr) 
        {
          *ptr++ = 0;
          while (*ptr && isspace(*ptr))
            ptr++;
        }
      }
      */

      logtype = ptr; 
      if (ptr)
      {
        ptr = strpbrk(ptr, " \t");
        if (ptr) 
        {
          *ptr++ = 0;
          while (*ptr && isspace(*ptr))
            ptr++;
        }
      }

      loglevel = ptr;
      if (ptr)
      {
        ptr = strpbrk(ptr, " \t");
        if (ptr) 
        {
          *ptr++ = 0;
          while (*ptr && isspace(*ptr))
            ptr++;
        }
      }

      filename = ptr;
      if (ptr)
      {
        ptr = strpbrk(ptr, " \t");
        if (ptr) 
        {
          *ptr++ = 0;
          while (*ptr && isspace(*ptr))
            ptr++;
        }
      }

      LOG(log_debug7, logtype_afpd, "calling setuplog %s %s %s", 
          logtype, loglevel, filename);

      setuplog(logtype, loglevel, filename);
    }

    if ((c = getoption(buf, "-unsetuplog")))
    {
      char *ptr, *logtype, *loglevel, *filename;

      LOG(log_debug6, logtype_afpd, "unsetting up logtype, c is %s", c);

      ptr = c;
      logtype = ptr;
      if (ptr)
      {
        ptr = strpbrk(ptr, " \t");
        if (ptr)
        {
          *ptr++ = 0;
          while (*ptr && isspace(*ptr))
            ptr++;
        }
      }

      loglevel = ptr;
      if (ptr)
      {
        ptr = strpbrk(ptr, " \t");
        if (ptr)
        {
          *ptr++ = 0;
           while (*ptr && isspace(*ptr))
             ptr++;
        }
      }

      filename = ptr;
      if (ptr)
      {
        ptr = strpbrk(ptr, " \t");
        if (ptr)
        {
          *ptr++ = 0;
          while (*ptr && isspace(*ptr))
            ptr++;
        }
      }
      
      LOG(log_debug7, logtype_afpd, "calling setuplog %s %s %s",
              logtype, NULL, filename);

      setuplog(logtype, NULL, filename);
    }
#endif /* DISABLE_LOGGER */
#ifdef ADMIN_GRP
    if ((c = getoption(buf, "-admingroup"))) {
        struct group *gr = getgrnam(c);
        if (gr != NULL) {
            options->admingid = gr->gr_gid;
        }
    }
#endif /* ADMIN_GRP */

    if ((c = getoption(buf, "-k5service")) && (opt = strdup(c)))
	options->k5service = opt;
    if ((c = getoption(buf, "-k5realm")) && (opt = strdup(c)))
	options->k5realm = opt;
    if ((c = getoption(buf, "-k5keytab"))) {
	if ( NULL == (options->k5keytab = (char *) malloc(sizeof(char)*(strlen(c)+14)) )) {
		LOG(log_error, logtype_afpd, "malloc failed");
		exit(-1);
	}
	snprintf(options->k5keytab, strlen(c)+14, "KRB5_KTNAME=%s", c);
	putenv(options->k5keytab);
	/* setenv( "KRB5_KTNAME", c, 1 ); */
    }
    if ((c = getoption(buf, "-authprintdir")) && (opt = strdup(c)))
        options->authprintdir = opt;
    if ((c = getoption(buf, "-uampath")) && (opt = strdup(c)))
        options->uampath = opt;
    if ((c = getoption(buf, "-uamlist")) && (opt = strdup(c)))
        options->uamlist = opt;

    if ((c = getoption(buf, "-ipaddr"))) {
        struct in_addr inaddr;
        if (inet_aton(c, &inaddr) && (opt = strdup(c))) {
            if (!gethostbyaddr((const char *) &inaddr, sizeof(inaddr), AF_INET))
                LOG(log_info, logtype_afpd, "WARNING: can't find %s", opt);
            options->ipaddr = opt;
        }
        else {
            LOG(log_error, logtype_afpd, "Error parsing -ipaddr, is %s in numbers-and-dots notation?", c);
        }
    }

    /* FIXME CNID Cnid_srv is a server attribute */
    if ((c = getoption(buf, "-cnidserver"))) {
        char *p;
	int len;        
        p = strchr(c, ':');
	if (p != NULL && (len = p - c) <= MAXHOSTNAMELEN) {
	    memcpy(Cnid_srv, c, len);
	    Cnid_srv[len] = 0;
	    Cnid_port = atoi(p +1);
	}
    }

    if ((c = getoption(buf, "-port")))
        options->port = atoi(c);
    if ((c = getoption(buf, "-ddpaddr")))
        atalk_aton(c, &options->ddpaddr);
    if ((c = getoption(buf, "-signature")) && (opt = strdup(c)))
        options->signature = opt;

    /* do a little checking for the domain name. */
    if ((c = getoption(buf, "-fqdn"))) {
        char *p = strchr(c, ':');
        if (p)
            *p = '\0';
        if (gethostbyname(c)) {
            if (p)
                *p = ':';
            if ((opt = strdup(c)))
                options->fqdn = opt;
        }
	else {
            LOG(log_error, logtype_afpd, "error parsing -fqdn, gethostbyname failed for: %s", c);
	}
    }

    if ((c = getoption(buf, "-unixcodepage"))) {
    	if ((charset_t)-1  == ( options->unixcharset = add_charset(c)) ) {
            options->unixcharset = CH_UNIX;
            LOG(log_warning, logtype_afpd, "setting Unix codepage to '%s' failed", c);
    	}
	else {
            if ((opt = strdup(c)))
                options->unixcodepage = opt;
	}
    }
	
    if ((c = getoption(buf, "-maccodepage"))) {
    	if ((charset_t)-1 == ( options->maccharset = add_charset(c)) ) {
            options->maccharset = CH_MAC;
            LOG(log_warning, logtype_afpd, "setting Mac codepage to '%s' failed", c);
    	}
	else {
            if ((opt = strdup(c)))
                options->maccodepage = opt;
	}
    }

    return 1;
}

/*
 * Show version information about afpd.
 * Used by "afp -v".
 */
void show_version( )
{
	printf( "afpd %s - Apple Filing Protocol (AFP) daemon of Netatalk\n\n", VERSION );

	puts( "This program is free software; you can redistribute it and/or modify it under" );
	puts( "the terms of the GNU General Public License as published by the Free Software" );
	puts( "Foundation; either version 2 of the License, or (at your option) any later" );
	puts( "version. Please see the file COPYING for further information and details.\n" );

	puts( "afpd has been compiled with support for these features:\n" );

	printf( "        AFP3.1 support:\t" );
#ifdef AFP3x
	puts( "Yes" );
#else
	puts( "No" );
#endif

	printf( "      Transport layers:\t" );
#ifdef NO_DDP
	puts( "TCP/IP" );
#else
	puts( "TCP/IP DDP" );
#endif

	printf( "         CNID backends:\t" );
#ifdef CNID_BACKEND_CDB
	printf( "cdb ");
#endif
#ifdef CNID_BACKEND_DB3
	printf( "db3 " );
#endif
#ifdef CNID_BACKEND_DBD
#ifdef CNID_BACKEND_DBD_TXN
	printf( "dbd-txn " );
#else
	printf( "dbd " );
#endif
#endif
#ifdef CNID_BACKEND_HASH
	printf( "hash " );
#endif
#ifdef CNID_BACKEND_LAST
	printf( "last " );
#endif
#ifdef CNID_BACKEND_MTAB
	printf( "mtab " );
#endif
#ifdef CNID_BACKEND_TDB
	printf( "tdb " );
#endif
	puts( "" );
}

/*
 * Show extended version information about afpd and Netatalk.
 * Used by "afp -V".
 */
void show_version_extended( )
{
	show_version( );

	printf( "           SLP support:\t" );
#ifdef USE_SRVLOC
	puts( "Yes" );
#else
	puts( "No" );
#endif

	printf( "  TCP wrappers support:\t" );
#ifdef TCPWRAP
	puts( "Yes" );
#else
	puts( "No" );
#endif

	printf( "         Quota support:\t" );
#ifndef NO_QUOTA_SUPPORT
	puts( "Yes" );
#else
	puts( "No" );
#endif

	printf( "   Admin group support:\t" );
#ifdef ADMIN_GRP
	puts( "Yes" );
#else
	puts( "No" );
#endif

	printf( "    Valid shell checks:\t" );
#ifndef DISABLE_SHELLCHECK
	puts( "Yes" );
#else
	puts( "No" );
#endif

	printf( "      cracklib support:\t" );
#ifdef USE_CRACKLIB
	puts( "Yes" );
#else
	puts( "No" );
#endif

	printf( "        Dropbox kludge:\t" );
#ifdef DROPKLUDGE
	puts( "Yes" );
#else
	puts( "No" );
#endif

	printf( "  Force volume uid/gid:\t" );
#ifdef FORCE_UIDGID
	puts( "Yes" );
#else
	puts( "No" );
#endif
}

/*
 * Display compiled-in default paths
 */
void show_paths( void )
{
	printf( "             afpd.conf:\t%s\n", _PATH_AFPDCONF );
	printf( "   AppleVolumes.system:\t%s\n", _PATH_AFPDSYSVOL );
	printf( "  AppleVolumes.default:\t%s\n", _PATH_AFPDDEFVOL );
	printf( "       UAM search path:\t%s\n", _PATH_AFPDUAMPATH );
}

/*
 * Display usage information about adpd.
 */
void show_usage( char *name )
{
	fprintf( stderr, "Usage:\t%s [-dDIptTu] [-c maxconnections] [-f defaultvolumes] [-F config]\n", name );
	fprintf( stderr, "\t     [-g guest] [-L message] [-m umask][-n nbpname] [-P pidfile]\n" );
	fprintf( stderr, "\t     [-s systemvolumes] [-S port] [-U uams]\n" );
	fprintf( stderr, "\t%s -h|-v|-V\n", name );
}

int afp_options_parse(int ac, char **av, struct afp_options *options)
{
    extern char *optarg;
    extern int optind;

    char *p;
    char *tmp;	/* Used for error checking the result of strtol */
    int c, err = 0;

    if (gethostname(options->hostname, sizeof(options->hostname )) < 0 ) {
        perror( "gethostname" );
        return 0;
    }
    if (NULL != ( p = strchr(options->hostname, '.' )) ) {
        *p = '\0';
    }

    if (NULL == ( p = strrchr( av[ 0 ], '/' )) ) {
        p = av[ 0 ];
    } else {
        p++;
    }

    while (EOF != ( c = getopt( ac, av, OPTIONS )) ) {
        switch ( c ) {
        case 'd' :
            options->flags |= OPTION_DEBUG;
            break;
        case 'n' :
            options->server = optarg;
            break;
        case 'f' :
            options->defaultvol.name = optarg;
            break;
        case 's' :
            options->systemvol.name = optarg;
            break;
        case 'u' :
            options->flags |= OPTION_USERVOLFIRST;
            break;
        case 'c' :
            options->connections = atoi( optarg );
            break;
        case 'g' :
            options->guest = optarg;
            break;

        case 'P' :
            options->pidfile = optarg;
            break;

        case 'p':
            options->passwdbits |= PASSWD_NOSAVE;
            break;
        case 't':
            options->passwdbits |= PASSWD_SET;
            break;

        case 'D':
            options->transports &= ~AFPTRANS_DDP;
            break;
        case 'S':
            options->port = atoi(optarg);
            break;
        case 'T':
            options->transports &= ~AFPTRANS_TCP;
            break;
        case 'L':
            options->loginmesg = optarg;
            break;
        case 'F':
            options->configfile = optarg;
            break;
        case 'U':
            options->uamlist = optarg;
            break;
        case 'v':	/* version */
            show_version( ); puts( "" );
	    show_paths( ); puts( "" );
            exit( 0 );
            break;
        case 'V':	/* extended version */
            show_version_extended( ); puts( "" );
	    show_paths( ); puts( "" );
            exit( 0 );
            break;
        case 'h':	/* usage */
            show_usage( p );
            exit( 0 );
            break;
        case 'I':
            options->flags |= OPTION_CUSTOMICON;
            break;
        case 'm':
            options->umask = strtoul(optarg, &tmp, 8);
            if ((options->umask > 0777)) {
                fprintf(stderr, "%s: out of range umask setting provided\n", p);
                err++;
            }
            if (tmp[0] != '\0') {
                fprintf(stderr, "%s: invalid characters in umask setting provided\n", p);
                err++;
            }
            break;
        default :
            err++;
        }
    }
    if ( err || optind != ac ) {
        show_usage( p );
        exit( 2 );
    }

#ifdef ultrix
    openlog( p, LOG_PID ); /* ultrix only */
#else /* ultrix */
    set_processname(p);
    syslog_setup(log_debug, logtype_default, logoption_ndelay|logoption_pid, logfacility_daemon);
#endif /* ultrix */

    return 1;
}
