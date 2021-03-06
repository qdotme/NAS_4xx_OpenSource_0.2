/*
 * $Id: cnid_tdb_update.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef CNID_BACKEND_TDB

#include "cnid_tdb.h"
#include <atalk/logger.h>

int cnid_tdb_update(struct _cnid_db *cdb, const cnid_t id, const struct stat *st,
                     const cnid_t did, char *name, const int len
                     /*, const char *info, const int infolen */ )
{
    struct _cnid_tdb_private *db;
    TDB_DATA key, data, altdata;

    if (!cdb || !(db = cdb->_private) || !id || !st || !name || (db->flags & TDBFLAG_DB_RO)) {
        return -1;
    }

    memset(&key, 0, sizeof(key));
    memset(&altdata, 0, sizeof(altdata));


    /* Get the old info. */
    key.dptr = (char *)&id;
    key.dsize = sizeof(id);
    memset(&data, 0, sizeof(data));
    data = tdb_fetch(db->tdb_cnid, key);
    if (!data.dptr)
        return 0;

    key.dptr = data.dptr;
    key.dsize = TDB_DEVINO_LEN;
    tdb_delete(db->tdb_devino, key); 

    key.dptr = (char *)data.dptr + TDB_DEVINO_LEN;
    key.dsize = data.dsize - TDB_DEVINO_LEN;
    tdb_delete(db->tdb_didname, key); 

    free(data.dptr);
    /* Make a new entry. */
    data.dptr = make_tdb_data(st, did, name, len);
    data.dsize = TDB_HEADER_LEN + len + 1;

    /* Update the old CNID with the new info. */
    key.dptr = (char *) &id;
    key.dsize = sizeof(id);
    if (tdb_store(db->tdb_cnid, key, data, TDB_REPLACE)) {
        goto update_err;
    }

    /* Put in a new dev/ino mapping. */
    key.dptr = data.dptr;
    key.dsize = TDB_DEVINO_LEN;
    altdata.dptr  = (char *) &id;
    altdata.dsize = sizeof(id);
    if (tdb_store(db->tdb_devino, key, altdata, TDB_REPLACE)) {
        goto update_err;
    }
    /* put in a new did/name mapping. */
    key.dptr = (char *) data.dptr + TDB_DEVINO_LEN;
    key.dsize = data.dsize - TDB_DEVINO_LEN;
    if (tdb_store(db->tdb_didname, key, altdata, TDB_REPLACE)) {
        goto update_err;
    }

    return 0;
update_err:
    LOG(log_error, logtype_default, "cnid_update: Unable to update CNID %u",
        ntohl(id));
    return -1;

}

#endif
