/*
 * $Id: cnid_dbd.h,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (C) Joerg Lenneis 2003
 * All Rights Reserved.  See COPYING.
 */


#ifndef _ATALK_CNID_DBD__H
#define _ATALK_CNID_DBD__H 1

#include <sys/cdefs.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <netatalk/endian.h>
#include <atalk/cnid.h>

extern struct _cnid_module cnid_dbd_module;
extern struct _cnid_db *cnid_dbd_open __P((const char *, mode_t));
extern void cnid_dbd_close __P((struct _cnid_db *));
extern cnid_t cnid_dbd_add __P((struct _cnid_db *, const struct stat *, const cnid_t,
			    char *, const int, cnid_t));
extern cnid_t cnid_dbd_get __P((struct _cnid_db *, const cnid_t, char *, const int)); 
extern char *cnid_dbd_resolve __P((struct _cnid_db *, cnid_t *, void *, u_int32_t )); 
extern int cnid_dbd_getstamp __P((struct _cnid_db *, void *, const int )); 
extern cnid_t cnid_dbd_lookup __P((struct _cnid_db *, const struct stat *, const cnid_t,
			       char *, const int));
extern int cnid_dbd_update __P((struct _cnid_db *, const cnid_t, const struct stat *,
			    const cnid_t, char *, int));
extern int cnid_dbd_delete __P((struct _cnid_db *, const cnid_t));

#endif /* include/atalk/cnid_dbd.h */

