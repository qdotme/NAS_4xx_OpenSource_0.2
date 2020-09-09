/* revision.h -- define the version number
 * Copyright (C) 1998, 1999, 2001, 2002 Free Software Foundation, Inc.
 * Copyright (C) 1992-1993 Jean-loup Gailly.
 * This is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License, see the file COPYING.
 */

#define PATCHLEVEL 0
#define REVDATE "2002-09-30"

/* This version does not support compression into old compress format: */
#ifdef LZW
#  undef LZW
#endif

/* $Id: revision.h,v 1.1.1.1 2008/06/25 12:13:03 jason Exp $ */
