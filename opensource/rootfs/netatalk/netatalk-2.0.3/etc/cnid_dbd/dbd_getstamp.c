/*
 * $Id: dbd_getstamp.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (C) Joerg Lenneis 2003
 * All Rights Reserved.  See COPYING.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <string.h>
#include <atalk/logger.h>
#include <errno.h>
#include <netatalk/endian.h>
#include <atalk/cnid_dbd_private.h>

#include "dbif.h"
#include "dbd.h"
#include "pack.h"

/* Return the unique stamp associated with this database */

int dbd_getstamp(struct cnid_dbd_rqst *rqst, struct cnid_dbd_rply *rply)
{
    DBT key, data;
    int rc;


    memset(&key, 0, sizeof(key));
    memset(&data, 0, sizeof(data));

    rply->namelen = 0;

    key.data = ROOTINFO_KEY;
    key.size = ROOTINFO_KEYLEN;

    if ((rc = dbif_get(DBIF_IDX_CNID, &key, &data, 0)) < 0) {
        LOG(log_error, logtype_cnid, "dbd_getstamp: Error getting rootinfo record");
        rply->result = CNID_DBD_RES_ERR_DB;
        return -1;
    }
     
    if (rc == 0) {
	LOG(log_error, logtype_cnid, "dbd_getstamp: No rootinfo record found");
        rply->result = CNID_DBD_RES_NOTFOUND;
        return 1;
    }
    
    rply->namelen = CNID_DEV_LEN;
    rply->name = (char *)data.data + CNID_DEV_OFS;
    
#ifdef DEBUG
    LOG(log_info, logtype_cnid, "cnid_getstamp: Returning stamp");
#endif
    rply->result = CNID_DBD_RES_OK;
    return 1;
}
