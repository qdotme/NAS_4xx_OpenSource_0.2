/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1997-2003
 *	Sleepycat Software.  All rights reserved.
 */

#include "db_config.h"

#ifndef lint
static const char revid[] = "$Id: os_sleep.c,v 1.1.1.1 2008/06/18 10:53:15 jason Exp $";
#endif /* not lint */

#ifndef NO_SYSTEM_INCLUDES
#include <sys/types.h>

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#ifdef HAVE_VXWORKS
#include <sys/times.h>
#include <time.h>
#include <selectLib.h>
#else
#if TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH SYS_TIME */
#endif /* HAVE_VXWORKS */

#include <string.h>
#include <unistd.h>
#endif

#include "db_int.h"

/*
 * __os_sleep --
 *	Yield the processor for a period of time.
 *
 * PUBLIC: int __os_sleep __P((DB_ENV *, u_long, u_long));
 */
int
__os_sleep(dbenv, secs, usecs)
	DB_ENV *dbenv;
	u_long secs, usecs;		/* Seconds and microseconds. */
{
	struct timeval t;
	int ret;

	/* Don't require that the values be normalized. */
	for (; usecs >= 1000000; usecs -= 1000000)
		++secs;

	if (DB_GLOBAL(j_sleep) != NULL)
		return (DB_GLOBAL(j_sleep)(secs, usecs));

	/*
	 * It's important that we yield the processor here so that other
	 * processes or threads are permitted to run.
	 *
	 * Sheer raving paranoia -- don't select for 0 time.
	 */
	t.tv_sec = secs;
	if (secs == 0 && usecs == 0)
		t.tv_usec = 1;
	else
		t.tv_usec = usecs;

	/*
	 * We don't catch interrupts and restart the system call here, unlike
	 * other Berkeley DB system calls.  This may be a user attempting to
	 * interrupt a sleeping DB utility (for example, db_checkpoint), and
	 * we want the utility to see the signal and quit.  This assumes it's
	 * always OK for DB to sleep for less time than originally scheduled.
	 */
	if ((ret = select(0, NULL, NULL, NULL, &t)) != 0)
		if ((ret = __os_get_errno()) == EINTR)
			ret = 0;

	if (ret != 0)
		__db_err(dbenv, "select: %s", strerror(ret));

	return (ret);
}
