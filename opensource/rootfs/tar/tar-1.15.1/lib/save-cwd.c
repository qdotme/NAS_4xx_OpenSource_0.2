/* save-cwd.c -- Save and restore current working directory.
   Copyright (C) 1995, 1997, 1998, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* Written by Jim Meyering.  */

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#if HAVE_FCNTL_H
# include <fcntl.h>
#else
# include <sys/file.h>
#endif

#include <errno.h>

#ifndef O_DIRECTORY
# define O_DIRECTORY 0
#endif

#include "save-cwd.h"
#include "xgetcwd.h"

/* Record the location of the current working directory in CWD so that
   the program may change to other directories and later use restore_cwd
   to return to the recorded location.  This function may allocate
   space using malloc (via xgetcwd) or leave a file descriptor open;
   use free_cwd to perform the necessary free or close.  Upon failure,
   no memory is allocated, any locally opened file descriptors are
   closed;  return non-zero -- in that case, free_cwd need not be
   called, but doing so is ok.  Otherwise, return zero.

   The `raison d'etre' for this interface is that some systems lack
   support for fchdir, and getcwd is not robust or as efficient.
   So, we prefer to use the open/fchdir approach, but fall back on
   getcwd if necessary.  Some systems lack fchdir altogether: OS/2,
   Cygwin (as of March 2003), SCO Xenix.  At least SunOS 4 and Irix 5.3
   provide the function, yet it doesn't work for partitions on which
   auditing is enabled.  */

int
save_cwd (struct saved_cwd *cwd)
{
  static bool have_working_fchdir = true;

  cwd->desc = -1;
  cwd->name = NULL;

  if (have_working_fchdir)
    {
#if HAVE_FCHDIR
      cwd->desc = open (".", O_RDONLY | O_DIRECTORY);
      if (cwd->desc < 0)
	cwd->desc = open (".", O_WRONLY | O_DIRECTORY);
      if (cwd->desc < 0)
	{
	  cwd->name = xgetcwd ();
	  return cwd->name ? 0 : -1;
	}

# if __sun__ || sun
      /* On SunOS 4 and IRIX 5.3, fchdir returns EINVAL when auditing
	 is enabled, so we have to fall back to chdir.  */
      if (fchdir (cwd->desc))
	{
	  if (errno == EINVAL)
	    {
	      close (cwd->desc);
	      cwd->desc = -1;
	      have_working_fchdir = false;
	    }
	  else
	    {
	      int saved_errno = errno;
	      close (cwd->desc);
	      cwd->desc = -1;
	      errno = saved_errno;
	      return -1;
	    }
	}
# endif /* __sun__ || sun */
#else
# define fchdir(x) (abort (), 0)
      have_working_fchdir = false;
#endif
    }

  if (!have_working_fchdir)
    {
      cwd->name = xgetcwd ();
      if (cwd->name == NULL)
	return -1;
    }
  return 0;
}

/* Change to recorded location, CWD, in directory hierarchy.
   Upon failure, return -1 (errno is set by chdir or fchdir).
   Upon success, return zero.  */

int
restore_cwd (const struct saved_cwd *cwd)
{
  if (0 <= cwd->desc)
    return fchdir (cwd->desc);
  else
    return chdir (cwd->name);
}

void
free_cwd (struct saved_cwd *cwd)
{
  if (cwd->desc >= 0)
    close (cwd->desc);
  if (cwd->name)
    free (cwd->name);
}
