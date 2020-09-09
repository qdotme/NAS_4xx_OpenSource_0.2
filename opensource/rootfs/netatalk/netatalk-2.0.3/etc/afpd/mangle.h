/*
 * $Id: mangle.h,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 */

#ifndef AFPD_MANGLE_H 
#define AFPD_MANGLE_H 1

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <atalk/adouble.h>
#include <atalk/cnid.h>
#include <atalk/logger.h>

#include "globals.h"
#include "volume.h"
#include "directory.h"

#define MANGLE_CHAR '#'
#define MAX_MANGLE_SUFFIX_LENGTH 999
#define MAX_EXT_LENGTH 5 /* XXX This cannot be greater than 27 */
#define MANGLE_LENGTH  9 /* #ffffffff This really can't be changed. */
#define MAX_LENGTH MACFILELEN 

extern unsigned char *mangle __P((const struct vol *, unsigned char *, size_t, unsigned char *, cnid_t, int));
extern char *demangle __P((const struct vol *, char *, cnid_t did));
extern char *demangle_osx __P((const struct vol *, char *, cnid_t did, cnid_t *fileid));

#endif /* AFPD_MANGLE_H */
