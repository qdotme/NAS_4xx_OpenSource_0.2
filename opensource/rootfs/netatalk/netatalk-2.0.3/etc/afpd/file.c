/*
 * $Id: file.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (c) 1990,1993 Regents of The University of Michigan.
 * All Rights Reserved.  See COPYRIGHT.
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

#include <atalk/adouble.h>
#include <utime.h>
#include <dirent.h>
#include <errno.h>

#include <atalk/logger.h>
#include <sys/param.h>


#include <atalk/afp.h>
#include <atalk/util.h>
#include <atalk/cnid.h>
#include "directory.h"
#include "desktop.h"
#include "volume.h"
#include "fork.h"
#include "file.h"
#include "filedir.h"
#include "globals.h"
#include "unix.h"

/* the format for the finderinfo fields (from IM: Toolbox Essentials):
 * field         bytes        subfield    bytes
 * 
 * files:
 * ioFlFndrInfo  16      ->       type    4  type field
 *                             creator    4  creator field
 *                               flags    2  finder flags:
 *					     alias, bundle, etc.
 *                            location    4  location in window
 *                              folder    2  window that contains file
 * 
 * ioFlXFndrInfo 16      ->     iconID    2  icon id
 *                              unused    6  reserved 
 *                              script    1  script system
 *                              xflags    1  reserved
 *                           commentID    2  comment id
 *                           putawayID    4  home directory id
 */

const u_char ufinderi[ADEDLEN_FINDERI] = {
                              0, 0, 0, 0, 0, 0, 0, 0,
                              1, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0
                          };

static const u_char old_ufinderi[] = {
                              'T', 'E', 'X', 'T', 'U', 'N', 'I', 'X'
                          };

/* ---------------------- 
*/
static int default_type(void *finder) 
{
    if (!memcmp(finder, ufinderi, 8) || !memcmp(finder, old_ufinderi, 8))
        return 1;
    return 0;
}

/* FIXME path : unix or mac name ? (for now it's unix name ) */
void *get_finderinfo(const char *upath, struct adouble *adp, void *data)
{
    struct extmap	*em;
    void                *ad_finder = NULL;
    int                 chk_ext = 0;
    
    if (adp)
        ad_finder = ad_entry(adp, ADEID_FINDERI);

    if (ad_finder) {
        memcpy(data, ad_finder, ADEDLEN_FINDERI);
        /* default type ? */
        if (default_type(ad_finder)) 
            chk_ext = 1;
    }
    else {
        memcpy(data, ufinderi, ADEDLEN_FINDERI);
        chk_ext = 1;
        if (*upath == '.') { /* make it invisible */
            u_int16_t ashort;
            
            ashort = htons(FINDERINFO_INVISIBLE);
            memcpy(data + FINDERINFO_FRFLAGOFF, &ashort, sizeof(ashort));
        }
    }
    /** Only enter if no appledouble information and no finder information found. */
    if (chk_ext && (em = getextmap( upath ))) {
        memcpy(data, em->em_type, sizeof( em->em_type ));
        memcpy((char *)data + 4, em->em_creator, sizeof(em->em_creator));
    }
    return data;
}

/* ---------------------
*/
char *set_name(const struct vol *vol, char *data, cnid_t pid, char *name, cnid_t id, u_int32_t utf8) 
{
    u_int32_t   aint;
    char        *tp = NULL;
    char        *src = name;
    aint = strlen( name );

    if (!utf8) {
        /* want mac name */
        if (utf8_encoding()) {
            /* but name is an utf8 mac name */
            char *u, *m;
           
            /* global static variable... */
            tp = strdup(name);
            if (!(u = mtoupath(vol, name, pid, 1)) || !(m = utompath(vol, u, id, 0))) {
               aint = 0;
            }
            else {
                aint = strlen(m);
                src = m;
            }
            
        }
        if (aint > MACFILELEN)
            aint = MACFILELEN;
        *data++ = aint;
    }
    else {
        u_int16_t temp;

        if (aint > 255)  /* FIXME safeguard, anyway if no ascii char it's game over*/
           aint = 255;

        utf8 = vol->v_mac?htonl(vol->v_mac->kTextEncoding):0;         /* htonl(utf8) */
        memcpy(data, &utf8, sizeof(utf8));
        data += sizeof(utf8);
        
        temp = htons(aint);
        memcpy(data, &temp, sizeof(temp));
        data += sizeof(temp);
    }

    memcpy( data, src, aint );
    data += aint;
    if (tp) {
        strcpy(name, tp);
        free(tp);
    }
    return data;
}

/*
 * FIXME: PDINFO is UTF8 and doesn't need adp
*/
#define PARAM_NEED_ADP(b) ((b) & ((1 << FILPBIT_ATTR)  |\
				  (1 << FILPBIT_CDATE) |\
				  (1 << FILPBIT_MDATE) |\
				  (1 << FILPBIT_BDATE) |\
				  (1 << FILPBIT_FINFO) |\
				  (1 << FILPBIT_RFLEN) |\
				  (1 << FILPBIT_EXTRFLEN) |\
				  (1 << FILPBIT_PDINFO) |\
				  (1 << FILPBIT_UNIXPR)))

/* -------------------------- */
u_int32_t get_id(struct vol *vol, struct adouble *adp,  const struct stat *st,
             const cnid_t did, char *upath, const int len) 
{
u_int32_t aint = 0;

#if AD_VERSION > AD_VERSION1

    if ((aint = ad_getid(adp, st->st_dev, st->st_ino, did, vol->v_stamp))) {
    	return aint;
    }
#endif

    if (vol->v_cdb != NULL) {
	    aint = cnid_add(vol->v_cdb, st, did, upath, len, aint);
	    /* Throw errors if cnid_add fails. */
	    if (aint == CNID_INVALID) {
            switch (errno) {
            case CNID_ERR_CLOSE: /* the db is closed */
                break;
            case CNID_ERR_PARAM:
                LOG(log_error, logtype_afpd, "get_id: Incorrect parameters passed to cnid_add");
                afp_errno = AFPERR_PARAM;
                return CNID_INVALID;
            case CNID_ERR_PATH:
                afp_errno = AFPERR_PARAM;
                return CNID_INVALID;
            default:
                afp_errno = AFPERR_MISC;
                return CNID_INVALID;
            }
        }
#if AD_VERSION > AD_VERSION1
        else if (adp ) {
            /* update the ressource fork
             * for a folder adp is always null
             */
            if (ad_setid(adp, st->st_dev, st->st_ino, aint, did, vol->v_stamp)) {
                ad_flush(adp, ADFLAGS_HF);
            }
        }
#endif    
    }
    return aint;
}
             
/* -------------------------- */
int getmetadata(struct vol *vol,
                 u_int16_t bitmap,
                 struct path *path, struct dir *dir, 
                 char *buf, int *buflen, struct adouble *adp, int attrbits )
{
    char		*data, *l_nameoff = NULL, *upath;
    char                *utf_nameoff = NULL;
    int			bit = 0;
    u_int32_t		aint;
    cnid_t              id = 0;
    u_int16_t		ashort;
    u_char              achar, fdType[4];
    u_int32_t           utf8 = 0;
    struct stat         *st;
    struct maccess	ma;

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin getmetadata:");
#endif /* DEBUG */

    upath = path->u_name;
    st = &path->st;

    data = buf;

    if ( ((bitmap & ( (1 << FILPBIT_FINFO)|(1 << FILPBIT_LNAME)|(1 <<FILPBIT_PDINFO) ) ) && !path->m_name)
         || (bitmap & ( (1 << FILPBIT_LNAME) ) && utf8_encoding()) /* FIXME should be m_name utf8 filename */
         || (bitmap & (1 << FILPBIT_FNUM))) {
         
        id = get_id(vol, adp, st, dir->d_did, upath, strlen(upath));
        if (id == 0)
            return afp_errno;
        if (!path->m_name) {
            path->m_name = utompath(vol, upath, id, utf8_encoding());
        }
    }
    while ( bitmap != 0 ) {
        while (( bitmap & 1 ) == 0 ) {
            bitmap = bitmap>>1;
            bit++;
        }

        switch ( bit ) {
        case FILPBIT_ATTR :
            if ( adp ) {
                ad_getattr(adp, &ashort);
            } else if (*upath == '.') {
                ashort = htons(ATTRBIT_INVISIBLE);
            } else
                ashort = 0;
#if 0
            /* FIXME do we want a visual clue if the file is read only
             */
            struct maccess	ma;
            accessmode( ".", &ma, dir , NULL);
            if ((ma.ma_user & AR_UWRITE)) {
            	accessmode( upath, &ma, dir , st);
            	if (!(ma.ma_user & AR_UWRITE)) {
                	attrbits |= ATTRBIT_NOWRITE;
                }
            }
#endif
            if (attrbits)
                ashort = htons(ntohs(ashort) | attrbits);
            memcpy(data, &ashort, sizeof( ashort ));
            data += sizeof( ashort );
            break;

        case FILPBIT_PDID :
            memcpy(data, &dir->d_did, sizeof( u_int32_t ));
            data += sizeof( u_int32_t );
            break;

        case FILPBIT_CDATE :
            if (!adp || (ad_getdate(adp, AD_DATE_CREATE, &aint) < 0))
                aint = AD_DATE_FROM_UNIX(st->st_mtime);
            memcpy(data, &aint, sizeof( aint ));
            data += sizeof( aint );
            break;

        case FILPBIT_MDATE :
            if ( adp && (ad_getdate(adp, AD_DATE_MODIFY, &aint) == 0)) {
                if ((st->st_mtime > AD_DATE_TO_UNIX(aint))) {
                   aint = AD_DATE_FROM_UNIX(st->st_mtime);
                }
            } else {
                aint = AD_DATE_FROM_UNIX(st->st_mtime);
            }
            memcpy(data, &aint, sizeof( int ));
            data += sizeof( int );
            break;

        case FILPBIT_BDATE :
            if (!adp || (ad_getdate(adp, AD_DATE_BACKUP, &aint) < 0))
                aint = AD_DATE_START;
            memcpy(data, &aint, sizeof( int ));
            data += sizeof( int );
            break;

        case FILPBIT_FINFO :
	    get_finderinfo(upath, adp, (char *)data);
            data += ADEDLEN_FINDERI;
            break;

        case FILPBIT_LNAME :
            l_nameoff = data;
            data += sizeof( u_int16_t );
            break;

        case FILPBIT_SNAME :
            memset(data, 0, sizeof(u_int16_t));
            data += sizeof( u_int16_t );
            break;

        case FILPBIT_FNUM :
            memcpy(data, &id, sizeof( id ));
            data += sizeof( id );
            break;

        case FILPBIT_DFLEN :
            if  (st->st_size > 0xffffffff)
               aint = 0xffffffff;
            else
               aint = htonl( st->st_size );
            memcpy(data, &aint, sizeof( aint ));
            data += sizeof( aint );
            break;

        case FILPBIT_RFLEN :
            if ( adp ) {
                if (adp->ad_rlen > 0xffffffff)
                    aint = 0xffffffff;
                else
                    aint = htonl( adp->ad_rlen);
            } else {
                aint = 0;
            }
            memcpy(data, &aint, sizeof( aint ));
            data += sizeof( aint );
            break;

            /* Current client needs ProDOS info block for this file.
               Use simple heuristic and let the Mac "type" string tell
               us what the PD file code should be.  Everything gets a
               subtype of 0x0000 unless the original value was hashed
               to "pXYZ" when we created it.  See IA, Ver 2.
               <shirsch@adelphia.net> */
        case FILPBIT_PDINFO :
            if (afp_version >= 30) { /* UTF8 name */
                utf8 = kTextEncodingUTF8;
                utf_nameoff = data;
                data += sizeof( u_int16_t );
                aint = 0;
                memcpy(data, &aint, sizeof( aint ));
                data += sizeof( aint );
            }
            else {
                if ( adp ) {
                    memcpy(fdType, ad_entry( adp, ADEID_FINDERI ), 4 );

                    if ( memcmp( fdType, "TEXT", 4 ) == 0 ) {
                        achar = '\x04';
                        ashort = 0x0000;
                    }
                    else if ( memcmp( fdType, "PSYS", 4 ) == 0 ) {
                        achar = '\xff';
                        ashort = 0x0000;
                    }
                    else if ( memcmp( fdType, "PS16", 4 ) == 0 ) {
                        achar = '\xb3';
                        ashort = 0x0000;
                    }
                    else if ( memcmp( fdType, "BINA", 4 ) == 0 ) {
                        achar = '\x00';
                        ashort = 0x0000;
                    }
                    else if ( fdType[0] == 'p' ) {
                        achar = fdType[1];
                        ashort = (fdType[2] * 256) + fdType[3];
                    }
                    else {
                        achar = '\x00';
                        ashort = 0x0000;
                    }
                }
                else {
                    achar = '\x00';
                    ashort = 0x0000;
                }

                *data++ = achar;
                *data++ = 0;
                memcpy(data, &ashort, sizeof( ashort ));
                data += sizeof( ashort );
                memset(data, 0, sizeof( ashort ));
                data += sizeof( ashort );
            }
            break;
        case FILPBIT_EXTDFLEN:
            aint = htonl(st->st_size >> 32);
            memcpy(data, &aint, sizeof( aint ));
            data += sizeof( aint );
            aint = htonl(st->st_size);
            memcpy(data, &aint, sizeof( aint ));
            data += sizeof( aint );
            break;
        case FILPBIT_EXTRFLEN:
            aint = 0;
            if (adp) 
                aint = htonl(adp->ad_rlen >> 32);
            memcpy(data, &aint, sizeof( aint ));
            data += sizeof( aint );
            if (adp) 
                aint = htonl(adp->ad_rlen);
            memcpy(data, &aint, sizeof( aint ));
            data += sizeof( aint );
            break;
        case FILPBIT_UNIXPR :
            /* accessmode may change st_mode with ACLs */
            accessmode( upath, &ma, dir , st);

            aint = htonl(st->st_uid);
            memcpy( data, &aint, sizeof( aint ));
            data += sizeof( aint );
            aint = htonl(st->st_gid);
            memcpy( data, &aint, sizeof( aint ));
            data += sizeof( aint );

	    /* FIXME: ugly hack
               type == slnk indicates an OSX style symlink, 
               we have to add S_IFLNK to the mode, otherwise
               10.3 clients freak out. */

    	    aint = st->st_mode;
 	    if (adp) {
	        memcpy(fdType, ad_entry( adp, ADEID_FINDERI ), 4 );
                if ( memcmp( fdType, "slnk", 4 ) == 0 ) {
	 	    aint |= S_IFLNK;
            	}
	    }
            aint = htonl(aint);

            memcpy( data, &aint, sizeof( aint ));
            data += sizeof( aint );

            *data++ = ma.ma_user;
            *data++ = ma.ma_world;
            *data++ = ma.ma_group;
            *data++ = ma.ma_owner;
            break;
            
        default :
            return( AFPERR_BITMAP );
        }
        bitmap = bitmap>>1;
        bit++;
    }
    if ( l_nameoff ) {
        ashort = htons( data - buf );
        memcpy(l_nameoff, &ashort, sizeof( ashort ));
        data = set_name(vol, data, dir->d_did, path->m_name, id, 0);
    }
    if ( utf_nameoff ) {
        ashort = htons( data - buf );
        memcpy(utf_nameoff, &ashort, sizeof( ashort ));
        data = set_name(vol, data, dir->d_did, path->m_name, id, utf8);
    }
    *buflen = data - buf;
    return (AFP_OK);
}
                
/* ----------------------- */
int getfilparams(struct vol *vol,
                 u_int16_t bitmap,
                 struct path *path, struct dir *dir, 
                 char *buf, int *buflen )
{
    struct adouble	ad, *adp;
    struct ofork        *of;
    char		    *upath;
    u_int16_t		attrbits = 0;
    int                 opened = 0;
    int rc;    

#ifdef DEBUG
    LOG(log_info, logtype_default, "begin getfilparams:");
#endif /* DEBUG */

    opened = PARAM_NEED_ADP(bitmap);
    adp = NULL;
    if (opened) {
        upath = path->u_name;
        if ((of = of_findname(path))) {
            adp = of->of_ad;
    	    attrbits = ((of->of_ad->ad_df.adf_refcount > 0) ? ATTRBIT_DOPEN : 0);
    	    attrbits |= ((of->of_ad->ad_hf.adf_refcount > of->of_ad->ad_df.adf_refcount)? ATTRBIT_ROPEN : 0);
        } else {
            ad_init(&ad, vol->v_adouble, vol->v_ad_options);
            adp = &ad;
        }

        if ( ad_metadata( upath, 0, adp) < 0 ) {
            switch (errno) {
            case EACCES:
                LOG(log_error, logtype_afpd, "getfilparams(%s): %s: check resource fork permission?",
                upath, strerror(errno));
                return AFPERR_ACCESS;
            case EIO:
                LOG(log_error, logtype_afpd, "getfilparams(%s): bad resource fork", upath);
                /* fall through */
            case ENOENT:
            default:
                adp = NULL;
                break;
            }
        }
        if (adp) {
    	    /* FIXME 
    	       we need to check if the file is open by another process.
    	       it's slow so we only do it if we have to:
    	       - bitmap is requested.
    	       - we don't already have the answer!
    	    */
    	    if ((bitmap & (1 << FILPBIT_ATTR))) {
	         if (!(attrbits & ATTRBIT_ROPEN)) {
    		     attrbits |= ad_testlock(adp, ADEID_RFORK, AD_FILELOCK_OPEN_RD) > 0? ATTRBIT_ROPEN : 0;
    		     attrbits |= ad_testlock(adp, ADEID_RFORK, AD_FILELOCK_OPEN_WR) > 0? ATTRBIT_ROPEN : 0;
	    	 }
    		 if (!(attrbits & ATTRBIT_DOPEN)) {
    		     attrbits |= ad_testlock(adp, ADEID_DFORK, AD_FILELOCK_OPEN_RD) > 0? ATTRBIT_DOPEN : 0;
    		     attrbits |= ad_testlock(adp, ADEID_DFORK, AD_FILELOCK_OPEN_WR) > 0? ATTRBIT_DOPEN : 0;
	    	 }
    	    }
    	}
    }
    rc = getmetadata(vol, bitmap, path, dir, buf, buflen, adp, attrbits);
    if ( adp ) {
        ad_close( adp, ADFLAGS_HF );
    }
#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end getfilparams:");
#endif /* DEBUG */

    return( rc );
}

/* ----------------------------- */
int afp_createfile(obj, ibuf, ibuflen, rbuf, rbuflen )
AFPObj      *obj;
char	*ibuf, *rbuf;
int		ibuflen, *rbuflen;
{
    struct adouble	ad, *adp;
    struct vol		*vol;
    struct dir		*dir;
    struct ofork        *of = NULL;
    char		*path, *upath;
    int			creatf, did, openf, retvalue = AFP_OK;
    u_int16_t		vid;
    int                 ret;
    struct path		*s_path;
    
#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin afp_createfile:");
#endif /* DEBUG */

    *rbuflen = 0;
    ibuf++;
    creatf = (unsigned char) *ibuf++;

    memcpy(&vid, ibuf, sizeof( vid ));
    ibuf += sizeof( vid );

    if (NULL == ( vol = getvolbyvid( vid )) ) {
        return( AFPERR_PARAM );
    }

    if (vol->v_flags & AFPVOL_RO)
        return AFPERR_VLOCK;

    memcpy(&did, ibuf, sizeof( did));
    ibuf += sizeof( did );

    if (NULL == ( dir = dirlookup( vol, did )) ) {
        return afp_errno;
    }

    if (NULL == ( s_path = cname( vol, dir, &ibuf )) ) {
        return get_afp_errno(AFPERR_PARAM);
    }

    if ( *s_path->m_name == '\0' ) {
        return( AFPERR_BADTYPE );
    }

    upath = s_path->u_name;
    if (0 != (ret = check_name(vol, upath))) 
       return  ret;
    
    /* if upath is deleted we already in trouble anyway */
    if ((of = of_findname(s_path))) {
        adp = of->of_ad;
    } else {
        ad_init(&ad, vol->v_adouble, vol->v_ad_options);
        adp = &ad;
    }
    if ( creatf) {
        /* on a hard create, fail if file exists and is open */
        if (of)
            return AFPERR_BUSY;
        openf = O_RDWR|O_CREAT|O_TRUNC;
    } else {
    	/* on a soft create, if the file is open then ad_open won't fail
    	   because open syscall is not called
    	*/
    	if (of) {
    		return AFPERR_EXIST;
    	}
        openf = O_RDWR|O_CREAT|O_EXCL;
    }

    if ( ad_open( upath, vol_noadouble(vol)|ADFLAGS_DF|ADFLAGS_HF|ADFLAGS_NOHF,
                  openf, 0666, adp) < 0 ) {
        switch ( errno ) {
        case EROFS:
            return AFPERR_VLOCK;
        case ENOENT : /* we were already in 'did folder' so chdir() didn't fail */
            return ( AFPERR_NOOBJ );
        case EEXIST :
            return( AFPERR_EXIST );
        case EACCES :
            return( AFPERR_ACCESS );
        default :
            return( AFPERR_PARAM );
        }
    }
    if ( ad_hfileno( adp ) == -1 ) {
         /* on noadouble volumes, just creating the data fork is ok */
         if (vol_noadouble(vol)) {
             ad_close( adp, ADFLAGS_DF );
             goto createfile_done;
         }
         /* FIXME with hard create on an existing file, we already
          * corrupted the data file.
          */
         netatalk_unlink( upath );
         ad_close( adp, ADFLAGS_DF );
         return AFPERR_ACCESS;
    }

    path = s_path->m_name;
    ad_setname(adp, path);
    ad_flush( adp, ADFLAGS_DF|ADFLAGS_HF );
    ad_close( adp, ADFLAGS_DF|ADFLAGS_HF );

createfile_done:
    curdir->offcnt++;

#ifdef DROPKLUDGE
    if (vol->v_flags & AFPVOL_DROPBOX) {
        retvalue = matchfile2dirperms(upath, vol, did);
    }
#endif /* DROPKLUDGE */

    setvoltime(obj, vol );

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end afp_createfile");
#endif /* DEBUG */

    return (retvalue);
}

int afp_setfilparams(obj, ibuf, ibuflen, rbuf, rbuflen )
AFPObj      *obj;
char	*ibuf, *rbuf;
int		ibuflen, *rbuflen;
{
    struct vol	*vol;
    struct dir	*dir;
    struct path *s_path;
    int		did, rc;
    u_int16_t	vid, bitmap;

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin afp_setfilparams:");
#endif /* DEBUG */

    *rbuflen = 0;
    ibuf += 2;

    memcpy(&vid, ibuf, sizeof( vid ));
    ibuf += sizeof( vid );
    if (NULL == ( vol = getvolbyvid( vid )) ) {
        return( AFPERR_PARAM );
    }

    if (vol->v_flags & AFPVOL_RO)
        return AFPERR_VLOCK;

    memcpy(&did, ibuf, sizeof( did ));
    ibuf += sizeof( did );
    if (NULL == ( dir = dirlookup( vol, did )) ) {
        return afp_errno; /* was AFPERR_NOOBJ */
    }

    memcpy(&bitmap, ibuf, sizeof( bitmap ));
    bitmap = ntohs( bitmap );
    ibuf += sizeof( bitmap );

    if (NULL == ( s_path = cname( vol, dir, &ibuf )) ) {
        return get_afp_errno(AFPERR_PARAM);
    }

    if (path_isadir(s_path)) {
        return( AFPERR_BADTYPE ); /* it's a directory */
    }

    if ( s_path->st_errno != 0 ) {
        return( AFPERR_NOOBJ );
    }

    if ((u_long)ibuf & 1 ) {
        ibuf++;
    }

    if (AFP_OK == ( rc = setfilparams(vol, s_path, bitmap, ibuf )) ) {
        setvoltime(obj, vol );
    }

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end afp_setfilparams:");
#endif /* DEBUG */

    return( rc );
}

/*
 * cf AFP3.0.pdf page 252 for change_mdate and change_parent_mdate logic  
 * 
*/
extern struct path Cur_Path;

int setfilparams(struct vol *vol,
                 struct path *path, u_int16_t f_bitmap, char *buf )
{
    struct adouble	ad, *adp;
    struct extmap	*em;
    int			bit, isad = 1, err = AFP_OK;
    char                *upath;
    u_char              achar, *fdType, xyy[4]; /* uninitialized, OK 310105 */
    u_int16_t		ashort, bshort;
    u_int32_t		aint;
    u_int32_t		upriv;
    u_int16_t           upriv_bit = 0;
    
    struct utimbuf	ut;

    int                 change_mdate = 0;
    int                 change_parent_mdate = 0;
    int                 newdate = 0;
    struct timeval      tv;
    uid_t		f_uid;
    gid_t		f_gid;
    u_int16_t           bitmap = f_bitmap;
    u_int32_t           cdate,bdate;
    u_char              finder_buf[32];

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin setfilparams:");
#endif /* DEBUG */

    upath = path->u_name;
    adp = of_ad(vol, path, &ad);
    

    if (!vol_unix_priv(vol) && check_access(upath, OPENACC_WR ) < 0) {
        return AFPERR_ACCESS;
    }

    /* with unix priv maybe we have to change adouble file priv first */
    bit = 0;
    while ( bitmap != 0 ) {
        while (( bitmap & 1 ) == 0 ) {
            bitmap = bitmap>>1;
            bit++;
        }
        switch(  bit ) {
        case FILPBIT_ATTR :
            change_mdate = 1;
            memcpy(&ashort, buf, sizeof( ashort ));
            buf += sizeof( ashort );
            break;
        case FILPBIT_CDATE :
            change_mdate = 1;
            memcpy(&cdate, buf, sizeof(cdate));
            buf += sizeof( cdate );
            break;
        case FILPBIT_MDATE :
            memcpy(&newdate, buf, sizeof( newdate ));
            buf += sizeof( newdate );
            break;
        case FILPBIT_BDATE :
            change_mdate = 1;
            memcpy(&bdate, buf, sizeof( bdate));
            buf += sizeof( bdate );
            break;
        case FILPBIT_FINFO :
            change_mdate = 1;
            memcpy(finder_buf, buf, 32 );
            buf += 32;
            break;
        case FILPBIT_UNIXPR :
            if (!vol_unix_priv(vol)) {
            	/* this volume doesn't use unix priv */
            	err = AFPERR_BITMAP;
            	bitmap = 0;
            	break;
            }
            change_mdate = 1;
            change_parent_mdate = 1;

            memcpy( &aint, buf, sizeof( aint ));
            f_uid = ntohl (aint);
            buf += sizeof( aint );
            memcpy( &aint, buf, sizeof( aint ));
            f_gid = ntohl (aint);
            buf += sizeof( aint );
            setfilowner(vol, f_uid, f_gid, path);

            memcpy( &upriv, buf, sizeof( upriv ));
            buf += sizeof( upriv );
            upriv = ntohl (upriv);
            if ((upriv & S_IWUSR)) {
            	setfilunixmode(vol, path, upriv);
            }
            else {
            	/* do it later */
            	upriv_bit = 1;
            }
            break;
        case FILPBIT_PDINFO :
            if (afp_version < 30) { /* else it's UTF8 name */
                achar = *buf;
                buf += 2;
                /* Keep special case to support crlf translations */
                if ((unsigned int) achar == 0x04) {
	       	    fdType = (u_char *)"TEXT";
		    buf += 2;
                } else {
            	    xyy[0] = ( u_char ) 'p';
            	    xyy[1] = achar;
            	    xyy[3] = *buf++;
            	    xyy[2] = *buf++;
            	    fdType = xyy;
	        }
                break;
            }
            /* fallthrough */
        default :
            err = AFPERR_BITMAP;
            /* break while loop */
            bitmap = 0;
            break;
        }

        bitmap = bitmap>>1;
        bit++;
    }

    /* second try with adouble open 
    */
    if (ad_open( upath, vol_noadouble(vol) | ADFLAGS_HF,
                 O_RDWR|O_CREAT, 0666, adp) < 0) {
        /* for some things, we don't need an adouble header */
        if (f_bitmap & ~(1<<FILPBIT_MDATE)) {
            return vol_noadouble(vol) ? AFP_OK : AFPERR_ACCESS;
        }
        isad = 0;
    } else if ((ad_get_HF_flags( adp ) & O_CREAT) ) {
        ad_setname(adp, path->m_name);
    }
    
    bit = 0;
    bitmap = f_bitmap;
    while ( bitmap != 0 ) {
        while (( bitmap & 1 ) == 0 ) {
            bitmap = bitmap>>1;
            bit++;
        }

        switch(  bit ) {
        case FILPBIT_ATTR :
            ad_getattr(adp, &bshort);
            if ((bshort & htons(ATTRBIT_INVISIBLE)) !=
                (ashort & htons(ATTRBIT_INVISIBLE) & htons(ATTRBIT_SETCLR)) )
                change_parent_mdate = 1;
            if ( ntohs( ashort ) & ATTRBIT_SETCLR ) {
                bshort |= htons( ntohs( ashort ) & ~ATTRBIT_SETCLR );
            } else {
                bshort &= ~ashort;
            }
            ad_setattr(adp, bshort);
            break;
        case FILPBIT_CDATE :
            ad_setdate(adp, AD_DATE_CREATE, cdate);
            break;
        case FILPBIT_MDATE :
            break;
        case FILPBIT_BDATE :
            ad_setdate(adp, AD_DATE_BACKUP, bdate);
            break;
        case FILPBIT_FINFO :
            if (default_type( ad_entry( adp, ADEID_FINDERI ))
                    && ( 
                     ((em = getextmap( path->m_name )) &&
                      !memcmp(finder_buf, em->em_type, sizeof( em->em_type )) &&
                      !memcmp(finder_buf + 4, em->em_creator,sizeof( em->em_creator)))
                     || ((em = getdefextmap()) &&
                      !memcmp(finder_buf, em->em_type, sizeof( em->em_type )) &&
                      !memcmp(finder_buf + 4, em->em_creator,sizeof( em->em_creator)))
            )) {
                memcpy(finder_buf, ufinderi, 8 );
            }
            memcpy(ad_entry( adp, ADEID_FINDERI ), finder_buf, 32 );
            break;
        case FILPBIT_UNIXPR :
            if (upriv_bit) {
            	setfilunixmode(vol, path, upriv);
            }
            break;
        case FILPBIT_PDINFO :
            if (afp_version < 30) { /* else it's UTF8 name */
                memcpy(ad_entry( adp, ADEID_FINDERI ), fdType, 4 );
                memcpy(ad_entry( adp, ADEID_FINDERI ) + 4, "pdos", 4 );
                break;
            }
            /* fallthrough */
        default :
            err = AFPERR_BITMAP;
            goto setfilparam_done;
        }
        bitmap = bitmap>>1;
        bit++;
    }

setfilparam_done:
    if (change_mdate && newdate == 0 && gettimeofday(&tv, NULL) == 0) {
       newdate = AD_DATE_FROM_UNIX(tv.tv_sec);
    }
    if (newdate) {
       if (isad)
          ad_setdate(adp, AD_DATE_MODIFY, newdate);
       ut.actime = ut.modtime = AD_DATE_TO_UNIX(newdate);
       utime(upath, &ut);
    }

    if (isad) {
        ad_flush( adp, ADFLAGS_HF );
        ad_close( adp, ADFLAGS_HF );

    }

    if (change_parent_mdate && gettimeofday(&tv, NULL) == 0) {
        newdate = AD_DATE_FROM_UNIX(tv.tv_sec);
        bitmap = 1<<FILPBIT_MDATE;
        setdirparams(vol, &Cur_Path, bitmap, (char *)&newdate);
    }

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end setfilparams:");
#endif /* DEBUG */
    return err;
}

/*
 * renamefile and copyfile take the old and new unix pathnames
 * and the new mac name.
 *
 * src         the source path 
 * dst         the dest filename in current dir
 * newname     the dest mac name
 * adp         adouble struct of src file, if open, or & zeroed one
 *
 */
int renamefile(vol, src, dst, newname, adp )
const struct vol *vol;
char	*src, *dst, *newname;
struct adouble    *adp;
{
    char	adsrc[ MAXPATHLEN + 1];
    int		rc;

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin renamefile:");
#endif /* DEBUG */

    if ( unix_rename( src, dst ) < 0 ) {
        switch ( errno ) {
        case ENOENT :
            return( AFPERR_NOOBJ );
        case EPERM:
        case EACCES :
            return( AFPERR_ACCESS );
        case EROFS:
            return AFPERR_VLOCK;
        case EXDEV :			/* Cross device move -- try copy */
           /* NOTE: with open file it's an error because after the copy we will 
            * get two files, it's fixable for our process (eg reopen the new file, get the
            * locks, and so on. But it doesn't solve the case with a second process
            */
    	    if (adp->ad_df.adf_refcount || adp->ad_hf.adf_refcount) {
    	        /* FIXME  warning in syslog so admin'd know there's a conflict ?*/
    	        return AFPERR_OLOCK; /* little lie */
    	    }
            if (AFP_OK != ( rc = copyfile(vol, vol, src, dst, newname, NULL )) ) {
                /* on error copyfile delete dest */
                return( rc );
            }
            return deletefile(vol, src, 0);
        default :
            return( AFPERR_PARAM );
        }
    }

    strcpy( adsrc, vol->ad_path( src, 0 ));

    if (unix_rename( adsrc, vol->ad_path( dst, 0 )) < 0 ) {
        struct stat st;
        int err;
        
        err = errno;        
	if (errno == ENOENT) {
	    struct adouble    ad;

            if (stat(adsrc, &st)) /* source has no ressource fork, */
                return AFP_OK;
            
            /* We are here  because :
             * -there's no dest folder. 
             * -there's no .AppleDouble in the dest folder.
             * if we use the struct adouble passed in parameter it will not
             * create .AppleDouble if the file is already opened, so we
             * use a diff one, it's not a pb,ie it's not the same file, yet.
             */
            ad_init(&ad, vol->v_adouble, vol->v_ad_options); 
            if (!ad_open(dst, ADFLAGS_HF, O_RDWR | O_CREAT, 0666, &ad)) {
            	ad_close(&ad, ADFLAGS_HF);
    	        if (!unix_rename( adsrc, vol->ad_path( dst, 0 )) ) 
                   err = 0;
                else 
                   err = errno;
            }
            else { /* it's something else, bail out */
	        err = errno;
	    }
	}
	/* try to undo the data fork rename,
	 * we know we are on the same device 
	*/
	if (err) {
    	    unix_rename( dst, src ); 
    	    /* return the first error */
    	    switch ( err) {
            case ENOENT :
                return AFPERR_NOOBJ;
            case EPERM:
            case EACCES :
                return AFPERR_ACCESS ;
            case EROFS:
                return AFPERR_VLOCK;
            default :
                return AFPERR_PARAM ;
            }
        }
    }

    /* don't care if we can't open the newly renamed ressource fork
     */
    if (!ad_open( dst, ADFLAGS_HF, O_RDWR, 0666, adp)) {
        ad_setname(adp, newname);
        ad_flush( adp, ADFLAGS_HF );
        ad_close( adp, ADFLAGS_HF );
    }
#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end renamefile:");
#endif /* DEBUG */

    return( AFP_OK );
}

/* ---------------- 
   convert a Mac long name to an utf8 name,
*/
size_t mtoUTF8(const struct vol *vol, const char *src, size_t srclen, char *dest, size_t destlen)
{
size_t    outlen;

    if ((size_t)-1 == (outlen = convert_string ( vol->v_maccharset, CH_UTF8_MAC, src, srclen, dest, destlen)) ) {
	return -1;
    }
    return outlen;
}

/* ---------------- */
int copy_path_name(const struct vol *vol, char *newname, char *ibuf)
{
char        type = *ibuf;
size_t      plen = 0;
u_int16_t   len16;
u_int32_t   hint;

    if ( type != 2 && !(afp_version >= 30 && type == 3) ) {
        return -1;
    }
    ibuf++;
    switch (type) {
    case 2:
        if (( plen = (unsigned char)*ibuf++ ) != 0 ) {
            if (afp_version >= 30) {
                /* convert it to UTF8 
                */
                if ((plen = mtoUTF8(vol, ibuf, plen, newname, AFPOBJ_TMPSIZ)) == (size_t)-1)
                   return -1;
            }
            else {
                strncpy( newname, ibuf, plen );
                newname[ plen ] = '\0';
            }
            if (strlen(newname) != plen) {
                /* there's \0 in newname, e.g. it's a pathname not
                 * only a filename. 
                */
            	return -1;
            }
        }
        break;
    case 3:
        memcpy(&hint, ibuf, sizeof(hint));
        ibuf += sizeof(hint);
           
        memcpy(&len16, ibuf, sizeof(len16));
        ibuf += sizeof(len16);
        plen = ntohs(len16);
        
        if (plen) {
            if (plen > AFPOBJ_TMPSIZ) {
            	return -1;
            }
            strncpy( newname, ibuf, plen );
            newname[ plen ] = '\0';
            if (strlen(newname) != plen) {
            	return -1;
            }
        }
        break;
    }
    return plen;
}

/* -----------------------------------
*/
int afp_copyfile(obj, ibuf, ibuflen, rbuf, rbuflen )
AFPObj      *obj;
char	*ibuf, *rbuf;
int		ibuflen, *rbuflen;
{
    struct vol	*s_vol, *d_vol;
    struct dir	*dir;
    char	*newname, *p, *upath;
    struct path *s_path;
    u_int32_t	sdid, ddid;
    int         err, retvalue = AFP_OK;
    u_int16_t	svid, dvid;

    struct adouble ad, *adp;
    int denyreadset;
    
#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin afp_copyfile:");
#endif /* DEBUG */

    *rbuflen = 0;
    ibuf += 2;

    memcpy(&svid, ibuf, sizeof( svid ));
    ibuf += sizeof( svid );
    if (NULL == ( s_vol = getvolbyvid( svid )) ) {
        return( AFPERR_PARAM );
    }

    memcpy(&sdid, ibuf, sizeof( sdid ));
    ibuf += sizeof( sdid );
    if (NULL == ( dir = dirlookup( s_vol, sdid )) ) {
        return afp_errno;
    }

    memcpy(&dvid, ibuf, sizeof( dvid ));
    ibuf += sizeof( dvid );
    memcpy(&ddid, ibuf, sizeof( ddid ));
    ibuf += sizeof( ddid );

    if (NULL == ( s_path = cname( s_vol, dir, &ibuf )) ) {
        return get_afp_errno(AFPERR_PARAM);
    }
    if ( path_isadir(s_path) ) {
        return( AFPERR_BADTYPE );
    }

    /* don't allow copies when the file is open.
     * XXX: the spec only calls for read/deny write access.
     *      however, copyfile doesn't have any of that info,
     *      and locks need to stay coherent. as a result,
     *      we just balk if the file is opened already. */

    adp = of_ad(s_vol, s_path, &ad);

    if (ad_open(s_path->u_name , ADFLAGS_DF |ADFLAGS_HF | ADFLAGS_NOHF, O_RDONLY, 0, adp) < 0) {
        return AFPERR_DENYCONF;
    }
    denyreadset = (getforkmode(adp, ADEID_DFORK, AD_FILELOCK_DENY_RD) != 0 || 
                  getforkmode(adp, ADEID_RFORK, AD_FILELOCK_DENY_RD) != 0 );
    ad_close( adp, ADFLAGS_DF |ADFLAGS_HF );
    if (denyreadset) {
        return AFPERR_DENYCONF;
    }

    newname = obj->newtmp;
    strcpy( newname, s_path->m_name );

    p = ctoupath( s_vol, curdir, newname );
    if (!p) {
        return AFPERR_PARAM;
    
    }
#ifdef FORCE_UIDGID
    /* FIXME svid != dvid && dvid's user can't read svid */
#endif
    if (NULL == ( d_vol = getvolbyvid( dvid )) ) {
        return( AFPERR_PARAM );
    }

    if (d_vol->v_flags & AFPVOL_RO)
        return AFPERR_VLOCK;

    if (NULL == ( dir = dirlookup( d_vol, ddid )) ) {
        return afp_errno;
    }

    if (( s_path = cname( d_vol, dir, &ibuf )) == NULL ) {
        return get_afp_errno(AFPERR_NOOBJ); 
    }
    if ( *s_path->m_name != '\0' ) {
	path_error(s_path, AFPERR_PARAM);
    }

    /* one of the handful of places that knows about the path type */
    if (copy_path_name(d_vol, newname, ibuf) < 0) {
        return( AFPERR_PARAM );
    }
    /* newname is always only a filename so curdir *is* its
     * parent folder
    */
    if (NULL == (upath = mtoupath(d_vol, newname, curdir->d_did, utf8_encoding()))) {
        return( AFPERR_PARAM );
    }
    if ( (err = copyfile(s_vol, d_vol, p, upath , newname, adp)) < 0 ) {
        return err;
    }
    curdir->offcnt++;

#ifdef DROPKLUDGE
    if (vol->v_flags & AFPVOL_DROPBOX) {
        retvalue=matchfile2dirperms(upath, vol, ddid); /* FIXME sdir or ddid */
    }
#endif /* DROPKLUDGE */

    setvoltime(obj, d_vol );

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end afp_copyfile:");
#endif /* DEBUG */

    return( retvalue );
}

/* ----------------------- */
static __inline__ int copy_all(const int dfd, const void *buf,
                               size_t buflen)
{
    ssize_t cc;

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin copy_all:");
#endif /* DEBUG */

    while (buflen > 0) {
        if ((cc = write(dfd, buf, buflen)) < 0) {
            switch (errno) {
            case EINTR:
                continue;
            default:
                return -1;
            }
        }
        buflen -= cc;
    }

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end copy_all:");
#endif /* DEBUG */

    return 0;
}

/* -------------------------- */
static int copy_fd(int dfd, int sfd)
{
    ssize_t cc;
    int     err = 0;
    char    filebuf[8192];
    
#if 0 /* ifdef SENDFILE_FLAVOR_LINUX */
    /* doesn't work With 2.6 FIXME, only check for EBADFD ? */
    off_t   offset = 0;
    size_t  size;
    struct stat         st;
    #define BUF 128*1024*1024

    if (fstat(sfd, &st) == 0) {
        
        while (1) {
            if ( offset >= st.st_size) {
               return 0;
            }
            size = (st.st_size -offset > BUF)?BUF:st.st_size -offset;
            if ((cc = sys_sendfile(dfd, sfd, &offset, size)) < 0) {
                switch (errno) {
                case ENOSYS:
                case EINVAL:  /* there's no guarantee that all fs support sendfile */
                    goto no_sendfile;
                default:
                    return -1;
                }
            }
        }
    }
    no_sendfile:
    lseek(sfd, offset, SEEK_SET);
#endif 

    while (1) {
        if ((cc = read(sfd, filebuf, sizeof(filebuf))) < 0) {
            if (errno == EINTR)
                continue;
            err = -1;
            break;
        }

        if (!cc || ((err = copy_all(dfd, filebuf, cc)) < 0)) {
            break;
        }
    }
    return err;
}

/* ----------------------------------
 * if newname is NULL (from directory.c) we don't want to copy ressource fork.
 * because we are doing it elsewhere.
 */
int copyfile(s_vol, d_vol, src, dst, newname, adp )
const struct vol *s_vol, *d_vol;
char	*src, *dst, *newname;
struct adouble *adp;
{
    struct adouble	ads, add;
    int			err = 0;
    int                 ret_err = 0;
    int                 adflags;
    int                 noadouble = vol_noadouble(d_vol);
    struct stat         st;
    
#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin copyfile:");
#endif /* DEBUG */

    if (adp == NULL) {
        ad_init(&ads, s_vol->v_adouble, s_vol->v_ad_options); 
        adp = &ads;
    }
    ad_init(&add, d_vol->v_adouble, d_vol->v_ad_options);
    adflags = ADFLAGS_DF;
    if (newname) {
        adflags |= ADFLAGS_HF;
    }

    if (ad_open(src , adflags | ADFLAGS_NOHF, O_RDONLY, 0, adp) < 0) {
        ret_err = errno;
        goto done;
    }

    if (ad_hfileno(adp) == -1) {
        /* no resource fork, don't create one for dst file */
        adflags &= ~ADFLAGS_HF;
    }

    if (ad_open(dst , adflags | noadouble, O_RDWR|O_CREAT|O_EXCL, 0666, &add) < 0) {
        ret_err = errno;
        ad_close( adp, adflags );
        if (EEXIST != ret_err) {
            deletefile(d_vol, dst, 0);
            goto done;
        }
        return AFPERR_EXIST;
    }
    if (ad_hfileno(adp) == -1 || 0 == (err = copy_fd(ad_hfileno(&add), ad_hfileno(adp)))){
        /* copy the data fork */
	err = copy_fd(ad_dfileno(&add), ad_dfileno(adp));
    }

    /* Now, reopen destination file */
    if (err < 0) {
       ret_err = errno;
    }
    ad_close( adp, adflags );

    if (ad_close( &add, adflags ) <0) {
        deletefile(d_vol, dst, 0);
        ret_err = errno;
        goto done;
    } 
    else {
	ad_init(&add, d_vol->v_adouble, d_vol->v_ad_options);
	if (ad_open(dst , adflags | noadouble, O_RDWR, 0666, &add) < 0) {
	    ret_err = errno;
	}
    }

    if (!ret_err && newname) {
        ad_setname(&add, newname);
    }

    ad_flush( &add, adflags );
    if (ad_close( &add, adflags ) <0) {
       ret_err = errno;
    }
    if (ret_err) {
        deletefile(d_vol, dst, 0);
    }
    else if (!stat(src, &st)) {
        /* set dest modification date to src date */
        struct utimbuf	ut;

    	ut.actime = ut.modtime = st.st_mtime;
    	utime(dst, &ut);
    	/* FIXME netatalk doesn't use resource fork file date
    	 * but maybe we should set its modtime too.
    	*/
    }

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end copyfile:");
#endif /* DEBUG */

done:
    switch ( ret_err ) {
    case 0:
        return AFP_OK;
    case EDQUOT:
    case EFBIG:
    case ENOSPC:
        return AFPERR_DFULL;
    case ENOENT:
        return AFPERR_NOOBJ;
    case EACCES:
        return AFPERR_ACCESS;
    case EROFS:
        return AFPERR_VLOCK;
    }
    return AFPERR_PARAM;
}


/* -----------------------------------
   vol: not NULL delete cnid entry. then we are in curdir and file is a only filename
   checkAttrib:   1 check kFPDeleteInhibitBit (deletfile called by afp_delete)

   when deletefile is called we don't have lock on it, file is closed (for us)
   untrue if called by renamefile
   
   ad_open always try to open file RDWR first and ad_lock takes care of
   WRITE lock on read only file.
*/
int deletefile( vol, file, checkAttrib )
const struct vol      *vol;
char		*file;
int         checkAttrib;
{
    struct adouble	ad;
    struct adouble      *adp = &ad;
    int			adflags, err = AFP_OK;

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin deletefile:");
#endif /* DEBUG */

    /* try to open both forks at once */
    adflags = ADFLAGS_DF|ADFLAGS_HF;
    ad_init(&ad, vol->v_adouble, vol->v_ad_options);  /* OK */
    while(1) {
        if ( ad_open( file, adflags, O_RDONLY, 0, &ad ) < 0 ) {
            switch (errno) {
            case ENOENT:
                if (adflags == ADFLAGS_DF)
                    return AFPERR_NOOBJ;
                   
                /* that failed. now try to open just the data fork */
                adflags = ADFLAGS_DF;
                continue;

            case EACCES:
                adp = NULL; /* maybe it's a file we no rw mode for us */
                break;      /* was return AFPERR_ACCESS;*/
            case EROFS:
                return AFPERR_VLOCK;
            default:
                return( AFPERR_PARAM );
            }
        }
        break;	/* from the while */
    }
    /*
     * Does kFPDeleteInhibitBit (bit 8) set?
     */
    if (checkAttrib) {
        u_int16_t   bshort;
        
        if (adp && (adflags & ADFLAGS_HF)) {

            ad_getattr(&ad, &bshort);
            if ((bshort & htons(ATTRBIT_NODELETE))) {
                ad_close( &ad, adflags );
                return(AFPERR_OLOCK);
            }
        }
        else if (!adp) {
            /* was EACCESS error try to get only metadata */
            ad_init(&ad, vol->v_adouble, vol->v_ad_options);  /* OK */
            if ( ad_metadata( file , 0, &ad) == 0 ) {
                ad_getattr(&ad, &bshort);
                ad_close( &ad, ADFLAGS_HF );
                if ((bshort & htons(ATTRBIT_NODELETE))) {
                    return  AFPERR_OLOCK;
                }
            }
        }
    }
    
    if (adp && (adflags & ADFLAGS_HF) ) {
        /* FIXME we have a pb here because we want to know if a file is open 
         * there's a 'priority inversion' if you can't open the ressource fork RW
         * you can delete it if it's open because you can't get a write lock.
         * 
         * ADLOCK_FILELOCK means the whole ressource fork, not only after the 
         * metadatas
         *
         * FIXME it doesn't work for RFORK open read only and fork open without deny mode
         */
        if (ad_tmplock(&ad, ADEID_RFORK, ADLOCK_WR |ADLOCK_FILELOCK, 0, 0, 0) < 0 ) {
            ad_close( &ad, adflags );
            return( AFPERR_BUSY );
        }
    }

    if (adp && ad_tmplock( &ad, ADEID_DFORK, ADLOCK_WR, 0, 0, 0 ) < 0) {
        err = AFPERR_BUSY;
    }
    else if (!(err = netatalk_unlink( vol->ad_path( file, ADFLAGS_HF)) ) &&
             !(err = netatalk_unlink( file )) ) {
        cnid_t id;
        if (checkAttrib && (id = cnid_get(vol->v_cdb, curdir->d_did, file, strlen(file)))) 
        {
            cnid_delete(vol->v_cdb, id);
        }

    }
    if (adp)
        ad_close( &ad, adflags );  /* ad_close removes locks if any */

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end deletefile:");
#endif /* DEBUG */

    return err;
}

/* ------------------------------------ */
/* return a file id */
int afp_createid(obj, ibuf, ibuflen, rbuf, rbuflen )
AFPObj      *obj;
char	*ibuf, *rbuf;
int		ibuflen, *rbuflen;
{
    struct stat         *st;
    struct vol		*vol;
    struct dir		*dir;
    char		*upath;
    int                 len;
    cnid_t		did, id;
    u_short		vid;
    struct path         *s_path;

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin afp_createid:");
#endif /* DEBUG */

    *rbuflen = 0;

    ibuf += 2;

    memcpy(&vid, ibuf, sizeof(vid));
    ibuf += sizeof(vid);

    if (NULL == ( vol = getvolbyvid( vid )) ) {
        return( AFPERR_PARAM);
    }

    if (vol->v_cdb == NULL || !(vol->v_cdb->flags & CNID_FLAG_PERSISTENT)) {
        return AFPERR_NOOP;
    }

    if (vol->v_flags & AFPVOL_RO)
        return AFPERR_VLOCK;

    memcpy(&did, ibuf, sizeof( did ));
    ibuf += sizeof(did);

    if (NULL == ( dir = dirlookup( vol, did )) ) {
        return afp_errno; /* was AFPERR_PARAM */
    }

    if (NULL == ( s_path = cname( vol, dir, &ibuf )) ) {
        return get_afp_errno(AFPERR_NOOBJ); /* was AFPERR_PARAM */
    }

    if ( path_isadir(s_path) ) {
        return( AFPERR_BADTYPE );
    }

    upath = s_path->u_name;
    switch (s_path->st_errno) {
        case 0:
             break; /* success */
        case EPERM:
        case EACCES:
            return AFPERR_ACCESS;
        case ENOENT:
            return AFPERR_NOOBJ;
        default:
            return AFPERR_PARAM;
    }
    st = &s_path->st;
    if ((id = cnid_lookup(vol->v_cdb, st, did, upath, len = strlen(upath)))) {
        memcpy(rbuf, &id, sizeof(id));
        *rbuflen = sizeof(id);
        return AFPERR_EXISTID;
    }

    if ((id = get_id(vol, NULL, st, did, upath, len)) != CNID_INVALID) {
        memcpy(rbuf, &id, sizeof(id));
        *rbuflen = sizeof(id);
        return AFP_OK;
    }

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "ending afp_createid...:");
#endif /* DEBUG */
    return afp_errno;
}

static int
reenumerate_id(const struct vol *vol, char *name, cnid_t did)
{
    DIR             *dp;
    struct dirent   *de;
    int             ret;
    cnid_t	    aint;
    struct path     path;
    
    memset(&path, 0, sizeof(path));
    if (vol->v_cdb == NULL) {
	return -1;
    }
    if (NULL == ( dp = opendir( name)) ) {
        return -1;
    }
    ret = 0;
    for ( de = readdir( dp ); de != NULL; de = readdir( dp )) {
        if (NULL == check_dirent(vol, de->d_name))
            continue;

        if ( stat(de->d_name, &path.st)<0 )
            continue;
	
	/* update or add to cnid */
        aint = cnid_add(vol->v_cdb, &path.st, did, de->d_name, strlen(de->d_name), 0); /* ignore errors */

#if AD_VERSION > AD_VERSION1
        if (aint != CNID_INVALID && !S_ISDIR(path.st.st_mode)) {
            struct adouble  ad, *adp;

            path.st_errno = 0;
            path.st_valid = 1;
            path.u_name = de->d_name;
            
            adp = of_ad(vol, &path, &ad);
            
            if ( ad_open( de->d_name, ADFLAGS_HF, O_RDWR, 0, adp ) < 0 ) {
                continue;
            }
            if (ad_setid(adp, path.st.st_dev, path.st.st_ino, aint, did, vol->v_stamp)) {
            	ad_flush(adp, ADFLAGS_HF);
            }
            ad_close(adp, ADFLAGS_HF);
        }
#endif /* AD_VERSION > AD_VERSION1 */

        ret++;
    }
    closedir(dp);
    return ret;
}

    
/* ------------------------------
   resolve a file id */
int afp_resolveid(obj, ibuf, ibuflen, rbuf, rbuflen )
AFPObj      *obj;
char	*ibuf, *rbuf;
int		ibuflen, *rbuflen;
{
    struct vol		*vol;
    struct dir		*dir;
    char		*upath;
    struct path         path;
    int                 err, buflen, retry=0;
    cnid_t		id, cnid;
    u_int16_t		vid, bitmap;

    static char buffer[12 + MAXPATHLEN + 1];
    int len = 12 + MAXPATHLEN + 1;

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin afp_resolveid:");
#endif /* DEBUG */

    *rbuflen = 0;
    ibuf += 2;

    memcpy(&vid, ibuf, sizeof(vid));
    ibuf += sizeof(vid);

    if (NULL == ( vol = getvolbyvid( vid )) ) {
        return( AFPERR_PARAM);
    }

    if (vol->v_cdb == NULL || !(vol->v_cdb->flags & CNID_FLAG_PERSISTENT)) {
        return AFPERR_NOOP;
    }

    memcpy(&id, ibuf, sizeof( id ));
    ibuf += sizeof(id);
    cnid = id;
    
    if (!id) {
        /* some MacOS versions after a catsearch do a *lot* of afp_resolveid with 0 */
        return AFPERR_NOID;
    }
retry:
    if (NULL == (upath = cnid_resolve(vol->v_cdb, &id, buffer, len)) ) {
        return AFPERR_NOID; /* was AFPERR_BADID, but help older Macs */
    }

    if (NULL == ( dir = dirlookup( vol, id )) ) {
        return AFPERR_NOID; /* idem AFPERR_PARAM */
    }
    path.u_name = upath;
    if (movecwd(vol, dir) < 0) {
        switch (errno) {
        case EACCES:
        case EPERM:
            return AFPERR_ACCESS;
        case ENOENT:
            return AFPERR_NOID;
        default:
            return AFPERR_PARAM;
        }
    }

    if ( of_stat(&path) < 0 ) {
#ifdef ESTALE
        /* with nfs and our working directory is deleted */
	if (errno == ESTALE) {
	    errno = ENOENT;
	}
#endif	
	if ( errno == ENOENT && !retry) {
	    /* cnid db is out of sync, reenumerate the directory and updated ids */
	    reenumerate_id(vol, ".", id);
	    id = cnid;
	    retry = 1;
	    goto retry;
        }
        switch (errno) {
        case EACCES:
        case EPERM:
            return AFPERR_ACCESS;
        case ENOENT:
            return AFPERR_NOID;
        default:
            return AFPERR_PARAM;
        }
    }

    /* directories are bad */
    if (S_ISDIR(path.st.st_mode))
        return AFPERR_BADTYPE;

    memcpy(&bitmap, ibuf, sizeof(bitmap));
    bitmap = ntohs( bitmap );
    if (NULL == (path.m_name = utompath(vol, upath, cnid, utf8_encoding()))) {
        return AFPERR_NOID;
    }
    if (AFP_OK != (err = getfilparams(vol, bitmap, &path , curdir, 
                            rbuf + sizeof(bitmap), &buflen))) {
        return err;
    }
    *rbuflen = buflen + sizeof(bitmap);
    memcpy(rbuf, ibuf, sizeof(bitmap));

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end afp_resolveid:");
#endif /* DEBUG */

    return AFP_OK;
}

/* ------------------------------ */
int afp_deleteid(obj, ibuf, ibuflen, rbuf, rbuflen )
AFPObj      *obj;
char	*ibuf, *rbuf;
int		ibuflen, *rbuflen;
{
    struct stat         st;
    struct vol		*vol;
    struct dir		*dir;
    char                *upath;
    int                 err;
    cnid_t		id;
    cnid_t		fileid;
    u_short		vid;
    static char buffer[12 + MAXPATHLEN + 1];
    int len = 12 + MAXPATHLEN + 1;

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin afp_deleteid:");
#endif /* DEBUG */

    *rbuflen = 0;
    ibuf += 2;

    memcpy(&vid, ibuf, sizeof(vid));
    ibuf += sizeof(vid);

    if (NULL == ( vol = getvolbyvid( vid )) ) {
        return( AFPERR_PARAM);
    }

    if (vol->v_cdb == NULL || !(vol->v_cdb->flags & CNID_FLAG_PERSISTENT)) {
        return AFPERR_NOOP;
    }

    if (vol->v_flags & AFPVOL_RO)
        return AFPERR_VLOCK;

    memcpy(&id, ibuf, sizeof( id ));
    ibuf += sizeof(id);
    fileid = id;

    if (NULL == (upath = cnid_resolve(vol->v_cdb, &id, buffer, len)) ) {
        return AFPERR_NOID;
    }

    if (NULL == ( dir = dirlookup( vol, id )) ) {
        return( AFPERR_PARAM );
    }

    err = AFP_OK;
    if ((movecwd(vol, dir) < 0) || (stat(upath, &st) < 0)) {
        switch (errno) {
        case EACCES:
        case EPERM:
            return AFPERR_ACCESS;
#ifdef ESTALE
	case ESTALE:
#endif	
        case ENOENT:
            /* still try to delete the id */
            err = AFPERR_NOOBJ;
            break;
        default:
            return AFPERR_PARAM;
        }
    }
    else if (S_ISDIR(st.st_mode)) /* directories are bad */
        return AFPERR_BADTYPE;

    if (cnid_delete(vol->v_cdb, fileid)) {
        switch (errno) {
        case EROFS:
            return AFPERR_VLOCK;
        case EPERM:
        case EACCES:
            return AFPERR_ACCESS;
        default:
            return AFPERR_PARAM;
        }
    }

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "end afp_deleteid:");
#endif /* DEBUG */

    return err;
}

/* ------------------------------ */
static struct adouble *find_adouble(struct path *path, struct ofork **of, struct adouble *adp)
{
    int             ret;

    if (path->st_errno) {
        switch (path->st_errno) {
        case ENOENT:
            afp_errno = AFPERR_NOID;
            break;
        case EPERM:
        case EACCES:
            afp_errno = AFPERR_ACCESS;
            break;
        default:
            afp_errno = AFPERR_PARAM;
            break;
        }
        return NULL;
    }
    /* we use file_access both for legacy Mac perm and
     * for unix privilege, rename will take care of folder perms
    */
    if (file_access(path, OPENACC_WR ) < 0) {
        afp_errno = AFPERR_ACCESS;
        return NULL;
    }
    
    if ((*of = of_findname(path))) {
        /* reuse struct adouble so it won't break locks */
        adp = (*of)->of_ad;
    }
    else {
        ret = ad_open( path->u_name, ADFLAGS_HF, O_RDONLY, 0, adp);
        if ( !ret && ad_hfileno(adp) != -1 && !(adp->ad_hf.adf_flags & ( O_RDWR | O_WRONLY))) {
            /* from AFP spec.
             * The user must have the Read & Write privilege for both files in order to use this command.
             */
            ad_close(adp, ADFLAGS_HF);
            afp_errno = AFPERR_ACCESS;
            return NULL;
        }
    }
    return adp;
}

#define APPLETEMP ".AppleTempXXXXXX"

int afp_exchangefiles(obj, ibuf, ibuflen, rbuf, rbuflen )
AFPObj      *obj;
char	*ibuf, *rbuf;
int		ibuflen, *rbuflen;
{
    struct stat         srcst, destst;
    struct vol		*vol;
    struct dir		*dir, *sdir;
    char		*spath, temp[17], *p;
    char                *supath, *upath;
    struct path         *path;
    int                 err;
    struct adouble	ads;
    struct adouble	add;
    struct adouble	*adsp = NULL;
    struct adouble	*addp = NULL;
    struct ofork	*s_of = NULL;
    struct ofork	*d_of = NULL;
    int                 crossdev;
    
    int                 slen, dlen;
    u_int32_t		sid, did;
    u_int16_t		vid;

    uid_t              uid;
    gid_t              gid;

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "begin afp_exchangefiles:");
#endif /* DEBUG */

    *rbuflen = 0;
    ibuf += 2;

    memcpy(&vid, ibuf, sizeof(vid));
    ibuf += sizeof(vid);

    if (NULL == ( vol = getvolbyvid( vid )) ) {
        return( AFPERR_PARAM);
    }

    if ((vol->v_flags & AFPVOL_RO))
        return AFPERR_VLOCK;

    /* source and destination dids */
    memcpy(&sid, ibuf, sizeof(sid));
    ibuf += sizeof(sid);
    memcpy(&did, ibuf, sizeof(did));
    ibuf += sizeof(did);

    /* source file */
    if (NULL == (dir = dirlookup( vol, sid )) ) {
        return afp_errno; /* was AFPERR_PARAM */
    }

    if (NULL == ( path = cname( vol, dir, &ibuf )) ) {
        return get_afp_errno(AFPERR_NOOBJ); 
    }

    if ( path_isadir(path) ) {
        return AFPERR_BADTYPE;   /* it's a dir */
    }

    /* save some stuff */
    srcst = path->st;
    sdir = curdir;
    spath = obj->oldtmp;
    supath = obj->newtmp;
    strcpy(spath, path->m_name);
    strcpy(supath, path->u_name); /* this is for the cnid changing */
    p = absupath( vol, sdir, supath);
    if (!p) {
        /* pathname too long */
        return AFPERR_PARAM ;
    }
    
    ad_init(&ads, vol->v_adouble, vol->v_ad_options);
    if (!(adsp = find_adouble( path, &s_of, &ads))) {
        return afp_errno;
    }

    /* ***** from here we may have resource fork open **** */
    
    /* look for the source cnid. if it doesn't exist, don't worry about
     * it. */
    sid = cnid_lookup(vol->v_cdb, &srcst, sdir->d_did, supath,slen = strlen(supath));

    if (NULL == ( dir = dirlookup( vol, did )) ) {
        err = afp_errno; /* was AFPERR_PARAM */
        goto err_exchangefile;
    }

    if (NULL == ( path = cname( vol, dir, &ibuf )) ) {
        err = get_afp_errno(AFPERR_NOOBJ); 
        goto err_exchangefile;
    }

    if ( path_isadir(path) ) {
        err = AFPERR_BADTYPE;
        goto err_exchangefile;
    }

    /* FPExchangeFiles is the only call that can return the SameObj
     * error */
    if ((curdir == sdir) && strcmp(spath, path->m_name) == 0) {
        err = AFPERR_SAMEOBJ;
        goto err_exchangefile;
    }

    ad_init(&add, vol->v_adouble, vol->v_ad_options);
    if (!(addp = find_adouble( path, &d_of, &add))) {
        err = afp_errno;
        goto err_exchangefile;
    }
    destst = path->st;

    /* they are not on the same device and at least one is open
     * FIXME broken for for crossdev and adouble v2
     * return an error 
    */
    crossdev = (srcst.st_dev != destst.st_dev);
    if (/* (d_of || s_of)  && */ crossdev) {
        err = AFPERR_MISC;
        goto err_exchangefile;
    }

    /* look for destination id. */
    upath = path->u_name;
    did = cnid_lookup(vol->v_cdb, &destst, curdir->d_did, upath, dlen = strlen(upath));

    /* construct a temp name.
     * NOTE: the temp file will be in the dest file's directory. it
     * will also be inaccessible from AFP. */
    memcpy(temp, APPLETEMP, sizeof(APPLETEMP));
    if (!mktemp(temp)) {
        err = AFPERR_MISC;
        goto err_exchangefile;
    }
    
    if (crossdev) {
        /* FIXME we need to close fork for copy, both s_of and d_of are null */
       ad_close(adsp, ADFLAGS_HF);
       ad_close(addp, ADFLAGS_HF);
    }

    /* now, quickly rename the file. we error if we can't. */
    if ((err = renamefile(vol, p, temp, temp, adsp)) != AFP_OK)
        goto err_exchangefile;
    of_rename(vol, s_of, sdir, spath, curdir, temp);

    /* rename destination to source */
    if ((err = renamefile(vol, upath, p, spath, addp)) != AFP_OK)
        goto err_src_to_tmp;
    of_rename(vol, d_of, curdir, path->m_name, sdir, spath);

    /* rename temp to destination */
    if ((err = renamefile(vol, temp, upath, path->m_name, adsp)) != AFP_OK)
        goto err_dest_to_src;
    of_rename(vol, s_of, curdir, temp, curdir, path->m_name);

    /* id's need switching. src -> dest and dest -> src. 
     * we need to re-stat() if it was a cross device copy.
    */
    if (sid) {
	cnid_delete(vol->v_cdb, sid);
    }
    if (did) {
	cnid_delete(vol->v_cdb, did);
    }
    if ((did && ( (crossdev && stat( upath, &srcst) < 0) || 
                cnid_update(vol->v_cdb, did, &srcst, curdir->d_did,upath, dlen) < 0))
       ||
       (sid && ( (crossdev && stat(p, &destst) < 0) ||
                cnid_update(vol->v_cdb, sid, &destst, sdir->d_did,supath, slen) < 0))
    ) {
        switch (errno) {
        case EPERM:
        case EACCES:
            err = AFPERR_ACCESS;
            break;
        default:
            err = AFPERR_PARAM;
        }
        goto err_temp_to_dest;
    }
    
    /* here we need to reopen if crossdev */
    if (sid && ad_setid(addp, destst.st_dev, destst.st_ino,  sid, sdir->d_did, vol->v_stamp)) 
    {
       ad_flush( addp, ADFLAGS_HF );
    }
        
    if (did && ad_setid(adsp, srcst.st_dev, srcst.st_ino,  did, curdir->d_did, vol->v_stamp)) 
    {
       ad_flush( adsp, ADFLAGS_HF );
    }

    /* change perms, src gets dest perm and vice versa */

    uid = geteuid();
    gid = getegid();
    if (seteuid(0)) {
        LOG(log_error, logtype_afpd, "seteuid failed %s", strerror(errno));
        err = AFP_OK; /* ignore error */
        goto err_temp_to_dest;
    }

    /*
     * we need to exchange ACL entries as well
     */
    /* exchange_acls(vol, p, upath); */

    path->st = srcst;
    path->st_valid = 1;
    path->st_errno = 0;
    path->m_name = NULL;
    path->u_name = upath;

    setfilunixmode(vol, path, destst.st_mode);
    setfilowner(vol, destst.st_uid, destst.st_gid, path);

    path->st = destst;
    path->st_valid = 1;
    path->st_errno = 0;
    path->u_name = p;

    setfilunixmode(vol, path, srcst.st_mode);
    setfilowner(vol, srcst.st_uid, srcst.st_gid, path);

    if ( setegid(gid) < 0 || seteuid(uid) < 0) {
        LOG(log_error, logtype_afpd, "can't seteuid back %s", strerror(errno));
        exit(EXITERR_SYS);
    }

#ifdef DEBUG
    LOG(log_info, logtype_afpd, "ending afp_exchangefiles:");
#endif /* DEBUG */

    err = AFP_OK;
    goto err_exchangefile;

    /* all this stuff is so that we can unwind a failed operation
     * properly. */
err_temp_to_dest:
    /* rename dest to temp */
    renamefile(vol, upath, temp, temp, adsp);
    of_rename(vol, s_of, curdir, upath, curdir, temp);

err_dest_to_src:
    /* rename source back to dest */
    renamefile(vol, p, upath, path->m_name, addp);
    of_rename(vol, d_of, sdir, spath, curdir, path->m_name);

err_src_to_tmp:
    /* rename temp back to source */
    renamefile(vol, temp, p, spath, adsp);
    of_rename(vol, s_of, curdir, temp, sdir, spath);

err_exchangefile:
    if ( !s_of && adsp && ad_hfileno(adsp) != -1 ) {
       ad_close(adsp, ADFLAGS_HF);
    }
    if ( !d_of && addp && ad_hfileno(addp) != -1 ) {
       ad_close(addp, ADFLAGS_HF);
    }

    return err;
}
