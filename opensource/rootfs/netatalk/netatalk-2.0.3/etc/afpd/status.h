/*
 * $Id: status.h,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 */

#ifndef AFPD_STATUS_H
#define AFPD_STATUS_H 1

#include <sys/cdefs.h>
#include <atalk/dsi.h>
#include <atalk/asp.h>
#include "globals.h"
#include "afp_config.h"

/* we use these to prevent whacky alignment problems */
#define AFPSTATUS_MACHOFF     0
#define AFPSTATUS_VERSOFF     2
#define AFPSTATUS_UAMSOFF     4
#define AFPSTATUS_ICONOFF     6
#define AFPSTATUS_FLAGOFF     8
/* AFPSTATUS_PRELEN is the number of bytes for status data prior to 
 * the ServerName field.
 *
 * This is two bytes of offset space for the MachineType, AFPVersionCount,
 * UAMCount, VolumeIconAndMask, and the 16-bit "Fixed" status flags.
 */
#define AFPSTATUS_PRELEN     10
/* AFPSTATUS_POSTLEN is the number of bytes for offset records
 * after the ServerName field.
 *
 * Right now, this is 2 bytes each for ServerSignature, networkAddressCount,
 * DirectoryNameCount, and UTF-8 ServerName
 */
#define AFPSTATUS_POSTLEN     8
#define AFPSTATUS_LEN        (AFPSTATUS_PRELEN + AFPSTATUS_POSTLEN)


#define PASSWD_NONE     0
#define PASSWD_SET     (1 << 0)
#define PASSWD_NOSAVE  (1 << 1)
#define PASSWD_ALL     (PASSWD_SET | PASSWD_NOSAVE)

extern void status_versions __P((char * /*status*/));
extern void status_uams __P((char * /*status*/, const char * /*authlist*/));
extern void status_reset __P((void ));
extern void status_init __P((AFPConfig *, AFPConfig *,
                                 const struct afp_options *));
extern int      afp_getsrvrinfo __P((AFPObj *, char *, int, char *, int *));

#endif
