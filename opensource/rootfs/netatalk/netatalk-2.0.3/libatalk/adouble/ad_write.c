/*
 * $Id: ad_write.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (c) 1990,1995 Regents of The University of Michigan.
 * All Rights Reserved.  See COPYRIGHT.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <atalk/adouble.h>

#include <string.h>
#include <sys/param.h>
#include <errno.h>


#ifndef MIN
#define MIN(a,b)	((a)<(b)?(a):(b))
#endif /* ! MIN */

/* XXX: locking has to be checked before each stream of consecutive
 *      ad_writes to prevent a lock in the middle from causing problems. 
 */

ssize_t adf_pwrite(struct ad_fd *ad_fd, const void *buf, size_t count, off_t offset)
{
    ssize_t		cc;

#ifndef  HAVE_PWRITE
    if ( ad_fd->adf_off != offset ) {
	if ( lseek( ad_fd->adf_fd, offset, SEEK_SET ) < 0 ) {
	    return -1;
	}
	ad_fd->adf_off = offset;
    }
    cc = write( ad_fd->adf_fd, buf, count );
    if ( cc < 0 ) {
        return -1;
    }
    ad_fd->adf_off += cc;
#else
   cc = pwrite(ad_fd->adf_fd, buf, count, offset );
#endif
    return cc;
}

/* end is always 0 */
ssize_t ad_write( ad, eid, off, end, buf, buflen )
    struct adouble	*ad;
    const u_int32_t	eid;
    off_t               off;
    const int		end;
    const char		*buf;
    const size_t	buflen;
{
    struct stat		st;
    ssize_t		cc;
    
    if ( eid == ADEID_DFORK ) {
	if ( end ) {
	    if ( fstat( ad->ad_df.adf_fd, &st ) < 0 ) {
		return( -1 );
	    }
	    off = st.st_size - off;
	}
	cc = adf_pwrite(&ad->ad_df, buf, buflen, off);
    } else if ( eid == ADEID_RFORK ) {
        off_t    r_off;

	if ( end ) {
	    if ( fstat( ad->ad_df.adf_fd, &st ) < 0 ) {
		return( -1 );
	    }
	    off = st.st_size - off -ad_getentryoff(ad, eid);
	}
	r_off = ad_getentryoff(ad, eid) + off;
	cc = adf_pwrite(&ad->ad_hf, buf, buflen, r_off);

	/* sync up our internal buffer  FIXME always false? */
	if (r_off < ad_getentryoff(ad, ADEID_RFORK)) {
	    memcpy(ad->ad_data + r_off, buf, MIN(sizeof(ad->ad_data) -r_off, cc));
        }
        if ( ad->ad_rlen  < r_off + cc ) {
             ad->ad_rlen = r_off + cc;
        }
    }
    else {
        return -1; /* we don't know how to write if it's not a ressource or data fork */
    }
    return( cc );
}

/* 
 * the caller set the locks
 * ftruncate is undefined when the file length is smaller than 'size'
 */
int sys_ftruncate(int fd, off_t length)
{

#ifndef  HAVE_PWRITE
off_t           curpos;
#endif
int             err;
struct stat	st;
char            c = 0;

    if (!ftruncate(fd, length)) {
        return 0;
    }
    /* maybe ftruncate doesn't work if we try to extend the size */
    err = errno;

#ifndef  HAVE_PWRITE
    /* we only care about file pointer if we don't use pwrite */
    if ((off_t)-1 == (curpos = lseek(fd, 0, SEEK_CUR)) ) {
        errno = err;
        return -1;
    }
#endif

    if ( fstat( fd, &st ) < 0 ) {
        errno = err;
        return -1;
    }
    
    if (st.st_size > length) {
        errno = err;
        return -1;
    }

    if (lseek(fd, length -1, SEEK_SET) != length -1) {
        errno = err;
        return -1;
    }

    if (1 != write( fd, &c, 1 )) {
        /* return the write errno */
        return -1;
    }

#ifndef  HAVE_PWRITE
    if (curpos != lseek(fd, curpos,  SEEK_SET)) {
        errno = err;
        return -1;
    }
#endif

    return 0;    
}

/* ------------------------ */
int ad_rtruncate( ad, size )
    struct adouble	*ad;
    const off_t  	size;
{
    if ( sys_ftruncate( ad->ad_hf.adf_fd,
	    size + ad->ad_eid[ ADEID_RFORK ].ade_off ) < 0 ) {
	return -1;
    }
    ad->ad_rlen = size;    

    return 0;
}

int ad_dtruncate(ad, size)
    struct adouble	*ad;
    const off_t 	size;
{
    if (sys_ftruncate(ad->ad_df.adf_fd, size) < 0) {
      return -1;
    }
    return 0;
}
