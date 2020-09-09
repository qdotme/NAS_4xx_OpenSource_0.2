/*
 * $Id: globals.h,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (c) 1990,1993 Regents of The University of Michigan.
 * All Rights Reserved.  See COPYRIGHT.
 */

#ifndef AFPD_GLOBALS_H
#define AFPD_GLOBALS_H 1

#include <sys/param.h>
#include <sys/cdefs.h>

#ifdef ADMIN_GRP
#include <grp.h>
#include <sys/types.h>
#endif /* ADMIN_GRP */

#ifdef HAVE_NETDB_H
#include <netdb.h>  /* this isn't header-protected under ultrix */
#endif /* HAVE_NETDB_H */

#include <netatalk/at.h>
#include <atalk/afp.h>
#include <atalk/compat.h>
#include <atalk/unicode.h>
#include <atalk/uam.h>

/* test for inline */
#ifndef __inline__
#define __inline__
#endif

#define MACFILELEN 31

#define OPTION_DEBUG         (1 << 0)
#define OPTION_USERVOLFIRST  (1 << 1)
#define OPTION_NOUSERVOL     (1 << 2)
#define OPTION_PROXY         (1 << 3)
#define OPTION_CUSTOMICON    (1 << 4)
#define OPTION_NOSLP         (1 << 5)
#define OPTION_ANNOUNCESSH   (1 << 6)

#ifdef FORCE_UIDGID
/* set up a structure for this */
typedef struct uidgidset_t {
    uid_t uid;
    gid_t gid;
} uidgidset;
#endif /* FORCE_UIDGID */

/* a couple of these options could get stuck in unions to save
 * space. */
struct afp_volume_name {
    time_t     mtime;
    char       *name;
    char       *full_name;
    int        loaded;
};

struct afp_options {
    int connections, port, transports, tickleval, timeout, server_notif, flags;
    unsigned char passwdbits, passwdminlen, loginmaxfail;
    u_int32_t server_quantum;
    char hostname[MAXHOSTNAMELEN + 1], *server, *ipaddr, *configfile;
    struct at_addr ddpaddr;
    char *uampath, *fqdn;
    char *pidfile;
    struct afp_volume_name defaultvol, systemvol, uservol;

    char *guest, *loginmesg, *keyfile, *passwdfile;
    char *uamlist;
    char *authprintdir;
    char *signature;
    char *k5service, *k5realm, *k5keytab;
    char *unixcodepage,*maccodepage;
    charset_t maccharset, unixcharset; 
    mode_t umask;
    mode_t save_mask;
    int    sleep;
#ifdef ADMIN_GRP
    gid_t admingid;
#endif /* ADMIN_GRP */
};

#define AFPOBJ_TMPSIZ (MAXPATHLEN)
typedef struct AFPObj {
    int proto;
    unsigned long servernum;
    void *handle, *config;
    struct afp_options options;
    char *Obj, *Type, *Zone;
    char username[MACFILELEN + 1];
    void (*logout)(void), (*exit)(int);
    int (*reply)(void *, int);
    int (*attention)(void *, AFPUserBytes);
    void (*sleep)(void);
    /* to prevent confusion, only use these in afp_* calls */
    char oldtmp[AFPOBJ_TMPSIZ + 1], newtmp[AFPOBJ_TMPSIZ + 1];
    void *uam_cookie; /* cookie for uams */
    struct session_info  sinfo;

#ifdef FORCE_UIDGID
    int                 force_uid;
    uidgidset		uidgid;
#endif
} AFPObj;

extern int		afp_version;
extern int		afp_errno;
extern unsigned char	nologin;
extern struct dir	*curdir;
extern char		getwdbuf[];

/* FIXME CNID */
extern char             Cnid_srv[MAXHOSTNAMELEN + 1];
extern int              Cnid_port;

extern int  get_afp_errno   __P((const int param));
extern void afp_options_init __P((struct afp_options *));
extern int afp_options_parse __P((int, char **, struct afp_options *));
extern int afp_options_parseline __P((char *, struct afp_options *));
extern void afp_options_free __P((struct afp_options *,
                                      const struct afp_options *));
extern void setmessage __P((const char *));
extern void readmessage __P((AFPObj *));

/* gettok.c */
extern void initline   __P((int, char *));
extern int  parseline  __P((int, char *));

/* afp_util.c */
const char *AfpNum2name __P((int ));

#ifndef NO_DDP
extern void afp_over_asp __P((AFPObj *));
#endif /* NO_DDP */
extern void afp_over_dsi __P((AFPObj *));

#endif /* globals.h */
