/*
 * $Id: volume.h,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (c) 1990,1994 Regents of The University of Michigan.
 * All Rights Reserved.  See COPYRIGHT.
 */

#ifndef AFPD_VOLUME_H
#define AFPD_VOLUME_H 1

#include <sys/cdefs.h>
#include <sys/types.h>
#include <netatalk/endian.h>

#include "atalk/unicode.h"
#include "globals.h"

#define AFPVOL_NAMELEN   27

#include <atalk/cnid.h>

struct vol {
    struct vol		*v_next;
    ucs2_t		*v_name;
    char		*v_path;
    struct dir		*v_dir, *v_root;
    int			v_flags;
#ifdef __svr4__
    int			v_qfd;
#endif /*__svr4__*/
    char		*v_gvs;
    time_t		v_mtime;
    time_t		v_ctime;  /* volume creation date, not unix ctime */

    u_int16_t		v_vid;
    void                *v_nfsclient;
    int                 v_nfs;
    
    int                 v_casefold;
    size_t              max_filename;
    
    char                *v_password;
    char                *v_veto;

    char                *v_cnidscheme;
    char                *v_dbpath;
    dev_t               v_dev;              /* Unix volume device */
    struct _cnid_db     *v_cdb;
    char                v_stamp[ADEDLEN_PRIVSYN];
    mode_t		v_umask;

#ifdef FORCE_UIDGID
    char		*v_forceuid;
    char		*v_forcegid;
#endif 

    char                *v_volcodepage;
    charset_t		v_volcharset;	
    struct charset_functions	*v_vol;
    char		*v_maccodepage;
    charset_t		v_maccharset;
    struct charset_functions	*v_mac;

    int                 v_deleted;    /* volume open but deleted in new config file */
    int                 v_hide;       /* new volume wait open volume */
    int                 v_adouble;    /* default adouble format */
    int			v_ad_options; /* adouble option NODEV, NOCACHE, etc.. */
    
    char                *v_root_preexec;
    char                *v_preexec;

    char                *v_root_postexec;
    char                *v_postexec;

    int                 v_root_preexec_close;
    int                 v_preexec_close;
    
    /* adouble indirection */
    int                 (*validupath)(const struct vol *, const char *);
    char                *(*ad_path)(const char *, int);
};

#ifdef NO_LARGE_VOL_SUPPORT
typedef u_int32_t VolSpace;
#else /* NO_LARGE_VOL_SUPPORT */
typedef u_int64_t VolSpace;
#endif /* NO_LARGE_VOL_SUPPORT */

#define AFPVOL_OPEN	(1<<0)
#define AFPVOL_DT	(1<<1)

#define AFPVOL_GVSMASK	(7<<2)
#define AFPVOL_NONE	(0<<2)
#define AFPVOL_AFSGVS	(1<<2)
#define AFPVOL_USTATFS	(2<<2)
#define AFPVOL_UQUOTA	(4<<2)

/* flags that alter volume behaviour. */
#define AFPVOL_A2VOL     (1 << 5)   /* prodos volume */
#define AFPVOL_CRLF      (1 << 6)   /* cr/lf translation */
#define AFPVOL_NOADOUBLE (1 << 7)   /* don't create .AppleDouble by default */
#define AFPVOL_RO        (1 << 8)   /* read-only volume */
#define AFPVOL_MSWINDOWS (1 << 9)   /* deal with ms-windows yuckiness.
this is going away. */
#define AFPVOL_NOHEX     (1 << 10)  /* don't do :hex translation */
#define AFPVOL_USEDOTS   (1 << 11)  /* use real dots */
#define AFPVOL_LIMITSIZE (1 << 12)  /* limit size for older macs */
#define AFPVOL_MAPASCII  (1 << 13)  /* map the ascii range as well */
#define AFPVOL_DROPBOX   (1 << 14)  /* dropkludge dropbox support */
#define AFPVOL_NOFILEID  (1 << 15)  /* don't advertise createid resolveid and deleteid calls */
#define AFPVOL_NOSTAT    (1 << 16)  /* advertise the volume even if we can't stat() it
                                     * maybe because it will be mounted later in preexec */
#define AFPVOL_UNIX_PRIV (1 << 17)  /* support unix privileges */
#define AFPVOL_NODEV     (1 << 18)  /* always use 0 for device number in cnid calls 
                                     * help if device number is notconsistent across reboot 
                                     * NOTE symlink to a different device will return an ACCESS error
                                     */
#define AFPVOL_CACHE      (1 << 19)  /* Use adouble v2 CNID caching, default don't use it */

/* FPGetSrvrParms options */
#define AFPSRVR_CONFIGINFO     (1 << 0)
#define AFPSRVR_PASSWD         (1 << 7)

/* handle casefolding */
#define AFPVOL_MTOUUPPER       (1 << 0) 
#define AFPVOL_MTOULOWER       (1 << 1) 
#define AFPVOL_UTOMUPPER       (1 << 2) 
#define AFPVOL_UTOMLOWER       (1 << 3) 
#define AFPVOL_UMLOWER         (AFPVOL_MTOULOWER | AFPVOL_UTOMLOWER)
#define AFPVOL_UMUPPER         (AFPVOL_MTOUUPPER | AFPVOL_UTOMUPPER)
#define AFPVOL_UUPPERMLOWER    (AFPVOL_MTOUUPPER | AFPVOL_UTOMLOWER)
#define AFPVOL_ULOWERMUPPER    (AFPVOL_MTOULOWER | AFPVOL_UTOMUPPER)

#define MSWINDOWS_BADCHARS ":\t\\/<>*?|\""

int wincheck(const struct vol *vol, const char *path);

#define AFPVOLSIG_FLAT          0x0001 /* flat fs */
#define AFPVOLSIG_FIX	        0x0002 /* fixed ids */
#define AFPVOLSIG_VAR           0x0003 /* variable ids */
#define AFPVOLSIG_DEFAULT       AFPVOLSIG_FIX

/* volume attributes */
#define VOLPBIT_ATTR_RO           (1 << 0)
#define VOLPBIT_ATTR_PASSWD       (1 << 1)
#define VOLPBIT_ATTR_FILEID       (1 << 2)
#define VOLPBIT_ATTR_CATSEARCH    (1 << 3)
#define VOLPBIT_ATTR_BLANKACCESS  (1 << 4)
#define VOLPBIT_ATTR_UNIXPRIV     (1 << 5)
#define VOLPBIT_ATTR_UTF8         (1 << 6)
#define VOLPBIT_ATTR_NONETUID     (1 << 7)

#define VOLPBIT_ATTR	0
#define VOLPBIT_SIG	1
#define VOLPBIT_CDATE	2
#define VOLPBIT_MDATE	3
#define VOLPBIT_BDATE	4
#define VOLPBIT_VID	5
#define VOLPBIT_BFREE	6
#define VOLPBIT_BTOTAL	7
#define VOLPBIT_NAME	8
/* handle > 4GB volumes */
#define VOLPBIT_XBFREE  9
#define VOLPBIT_XBTOTAL 10
#define VOLPBIT_BSIZE   11        /* block size */


#define vol_noadouble(vol) (((vol)->v_flags & AFPVOL_NOADOUBLE) ? \
			    ADFLAGS_NOADOUBLE : 0)
#ifdef AFP3x
#define utf8_encoding() (afp_version >= 30)
#else
#define utf8_encoding() (0)
#endif

#define vol_nodev(vol) (((vol)->v_flags & AFPVOL_NODEV) ? 1 : 0)

#define vol_unix_priv(vol) (afp_version >= 30 && ((vol)->v_flags & AFPVOL_UNIX_PRIV))

extern struct vol	*getvolbyvid __P((const u_int16_t));
extern int              ustatfs_getvolspace __P((const struct vol *,
            VolSpace *, VolSpace *,
            u_int32_t *));
extern void             setvoltime __P((AFPObj *, struct vol *));
extern int              pollvoltime __P((AFPObj *));
extern void             load_volumes __P((AFPObj *obj));

/* FP functions */
extern int	afp_openvol      __P((AFPObj *, char *, int, char *, int *));
extern int	afp_getvolparams __P((AFPObj *, char *, int, char *, int *));
extern int	afp_setvolparams __P((AFPObj *, char *, int, char *, int *));
extern int	afp_getsrvrparms __P((AFPObj *, char *, int, char *, int *));
extern int	afp_closevol     __P((AFPObj *, char *, int, char *, int *));

/* netatalk functions */
extern void     close_all_vol   __P((void));

#endif
