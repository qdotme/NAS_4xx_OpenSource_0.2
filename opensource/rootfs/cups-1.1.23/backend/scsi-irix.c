/*
 * "$Id: scsi-irix.c,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $"
 *
 *   IRIX SCSI printer support for the Common UNIX Printing System (CUPS).
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
 *   list_devices() - List the available SCSI printer devices.
 *   print_device() - Print a file to a SCSI device.
 */

/*
 * Include necessary headers.
 */

#include <bstring.h>		/* memcpy() and friends */
#include <sys/dsreq.h>		/* SCSI interface stuff */


/*
 * 'list_devices()' - List the available SCSI printer devices.
 */

void
list_devices(void)
{
  puts("direct scsi \"Unknown\" \"SCSI Printer\"");
}


/*
 * 'print_device()' - Print a file to a SCSI device.
 */

int					/* O - Print status */
print_device(const char *resource,	/* I - SCSI device */
             int        fd,		/* I - File to print */
	     int        copies)		/* I - Number of copies to print */
{
  int		scsi_fd;		/* SCSI file descriptor */
  char		buffer[8192];		/* Data buffer */
  int		bytes;			/* Number of bytes */
  int		try;			/* Current try */
  dsreq_t	scsi_req;		/* SCSI request */
  char		scsi_cmd[6];		/* SCSI command data */
#if defined(HAVE_SIGACTION) && !defined(HAVE_SIGSET)
  struct sigaction action;		/* Actions for POSIX signals */
#endif /* HAVE_SIGACTION && !HAVE_SIGSET */


 /*
  * Make sure we have a valid resource name...
  */

  if (strncmp(resource, "/dev/scsi/", 10) != 0)
  {
    fprintf(stderr, "ERROR: Bad SCSI device file \"%s\"!\n", resource);
    return (1);
  }

 /*
  * Open the SCSI device file...
  */

  do
  {
    if ((scsi_fd = open(resource, O_RDWR | O_EXCL)) == -1)
    {
      if (errno != EAGAIN && errno != EBUSY)
      {
	fprintf(stderr, "ERROR: Unable to open SCSI device \"%s\" - %s\n",
        	resource, strerror(errno));
	return (1);
      }
      else
      {
        fprintf(stderr, "INFO: SCSI device \"%s\" busy; retrying...\n",
	        resource);
        sleep(30);
      }
    }
  }
  while (scsi_fd == -1);

 /*
  * Now that we are "connected" to the port, ignore SIGTERM so that we
  * can finish out any page data the driver sends (e.g. to eject the
  * current page...  Only ignore SIGTERM if we are printing data from
  * stdin (otherwise you can't cancel raw jobs...)
  */

  if (fd != 0)
  {
#ifdef HAVE_SIGSET /* Use System V signals over POSIX to avoid bugs */
    sigset(SIGTERM, SIG_IGN);
#elif defined(HAVE_SIGACTION)
    memset(&action, 0, sizeof(action));

    sigemptyset(&action.sa_mask);
    action.sa_handler = SIG_IGN;
    sigaction(SIGTERM, &action, NULL);
#else
    signal(SIGTERM, SIG_IGN);
#endif /* HAVE_SIGSET */
  }

 /*
  * Copy the print file to the device...
  */

  while (copies > 0)
  {
    if (fd != 0)
      lseek(fd, 0, SEEK_SET);

    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
    {
      memset(&scsi_req, 0, sizeof(scsi_req));

      scsi_req.ds_flags   = DSRQ_WRITE;
      scsi_req.ds_time    = 60 * 1000;
      scsi_req.ds_cmdbuf  = scsi_cmd;
      scsi_req.ds_cmdlen  = 6;
      scsi_req.ds_databuf = buffer;
      scsi_req.ds_datalen = bytes;

      scsi_cmd[0] = 0x0a;	/* Group 0 print command */
      scsi_cmd[1] = 0x00;
      scsi_cmd[2] = bytes / 65536;
      scsi_cmd[3] = bytes / 256;
      scsi_cmd[4] = bytes;
      scsi_cmd[5] = 0x00;

      for (try = 0; try < 10; try ++)
	if (ioctl(scsi_fd, DS_ENTER, &scsi_req) < 0 ||
            scsi_req.ds_status != 0)
        {
	  fprintf(stderr, "WARNING: SCSI command timed out (%d); retrying...\n",
	          scsi_req.ds_status);
          sleep(try + 1);
	}
	else
          break;

      if (try >= 10)
      {
	fprintf(stderr, "ERROR: Unable to send print data (%d)\n",
	        scsi_req.ds_status);
        close(scsi_fd);
	return (1);
      }
    }

    copies --;
  }

 /*
  * Close the device and return...
  */

  close(fd);

  return (0);
}


/*
 * End of "$Id: scsi-irix.c,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $".
 */
