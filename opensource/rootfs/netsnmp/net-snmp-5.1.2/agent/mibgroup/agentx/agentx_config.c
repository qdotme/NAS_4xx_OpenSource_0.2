/*
 *  AgentX Configuration
 */
#include <net-snmp/net-snmp-config.h>

#include <stdio.h>
#include <sys/types.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#if HAVE_PWD_H
#include <pwd.h>
#endif
#if HAVE_GRP_H
#include <grp.h>
#endif

#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "snmpd.h"
#include "agentx/agentx_config.h"

#ifdef USING_AGENTX_MASTER_MODULE
void
agentx_parse_master(const char *token, char *cptr)
{
    int             i = -1;
    char            buf[BUFSIZ];

    if (!strcmp(cptr, "agentx") ||
        !strcmp(cptr, "all") ||
        !strcmp(cptr, "yes") || !strcmp(cptr, "on")) {
        i = 1;
        snmp_log(LOG_INFO, "Turning on AgentX master support.\n");
    } else if (!strcmp(cptr, "no") || !strcmp(cptr, "off"))
        i = 0;
    else
        i = atoi(cptr);

    if (i < 0 || i > 1) {
        sprintf(buf, "master '%s' unrecognised", cptr);
        config_perror(buf);
    } else
        netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_AGENTX_MASTER, i);
}
#endif                          /* USING_AGENTX_MASTER_MODULE */

void
agentx_parse_agentx_socket(const char *token, char *cptr)
{
    DEBUGMSGTL(("agentx/config", "port spec: %s\n", cptr));
    netsnmp_ds_set_string(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_X_SOCKET, cptr);
}

void
agentx_parse_agentx_perms(const char *token, char *cptr)
{
    char *socket_perm, *dir_perm, *socket_user, *socket_group;
    int uid = -1;
    int gid = -1;
    int s_perm = -1;
    int d_perm = -1;
#if HAVE_GETPWNAM && HAVE_PWD_H
    struct passwd *pwd;
#endif
#if HAVE_GETGRNAM && HAVE_GRP_H
    struct group  *grp;
#endif

    DEBUGMSGTL(("agentx/config", "port permissions: %s\n", cptr));
    socket_perm = strtok(cptr, " \t");
    dir_perm    = strtok(NULL, " \t");
    socket_user = strtok(NULL, " \t");
    socket_group = strtok(NULL, " \t");

    if (socket_perm) {
        s_perm = strtol(socket_perm, NULL, 8);
        netsnmp_ds_set_int(NETSNMP_DS_APPLICATION_ID,
                           NETSNMP_DS_AGENT_X_SOCK_PERM, s_perm);
        DEBUGMSGTL(("agentx/config", "socket permissions: %o (%d)\n",
                    s_perm, s_perm));
    }
    if (dir_perm) {
        d_perm = strtol(dir_perm, NULL, 8);
        netsnmp_ds_set_int(NETSNMP_DS_APPLICATION_ID,
                           NETSNMP_DS_AGENT_X_DIR_PERM, d_perm);
        DEBUGMSGTL(("agentx/config", "directory permissions: %o (%d)\n",
                    d_perm, d_perm));
    }

    /*
     * Try to handle numeric UIDs or user names for the socket owner
     */
    if (socket_user) {
        uid = atoi(socket_user);
        if ( uid == 0 ) {
#if HAVE_GETPWNAM && HAVE_PWD_H
            pwd = getpwnam( socket_user );
            if (pwd)
                uid = pwd->pw_uid;
            else
#endif
                snmp_log(LOG_WARNING, "Can't identify AgentX socket user (%s).\n", socket_user);
        }
        if ( uid != 0 )
            netsnmp_ds_set_int(NETSNMP_DS_APPLICATION_ID,
                               NETSNMP_DS_AGENT_X_SOCK_USER, uid);
        DEBUGMSGTL(("agentx/config", "socket owner: %s (%d)\n",
                    socket_user, uid));
    }

    /*
     * and similarly for the socket group ownership
     */
    if (socket_group) {
        gid = atoi(socket_group);
        if ( gid == 0 ) {
#if HAVE_GETGRNAM && HAVE_GRP_H
            grp = getgrnam( socket_group );
            if (grp)
                gid = grp->gr_gid;
            else
#endif
                snmp_log(LOG_WARNING, "Can't identify AgentX socket group (%s).\n", socket_group);
        }
        if ( gid != 0 )
            netsnmp_ds_set_int(NETSNMP_DS_APPLICATION_ID,
                               NETSNMP_DS_AGENT_X_SOCK_GROUP, gid);
        DEBUGMSGTL(("agentx/config", "socket group: %s (%d)\n",
                    socket_group, gid));
    }
}

void
agentx_parse_agentx_timeout(const char *token, char *cptr)
{
    int x = atoi(cptr);
    DEBUGMSGTL(("agentx/config/timeout", "%s\n", cptr));
    netsnmp_ds_set_int(NETSNMP_DS_APPLICATION_ID,
                       NETSNMP_DS_AGENT_AGENTX_TIMEOUT, x * ONE_SEC);
}

void
agentx_parse_agentx_retries(const char *token, char *cptr)
{
    int x = atoi(cptr);
    DEBUGMSGTL(("agentx/config/retries", "%s\n", cptr));
    netsnmp_ds_set_int(NETSNMP_DS_APPLICATION_ID,
                       NETSNMP_DS_AGENT_AGENTX_RETRIES, x);
}

void
init_agentx_config(void)
{
    /*
     * Don't set this up as part of the per-module initialisation.
     * Delay this until the 'init_master_agent()' routine is called,
     *   so that the config settings have been processed.
     * This means that we can use a config directive to determine
     *   whether or not to run as an AgentX master.
     */
#ifdef USING_AGENTX_MASTER_MODULE
    if (netsnmp_ds_get_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE) == MASTER_AGENT)
        snmpd_register_config_handler("master",
                                      agentx_parse_master, NULL,
                                      "specify 'agentx' for AgentX support");
#endif                          /* USING_AGENTX_MASTER_MODULE */
    snmpd_register_config_handler("agentxsocket",
                                  agentx_parse_agentx_socket, NULL,
                                  "AgentX bind address");
    snmpd_register_config_handler("agentxperms",
                                  agentx_parse_agentx_perms, NULL,
                                  "AgentX socket permissions: socket_perms [directory_perms [username|userid [groupname|groupid]]]");
    snmpd_register_config_handler("agentxRetries",
                                  agentx_parse_agentx_retries, NULL,
                                  "AgentX Retries");
    snmpd_register_config_handler("agentxTimeout",
                                  agentx_parse_agentx_timeout, NULL,
                                  "AgentX Timeout (seconds)");
}
