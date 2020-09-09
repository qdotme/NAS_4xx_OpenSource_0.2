/* 
 * Netatalk 2002 (c)
 * Copyright (C) 1990, 1993 Regents of The University of Michigan
 * All Rights Reserved. See COPYRIGHT
 */


/*
 * This file contains FPCatSearch implementation. FPCatSearch performs
 * file/directory search based on specified criteria. It is used by client
 * to perform fast searches on (propably) big volumes. So, it has to be
 * pretty fast.
 *
 * This implementation bypasses most of adouble/libatalk stuff as long as
 * possible and does a standard filesystem search. It calls higher-level
 * libatalk/afpd functions only when it is really needed, mainly while
 * returning some non-UNIX information or filtering by non-UNIX criteria.
 *
 * Initial version written by Rafal Lewczuk <rlewczuk@pronet.pl>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#if STDC_HEADERS
#include <string.h>
#else
#ifndef HAVE_MEMCPY
#define memcpy(d,s,n) bcopy ((s), (d), (n))
#define memmove(d,s,n) bcopy ((s), (d), (n))
#endif /* ! HAVE_MEMCPY */
#endif

#include <sys/file.h>
#include <netinet/in.h>

#include <atalk/afp.h>
#include <atalk/adouble.h>
#include <atalk/logger.h>
#ifdef CNID_DB
#include <atalk/cnid.h>
#endif /* CNID_DB */
#include "desktop.h"
#include "directory.h"
#include "file.h"
#include "volume.h"
#include "globals.h"
#include "filedir.h"
#include "fork.h"


struct finderinfo {
	u_int32_t f_type;
	u_int32_t creator;
	u_int16_t attrs;    /* File attributes (high 8 bits)*/
	u_int16_t label;    /* Label (low 8 bits )*/
	char reserved[22]; /* Unknown (at least for now...) */
};

typedef char packed_finder[ADEDLEN_FINDERI];

/* Known attributes:
 * 0x04 - has a custom icon
 * 0x20 - name/icon is locked
 * 0x40 - is invisible
 * 0x80 - is alias
 *
 * Known labels:
 * 0x02 - project 2
 * 0x04 - project 1
 * 0x06 - personal
 * 0x08 - cool
 * 0x0a - in progress
 * 0x0c - hot
 * 0x0e - essential
 */

/* This is our search-criteria structure. */
struct scrit {
	u_int32_t rbitmap;          /* Request bitmap - which values should we check ? */
	u_int16_t fbitmap, dbitmap; /* file & directory bitmap - which values should we return ? */
	u_int16_t attr;             /* File attributes */
	time_t cdate;               /* Creation date */
	time_t mdate;               /* Last modification date */
	time_t bdate;               /* Last backup date */
	u_int32_t pdid;             /* Parent DID */
    u_int16_t offcnt;           /* Offspring count */
	struct finderinfo finfo;    /* Finder info */
	char lname[64];             /* Long name */ 
	char utf8name[512];         /* UTF8 name */
};

/*
 * Directory tree search is recursive by its nature. But AFP specification
 * requires FPCatSearch to pause after returning n results and be able to
 * resume the search later. So we have to do recursive search using flat
 * (iterative) algorithm and remember all directories to look into in an
 * stack-like structure. The structure below is one item of directory stack.
 *
 */
struct dsitem {
	struct dir *dir; /* Structure describing this directory */
	int pidx;        /* Parent's dsitem structure index. */
	int checked;     /* Have we checked this directory ? */
	char *path;      /* absolute UNIX path to this directory */
};
 

#define DS_BSIZE 128
static u_int32_t cur_pos = 0;    /* Saved position index (ID) - used to remember "position" across FPCatSearch calls */
static DIR *dirpos = NULL; /* UNIX structure describing currently opened directory. */
static int save_cidx = -1; /* Saved index of currently scanned directory. */

static struct dsitem *dstack = NULL; /* Directory stack data... */
static int dssize = 0;  	     /* Directory stack (allocated) size... */
static int dsidx = 0;   	     /* First free item index... */

static struct scrit c1, c2;          /* search criteria */

/* Puts new item onto directory stack. */
static int addstack(char *uname, struct dir *dir, int pidx)
{
	struct dsitem *ds;
	int           l;

	/* check if we have some space on stack... */
	if (dsidx >= dssize) {
		dssize += DS_BSIZE;
		dstack = realloc(dstack, dssize * sizeof(struct dsitem));	
		if (dstack == NULL)
			return -1;
	}

	/* Put new element. Allocate and copy lname and path. */
	ds = dstack + dsidx++;
	ds->dir = dir;
	ds->pidx = pidx;
	if (pidx >= 0) {
	    l = strlen(dstack[pidx].path);
	    if (!(ds->path = malloc(l + strlen(uname) + 2) ))
			return -1;
		strcpy(ds->path, dstack[pidx].path);
		strcat(ds->path, "/");
		strcat(ds->path, uname);
	}

	ds->checked = 0;

	return 0;
}

/* Removes checked items from top of directory stack. Returns index of the first unchecked elements or -1. */
static int reducestack()
{
	int r;
	if (save_cidx != -1) {
		r = save_cidx;
		save_cidx = -1;
		return r;
	}

	while (dsidx > 0) {
		if (dstack[dsidx-1].checked) {
			dsidx--;
			free(dstack[dsidx].path);
		} else
			return dsidx - 1;
	} 
	return -1;
} 

/* Clears directory stack. */
static void clearstack() 
{
	save_cidx = -1;
	while (dsidx > 0) {
		dsidx--;
		free(dstack[dsidx].path);
	}
} 

/* Looks up for an opened adouble structure, opens resource fork of selected file. 
 * FIXME What about noadouble?
*/
static struct adouble *adl_lkup(struct vol *vol, struct path *path, struct adouble *adp)
{
	static struct adouble ad;
	
	struct ofork *of;
	int isdir;
	
	if (adp)
	    return adp;
	    
	isdir  = S_ISDIR(path->st.st_mode);

	if (!isdir && (of = of_findname(path))) {
		adp = of->of_ad;
	} else {
		ad_init(&ad, vol->v_adouble, vol->v_ad_options);
		adp = &ad;
	} 

    if ( ad_metadata( path->u_name, ((isdir)?ADFLAGS_DIR:0), adp) < 0 ) {
        adp = NULL; /* FIXME without resource fork adl_lkup will be call again */
    }
    
	return adp;	
}

/* -------------------- */
static struct finderinfo *unpack_buffer(struct finderinfo *finfo, char *buffer)
{
	memcpy(&finfo->f_type,  buffer +FINDERINFO_FRTYPEOFF, sizeof(finfo->f_type));
	memcpy(&finfo->creator, buffer +FINDERINFO_FRCREATOFF, sizeof(finfo->creator));
	memcpy(&finfo->attrs,   buffer +FINDERINFO_FRFLAGOFF, sizeof(finfo->attrs));
	memcpy(&finfo->label,   buffer +FINDERINFO_FRFLAGOFF, sizeof(finfo->label));
	finfo->attrs &= 0xff00; /* high 8 bits */
	finfo->label &= 0xff;   /* low 8 bits */

	return finfo;
}

/* -------------------- */
static struct finderinfo *
unpack_finderinfo(char *upath, struct adouble *adp, struct finderinfo *finfo)
{
	packed_finder  buf;
	void           *ptr;
	
	ptr = get_finderinfo(upath, adp, &buf);
	return unpack_buffer(finfo, ptr);
}

/* -------------------- */
#define CATPBIT_PARTIAL 31
/* Criteria checker. This function returns a 2-bit value. */
/* bit 0 means if checked file meets given criteria. */
/* bit 1 means if it is a directory and we should descent into it. */
/* uname - UNIX name 
 * fname - our fname (translated to UNIX)
 * cidx - index in directory stack
 */
static int crit_check(struct vol *vol, struct path *path) {
	int result = 0;
	u_int16_t attr, flags = CONV_PRECOMPOSE;
	struct finderinfo *finfo = NULL, finderinfo;
	struct adouble *adp = NULL;
	time_t c_date, b_date;
	u_int32_t ac_date, ab_date;
	static char convbuf[512];
	size_t len;

	if (S_ISDIR(path->st.st_mode)) {
		if (!c1.dbitmap)
			return 0;
	}
	else {
		if (!c1.fbitmap)
			return 0;

		/* compute the Mac name 
		 * first try without id (it's slow to find it)
		 * An other option would be to call get_id in utompath but 
		 * we need to pass parent dir
		*/
        if (!(path->m_name = utompath(vol, path->u_name, 0 , utf8_encoding()) )) {
        	/*retry with the right id */
       
        	cnid_t id;
        	
        	adp = adl_lkup(vol, path, adp);
        	id = get_id(vol, adp, &path->st, path->d_dir->d_did, path->u_name, strlen(path->u_name));
        	if (!id) {
        		/* FIXME */
        		return 0;
        	}
        	if (!(path->m_name = utompath(vol, path->u_name, id , utf8_encoding()))) {
        		return 0;
        	}
        }
	}
		
	/* Kind of optimization: 
	 * -- first check things we've already have - filename
	 * -- last check things we get from ad_open()
	 * FIXME strmcp strstr (icase)
	 */

	/* Check for filename */
	if ((c1.rbitmap & (1<<DIRPBIT_LNAME))) { 
		if ( (size_t)(-1) == (len = convert_string(vol->v_maccharset, CH_UCS2, path->m_name, strlen(path->m_name), convbuf, 512)) )
			goto crit_check_ret;
		convbuf[len] = 0; 
		if ((c1.rbitmap & (1<<CATPBIT_PARTIAL))) {
			if (strcasestr_w( (ucs2_t*) convbuf, (ucs2_t*) c1.lname) == NULL)
				goto crit_check_ret;
		} else
			if (strcasecmp_w((ucs2_t*) convbuf, (ucs2_t*) c1.lname) != 0)
				goto crit_check_ret;
	} 
	
	if ((c1.rbitmap & (1<<FILPBIT_PDINFO))) { 
		if ( (size_t)(-1) == (len = convert_charset( CH_UTF8_MAC, CH_UCS2, CH_UTF8, path->m_name, strlen(path->m_name), convbuf, 512, &flags))) {
			goto crit_check_ret;
		}
		convbuf[len] = 0; 
		if (c1.rbitmap & (1<<CATPBIT_PARTIAL)) {
			if (strcasestr_w((ucs2_t *) convbuf, (ucs2_t*)c1.utf8name) == NULL)
				goto crit_check_ret;
		} else
			if (strcasecmp_w((ucs2_t *)convbuf, (ucs2_t*)c1.utf8name) != 0)
				goto crit_check_ret;
	} 


	/* FIXME */
	if ((unsigned)c2.mdate > 0x7fffffff)
		c2.mdate = 0x7fffffff;
	if ((unsigned)c2.cdate > 0x7fffffff)
		c2.cdate = 0x7fffffff;
	if ((unsigned)c2.bdate > 0x7fffffff)
		c2.bdate = 0x7fffffff;

	/* Check for modification date */
	if ((c1.rbitmap & (1<<DIRPBIT_MDATE))) {
		if (path->st.st_mtime < c1.mdate || path->st.st_mtime > c2.mdate)
			goto crit_check_ret;
	}
	
	/* Check for creation date... */
	if ((c1.rbitmap & (1<<DIRPBIT_CDATE))) {
		c_date = path->st.st_mtime;
		adp = adl_lkup(vol, path, adp);
		if (adp && ad_getdate(adp, AD_DATE_CREATE, &ac_date) >= 0)
		    c_date = AD_DATE_TO_UNIX(ac_date);

		if (c_date < c1.cdate || c_date > c2.cdate)
			goto crit_check_ret;
	}

	/* Check for backup date... */
	if ((c1.rbitmap & (1<<DIRPBIT_BDATE))) {
		b_date = path->st.st_mtime;
		adp = adl_lkup(vol, path, adp);
		if (adp && ad_getdate(adp, AD_DATE_BACKUP, &ab_date) >= 0)
			b_date = AD_DATE_TO_UNIX(ab_date);

		if (b_date < c1.bdate || b_date > c2.bdate)
			goto crit_check_ret;
	}
				
	/* Check attributes */
	if ((c1.rbitmap & (1<<DIRPBIT_ATTR)) && c2.attr != 0) {
		if ((adp = adl_lkup(vol, path, adp))) {
			ad_getattr(adp, &attr);
			if ((attr & c2.attr) != c1.attr)
				goto crit_check_ret;
		} else 
			goto crit_check_ret;
	}		

        /* Check file type ID */
	if ((c1.rbitmap & (1<<DIRPBIT_FINFO)) && c2.finfo.f_type != 0) {
		adp = adl_lkup(vol, path, adp);
	    finfo = unpack_finderinfo(path->u_name, adp, &finderinfo);
		if (finfo->f_type != c1.finfo.f_type)
			goto crit_check_ret;
	}
	
	/* Check creator ID */
	if ((c1.rbitmap & (1<<DIRPBIT_FINFO)) && c2.finfo.creator != 0) {
		if (!finfo) {
	    	adp = adl_lkup(vol, path, adp);
			finfo = unpack_finderinfo(path->u_name, adp, &finderinfo);
		}
		if (finfo->creator != c1.finfo.creator)
			goto crit_check_ret;
	}
		
	/* Check finder info attributes */
	if ((c1.rbitmap & (1<<DIRPBIT_FINFO)) && c2.finfo.attrs != 0) {
		if (!finfo) {
	    	adp = adl_lkup(vol, path, adp);
			finfo = unpack_finderinfo(path->u_name, adp, &finderinfo);
		}

		if ((finfo->attrs & c2.finfo.attrs) != c1.finfo.attrs)
			goto crit_check_ret;
	}
	
	/* Check label */
	if ((c1.rbitmap & (1<<DIRPBIT_FINFO)) && c2.finfo.label != 0) {
		if (!finfo) {
	    	adp = adl_lkup(vol, path, adp);
			finfo = unpack_finderinfo(path->u_name, adp, &finderinfo);
		}
		if ((finfo->label & c2.finfo.label) != c1.finfo.label)
			goto crit_check_ret;
	}	
	/* FIXME: Attributes check ! */
	
	/* All criteria are met. */
	result |= 1;
crit_check_ret:
	if (adp != NULL)
		ad_close(adp, ADFLAGS_HF);
	return result;
}  

/* ------------------------------ */
static int rslt_add ( struct vol *vol, struct path *path, char **buf, int ext)
{

	char 		*p = *buf;
	int 		ret, tbuf =0;
	u_int16_t	resultsize;
	int 		isdir = S_ISDIR(path->st.st_mode); 

	/* Skip resultsize */
	if (ext) {
		p += sizeof(resultsize); 
	}
	else {
		p++;
	}
	*p++ = isdir ? FILDIRBIT_ISDIR : FILDIRBIT_ISFILE;    /* IsDir ? */

	if (ext) {
		*p++ = 0;                  /* Pad */
	}
	
	if ( isdir ) {
        ret = getdirparams(vol, c1.dbitmap, path, path->d_dir, p , &tbuf ); 
	}
	else {
	    /* FIXME slow if we need the file ID, we already know it */
		ret = getfilparams ( vol, c1.fbitmap, path, path->d_dir, p, &tbuf);
	}

	if ( ret != AFP_OK )
		return 0;

	/* Make sure entry length is even */
	if ((tbuf & 1)) {
	   *p++ = 0;
	   tbuf++;
	}

	if (ext) {
		resultsize = htons(tbuf);
		memcpy ( *buf, &resultsize, sizeof(resultsize) );
		*buf += tbuf + 4;
	}
	else {
		**buf = tbuf;
		*buf += tbuf + 2;
	}

	return 1;
} 
	
#define VETO_STR \
        "./../.AppleDouble/.AppleDB/Network Trash Folder/TheVolumeSettingsFolder/TheFindByContentFolder/.AppleDesktop/.Parent/"

/* This function performs search. It is called directly from afp_catsearch 
 * vol - volume we are searching on ...
 * dir - directory we are starting from ...
 * c1, c2 - search criteria
 * rmatches - maximum number of matches we can return
 * pos - position we've stopped recently
 * rbuf - output buffer
 * rbuflen - output buffer length
 */
#define NUM_ROUNDS 100
static int catsearch(struct vol *vol, struct dir *dir,  
		     int rmatches, u_int32_t *pos, char *rbuf, u_int32_t *nrecs, int *rsize, int ext)
{
	int cidx, r;
	struct dirent *entry;
	int result = AFP_OK;
	int ccr;
    struct path path;
	char *orig_dir = NULL;
	int orig_dir_len = 128;
	char *vpath = vol->v_path;
	char *rrbuf = rbuf;
    time_t start_time;
    int num_rounds = NUM_ROUNDS;
    int cached;
        
	if (*pos != 0 && *pos != cur_pos) {
		result = AFPERR_CATCHNG;
		goto catsearch_end;
	}

	/* FIXME: Category "offspring count ! */

	/* So we are beginning... */
    start_time = time(NULL);

	/* We need to initialize all mandatory structures/variables and change working directory appropriate... */
	if (*pos == 0) {
		clearstack();
		if (dirpos != NULL) {
			closedir(dirpos);
			dirpos = NULL;
		} 
		
		if (addstack("", dir, -1) == -1) {
			result = AFPERR_MISC;
			goto catsearch_end;
		}
		dstack[0].path = strdup(vpath);
		/* FIXME: Sometimes DID is given by client ! (correct this one above !) */
	}

	/* Save current path */
	orig_dir = (char*)malloc(orig_dir_len);
	while (getcwd(orig_dir, orig_dir_len-1)==NULL) {
		if (errno != ERANGE) {
			result = AFPERR_MISC;
			goto catsearch_end;
		}
		orig_dir_len += 128; 
		orig_dir = realloc(orig_dir, orig_dir_len);
	} /* while() */
	
	while ((cidx = reducestack()) != -1) {
		cached = 1;
		if (dirpos == NULL) {
			dirpos = opendir(dstack[cidx].path);
			cached = (dstack[cidx].dir->d_child != NULL);
		}
		if (dirpos == NULL) {
			switch (errno) {
			case EACCES:
				dstack[cidx].checked = 1;
				continue;
			case EMFILE:
			case ENFILE:
			case ENOENT:
				result = AFPERR_NFILE;
				break;
			case ENOMEM:
			case ENOTDIR:
			default:
				result = AFPERR_MISC;
			} /* switch (errno) */
			goto catsearch_end;
		}
		/* FIXME error in chdir, what do we do? */
		chdir(dstack[cidx].path);
		

		while ((entry=readdir(dirpos)) != NULL) {
			(*pos)++;

			if (!check_dirent(vol, entry->d_name))
			   continue;

			path.m_name = NULL;
			path.u_name = entry->d_name;
			if (of_stat(&path) != 0) {
				switch (errno) {
				case EACCES:
				case ELOOP:
				case ENOENT:
					continue;
				case ENOTDIR:
				case EFAULT:
				case ENOMEM:
				case ENAMETOOLONG:
				default:
					result = AFPERR_MISC;
					goto catsearch_end;
				} 
			}
			if (S_ISDIR(path.st.st_mode)) {
				/* here we can short cut 
				   ie if in the same loop the parent dir wasn't in the cache
				   ALL dirsearch_byname will fail.
				*/
				if (cached)
            		path.d_dir = dirsearch_byname(dstack[cidx].dir, path.u_name);
            	else
            		path.d_dir = NULL;
            	if (!path.d_dir) {
                	/* path.m_name is set by adddir */
            	    if (NULL == (path.d_dir = adddir( vol, dstack[cidx].dir, &path) ) ) {
						result = AFPERR_MISC;
						goto catsearch_end;
					}
                }
                path.m_name = path.d_dir->d_m_name; 
                	
				if (addstack(path.u_name, path.d_dir, cidx) == -1) {
					result = AFPERR_MISC;
					goto catsearch_end;
				} 
            }
            else {
            	/* yes it sucks for directory d_dir is the directory, for file it's the parent directory*/
            	path.d_dir = dstack[cidx].dir;
            }
			ccr = crit_check(vol, &path);

			/* bit 0 means that criteria has been met */
			if ((ccr & 1)) {
				r = rslt_add ( vol, &path, &rrbuf, ext);
				
				if (r == 0) {
					result = AFPERR_MISC;
					goto catsearch_end;
				} 
				*nrecs += r;
				/* Number of matches limit */
				if (--rmatches == 0) 
					goto catsearch_pause;
				/* Block size limit */
				if (rrbuf - rbuf >= 448)
					goto catsearch_pause;
			}
			/* MacOS 9 doesn't like servers executing commands longer than few seconds */
			if (--num_rounds <= 0) {
			    if (start_time != time(NULL)) {
					result=AFP_OK;
					goto catsearch_pause;
			    }
			    num_rounds = NUM_ROUNDS;
			}
		} /* while ((entry=readdir(dirpos)) != NULL) */
		closedir(dirpos);
		dirpos = NULL;
		dstack[cidx].checked = 1;
	} /* while (current_idx = reducestack()) != -1) */

	/* We have finished traversing our tree. Return EOF here. */
	result = AFPERR_EOF;
	goto catsearch_end;

catsearch_pause:
	cur_pos = *pos; 
	save_cidx = cidx;

catsearch_end: /* Exiting catsearch: error condition */
	*rsize = rrbuf - rbuf;
	if (orig_dir != NULL) {
		chdir(orig_dir);
		free(orig_dir);
	}
	return result;
} /* catsearch() */

/* -------------------------- */
int catsearch_afp(AFPObj *obj, char *ibuf, int ibuflen,
                  char *rbuf, int *rbuflen, int ext)
{
    struct vol *vol;
    u_int16_t   vid;
    u_int16_t   spec_len;
    u_int32_t   rmatches, reserved;
    u_int32_t	catpos[4];
    u_int32_t   pdid = 0;
    int ret, rsize;
    u_int32_t nrecs = 0;
    unsigned char *spec1, *spec2, *bspec1, *bspec2;
    size_t	len;
    u_int16_t	namelen;
    u_int16_t	flags;
    char  	tmppath[256];

    *rbuflen = 0;

    /* min header size */
    if (ibuflen < 32) {
        return AFPERR_PARAM;
    }

    memset(&c1, 0, sizeof(c1));
    memset(&c2, 0, sizeof(c2));

    ibuf += 2;
    memcpy(&vid, ibuf, sizeof(vid));
    ibuf += sizeof(vid);

    if ((vol = getvolbyvid(vid)) == NULL) {
        return AFPERR_PARAM;
    }
    
    memcpy(&rmatches, ibuf, sizeof(rmatches));
    rmatches = ntohl(rmatches);
    ibuf += sizeof(rmatches); 

    /* FIXME: (rl) should we check if reserved == 0 ? */
    ibuf += sizeof(reserved);

    memcpy(catpos, ibuf, sizeof(catpos));
    ibuf += sizeof(catpos);

    memcpy(&c1.fbitmap, ibuf, sizeof(c1.fbitmap));
    c1.fbitmap = c2.fbitmap = ntohs(c1.fbitmap);
    ibuf += sizeof(c1.fbitmap);

    memcpy(&c1.dbitmap, ibuf, sizeof(c1.dbitmap));
    c1.dbitmap = c2.dbitmap = ntohs(c1.dbitmap);
    ibuf += sizeof(c1.dbitmap);

    memcpy(&c1.rbitmap, ibuf, sizeof(c1.rbitmap));
    c1.rbitmap = c2.rbitmap = ntohl(c1.rbitmap);
    ibuf += sizeof(c1.rbitmap);

    if (! (c1.fbitmap || c1.dbitmap)) {
	    return AFPERR_BITMAP;
    }

    if ( ext) {
        memcpy(&spec_len, ibuf, sizeof(spec_len));
        spec_len = ntohs(spec_len);
    }
    else {
        /* with catsearch only name and parent id are allowed */
    	c1.fbitmap &= (1<<FILPBIT_LNAME) | (1<<FILPBIT_PDID);
    	c1.dbitmap &= (1<<DIRPBIT_LNAME) | (1<<DIRPBIT_PDID);
        spec_len = *(unsigned char*)ibuf;
    }

    /* Parse file specifications */
    spec1 = ibuf;
    spec2 = ibuf + spec_len + 2;

    spec1 += 2; 
    spec2 += 2; 

    bspec1 = spec1;
    bspec2 = spec2;
    /* File attribute bits... */
    if (c1.rbitmap & (1 << FILPBIT_ATTR)) {
	    memcpy(&c1.attr, spec1, sizeof(c1.attr));
	    spec1 += sizeof(c1.attr);
	    memcpy(&c2.attr, spec2, sizeof(c2.attr));
	    spec2 += sizeof(c1.attr);
    }

    /* Parent DID */
    if (c1.rbitmap & (1 << FILPBIT_PDID)) {
            memcpy(&c1.pdid, spec1, sizeof(pdid));
	    spec1 += sizeof(c1.pdid);
	    memcpy(&c2.pdid, spec2, sizeof(pdid));
	    spec2 += sizeof(c2.pdid);
    } /* FIXME: PDID - do we demarshall this argument ? */

    /* Creation date */
    if (c1.rbitmap & (1 << FILPBIT_CDATE)) {
	    memcpy(&c1.cdate, spec1, sizeof(c1.cdate));
	    spec1 += sizeof(c1.cdate);
	    c1.cdate = AD_DATE_TO_UNIX(c1.cdate);
	    memcpy(&c2.cdate, spec2, sizeof(c2.cdate));
	    spec2 += sizeof(c1.cdate);
	    ibuf += sizeof(c1.cdate);;
	    c2.cdate = AD_DATE_TO_UNIX(c2.cdate);
    }

    /* Modification date */
    if (c1.rbitmap & (1 << FILPBIT_MDATE)) {
	    memcpy(&c1.mdate, spec1, sizeof(c1.mdate));
	    c1.mdate = AD_DATE_TO_UNIX(c1.mdate);
	    spec1 += sizeof(c1.mdate);
	    memcpy(&c2.mdate, spec2, sizeof(c2.mdate));
	    c2.mdate = AD_DATE_TO_UNIX(c2.mdate);
	    spec2 += sizeof(c1.mdate);
    }
    
    /* Backup date */
    if (c1.rbitmap & (1 << FILPBIT_BDATE)) {
	    memcpy(&c1.bdate, spec1, sizeof(c1.bdate));
	    spec1 += sizeof(c1.bdate);
	    c1.bdate = AD_DATE_TO_UNIX(c1.bdate);
	    memcpy(&c2.bdate, spec2, sizeof(c2.bdate));
	    spec2 += sizeof(c2.bdate);
	    c1.bdate = AD_DATE_TO_UNIX(c2.bdate);
    }

    /* Finder info */
    if (c1.rbitmap & (1 << FILPBIT_FINFO)) {
    	packed_finder buf;
    	
	    memcpy(buf, spec1, sizeof(buf));
	    unpack_buffer(&c1.finfo, buf);    	
	    spec1 += sizeof(buf);

	    memcpy(buf, spec2, sizeof(buf));
	    unpack_buffer(&c2.finfo, buf);
	    spec2 += sizeof(buf);
    } /* Finder info */

    if ((c1.rbitmap & (1 << DIRPBIT_OFFCNT)) != 0) {
        /* Offspring count - only directories */
		if (c1.fbitmap == 0) {
	    	memcpy(&c1.offcnt, spec1, sizeof(c1.offcnt));
	    	spec1 += sizeof(c1.offcnt);
	    	c1.offcnt = ntohs(c1.offcnt);
	    	memcpy(&c2.offcnt, spec2, sizeof(c2.offcnt));
	    	spec2 += sizeof(c2.offcnt);
	    	c2.offcnt = ntohs(c2.offcnt);
		}
		else if (c1.dbitmap == 0) {
			/* ressource fork length */
		}
		else {
	    	return AFPERR_BITMAP;  /* error */
		}
    } /* Offspring count/ressource fork length */

    /* Long name */
    if (c1.rbitmap & (1 << FILPBIT_LNAME)) {
        /* Get the long filename */	
		memcpy(tmppath, bspec1 + spec1[1] + 1, (bspec1 + spec1[1])[0]);
		tmppath[(bspec1 + spec1[1])[0]]= 0;
		len = convert_string ( vol->v_maccharset, CH_UCS2, tmppath, strlen(tmppath), c1.lname, 64);
        if (len == (size_t)(-1))
            return AFPERR_PARAM;
		c1.lname[len] = 0;

#if 0	
		/* FIXME: do we need it ? It's always null ! */
		memcpy(c2.lname, bspec2 + spec2[1] + 1, (bspec2 + spec2[1])[0]);
		c2.lname[(bspec2 + spec2[1])[0]]= 0;
#endif
    }
        /* UTF8 Name */
    if (c1.rbitmap & (1 << FILPBIT_PDINFO)) {

		/* offset */
		memcpy(&namelen, spec1, sizeof(namelen));
		namelen = ntohs (namelen);

		spec1 = bspec1+namelen+4; /* Skip Unicode Hint */

		/* length */
		memcpy(&namelen, spec1, sizeof(namelen));
		namelen = ntohs (namelen);
		if (namelen > 255)  /* Safeguard */
			namelen = 255;

		memcpy (c1.utf8name, spec1+2, namelen);
		c1.utf8name[(namelen+1)] =0;

 		/* convert charset */
		flags = CONV_PRECOMPOSE;
 		len = convert_charset(CH_UTF8_MAC, CH_UCS2, CH_UTF8, c1.utf8name, namelen, c1.utf8name, 512, &flags);
        if (len == (size_t)(-1))
            return AFPERR_PARAM;
 		c1.utf8name[len]=0;
    }
    
    /* Call search */
    *rbuflen = 24;
    ret = catsearch(vol, vol->v_dir, rmatches, &catpos[0], rbuf+24, &nrecs, &rsize, ext);
    memcpy(rbuf, catpos, sizeof(catpos));
    rbuf += sizeof(catpos);

    c1.fbitmap = htons(c1.fbitmap);
    memcpy(rbuf, &c1.fbitmap, sizeof(c1.fbitmap));
    rbuf += sizeof(c1.fbitmap);

    c1.dbitmap = htons(c1.dbitmap);
    memcpy(rbuf, &c1.dbitmap, sizeof(c1.dbitmap));
    rbuf += sizeof(c1.dbitmap);

    nrecs = htonl(nrecs);
    memcpy(rbuf, &nrecs, sizeof(nrecs));
    rbuf += sizeof(nrecs);
    *rbuflen += rsize;

    return ret;
} /* catsearch_afp */

/* -------------------------- */
int afp_catsearch (AFPObj *obj, char *ibuf, int ibuflen,
                  char *rbuf, int *rbuflen)
{
	return catsearch_afp( obj, ibuf, ibuflen, rbuf, rbuflen, 0);
}


int afp_catsearch_ext (AFPObj *obj, char *ibuf, int ibuflen,
                  char *rbuf, int *rbuflen)
{
	return catsearch_afp( obj, ibuf, ibuflen, rbuf, rbuflen, 1);
}

/* FIXME: we need a clean separation between afp stubs and 'real' implementation */
/* (so, all buffer packing/unpacking should be done in stub, everything else 
   should be done in other functions) */