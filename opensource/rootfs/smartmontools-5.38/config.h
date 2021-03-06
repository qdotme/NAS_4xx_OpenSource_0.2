/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

/* smartmontools CVS Tag */
#define CONFIG_H_CVSID "$Id: configure,v 1.1.1.1.4.2 2008/11/26 07:31:03 wiley Exp $"

/* use mailx as default mailer */
/* #undef DEFAULT_MAILER */

/* Define to 1 if you have the `ata_identify_is_cached' function in os_*.c. */
/* #undef HAVE_ATA_IDENTIFY_IS_CACHED */

/* Define to 1 if C++ compiler supports __attribute__((packed)) */
#define HAVE_ATTR_PACKED 1

/* Define to 1 if you have the <dev/ata/atavar.h> header file. */
/* #undef HAVE_DEV_ATA_ATAVAR_H */

/* Define to 1 if you have the <dev/ciss/cissio.h> header file. */
/* #undef HAVE_DEV_CISS_CISSIO_H */

/* Define to 1 if you have the `getdomainname' function. */
#define HAVE_GETDOMAINNAME 1

/* Define to 1 if you have the `gethostbyname' function. */
#define HAVE_GETHOSTBYNAME 1

/* Define to 1 if you have the `gethostname' function. */
#define HAVE_GETHOSTNAME 1

/* Define to 1 if you have the `getopt' function. */
#define HAVE_GETOPT 1

/* Define to 1 if you have the <getopt.h> header file. */
#define HAVE_GETOPT_H 1

/* Define to 1 if you have the `getopt_long' function. */
#define HAVE_GETOPT_LONG 1

/* Define to 1 if you have the `get_os_version_str' function in os_*.c. */
/* #undef HAVE_GET_OS_VERSION_STR */

/* Define to 1 if the system has the type `int64_t'. */
#define HAVE_INT64_T 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <linux/cciss_ioctl.h> header file. */
#define HAVE_LINUX_CCISS_IOCTL_H 1

/* Define to 1 if you have the <linux/compiler.h> header file. */
#define HAVE_LINUX_COMPILER_H 1

/* Define to 1 if you have the <locale.h> header file. */
#define HAVE_LOCALE_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the `sigset' function. */
#define HAVE_SIGSET 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strtoull' function. */
#define HAVE_STRTOULL 1

/* Define to 1 if you have the <sys/inttypes.h> header file. */
/* #undef HAVE_SYS_INTTYPES_H */

/* Define to 1 if you have the <sys/int_types.h> header file. */
/* #undef HAVE_SYS_INT_TYPES_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/tweio.h> header file. */
/* #undef HAVE_SYS_TWEIO_H */

/* Define to 1 if you have the <sys/twereg.h> header file. */
/* #undef HAVE_SYS_TWEREG_H */

/* Define to 1 if you have the <sys/tw_osl_ioctl.h> header file. */
/* #undef HAVE_SYS_TW_OSL_IOCTL_H */

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if the system has the type `uint64_t'. */
#define HAVE_UINT64_T 1

/* Define to 1 if you have the `uname' function. */
#define HAVE_UNAME 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if the `snprintf' function is sane */
/* #undef HAVE_WORKING_SNPRINTF */

/* need assembly code os_solaris_ata.s */
/* #undef NEED_SOLARIS_ATA_CODE */

/* Name of package */
#define PACKAGE "smartmontools"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "smartmontools-support@lists.sourceforge.net"

/* smartmontools Home Page */
#define PACKAGE_HOMEPAGE "http://smartmontools.sourceforge.net/"

/* Define to the full name of this package. */
#define PACKAGE_NAME "smartmontools"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "smartmontools 5.38"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "smartmontools"

/* Define to the version of this package. */
#define PACKAGE_VERSION "5.38"

/* smartmontools Build Host */
#define SMARTMONTOOLS_BUILD_HOST "arm-unknown-linux-gnu"

/* smartmontools Configure Arguments */
#define SMARTMONTOOLS_CONFIGURE_ARGS " '--target=arm-linux' '--host=arm-linux' '--build=i386-pc-linux-gnu' '--prefix=/usr' '--exec-prefix=/usr' '--bindir=/usr/bin' '--sbindir=/usr/sbin' '--libexecdir=/usr/lib' '--sysconfdir=/etc' '--datadir=/usr/share' '--localstatedir=/var' '--mandir=/usr/man' '--infodir=/usr/info' '--disable-nls' 'build_alias=i386-pc-linux-gnu' 'host_alias=arm-linux' 'target_alias=arm-linux' 'CXX=/opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-g++' 'CC=/opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-gcc' 'CFLAGS=-Os -pipe'"

/* smartmontools Configure Date */
#define SMARTMONTOOLS_CONFIGURE_DATE "2009/03/25 05:40:26 UTC"

/* smartmontools Release Date */
#define SMARTMONTOOLS_RELEASE_DATE "2008/03/10"

/* smartmontools Release Time */
#define SMARTMONTOOLS_RELEASE_TIME "10:44:07 GMT"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "5.38"
