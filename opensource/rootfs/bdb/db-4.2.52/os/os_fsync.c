/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1997-2003
 *	Sleepycat Software.  All rights reserved.
 */

#include "db_config.h"

#ifndef lint
static const char revid[] = "$Id: os_fsync.c,v 1.1.1.1 2008/06/18 10:53:15 jason Exp $";
#endif /* not lint */

#ifndef NO_SYSTEM_INCLUDES
#include <sys/types.h>

#include <fcntl.h>			/* XXX: Required by __hp3000s900 */
#include <unistd.h>
#include <string.h>
#endif

#include "db_int.h"

#ifdef	HAVE_VXWORKS
#include "ioLib.h"

#define	fsync(fd)	__vx_fsync(fd);

int
__vx_fsync(fd)
	int fd;
{
	int ret;

	/*
	 * The results of ioctl are driver dependent.  Some will return the
	 * number of bytes sync'ed.  Only if it returns 'ERROR' should we
	 * flag it.
	 */
	if ((ret = ioctl(fd, FIOSYNC, 0)) != ERROR)
		return (0);
	return (ret);
}
#endif

#ifdef __hp3000s900
#define	fsync(fd)	__mpe_fsync(fd);

int
__mpe_fsync(fd)
	int fd;
{
	extern FCONTROL(short, short, void *);

	FCONTROL(_MPE_FILENO(fd), 2, NULL);	/* Flush the buffers */
	FCONTROL(_MPE_FILENO(fd), 6, NULL);	/* Write the EOF */
	return (0);
}
#endif

/*
 * __os_fsync --
 *	Flush a file descriptor.
 *
 * PUBLIC: int __os_fsync __P((DB_ENV *, DB_FH *));
 */
int
__os_fsync(dbenv, fhp)
	DB_ENV *dbenv;
	DB_FH *fhp;
{
	int ret, retries;

	/*
	 * Do nothing if the file descriptor has been marked as not requiring
	 * any sync to disk.
	 */
	if (F_ISSET(fhp, DB_FH_NOSYNC))
		return (0);

	/* Check for illegal usage. */
	DB_ASSERT(F_ISSET(fhp, DB_FH_OPENED) && fhp->fd != -1);

	retries = 0;
	do {
		ret = DB_GLOBAL(j_fsync) != NULL ?
		    DB_GLOBAL(j_fsync)(fhp->fd) : fsync(fhp->fd);
	} while (ret != 0 &&
	    ((ret = __os_get_errno()) == EINTR || ret == EBUSY) &&
	    ++retries < DB_RETRY);

	if (ret != 0)
		__db_err(dbenv, "fsync %s", strerror(ret));
	return (ret);
}
