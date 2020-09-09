/*
 * smartctl.cpp
 *
 * Home page of code is: http://smartmontools.sourceforge.net
 *
 * Copyright (C) 2002-8 Bruce Allen <smartmontools-support@lists.sourceforge.net>
 * Copyright (C) 2000 Michael Cornwell <cornwell@acm.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * You should have received a copy of the GNU General Public License
 * (for example COPYING); if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * This code was originally developed as a Senior Thesis by Michael Cornwell
 * at the Concurrent Systems Laboratory (now part of the Storage Systems
 * Research Center), Jack Baskin School of Engineering, University of
 * California, Santa Cruz. http://ssrc.soe.ucsc.edu/
 *
 */

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdarg.h>

#include "config.h"
#ifdef HAVE_GETOPT_LONG
#include <getopt.h>
#endif
#if defined(__FreeBSD_version) && (__FreeBSD_version < 500000)
#include <unistd.h>
#endif

#if defined(__QNXNTO__) 
#include <unistd.h>
#endif


#include "int64.h"
#include "atacmds.h"
#include "ataprint.h"
#include "extern.h"
#include "knowndrives.h"
#include "scsicmds.h"
#include "scsiprint.h"
#include "smartctl.h"
#include "utility.h"

#ifdef NEED_SOLARIS_ATA_CODE
extern const char *os_solaris_ata_s_cvsid;
#endif
extern const char *atacmdnames_c_cvsid, *atacmds_c_cvsid, *ataprint_c_cvsid, *knowndrives_c_cvsid, *os_XXXX_c_cvsid, *scsicmds_c_cvsid, *scsiprint_c_cvsid, *utility_c_cvsid;
const char* smartctl_c_cvsid="$Id: smartmontools-5.38-01-add_sendidle_command.patch,v 1.1.2.2 2009/02/04 02:04:33 nick Exp $"
ATACMDS_H_CVSID ATAPRINT_H_CVSID CONFIG_H_CVSID EXTERN_H_CVSID INT64_H_CVSID KNOWNDRIVES_H_CVSID SCSICMDS_H_CVSID SCSIPRINT_H_CVSID SMARTCTL_H_CVSID UTILITY_H_CVSID;

// This is a block containing all the "control variables".  We declare
// this globally in this file, and externally in other files.
smartmonctrl *con=NULL;

// to hold onto exit code for atexit routine
extern int exitstatus;

// Track memory use
extern int64_t bytes;

void printslogan(){
#ifdef HAVE_GET_OS_VERSION_STR
  const char * ver = get_os_version_str();
#else
  const char * ver = SMARTMONTOOLS_BUILD_HOST;
#endif
  pout("smartctl version %s [%s] Copyright (C) 2002-8 Bruce Allen\n", PACKAGE_VERSION, ver);
  pout("Home page is " PACKAGE_HOMEPAGE "\n\n");
  return;
}

void PrintOneCVS(const char *a_cvs_id){
  char out[CVSMAXLEN];
  printone(out,a_cvs_id);
  pout("%s",out);
  return;
}

void printcopy(){
  const char *configargs=strlen(SMARTMONTOOLS_CONFIGURE_ARGS)?SMARTMONTOOLS_CONFIGURE_ARGS:"[no arguments given]";

  pout("smartctl comes with ABSOLUTELY NO WARRANTY. This\n");
  pout("is free software, and you are welcome to redistribute it\n");
  pout("under the terms of the GNU General Public License Version 2.\n");
  pout("See http://www.gnu.org for further details.\n\n");
  pout("CVS version IDs of files used to build this code are:\n");
  PrintOneCVS(atacmdnames_c_cvsid);
  PrintOneCVS(atacmds_c_cvsid);
  PrintOneCVS(ataprint_c_cvsid);
  PrintOneCVS(knowndrives_c_cvsid);
  PrintOneCVS(os_XXXX_c_cvsid);
#ifdef NEED_SOLARIS_ATA_CODE
  PrintOneCVS(os_solaris_ata_s_cvsid);
#endif
  PrintOneCVS(scsicmds_c_cvsid);
  PrintOneCVS(scsiprint_c_cvsid);
  PrintOneCVS(smartctl_c_cvsid);
  PrintOneCVS(utility_c_cvsid);
  pout("\nsmartmontools release " PACKAGE_VERSION " dated " SMARTMONTOOLS_RELEASE_DATE " at " SMARTMONTOOLS_RELEASE_TIME "\n");
  pout("smartmontools build host: " SMARTMONTOOLS_BUILD_HOST "\n");
  pout("smartmontools build configured: " SMARTMONTOOLS_CONFIGURE_DATE "\n");
  pout("smartctl compile dated " __DATE__ " at "__TIME__ "\n");
  pout("smartmontools configure arguments: %s\n", configargs);
  return;
}

void UsageSummary(){
  pout("\nUse smartctl -h to get a usage summary\n\n");
  return;
}

/*  void prints help information for command syntax */
/*  void prints help information for command syntax */
void Usage (void){
  printf("Usage: sendidle [options] device\n\n");
  printf("============================================ SHOW INFORMATION OPTIONS =====\n\n");
#ifdef HAVE_GETOPT_LONG
  printf(
"  -h, -?, --help, --usage\n"
"         Display this help and exit\n\n"
"  -V, --version\n"
"         Print version information and exit\n\n"
"  -i, --immediate                                                       \n"
"         Send IDLE_IMMEDITATE command for device\n\n"
"  -s, --standby                                                       \n"
"         Send STANDBY command for device\n\n"
"  -t VALUE, --timer VALUE                                                        \n"
"         The standby timer value\n\n"
  );
#else
  printf(
"  -h, -?    Display this help and exit\n"
"  -V        Print version information\n"
"  -i        Send IDLE_IMMEDITATE command for device\n"
"  -s        Send STANDBY command for device\n"
"  -t VALUE  The standby timer value\n\n"
  );
#endif
}

/* Returns a pointer to a static string containing a formatted list of the valid
   arguments to the option opt or NULL on failure. Note 'v' case different */
const char *getvalidarglist(char opt) {
  switch (opt) {
  case 'q':
    return "errorsonly, silent, noserial";
  case 'd':
    return "ata, scsi, marvell, sat, 3ware,N, hpt,L/M/N cciss,N";
  case 'T':
    return "normal, conservative, permissive, verypermissive";
  case 'b':
    return "warn, exit, ignore";
  case 'r':
    return "ioctl[,N], ataioctl[,N], scsiioctl[,N]";
  case 's':
  case 'o':
  case 'S':
    return "on, off";
  case 'l':
    return "error, selftest, selective, directory, background, scttemp[sts|hist]";
  case 'P':
    return "use, ignore, show, showall";
  case 't':
    return "offline, short, long, conveyance, select,M-N, pending,N, afterselect,[on|off], scttempint,N[,p]";
  case 'F':
    return "none, samsung, samsung2, samsung3, swapid";
  case 'n':
    return "never, sleep, standby, idle";
  case 'v':
  default:
    return NULL;
  }
}

/* Prints the message "=======> VALID ARGUMENTS ARE: <LIST> \n", where
   <LIST> is the list of valid arguments for option opt. */
void printvalidarglistmessage(char opt) {
  char *s;
  
  if (opt=='v')
    s=create_vendor_attribute_arg_list();
  else
    s=(char *)getvalidarglist(opt);
  
  if (!s) {
    pout("Error whilst constructing argument list for option %c", opt);
    return;
  }
 
  if (opt=='v'){
    pout("=======> VALID ARGUMENTS ARE:\n\thelp\n%s\n<=======\n", s);
    free(s);
  }
  else {
  // getvalidarglist() might produce a multiline or single line string.  We
  // need to figure out which to get the formatting right.
    char separator = strchr(s, '\n') ? '\n' : ' ';
    pout("=======> VALID ARGUMENTS ARE:%c%s%c<=======\n", separator, (char *)s, separator);
  }

  return;
}
unsigned char tryata=0,tryscsi=1;
unsigned char immediate = 0;
unsigned char standby = 0;
unsigned int timer = 0;

unsigned char powermode = 0;    //nick

/*      Takes command options and sets features to be run */
void ParseOpts (int argc, char** argv){
  int optchar;
  int badarg;
  int captive;
  extern char *optarg;
  extern int optopt, optind, opterr;
  // Please update getvalidarglist() if you edit shortopts
//nick  const char *shortopts = "h?V:t:i:s:";
  const char *shortopts = "h?V:t:i:s:pz:";
#ifdef HAVE_GETOPT_LONG
  char *arg;
  // Please update getvalidarglist() if you edit longopts
  struct option longopts[] = {
    { "help",            no_argument,       0, 'h' },
    { "usage",           no_argument,       0, 'h' },
    { "version",         no_argument,       0, 'V' },
    { "device",          required_argument, 0, 'd' },
    { "timer",           required_argument, 0, 't' },
    { "immediate",       no_argument,       0, 'i' },
    { "standby",         no_argument,       0, 's' },
    { "powermode",       no_argument,       0, 'p' }, //nick
    { "type",            no_argument, 	    0, 'z' }, //nick
    { 0,                 0,                 0, 0   }
  };
#endif

  memset(con,0,sizeof(*con));
  con->testcase=-1;
  opterr=optopt=0;
  badarg = captive = FALSE;

  //default is SAT
  con->controller_type = CONTROLLER_SAT;
  con->satpassthrulen = 0;
  // This miserable construction is needed to get emacs to do proper indenting. Sorry!
  while (-1 != (optchar =
#ifdef HAVE_GETOPT_LONG
        getopt_long(argc, argv, shortopts, longopts, NULL)
#else
        getopt(argc, argv, shortopts)
#endif
        )){
    switch (optchar){
    case 'i':
        immediate = 1;
      break;
    case 's':
        standby = 1;
      break;
//nick +
    case 'p':
//        printf(" check power mode\n");
        powermode = 1;
      break;
    case 'z':
//        printf(" MARVELL_SATA\n");
        con->controller_type = CONTROLLER_MARVELL_SATA;
      break;
//nick -
    case 't':
        timer = strtol(optarg, NULL, 0);
        printf(" timer %d\n", timer);
      break;
    case 'h':
    case '?':
    default:
      Usage();
      exit(0);
    } // closes switch statement to process command-line options
    }
}


// Printing function (controlled by global con->dont_print) 
// [From GLIBC Manual: Since the prototype doesn't specify types for
// optional arguments, in a call to a variadic function the default
// argument promotions are performed on the optional argument
// values. This means the objects of type char or short int (whether
// signed or not) are promoted to either int or unsigned int, as
// appropriate.]
void pout(const char *fmt, ...){
  va_list ap;
  
  // initialize variable argument list 
  va_start(ap,fmt);
  if (con->dont_print){
    va_end(ap);
    return;
  }

  // print out
  vprintf(fmt,ap);
  va_end(ap);
  fflush(stdout);
  return;
}
// This function is used by utility.cpp to report LOG_CRIT errors.
// The smartctl version prints to stdout instead of syslog().
void PrintOut(int priority, const char *fmt, ...) {
  va_list ap;

  // avoid warning message about unused variable from gcc -W: just
  // change value of local copy.
  priority=0;

  va_start(ap,fmt);
  vprintf(fmt,ap);
  va_end(ap);
  return;
}
int ataSendIdle (int device ){

  if (smartcommandhandler(device, SEND_IDLE, timer, NULL)){
    syserror("Error Send IDLE command failed");
    return -1;
  }
  return 0;
}
int ataSendIdleImmediate (int device ){

  if (smartcommandhandler(device, SEND_IDLE_IMMEDIATE, 0, NULL)){
    syserror("Error Send IDLE IMMEDIATE command failed");
    return -1;
  }
  return 0;
}

int o_ataCheckPowerMode (int device ){

unsigned char result;

  if (smartcommandhandler(device, CHECK_POWER_MODE, 0, (char *)&result)){
    syserror("Error Queue Power Mode command failed");
    return -1;
  }
  return 0;
}

int ataSendStandby (int device ){

/*  if (smartcommandhandler(device, SEND_STANDBY, timer, NULL)){
    syserror("Error Send STANDBY command failed");
    return -1;
  }*/
  if (smartcommandhandler(device, SEND_STANDBY_IMMEDIATE, 0, NULL)){
    syserror("Error Send STANDBY IMMEDIATE command failed");
    return -1;
  }
  return 0;
}
int ataSendStandbyImmediate (int device ){

  if (smartcommandhandler(device, SEND_STANDBY_IMMEDIATE, 0, NULL)){
    syserror("Error Send STANDBY IMMEDIATE command failed");
    return -1;
  }
  return 0;
}



/* Main Program */
int main (int argc, char **argv){
  int fd, retval=0;
  char *device;
  smartmonctrl control;
  char *mode=NULL;

  // define control block for external functions
  con=&control;
  // Part input arguments
  ParseOpts(argc,argv);

  device = argv[argc-1];
{
  //search for 1st present HDD
  char cmd[50], cmdbuf[10];
  FILE *cmdfp;
  int count;
  int i=0;
  do {
  	sprintf( cmd, "/bin/cat /sys/block/sd%c/device/scsi_level 2>&1", *(device+7)+i);
  	if ( (cmdfp = popen(cmd, "r")) != NULL) {
		count = fread(cmdbuf, sizeof(char), 10, cmdfp);
		pclose(cmdfp);
		if ( cmdbuf[0] == '6' ) break;
	}
  	i++;
  } while (i<4);
//  printf("cmdbuf[0]= %c i=%d\n", cmdbuf[0], i); //debug
  if ( i==4 ) {
  	//no HDD found, just return;
	return 0;
  } else {
  	*(device+7)+=i; 	
  }	
//  printf("device = %s\n", device); //debug
}  	
     printf(" i = %d, s = %d, t %d, tryscsi %d\n", immediate, standby, timer, tryscsi);
  mode="SCSI";
 // open device - SCSI devices are opened (O_RDWR | O_NONBLOCK) so the
  // scsi generci device can be used (needs write permission for MODE
  // SELECT command) plus O_NONBLOCK to stop open hanging if media not
  // present (e.g. with st).
  fd = deviceopen(device, mode);
  if (fd<0) {
    char errmsg[256];
    snprintf(errmsg,256,"Sendidle open device: %s failed",argv[argc-1]);
    errmsg[255]='\0';
    syserror(errmsg);
    return FAILDEV;
  }
//nick +
        if (powermode) {
//            if (printf( "checkpowermode=0x%x\n", o_ataCheckPowerMode(fd)))
//                  pout("Sendidle: CHECK POWER MODE Failed.\n\n");
//                else
//                    pout("CHECK POWER MODE command was sent.\n");
//            printf( "checkpowermode=0x%x\n", ataCheckPowerMode(fd));

            o_ataCheckPowerMode(fd);
            con->controller_type = CONTROLLER_MARVELL_SATA;
            o_ataCheckPowerMode(fd);

            return 0;
        }
//nick -
        if (standby)
        {
            if (immediate)
            {
                if (ataSendStandbyImmediate(fd))
                    pout("Sendidle: STANDBY IMMEDIATE Failed.\n\n");
                else
                    pout("STANDBY IMMEDIATE command was sent.\n");

                con->controller_type = CONTROLLER_MARVELL_SATA;
                if (ataSendStandbyImmediate(fd))
                    pout("Sendidle: STANDBY IMMEDIATE Failed.\n\n");
                else
                    pout("STANDBY IMMEDIATE command was sent.\n");

            }
            else
            {
                if (ataSendStandby(fd))
                    pout("Sendidle: STANDBY Failed.\n\n");
                else
                    pout("STANDBY command was sent.\n");
                con->controller_type = CONTROLLER_MARVELL_SATA;
                if (ataSendStandby(fd))
                    pout("Sendidle: STANDBY Failed.\n\n");
                else
                    pout("STANDBY command was sent.\n");
            }

        }
        else if (immediate)
        {
            if (ataSendIdleImmediate(fd)) 
                pout("Sendidle: IDLE IMMEDIATE Failed.\n\n");
            else
                pout("IDLE IMMEDIATE command was sent.\n");
            con->controller_type = CONTROLLER_MARVELL_SATA;
            if (ataSendIdleImmediate(fd)) 
                pout("Sendidle: IDLE IMMEDIATE Failed.\n\n");
            else
                pout("IDLE IMMEDIATE command was sent.\n");

        }
        else
        {
            if (ataSendIdle(fd)) 
                pout("Sendidle: IDLE Failed.\n\n");
            else
                pout("IDLE command was sent.\n");
            con->controller_type = CONTROLLER_MARVELL_SATA;
            if (ataSendIdle(fd)) 
                pout("Sendidle: IDLE Failed.\n\n");
            else
                pout("IDLE command was sent.\n");
	}
  return retval;
}
