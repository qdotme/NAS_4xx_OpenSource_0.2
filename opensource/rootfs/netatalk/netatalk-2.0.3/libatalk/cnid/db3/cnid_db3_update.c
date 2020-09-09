/*
 * $Id: cnid_db3_update.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef CNID_BACKEND_DB3

#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <errno.h>
#include <atalk/logger.h>

#ifdef HAVE_DB4_DB_H
#include <db4/db.h>
#else
#include <db.h>
#endif
#include <netatalk/endian.h>
#include <atalk/adouble.h>
#include "cnid_db3.h"

#include "cnid_db3_private.h"

/* cnid_update: takes the given cnid and updates the metadata.  To
 * handle the did/name data, there are a bunch of functions to get
 * and set the various fields. */
int cnid_db3_update(struct _cnid_db *cdb, const cnid_t id, const struct stat *st,
                const cnid_t did, char *name, const int len
                /*, const char *info, const int infolen*/)
{
    CNID_private *db;
    DBT key, data, altdata;
    DB_TXN *tid;
    int rc;

    if (!cdb || !(db = cdb->_private) || !id || !st || !name || (db->flags & CNIDFLAG_DB_RO)) {
        return -1;
    }

    memset(&key, 0, sizeof(key));
    memset(&altdata, 0, sizeof(altdata));

retry:
    if ((rc = db3_txn_begin(db->dbenv, NULL, &tid, 0))) {
        LOG(log_error, logtype_default, "cnid_update: Failed to begin transaction: %s", db_strerror(rc));
        return rc;
    }

    /* Get the old info. */
    key.data = (cnid_t *)&id;
    key.size = sizeof(id);
    memset(&data, 0, sizeof(data));
    if ((rc = db->db_cnid->get(db->db_cnid, tid, &key, &data, DB_RMW))) {
        db3_txn_abort(tid);
        switch (rc) {
        case DB_LOCK_DEADLOCK:
            goto retry;
        case DB_NOTFOUND:
            /* Silently fail here.  We're allowed to do this since this CNID
             * might have been deleted out from under us, or someone has
             * called cnid_lookup then cnid_update (which is redundant). */
            return 0;
        default:
            goto update_err;
        }
    }

    /* Delete the old dev/ino mapping. */
    key.data = data.data;
    key.size = CNID_DEVINO_LEN;
    if ((rc = db->db_devino->del(db->db_devino, tid, &key, 0))) {
        switch (rc) {
        case DB_LOCK_DEADLOCK:
            db3_txn_abort(tid);
            goto retry;
        case DB_NOTFOUND:
            break;
        default:
            db3_txn_abort(tid);
            goto update_err;
        }
    }

    /* Delete the old did/name mapping. */
    key.data = (char *) data.data + CNID_DEVINO_LEN;
    key.size = data.size - CNID_DEVINO_LEN;
    if ((rc = db->db_didname->del(db->db_didname, tid, &key, 0))) {
        switch (rc) {
        case DB_LOCK_DEADLOCK:
            db3_txn_abort(tid);
            goto retry;
        case DB_NOTFOUND:
            break;
        default:
            db3_txn_abort(tid);
            goto update_err;
        }
    }

    /* Make a new entry. */
    data.data = make_cnid_data(st, did, name, len);
    data.size = CNID_HEADER_LEN + len + 1;

    /* Update the old CNID with the new info. */
    key.data = (cnid_t *) &id;
    key.size = sizeof(id);
    if ((rc = db->db_cnid->put(db->db_cnid, tid, &key, &data, 0))) {
        db3_txn_abort(tid);
        switch (rc) {
        case DB_LOCK_DEADLOCK:
            goto retry;
        default:
            goto update_err;
        }
    }

    /* Put in a new dev/ino mapping. */
    key.data = data.data;
    key.size = CNID_DEVINO_LEN;
    altdata.data = (cnid_t *) &id;
    altdata.size = sizeof(id);
    if ((rc = db->db_devino->put(db->db_devino, tid, &key, &altdata, 0))) {
        db3_txn_abort(tid);
        switch (rc) {
        case DB_LOCK_DEADLOCK:
            goto retry;
        default:
            goto update_err;
        }
    }

    /* put in a new did/name mapping. */
    key.data = (char *) data.data + CNID_DEVINO_LEN;
    key.size = data.size - CNID_DEVINO_LEN;
    if ((rc = db->db_didname->put(db->db_didname, tid, &key, &altdata, 0))) {
        db3_txn_abort(tid);
        switch (rc) {
        case DB_LOCK_DEADLOCK:
            goto retry;
        default:
            goto update_err;
        }
    }


    return db3_txn_commit(tid, 0);

update_err:
    LOG(log_error, logtype_default, "cnid_update: Unable to update CNID %u: %s",
        ntohl(id), db_strerror(rc));
    return -1;
}

#endif /* CNID_BACKEND_DB3 */
