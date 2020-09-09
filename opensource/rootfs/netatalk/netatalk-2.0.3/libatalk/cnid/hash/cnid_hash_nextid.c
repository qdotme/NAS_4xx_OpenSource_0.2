/*
 * $Id: cnid_hash_nextid.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef CNID_BACKEND_HASH

#include "cnid_hash.h"

cnid_t cnid_hash_nextid(struct _cnid_db *cdb)
{
    return CNID_INVALID;
}

#endif /* CNID_BACKEND_HASH */
