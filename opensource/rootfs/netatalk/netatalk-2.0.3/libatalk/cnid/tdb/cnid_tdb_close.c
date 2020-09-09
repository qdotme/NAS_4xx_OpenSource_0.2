/*
 * $Id: cnid_tdb_close.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef CNID_BACKEND_TDB

#include "cnid_tdb.h"

void cnid_tdb_close(struct _cnid_db *cdb)
{
    struct _cnid_tdb_private *db;

    free(cdb->volpath);
    db = (struct _cnid_tdb_private *)cdb->_private;
    tdb_close(db->tdb_cnid);
    tdb_close(db->tdb_didname);
    tdb_close(db->tdb_devino);
    free(cdb->_private);
    free(cdb);
}

#endif
