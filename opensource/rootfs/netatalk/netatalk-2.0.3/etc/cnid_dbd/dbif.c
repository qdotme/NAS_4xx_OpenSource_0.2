/*
 * $Id: dbif.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (C) Joerg Lenneis 2003
 * All Rights Reserved.  See COPYING.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <atalk/logger.h>
#include <db.h>
#include "db_param.h"
#include "dbif.h"

#define DB_ERRLOGFILE "db_errlog"


static DB_ENV *db_env = NULL;
static DB_TXN *db_txn = NULL;
static FILE   *db_errlog = NULL;

#ifdef CNID_BACKEND_DBD_TXN
#define DBOPTIONS    (DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN)
#else
#define DBOPTIONS    (DB_CREATE | DB_INIT_CDB | DB_INIT_MPOOL | DB_PRIVATE) 
#endif

static struct db_table {
     char            *name;
     DB              *db;
     u_int32_t       general_flags;
     DBTYPE          type;
} db_table[] =
{
     { "cnid2.db",       NULL,      0, DB_BTREE},
     { "devino.db",      NULL,      0, DB_BTREE},
     { "didname.db",     NULL,      0, DB_BTREE},
};

static char *old_dbfiles[] = {"cnid.db", NULL};

extern int didname(DB *dbp, const DBT *pkey, const DBT *pdata, DBT *skey);
extern int devino(DB *dbp, const DBT *pkey, const DBT *pdata, DBT *skey);

/* --------------- */
static int  db_compat_associate (DB *p, DB *s,
                   int (*callback)(DB *, const DBT *,const DBT *, DBT *),
                   u_int32_t flags)
{
#if DB_VERSION_MAJOR > 4 || (DB_VERSION_MAJOR == 4 && DB_VERSION_MINOR >= 1)
    return p->associate(p, db_txn, s, callback, flags);
#else
#if (DB_VERSION_MAJOR == 4 && DB_VERSION_MINOR == 0)
    return p->associate(p,       s, callback, flags);
#else
    return 0;
#endif
#endif
}

/* --------------- */
static int db_compat_open(DB *db, char *file, char *name, DBTYPE type, int mode)
{
    int ret;

#if DB_VERSION_MAJOR > 4 || (DB_VERSION_MAJOR == 4 && DB_VERSION_MINOR >= 1)
    ret = db->open(db, db_txn, file, name, type, DB_CREATE, mode); 
#else
    ret = db->open(db,       file, name, type, DB_CREATE, mode); 
#endif

    if (ret) {
        LOG(log_error, logtype_cnid, "error opening database %s: %s", name, db_strerror(ret));
        return -1;
    } else {
        return 0;
    }
}

/* --------------- */
static int upgrade_required()
{
    int i;
    int found = 0;
    struct stat st;
    
    for (i = 0; old_dbfiles[i] != NULL; i++) {
	if ( !(stat(old_dbfiles[i], &st) < 0) ) {
	    found++;
	    continue;
	}
	if (errno != ENOENT) {
	    LOG(log_error, logtype_cnid, "cnid_open: Checking %s gave %s", old_dbfiles[i], strerror(errno));
	    found++;
	}
    }
    return found;
}

/* --------------- */
int dbif_stamp(void *buffer, int size)
{
    struct stat st;
    int         rc;

    if (size < 8)
        return -1;

    if ((rc = stat(db_table[0].name, &st)) < 0) {
        LOG(log_error, logtype_cnid, "error stating database %s: %s", db_table[0].name, db_strerror(rc));
        return -1;
    }
    memset(buffer, 0, size);
    memcpy(buffer, &st.st_ctime, sizeof(st.st_ctime));

    return 0;
}

/* --------------- */
/*
 *  We assume our current directory is already the BDB homedir. Otherwise
 *  opening the databases will not work as expected. If we use transactions,
 *  dbif_env_init(), dbif_close() and dbif_stamp() are the only interface
 *  functions that can be called without a valid transaction handle in db_txn.
 */
int dbif_env_init(struct db_param *dbp)
{
    int ret;
#ifdef CNID_BACKEND_DBD_TXN
    char **logfiles = NULL;
    char **file;
#endif

    /* Refuse to do anything if this is an old version of the CNID database */
    if (upgrade_required()) {
	LOG(log_error, logtype_cnid, "Found version 1 of the CNID database. Please upgrade to version 2");
	return -1;
    }

    if ((db_errlog = fopen(DB_ERRLOGFILE, "a")) == NULL)
        LOG(log_warning, logtype_cnid, "error creating/opening DB errlogfile: %s", strerror(errno));

#ifdef CNID_BACKEND_DBD_TXN
    if ((ret = db_env_create(&db_env, 0))) {
        LOG(log_error, logtype_cnid, "error creating DB environment: %s", 
            db_strerror(ret));
        db_env = NULL;
        return -1;
    }    
    if (db_errlog != NULL)
        db_env->set_errfile(db_env, db_errlog); 
    db_env->set_verbose(db_env, DB_VERB_RECOVERY, 1);
    db_env->set_verbose(db_env, DB_VERB_CHKPOINT, 1);
    if ((ret = db_env->open(db_env, ".", DBOPTIONS | DB_PRIVATE | DB_RECOVER, 0))) {
        LOG(log_error, logtype_cnid, "error opening DB environment: %s", 
            db_strerror(ret));
        db_env->close(db_env, 0);
        db_env = NULL;
        return -1;
    }

    if (db_errlog != NULL)
        fflush(db_errlog);

    if ((ret = db_env->close(db_env, 0))) {
        LOG(log_error, logtype_cnid, "error closing DB environment after recovery: %s", 
            db_strerror(ret));
        db_env = NULL;
        return -1;
    }
#endif
    if ((ret = db_env_create(&db_env, 0))) {
        LOG(log_error, logtype_cnid, "error creating DB environment after recovery: %s",
            db_strerror(ret));
        db_env = NULL;
        return -1;
    }
    if ((ret = db_env->set_cachesize(db_env, 0, 1024 * dbp->cachesize, 0))) {
        LOG(log_error, logtype_cnid, "error setting DB environment cachesize to %i: %s",
            dbp->cachesize, db_strerror(ret));
        db_env->close(db_env, 0);
        db_env = NULL;
        return -1;
    }
    
    if (db_errlog != NULL)
        db_env->set_errfile(db_env, db_errlog);
    if ((ret = db_env->open(db_env, ".", DBOPTIONS , 0))) {
        LOG(log_error, logtype_cnid, "error opening DB environment after recovery: %s",
            db_strerror(ret));
        db_env->close(db_env, 0);
        db_env = NULL;
        return -1;      
    }

#ifdef CNID_BACKEND_DBD_TXN
    if (dbp->nosync && (ret = db_env->set_flags(db_env, DB_TXN_NOSYNC, 1))) {
        LOG(log_error, logtype_cnid, "error setting TXN_NOSYNC flag: %s",
            db_strerror(ret));
        db_env->close(db_env, 0);
        db_env = NULL;
        return -1;
    }
    if (dbp->logfile_autoremove && db_env->log_archive(db_env, &logfiles, 0)) {
	LOG(log_error, logtype_cnid, "error getting list of stale logfiles: %s",
            db_strerror(ret));
        db_env->close(db_env, 0);
        db_env = NULL;
        return -1;
    }
    if (logfiles != NULL) {
	for (file = logfiles; *file != NULL; file++) {
	    if (unlink(*file) < 0)
		LOG(log_warning, logtype_cnid, "Error removing stale logfile %s: %s", *file, strerror(errno));
	}
	free(logfiles);
    }
#endif
    return 0;
}

/* --------------- */
int dbif_open(struct db_param *dbp, int do_truncate)
{
    int ret;
    int i;
    u_int32_t count;

    for (i = 0; i != DBIF_DB_CNT; i++) {
        if ((ret = db_create(&(db_table[i].db), db_env, 0))) {
            LOG(log_error, logtype_cnid, "error creating handle for database %s: %s", 
                db_table[i].name, db_strerror(ret));
            return -1;
        }

        if (db_table[i].general_flags) { 
            if ((ret = db_table[i].db->set_flags(db_table[i].db, db_table[i].general_flags))) {
                LOG(log_error, logtype_cnid, "error setting flags for database %s: %s", 
                    db_table[i].name, db_strerror(ret));
                return -1;
            }
        }

#if 0
#ifndef CNID_BACKEND_DBD_TXN
        if ((ret = db_table[i].db->set_cachesize(db_table[i].db, 0, 1024 * dbp->cachesize, 0))) {
            LOG(log_error, logtype_cnid, "error setting DB cachesize to %i for database %s: %s",
                dbp->cachesize, db_table[i].name, db_strerror(ret));
            return -1;
        }
#endif /* CNID_BACKEND_DBD_TXN */
#endif
        if (db_compat_open(db_table[i].db, db_table[0].name, db_table[i].name, db_table[i].type, 0664) < 0)
            return -1;
        if (db_errlog != NULL)
            db_table[i].db->set_errfile(db_table[i].db, db_errlog);

        if (do_truncate && i > 0) {
	    if ((ret = db_table[i].db->truncate(db_table[i].db, db_txn, &count, 0))) {
                LOG(log_error, logtype_cnid, "error truncating database %s: %s", 
                    db_table[i].name, db_strerror(ret));
                return -1;
            }
        }
    }

    /* TODO: Implement CNID DB versioning info on new databases. */
    /* TODO: Make transaction support a runtime option. */
    /* Associate the secondary with the primary. */
    if ((ret = db_compat_associate(db_table[0].db, db_table[DBIF_IDX_DIDNAME].db, didname, (do_truncate)?DB_CREATE:0)) != 0) {
        LOG(log_error, logtype_cnid, "Failed to associate didname database: %s",db_strerror(ret));
        return -1;
    }
 
    if ((ret = db_compat_associate(db_table[0].db, db_table[DBIF_IDX_DEVINO].db, devino, (do_truncate)?DB_CREATE:0)) != 0) {
        LOG(log_error, logtype_cnid, "Failed to associate devino database: %s",db_strerror(ret));
	return -1;
    }

    return 0;
}

/* ------------------------ */
int dbif_closedb()
{
    int i;
    int ret;
    int err = 0;

    for (i = DBIF_DB_CNT -1; i >= 0; i--) {
        if (db_table[i].db != NULL && (ret = db_table[i].db->close(db_table[i].db, 0))) {
            LOG(log_error, logtype_cnid, "error closing database %s: %s", db_table[i].name, db_strerror(ret));
            err++;
        }
    }
    if (err)
        return -1;
    return 0;
}

/* ------------------------ */
int dbif_close()
{
    int ret;
    int err = 0;
    
    if (dbif_closedb()) 
    	err++;
     
    if (db_env != NULL && (ret = db_env->close(db_env, 0))) { 
        LOG(log_error, logtype_cnid, "error closing DB environment: %s", db_strerror(ret));
        err++;
    }
    if (db_errlog != NULL && fclose(db_errlog) == EOF) {
        LOG(log_error, logtype_cnid, "error closing DB logfile: %s", strerror(errno));
        err++;
    }
    if (err)
        return -1;
    return 0;
}

/*
 *  The following three functions are wrappers for DB->get(), DB->put() and
 *  DB->del(). We define them here because we want access to the db_txn
 *  transaction handle and the database handles limited to the functions in this
 *  file. A consequence is that there is always only one transaction in
 *  progress. For nontransactional access db_txn is NULL. All three return -1 on
 *  error. dbif_get()/dbif_del return 1 if the key was found and 0
 *  otherwise. dbif_put() returns 0 if key/val was successfully updated and 1 if
 *  the DB_NOOVERWRITE flag was specified and the key already exists.
 *  
 *  All return codes other than DB_NOTFOUND and DB_KEYEXIST from the DB->()
 *  functions are not expected and therefore error conditions.
 */

int dbif_get(const int dbi, DBT *key, DBT *val, u_int32_t flags)
{
    int ret;
    DB *db = db_table[dbi].db;

    ret = db->get(db, db_txn, key, val, flags);
     
    if (ret == DB_NOTFOUND)
        return 0;
    if (ret) {
        LOG(log_error, logtype_cnid, "error retrieving value from %s: %s", db_table[dbi].name, db_strerror(errno));
        return -1;
    } else 
        return 1;
}

/* search by secondary return primary */
int dbif_pget(const int dbi, DBT *key, DBT *pkey, DBT *val, u_int32_t flags)
{
    int ret;
    DB *db = db_table[dbi].db;

    ret = db->pget(db, db_txn, key, pkey, val, flags);

#if DB_VERSION_MAJOR >= 4
    if (ret == DB_NOTFOUND || ret == DB_SECONDARY_BAD) {
#else
    if (ret == DB_NOTFOUND) {
#endif     
        return 0;
    }
    if (ret) {
        LOG(log_error, logtype_cnid, "error retrieving value from %s: %s", db_table[dbi].name, db_strerror(errno));
        return -1;
    } else 
        return 1;
}

/* -------------------------- */
int dbif_put(const int dbi, DBT *key, DBT *val, u_int32_t flags)
{
    int ret;
    DB *db = db_table[dbi].db;

    ret = db->put(db, db_txn, key, val, flags);
     
    if (ret) {
        if ((flags & DB_NOOVERWRITE) && ret == DB_KEYEXIST) {
            return 1;
        } else {
            LOG(log_error, logtype_cnid, "error setting key/value in %s: %s", db_table[dbi].name, db_strerror(errno));
            return -1;
        }
    } else
        return 0;
}

int dbif_del(const int dbi, DBT *key, u_int32_t flags)
{
    int ret;
    DB *db = db_table[dbi].db;

    ret = db->del(db, db_txn, key, flags);

#if DB_VERSION_MAJOR > 3
    if (ret == DB_NOTFOUND || ret == DB_SECONDARY_BAD)
#else
    if (ret == DB_NOTFOUND)
#endif
        return 0;
    if (ret) {
        LOG(log_error, logtype_cnid, "error deleting key/value from %s: %s", db_table[dbi].name, db_strerror(errno));
        return -1;
    } else
        return 1;
}

#ifdef CNID_BACKEND_DBD_TXN

int dbif_txn_begin()
{
    int ret;
#if DB_VERSION_MAJOR >= 4
    ret = db_env->txn_begin(db_env, NULL, &db_txn, 0);
#else     
    ret = txn_begin(db_env, NULL, &db_txn, 0);
#endif
    if (ret) {
        LOG(log_error, logtype_cnid, "error starting transaction: %s", db_strerror(errno));
        return -1;
    } else 
        return 0;
}

int dbif_txn_commit()
{
    int ret;
#if DB_VERSION_MAJOR >= 4
    ret = db_txn->commit(db_txn, 0);
#else
    ret = txn_commit(db_txn, 0);
#endif
    if (ret) {
        LOG(log_error, logtype_cnid, "error committing transaction: %s", db_strerror(errno));
        return -1;
    } else 
        return 0;
}

int dbif_txn_abort()
{
    int ret;
#if DB_VERSION_MAJOR >= 4
    ret = db_txn->abort(db_txn);
#else
    ret = txn_abort(db_txn);
#endif
    if (ret) {
        LOG(log_error, logtype_cnid, "error aborting transaction: %s", db_strerror(errno));
        return -1;
    } else
        return 0;
}

int dbif_txn_checkpoint(u_int32_t kbyte, u_int32_t min, u_int32_t flags)
{
    int ret;
#if DB_VERSION_MAJOR >= 4
    ret = db_env->txn_checkpoint(db_env, kbyte, min, flags);
#else 
    ret = txn_checkpoint(db_env, kbyte, min, flags);
#endif
    if (ret) {
        LOG(log_error, logtype_cnid, "error checkpointing transaction susystem: %s", db_strerror(errno));
        return -1;
    } else 
        return 0;
}

#else

int dbif_sync()
{
    int i;
    int ret;
    int err = 0;
     
    for (i = 0; i != /* DBIF_DB_CNT*/ 1; i++) {
        if ((ret = db_table[i].db->sync(db_table[i].db, 0))) {
            LOG(log_error, logtype_cnid, "error syncing database %s: %s", db_table[i].name, db_strerror(ret));
            err++;
        }
    }
 
    if (err)
        return -1;
    else
        return 0;
}


int dbif_count(const int dbi, u_int32_t *count) 
{
    int ret;
    DB_BTREE_STAT *sp;
    DB *db = db_table[dbi].db;

    ret = db->stat(db, &sp, 0);

    if (ret) {
        LOG(log_error, logtype_cnid, "error getting stat infotmation on database: %s", db_strerror(errno));
        return -1;
    }

    *count = sp->bt_ndata;
    free(sp);

    return 0;
}
#endif /* CNID_BACKEND_DBD_TXN */

