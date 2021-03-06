/*
 * "$Id: scsi.c,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $"
 *
 *   SCSI printer backend for the Common UNIX Printing System (CUPS).
 *
 *   Copyright 2003-2005 by Easy Software Products, all rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the
 *   following conditions are met:
 *
 *     1. Redistributions of source code must retain the above
 *	  copyright notice, this list of conditions and the
 *	  following disclaimer.
 *
 *     2. Redistributions in binary form must reproduce the
 *	  above copyright notice, this list of conditions and
 *	  the following disclaimer in the documentation and/or
 *	  other materials provided with the distribution.
 *
 *     3. All advertising materials mentioning features or use
 *	  of this software must display the following
 *	  acknowledgement:
 *
 *	    This product includes software developed by Easy
 *	    Software Products.
 *
 *     4. The name of Easy Software Products may not be used to
 *	  endorse or promote products derived from this software
 *	  without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS
 *   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 *   BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS
 *   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 *   DAMAGE.
 *
 * Contents:
 *
 *   main() - Send a file to the specified SCSI printer.
 */

/*
 * Include necessary headers.
 */

#include <cups/cups.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <cups/string.h>
#include <signal.h>

#ifdef WIN32
#  include <io.h>
#else
#  include <unistd.h>
#  include <fcntl.h>
#  ifdef HAVE_SYS_IOCTL_H
#    include <sys/ioctl.h>
#  endif /* HAVE_SYS_IOCTL_H */
#endif /* WIN32 */


/*
 * Local functions...
 */

void	list_devices(void);
int	print_device(const char *resource, int fd, int copies);


#ifdef __linux__
#  include "scsi-linux.c"
#elif defined(__sgi)
#  include "scsi-irix.c"
#else
/*
 * Dummy functions that do nothing on unsupported platforms...
 */
void	list_devices(void) {}
int	print_device(const char *resource, int fd, int copies) { return (1); }
#endif /* __linux */


/*
 * 'main()' - Send a file to the specified SCSI printer.
 *
 * Usage:
 *
 *    printer-uri job-id user title copies options [file]
 */

int			/* O - Exit status */
main(int  argc,		/* I - Number of command-line arguments (6 or 7) */
     char *argv[])	/* I - Command-line arguments */
{
  char		method[255],	/* Method in URI */
		hostname[1024],	/* Hostname */
		username[255],	/* Username info (not used) */
		resource[1024],	/* Resource info (device and options) */
		*options;	/* Pointer to options */
  int		port;		/* Port number (not used) */
  int		fp;		/* Print file */
  int		copies;		/* Number of copies to print */
  int		status;		/* Exit status */
#if defined(HAVE_SIGACTION) && !defined(HAVE_SIGSET)
  struct sigaction action;	/* Actions for POSIX signals */
#endif /* HAVE_SIGACTION && !HAVE_SIGSET */


 /*
  * Make sure status messages are not buffered...
  */

  setbuf(stderr, NULL);

 /*
  * Ignore SIGPIPE signals...
  */

#ifdef HAVE_SIGSET
  sigset(SIGPIPE, SIG_IGN);
#elif defined(HAVE_SIGACTION)
  memset(&action, 0, sizeof(action));
  action.sa_handler = SIG_IGN;
  sigaction(SIGPIPE, &action, NULL);
#else
  signal(SIGPIPE, SIG_IGN);
#endif /* HAVE_SIGSET */

 /*
  * Check command-line...
  */

  if (argc == 1)
  {
    list_devices();
    return (0);
  }
  else if (argc < 6 || argc > 7)
  {
    fputs("Usage: scsi:/dev/file job-id user title copies options [file]\n", stderr);
    return (1);
  }

 /*
  * If we have 7 arguments, print the file named on the command-line.
  * Otherwise, send stdin instead...
  */

  if (argc == 6)
  {
    fp     = 0;
    copies = 1;
  }
  else
  {
   /*
    * Try to open the print file...
    */

    if ((fp = open(argv[6], O_RDONLY)) < 0)
    {
      perror("ERROR: unable to open print file");
      return (1);
    }

    copies = atoi(argv[4]);
  }

 /*
  * Extract the device name and options from the URI...
  */

  httpSeparate(argv[0], method, username, hostname, &port, resource);

 /*
  * See if there are any options...
  */

  if ((options = strchr(resource, '?')) != NULL)
  {
   /*
    * Yup, terminate the device name string and move to the first
    * character of the options...
    */

    *options++ = '\0';
  }

 /*
  * Finally, send the print file...
  */

  status = print_device(resource, fp, copies);

 /*
  * Close input file and return...
  */

  if (fp != 0)
    close(fp);

  return (status);
}


/*
 * End of "$Id: scsi.c,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $".
 */
