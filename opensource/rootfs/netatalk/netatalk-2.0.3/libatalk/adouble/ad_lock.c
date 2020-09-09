/* 
 * $Id: ad_lock.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (c) 1998,1999 Adrian Sun (asun@zoology.washington.edu)
 * All Rights Reserved. See COPYRIGHT for more information.
 *
 * Byte-range locks. This uses either whole-file flocks to fake byte
 * locks or fcntl-based actual byte locks. Because fcntl locks are
 * process-oriented, we need to keep around a list of file descriptors
 * that refer to the same file. Currently, this doesn't serialize access 
 * to the locks. as a result, there's the potential for race conditions. 
 *
 * TODO: fix the race when reading/writing.
 *       keep a pool of both locks and reference counters around so that
 *       we can save on mallocs. we should also use a tree to keep things
 *       sorted.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <atalk/adouble.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <string.h>

#include "ad_private.h"

/* translate between ADLOCK styles and specific locking mechanisms */
#define XLATE_FLOCK(type) ((type) == ADLOCK_RD ? LOCK_SH : \
((type) == ADLOCK_WR ? LOCK_EX : \
 ((type) == ADLOCK_CLR ? LOCK_UN : -1)))

#ifdef DISABLE_LOCKING
#define fcntl(a, b, c ) (0)
#endif

/* ----------------------- */
static int XLATE_FCNTL_LOCK(int type) 
{
    switch(type) {
    case ADLOCK_RD:
        return F_RDLCK;
    case ADLOCK_WR:
         return F_WRLCK;
    case ADLOCK_CLR:
         return F_UNLCK;
    }
    return -1;
}

/* ----------------------- */
static int OVERLAP(off_t a, off_t alen, off_t b, off_t blen) 
{
 return (!alen && a <= b) || 
	(!blen && b <= a) || 
	( (a + alen > b) && (b + blen > a) );
}

/* allocation for lock regions. we allocate aggressively and shrink
 * only in large chunks. */
#define ARRAY_BLOCK_SIZE 10 
#define ARRAY_FREE_DELTA 100

/* remove a lock and compact space if necessary */
static __inline__ void adf_freelock(struct ad_fd *ad, const int i)
{
    adf_lock_t *lock = ad->adf_lock + i;

    if (--(*lock->refcount) < 1) {
	free(lock->refcount); 
	if (!ad->adf_excl) {
	    lock->lock.l_type = F_UNLCK;
	    fcntl(ad->adf_fd, F_SETLK, &lock->lock); /* unlock */
	}
    }

    ad->adf_lockcount--;

    /* move another lock into the empty space */
    if (i < ad->adf_lockcount) {
        memcpy(lock, lock + ad->adf_lockcount - i, sizeof(adf_lock_t));
    }

    /* free extra cruft if we go past a boundary. we always want to
     * keep at least some stuff around for allocations. this wastes
     * a bit of space to save time on reallocations. */
    if ((ad->adf_lockmax > ARRAY_FREE_DELTA) &&
	(ad->adf_lockcount + ARRAY_FREE_DELTA < ad->adf_lockmax)) {
	    struct adf_lock_t *tmp;

	    tmp = (struct adf_lock_t *) 
		    realloc(ad->adf_lock, sizeof(adf_lock_t)*
			    (ad->adf_lockcount + ARRAY_FREE_DELTA));
	    if (tmp) {
		ad->adf_lock = tmp;
		ad->adf_lockmax = ad->adf_lockcount + ARRAY_FREE_DELTA;
	    }
    }
}


/* this needs to deal with the following cases:
 * 1) user is the only user of the lock 
 * 2) user shares a read lock with another user
 *
 * i converted to using arrays of locks. everytime a lock
 * gets removed, we shift all of the locks down.
 */
static __inline__ void adf_unlock(struct ad_fd *ad, const int user)
{
    adf_lock_t *lock = ad->adf_lock;
    int i;

    for (i = 0; i < ad->adf_lockcount; i++) {
      if (lock[i].user == user) {
	/* we're really going to delete this lock. note: read locks
           are the only ones that allow refcounts > 1 */
	 adf_freelock(ad, i);
	 i--; /* we shifted things down, so we need to backtrack */
       }
    }
}

/* relock any byte lock that overlaps off/len. unlock everything
 * else. */
static __inline__ void adf_relockrange(struct ad_fd *ad, int fd,
				       const off_t off, const off_t len)
{
    adf_lock_t *lock = ad->adf_lock;
    int i;
    
    if (!ad->adf_excl) for (i = 0; i < ad->adf_lockcount; i++) {
      if (OVERLAP(off, len, lock[i].lock.l_start, lock[i].lock.l_len)) 
	fcntl(fd, F_SETLK, &lock[i].lock);
    }
}


/* find a byte lock that overlaps off/len for a particular user */
static __inline__ int adf_findlock(struct ad_fd *ad,
				   const int user, const int type,
				   const off_t off,
				   const off_t len)
{
  adf_lock_t *lock = ad->adf_lock;
  int i;
  
  for (i = 0; i < ad->adf_lockcount; i++) {
    if ((((type & ADLOCK_RD) && (lock[i].lock.l_type == F_RDLCK)) ||
	((type & ADLOCK_WR) && (lock[i].lock.l_type == F_WRLCK))) &&
	(lock[i].user == user) && 
	OVERLAP(off, len, lock[i].lock.l_start, lock[i].lock.l_len)) {
      return i;
    }
  }

  return -1;
}


/* search other user lock lists */
static __inline__  int adf_findxlock(struct ad_fd *ad, 
				     const int user, const int type,
				     const off_t off,
				     const off_t len)
{
  adf_lock_t *lock = ad->adf_lock;
  int i;
  
  for (i = 0; i < ad->adf_lockcount; i++) {
    if ((((type & ADLOCK_RD) && (lock[i].lock.l_type == F_RDLCK)) ||
	 ((type & ADLOCK_WR) && (lock[i].lock.l_type == F_WRLCK))) &&
	(lock[i].user != user) && 
	OVERLAP(off, len, lock[i].lock.l_start, lock[i].lock.l_len)) 
	    return i;
  } 
  return -1;
}

/* okay, this needs to do the following:
 * 1) check current list of locks. error on conflict.
 * 2) apply the lock. error on conflict with another process.
 * 3) update the list of locks this file has.
 * 
 * NOTE: this treats synchronization locks a little differently. we
 *       do the following things for those:
 *       1) if the header file exists, all the locks go in the beginning
 *          of that.
 *       2) if the header file doesn't exist, we stick the locks
 *          in the locations specified by AD_FILELOCK_RD/WR.
 */
#define LOCK_RSRC_RD (0)
#define LOCK_RSRC_WR (1)
#define LOCK_DATA_RD (2)
#define LOCK_DATA_WR (3)

#define LOCK_RSRC_DRD (4)
#define LOCK_RSRC_DWR (5)
#define LOCK_DATA_DRD (6)
#define LOCK_DATA_DWR (7)

#define LOCK_RSRC_NONE (8)
#define LOCK_DATA_NONE (9)

/* -------------- 
	translate a data fork lock to an offset
*/

static off_t df2off(int off)
{
int start = off;
	if (off == AD_FILELOCK_OPEN_WR)
		start = LOCK_DATA_WR;
	else if (off == AD_FILELOCK_OPEN_RD)
		start = LOCK_DATA_RD;
    else if (off == AD_FILELOCK_DENY_RD)
		start = LOCK_DATA_DRD;
	else if (off == AD_FILELOCK_DENY_WR)
		start = LOCK_DATA_DWR;
	else if (off == AD_FILELOCK_OPEN_NONE)
		start = LOCK_DATA_NONE;
	return start;
}

/* -------------- 
	translate a resource fork lock to an offset
*/

static off_t hf2off(int off)
{
int start = off;
	if (off == AD_FILELOCK_OPEN_WR)
		start = LOCK_RSRC_WR;
	else if (off == AD_FILELOCK_OPEN_RD)
		start = LOCK_RSRC_RD;
    else if (off == AD_FILELOCK_DENY_RD)
		start = LOCK_RSRC_DRD;
	else if (off == AD_FILELOCK_DENY_WR)
		start = LOCK_RSRC_DWR;
	else if (off == AD_FILELOCK_OPEN_NONE)
		start = LOCK_RSRC_NONE;
	return start;
}

/* ------------------ */
int ad_fcntl_lock(struct adouble *ad, const u_int32_t eid, const int locktype,
		  const off_t off, const off_t len, const int user)
{
  struct flock lock;
  struct ad_fd *adf;
  adf_lock_t *adflock;
  int oldlock;
  int i;
  int type;  

  lock.l_start = off;
  type = locktype;
  if (eid == ADEID_DFORK) {
    adf = &ad->ad_df;
    if ((type & ADLOCK_FILELOCK)) {
        if (ad_hfileno(ad) != -1) {
            lock.l_start = df2off(off);
            adf = &ad->ad_hf;
        }
    }
  } else { /* rfork */
    adf = &ad->ad_hf;
    if (adf->adf_fd == -1) {
        /* there's no resource fork. return a lock error
         * otherwise if a second process is able to create it
         * locks are a mess.
         */
        errno = EACCES;
        return -1;
    }
    if (type & ADLOCK_FILELOCK) 
      lock.l_start = hf2off(off);
    else
      lock.l_start += ad_getentryoff(ad, eid);
  }
  /* NOTE: we can't write lock a read-only file. on those, we just
    * make sure that we have a read lock set. that way, we at least prevent
    * someone else from really setting a deny read/write on the file. 
    */
  if (!(adf->adf_flags & O_RDWR) && (type & ADLOCK_WR)) {
      type = (type & ~ADLOCK_WR) | ADLOCK_RD;
  }
  
  lock.l_type = XLATE_FCNTL_LOCK(type & ADLOCK_MASK);
  lock.l_whence = SEEK_SET;
  lock.l_len = len;

  /* byte_lock(len=-1) lock whole file */
  if (len == BYTELOCK_MAX) {
      lock.l_len -= lock.l_start; /* otherwise  EOVERFLOW error */
  }

  /* see if it's locked by another user. 
   * NOTE: this guarantees that any existing locks must be at most
   * read locks. we use ADLOCK_WR/RD because F_RD/WRLCK aren't
   * guaranteed to be ORable. */
  if (adf_findxlock(adf, user, ADLOCK_WR | 
		    ((type & ADLOCK_WR) ? ADLOCK_RD : 0), 
		    lock.l_start, lock.l_len) > -1) {
    errno = EACCES;
    return -1;
  }
  
  /* look for any existing lock that we may have */
  i = adf_findlock(adf, user, ADLOCK_RD | ADLOCK_WR, lock.l_start, lock.l_len);
  adflock = (i < 0) ? NULL : adf->adf_lock + i;

  /* here's what we check for:
     1) we're trying to re-lock a lock, but we didn't specify an update.
     2) we're trying to free only part of a lock. 
     3) we're trying to free a non-existent lock. */
  if ((!adflock && (lock.l_type == F_UNLCK)) ||
      (adflock && !(type & ADLOCK_UPGRADE) && 
       ((lock.l_type != F_UNLCK) || (adflock->lock.l_start != lock.l_start) ||
	(adflock->lock.l_len != lock.l_len)))) {
    errno = EINVAL;
    return -1;
  }


  /* now, update our list of locks */
  /* clear the lock */
  if (lock.l_type == F_UNLCK) { 
    adf_freelock(adf, i);
    return 0;
  }

  /* attempt to lock the file. */
  if (!adf->adf_excl && fcntl(adf->adf_fd, F_SETLK, &lock) < 0) 
    return -1;

  /* we upgraded this lock. */
  if (adflock && (type & ADLOCK_UPGRADE)) {
    memcpy(&adflock->lock, &lock, sizeof(lock));
    return 0;
  } 

  /* it wasn't an upgrade */
  oldlock = -1;
  if (lock.l_type == F_RDLCK) {
    oldlock = adf_findxlock(adf, user, ADLOCK_RD, lock.l_start, lock.l_len);
  } 
    
  /* no more space. this will also happen if lockmax == lockcount == 0 */
  if (adf->adf_lockmax == adf->adf_lockcount) { 
    adf_lock_t *tmp = (adf_lock_t *) 
	    realloc(adf->adf_lock, sizeof(adf_lock_t)*
		    (adf->adf_lockmax + ARRAY_BLOCK_SIZE));
    if (!tmp)
      goto fcntl_lock_err;
    adf->adf_lock = tmp;
    adf->adf_lockmax += ARRAY_BLOCK_SIZE;
  } 
  adflock = adf->adf_lock + adf->adf_lockcount;

  /* fill in fields */
  memcpy(&adflock->lock, &lock, sizeof(lock));
  adflock->user = user;
  if (oldlock > -1) {
    adflock->refcount = (adf->adf_lock + oldlock)->refcount;
  } else if ((adflock->refcount = calloc(1, sizeof(int))) == NULL) {
    goto fcntl_lock_err;
  }
  
  (*adflock->refcount)++;
  adf->adf_lockcount++;
  return 0;

fcntl_lock_err:
  lock.l_type = F_UNLCK;
  if (!adf->adf_excl) fcntl(adf->adf_fd, F_SETLK, &lock);
  return -1;
}

/* -------------------------
   we are using lock as tristate variable
   
   we have a lock ==> 1
   no             ==> 0
   error          ==> -1
      
*/
int ad_testlock(struct adouble *ad, int eid, const off_t off)
{
  struct flock lock;
  struct ad_fd *adf;
  adf_lock_t *plock;
  int i;

  lock.l_start = off;
  if (eid == ADEID_DFORK) {
    adf = &ad->ad_df;
    if ((ad_hfileno(ad) != -1)) {
      	adf = &ad->ad_hf;
    	lock.l_start = df2off(off);
    }
  } 
  else { /* rfork */
    if ((ad_hfileno(ad) == -1)) {
        /* there's no resource fork. return no lock */
        return 0;
    }
    adf = &ad->ad_hf;
    lock.l_start = hf2off(off);
  }

  plock = adf->adf_lock;
  /* Does we have a lock? */
  lock.l_whence = SEEK_SET;
  lock.l_len = 1;
  for (i = 0; i < adf->adf_lockcount; i++) {
    if (OVERLAP(lock.l_start, 1, plock[i].lock.l_start, plock[i].lock.l_len)) 
        return 1;   /* */
  }
  /* Does another process have a lock? 
     FIXME F_GETLK ?
  */
  lock.l_type = (adf->adf_flags & O_RDWR) ?F_WRLCK : F_RDLCK;                                           

  if (fcntl(adf->adf_fd, F_SETLK, &lock) < 0) {
    return (errno == EACCES || errno == EAGAIN)?1:-1;
  }
  
  lock.l_type = F_UNLCK;
  return fcntl(adf->adf_fd, F_SETLK, &lock);
}

/* -------------------------
*/
int ad_fcntl_tmplock(struct adouble *ad, const u_int32_t eid, const int locktype,
	             const off_t off, const off_t len, const int user)
{
  struct flock lock;
  struct ad_fd *adf;
  int err;
  int type;  

  lock.l_start = off;
  type = locktype;
  if (eid == ADEID_DFORK) {
    adf = &ad->ad_df;
  } else {
    adf = &ad->ad_hf;
    if (adf->adf_fd == -1) {
        /* there's no resource fork. return success */
        return 0;
    }
    /* if ADLOCK_FILELOCK we want a lock from offset 0
     * it's used when deleting a file:
     * in open we put read locks on meta datas
     * in delete a write locks on the whole file
     * so if the file is open by somebody else it fails
    */
    if (!(type & ADLOCK_FILELOCK))
        lock.l_start += ad_getentryoff(ad, eid);
  }

  if (!(adf->adf_flags & O_RDWR) && (type & ADLOCK_WR)) {
      type = (type & ~ADLOCK_WR) | ADLOCK_RD;
  }
  
  lock.l_type = XLATE_FCNTL_LOCK(type & ADLOCK_MASK);
  lock.l_whence = SEEK_SET;
  lock.l_len = len;

  /* see if it's locked by another user. */
  if (user && adf_findxlock(adf, user, ADLOCK_WR | 
		    ((type & ADLOCK_WR) ? ADLOCK_RD : 0), 
		    lock.l_start, lock.l_len) > -1) {
    errno = EACCES;
    return -1;
  }

  /* okay, we might have ranges byte-locked. we need to make sure that
   * we restore the appropriate ranges once we're done. so, we check
   * for overlap on an unlock and relock. 
   * XXX: in the future, all the byte locks will be sorted and contiguous.
   *      we just want to upgrade all the locks and then downgrade them
   *      here. */
  if (!adf->adf_excl) {
       err = fcntl(adf->adf_fd, F_SETLK, &lock);
  }
  else {
      err = 0;
  }
  if (!err && (lock.l_type == F_UNLCK))
    adf_relockrange(adf, adf->adf_fd, lock.l_start, len);

  return err;
}

/* -------------------------
   the fork is opened in Read Write, Deny Read, Deny Write mode
   lock the whole file once   
*/
int ad_excl_lock(struct adouble *ad, const u_int32_t eid)
{
  struct ad_fd *adf;
  struct flock lock;
  int    err;
  
  lock.l_start = 0;
  lock.l_type = F_WRLCK;
  lock.l_whence = SEEK_SET;
  lock.l_len = 0;

  if (eid == ADEID_DFORK) {
    adf = &ad->ad_df;
  } else {
    adf = &ad->ad_hf;
    lock.l_start = ad_getentryoff(ad, eid);
  }
  
  err = fcntl(adf->adf_fd, F_SETLK, &lock);
  if (!err)
      adf->adf_excl = 1;
  return err;
}

/* --------------------- */
void ad_fcntl_unlock(struct adouble *ad, const int user)
{
  if (ad->ad_df.adf_fd != -1) {
    adf_unlock(&ad->ad_df, user);
  }
  if (ad->ad_hf.adf_fd != -1) {
    adf_unlock(&ad->ad_hf, user);
  }
}
