/*
 * $Id: cdefs.h,v 1.1.1.1 2008/06/18 10:55:40 jason Exp $
 */

#ifndef _SYS_CDEFS_H
#define _SYS_CDEFS_H 1

#if defined(__STDC__) || defined(__DECC)
/* Note that there must be exactly one space between __P(args) and args,
 * otherwise DEC C chokes.
 */
#define __P(args) args
#else /* __STDC__ */
#define __P(args)    ()
#endif /* __STDC__ */

#endif /* sys/cdefs.h */
