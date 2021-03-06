/*
 * "$Id: tempfile.c,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $"
 *
 *   Temp file utilities for the Common UNIX Printing System (CUPS).
 *
 *   Copyright 1997-2005 by Easy Software Products.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Easy Software Products and are protected by Federal
 *   copyright law.  Distribution and use rights are outlined in the file
 *   "LICENSE.txt" which should have been included with this file.  If this
 *   file is missing or damaged please contact Easy Software Products
 *   at:
 *
 *       Attn: CUPS Licensing Information
 *       Easy Software Products
 *       44141 Airport View Drive, Suite 204
 *       Hollywood, Maryland 20636 USA
 *
 *       Voice: (301) 373-9600
 *       EMail: cups-info@cups.org
 *         WWW: http://www.cups.org
 *
 *   This file is subject to the Apple OS-Developed Software exception.
 *
 * Contents:
 *
 *   cupsTempFd()   - Create a temporary file.
 *   cupsTempFile() - Generate a temporary filename.
 */

/*
 * Include necessary headers...
 */

#include "cups.h"
#include "string.h"
#include "debug.h"
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#if defined(WIN32) || defined(__EMX__)
#  include <io.h>
#else
#  include <unistd.h>
#endif /* WIN32 || __EMX__ */


/*
 * 'cupsTempFd()' - Create a temporary file.
 */

int					/* O - New file descriptor */
cupsTempFd(char *filename,		/* I - Pointer to buffer */
           int  len)			/* I - Size of buffer */
{
  int		fd;			/* File descriptor for temp file */
  int		tries;			/* Number of tries */
  const char	*tmpdir;		/* TMPDIR environment var */
#ifdef WIN32
  char		tmppath[1024];		/* Windows temporary directory */
  DWORD		curtime;		/* Current time */
#else
  struct timeval curtime;		/* Current time */
#endif /* WIN32 */
  static char	*buf = NULL;		/* Buffer if you pass in NULL and 0 */


 /*
  * See if a filename was specified...
  */

  if (filename == NULL)
  {
    if (buf == NULL)
      buf = calloc(1024, sizeof(char));

    if (buf == NULL)
      return (-1);

    filename = buf;
    len      = 1024;
  }

 /*
  * See if TMPDIR is defined...
  */

#ifdef WIN32
  if ((tmpdir = getenv("TEMP")) == NULL)
  {
    GetTempPath(sizeof(tmppath), tmppath);
    tmpdir = tmppath;
  }
#else
  if ((tmpdir = getenv("TMPDIR")) == NULL)
  {
   /*
    * Put root temp files in restricted temp directory...
    */

    if (getuid() == 0)
      tmpdir = CUPS_REQUESTS "/tmp";
    else
      tmpdir = "/var/tmp";
  }
#endif /* WIN32 */

 /*
  * Make the temporary name using the specified directory...
  */

  tries = 0;

  do
  {
#ifdef WIN32
   /*
    * Get the current time of day...
    */

    curtime =  GetTickCount() + tries;

   /*
    * Format a string using the hex time values...
    */

    snprintf(filename, len - 1, "%s/%05lx%08lx", tmpdir,
             GetCurrentProcessId(), curtime);
#else
   /*
    * Get the current time of day...
    */

    gettimeofday(&curtime, NULL);

   /*
    * Format a string using the hex time values...
    */

    snprintf(filename, len - 1, "%s/%08lx%05lx", tmpdir,
             (unsigned long)curtime.tv_sec, (unsigned long)curtime.tv_usec);
#endif /* WIN32 */

   /*
    * Open the file in "exclusive" mode, making sure that we don't
    * stomp on an existing file or someone's symlink crack...
    */

#ifdef WIN32
    fd = open(filename, _O_CREAT | _O_RDWR | _O_TRUNC | _O_BINARY,
              _S_IREAD | _S_IWRITE);
#elif defined(O_NOFOLLOW)
    fd = open(filename, O_RDWR | O_CREAT | O_EXCL | O_NOFOLLOW, 0600);
#else
    fd = open(filename, O_RDWR | O_CREAT | O_EXCL, 0600);
#endif /* WIN32 */

    if (fd < 0 && errno != EEXIST)
      break;

    tries ++;
  }
  while (fd < 0 && tries < 1000);

 /*
  * Return the file descriptor...
  */

  return (fd);
}


/*
 * 'cupsTempFile()' - Generate a temporary filename.
 */

char *					/* O - Filename */
cupsTempFile(char *filename,		/* I - Pointer to buffer */
             int  len)			/* I - Size of buffer */
{
  int		fd;			/* File descriptor for temp file */
  static char	buf[1024] = "";		/* Buffer if you pass in NULL and 0 */


 /*
  * See if a filename was specified...
  */

  if (filename == NULL)
  {
    filename = buf;
    len      = sizeof(buf);
  }

 /*
  * Create the temporary file...
  */

  if ((fd = cupsTempFd(filename, len)) < 0)
    return (NULL);

 /*
  * Close the temp file - it'll be reopened later as needed...
  */

  close(fd);

 /*
  * Return the temp filename...
  */

  return (filename);
}


/*
 * End of "$Id: tempfile.c,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $".
 */
