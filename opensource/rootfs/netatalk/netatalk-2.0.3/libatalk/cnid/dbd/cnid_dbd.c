/*
 * $Id: cnid_dbd.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (C) Joerg Lenneis 2003
 * All Rights Reserved.  See COPYING.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef CNID_BACKEND_DBD

#include <stdlib.h>
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif /* HAVE_SYS_STAT_H */
#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif /* HAVE_SYS_UIO_H */
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <sys/time.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <errno.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <time.h>    
 
#include <netatalk/endian.h>
#include <atalk/logger.h>
#include <atalk/adouble.h>
#include <atalk/cnid.h>
#include "cnid_dbd.h"
#include <atalk/cnid_dbd_private.h>

#ifndef SOL_TCP
#define SOL_TCP IPPROTO_TCP
#endif /* ! SOL_TCP */

static void RQST_RESET(struct cnid_dbd_rqst  *r) 
{ 
   memset(r, 0, sizeof(struct cnid_dbd_rqst ));
}

/* ----------- */
extern char             Cnid_srv[MAXHOSTNAMELEN + 1];
extern int              Cnid_port;

static int tsock_getfd(char *host, int port)
{
int sock;
struct sockaddr_in server;
struct hostent* hp;
int attr;
int err;

    server.sin_family=AF_INET;
    server.sin_port=htons((unsigned short)port);
    if (!host) {
        LOG(log_error, logtype_cnid, "getfd: -cnidserver not defined");
        return -1;
    }
    
    hp=gethostbyname(host);
    if (!hp) {
	unsigned long int addr=inet_addr(host);
	LOG(log_warning, logtype_cnid, "getfd: Could not resolve host %s, trying numeric address instead", host);
        if (addr!= (unsigned)-1)
            hp=gethostbyaddr((char*)addr,sizeof(addr),AF_INET);
 
    	if (!hp) {
	    LOG(log_error, logtype_cnid, "getfd: Could not resolve host %s", host);
    	    return(-1);
    	}
    }
    memcpy((char*)&server.sin_addr,(char*)hp->h_addr,sizeof(server.sin_addr));
    sock=socket(PF_INET,SOCK_STREAM,0);
    if (sock==-1) {
	LOG(log_error, logtype_cnid, "getfd: socket %s: %s", host, strerror(errno));
    	return(-1);
    }
    attr = 1;
    if (setsockopt(sock, SOL_TCP, TCP_NODELAY, &attr, sizeof(attr)) == -1) {
	LOG(log_error, logtype_cnid, "getfd: set TCP_NODELAY %s: %s", host, strerror(errno));
	close(sock);
	return(-1);
    }
    if(connect(sock ,(struct sockaddr*)&server,sizeof(server))==-1) {
        struct timeval tv;
        err = errno;
    	close(sock);
    	sock=-1;
	LOG(log_error, logtype_cnid, "getfd: connect %s: %s", host, strerror(err));
        switch (err) {
        case ENETUNREACH:
        case ECONNREFUSED: 
            
            tv.tv_usec = 0;
            tv.tv_sec  = 5;
            select(0, NULL, NULL, NULL, &tv);
            break;
        }
    }
    return(sock);
}

/* --------------------- */
static int init_tsock(CNID_private *db)
{
    int fd;
    int len;
    
    if ((fd = tsock_getfd(Cnid_srv, Cnid_port)) < 0) 
        return -1;
    len = strlen(db->db_dir);
    if (write(fd, &len, sizeof(int)) != sizeof(int)) {
        LOG(log_error, logtype_cnid, "init_tsock: Error/short write: %s", strerror(errno));
        close(fd);
        return -1;
    }
    if (write(fd, db->db_dir, len) != len) {
        LOG(log_error, logtype_cnid, "init_tsock: Error/short write dir: %s", strerror(errno));
        close(fd);
        return -1;
    }
    return fd;
}

/* --------------------- */
static int send_packet(CNID_private *db, struct cnid_dbd_rqst *rqst, int silent)
{
    struct iovec iov[2];
    size_t towrite;
    ssize_t len;
  
    iov[0].iov_base = rqst;
    iov[0].iov_len  = sizeof(struct cnid_dbd_rqst);
    
    if (!rqst->namelen) {
        if (write(db->fd, rqst, sizeof(struct cnid_dbd_rqst)) != sizeof(struct cnid_dbd_rqst)) {
	    if (!silent)
		LOG(log_warning, logtype_cnid, "send_packet: Error/short write rqst (db_dir %s): %s", 
		    db->db_dir, strerror(errno));
            return -1;
        }
        return 0;
    }
    iov[1].iov_base = rqst->name;
    iov[1].iov_len  = rqst->namelen;

    towrite = sizeof(struct cnid_dbd_rqst) +rqst->namelen;
    while (towrite > 0) {
        if (((len = writev(db->fd, iov, 2)) == -1 && errno == EINTR) || !len)
            continue;
 
        if (len == towrite) /* wrote everything out */
            break;
        else if (len < 0) { /* error */
	    if (!silent)
		LOG(log_warning, logtype_cnid, "send_packet: Error writev rqst (db_dir %s): %s", 
		    db->db_dir, strerror(errno));
            return -1;
        }
 
        towrite -= len;
        if (towrite > rqst->namelen) { /* skip part of header */
            iov[0].iov_base = (char *) iov[0].iov_base + len;
            iov[0].iov_len -= len;
        } else { /* skip to data */
            if (iov[0].iov_len) {
                len -= iov[0].iov_len;
                iov[0].iov_len = 0;
            }
            iov[1].iov_base = (char *) iov[1].iov_base + len;
            iov[1].iov_len -= len;
        }
    }
    return 0;
}

/* ------------------- */
static void dbd_initstamp(struct cnid_dbd_rqst *rqst)
{
    RQST_RESET(rqst);
    rqst->op = CNID_DBD_OP_GETSTAMP;
}

/* ------------------- */
static int dbd_reply_stamp(struct cnid_dbd_rply *rply)
{
    switch (rply->result) {
    case CNID_DBD_RES_OK:
        break;
    case CNID_DBD_RES_NOTFOUND:
        return -1;
    case CNID_DBD_RES_ERR_DB:
    default:
        errno = CNID_ERR_DB;
        return -1;
    }
    return 0;
}

/* --------------------- 
 * send a request and get reply
 * assume send is non blocking
 * if no answer after sometime (at least MAX_DELAY secondes) return an error
*/
#define MAX_DELAY 40
static int dbd_rpc(CNID_private *db, struct cnid_dbd_rqst *rqst, struct cnid_dbd_rply *rply, int silent)
{
    int  ret;
    char *nametmp;
    struct timeval tv;
    fd_set readfds;
    int    maxfd;
    size_t len;

    if (send_packet(db, rqst, silent) < 0) {
        return -1;
    }
    FD_ZERO(&readfds);
    FD_SET(db->fd, &readfds);
    maxfd = db->fd +1;
        
    tv.tv_usec = 0;
    tv.tv_sec  = MAX_DELAY;
    while ((ret = select(maxfd + 1, &readfds, NULL, NULL, &tv)) < 0 && errno == EINTR);

    if (ret < 0) {
	if (!silent)
	    LOG(log_error, logtype_cnid, "dbd_rpc: Error in select (db_dir %s): %s",
		db->db_dir, strerror(errno));
        return ret;
    }
    /* signal ? */
    if (!ret) {
        /* no answer */
	if (!silent)
	    LOG(log_error, logtype_cnid, "dbd_rpc: select timed out (db_dir %s)",
		db->db_dir);
        return -1;
    }

    len = rply->namelen;
    nametmp = rply->name;
    /* assume that if we have something then everything is there (doesn't sleep) */ 
    if ((ret = read(db->fd, rply, sizeof(struct cnid_dbd_rply))) != sizeof(struct cnid_dbd_rply)) {
	if (!silent)
	    LOG(log_error, logtype_cnid, "dbd_rpc: Error reading header from fd (db_dir %s): %s",
                db->db_dir, ret == -1?strerror(errno):"closed");
        rply->name = nametmp;
        return -1;
    }
    rply->name = nametmp;
    if (rply->namelen && rply->namelen > len) {
	if (!silent)
            LOG(log_error, logtype_cnid, 
                 "dbd_rpc: Error reading name (db_dir %s): %s name too long wanted %d only %d, garbage?",
                 db->db_dir, rply->namelen, len);
        return -1;
    }
    if (rply->namelen && (ret = read(db->fd, rply->name, rply->namelen)) != rply->namelen) {
	if (!silent)
            LOG(log_error, logtype_cnid, "dbd_rpc: Error reading name from fd (db_dir %s): %s",
                db->db_dir, ret == -1?strerror(errno):"closed");
        return -1;
    }
    return 0;
}

/* -------------------- */
static int transmit(CNID_private *db, struct cnid_dbd_rqst *rqst, struct cnid_dbd_rply *rply)
{
    struct timeval tv;
    time_t orig, t;
    int silent = 1;
    
    if (db->changed) {
        /* volume and db don't have the same timestamp
        */
        return -1;
    }
    time(&orig);
    while (1) {

        if (db->fd == -1) {
            if ((db->fd = init_tsock(db)) < 0) {
                time(&t);
                if (t - orig > MAX_DELAY)
                    return -1;
	        continue;
            }
            if (db->notfirst) {
                struct cnid_dbd_rqst rqst_stamp;
                struct cnid_dbd_rply rply_stamp;
                char  stamp[ADEDLEN_PRIVSYN];
                
                dbd_initstamp(&rqst_stamp);
        	memset(stamp, 0, ADEDLEN_PRIVSYN);
                rply_stamp.name = stamp;
                rply_stamp.namelen = ADEDLEN_PRIVSYN;
                
        	if (dbd_rpc(db, &rqst_stamp, &rply_stamp, silent) < 0)
        	    goto transmit_fail;
        	if (dbd_reply_stamp(&rply_stamp ) < 0)
        	    goto transmit_fail;
        	if (memcmp(stamp, db->stamp, ADEDLEN_PRIVSYN)) {
                    LOG(log_error, logtype_cnid, "transmit: not the same db!");
        	    db->changed = 1;
        	    return -1;
        	}
            }

        }
        if (!dbd_rpc(db, rqst, rply, silent))
            return 0;

transmit_fail:
	silent = 0; /* From now on dbd_rpc and subroutines called from there
                       will log messages if something goes wrong again */
        if (db->fd != -1) {
            close(db->fd);
        }
        time(&t);
        if (t - orig > MAX_DELAY) {
	    LOG(log_error, logtype_cnid, "transmit: Request to dbd daemon (db_dir %s) timed out.", db->db_dir);
            return -1;
	}

        /* sleep a little before retry */
        db->fd = -1;
        tv.tv_usec = 0;
        tv.tv_sec  = 5;
        select(0, NULL, NULL, NULL, &tv);
    }
    return -1;
}

/* ---------------------- */
static struct _cnid_db *cnid_dbd_new(const char *volpath)
{
    struct _cnid_db *cdb;
    
    if ((cdb = (struct _cnid_db *)calloc(1, sizeof(struct _cnid_db))) == NULL)
	return NULL;
	
    if ((cdb->volpath = strdup(volpath)) == NULL) {
        free(cdb);
	return NULL;
    }
    
    cdb->flags = CNID_FLAG_PERSISTENT;
    
    cdb->cnid_add = cnid_dbd_add;
    cdb->cnid_delete = cnid_dbd_delete;
    cdb->cnid_get = cnid_dbd_get;
    cdb->cnid_lookup = cnid_dbd_lookup;
    cdb->cnid_nextid = NULL;
    cdb->cnid_resolve = cnid_dbd_resolve;
    cdb->cnid_getstamp = cnid_dbd_getstamp;
    cdb->cnid_update = cnid_dbd_update;
    cdb->cnid_close = cnid_dbd_close;
    
    return cdb;
}

/* ---------------------- */
struct _cnid_db *cnid_dbd_open(const char *dir, mode_t mask)
{
    CNID_private *db = NULL;
    struct _cnid_db *cdb = NULL;

    if (!dir) {
         return NULL;
    }
    
    if ((cdb = cnid_dbd_new(dir)) == NULL) {
        LOG(log_error, logtype_default, "cnid_open: Unable to allocate memory for database");
	return NULL;
    }
        
    if ((db = (CNID_private *)calloc(1, sizeof(CNID_private))) == NULL) {
        LOG(log_error, logtype_cnid, "cnid_open: Unable to allocate memory for database");
        goto cnid_dbd_open_fail;
    }
    
    cdb->_private = db;

    /* We keep a copy of the directory in the db structure so that we can
       transparently reconnect later. */
    strcpy(db->db_dir, dir);
    db->magic = CNID_DB_MAGIC;
    db->fd = -1;
#ifdef DEBUG
    LOG(log_info, logtype_cnid, "opening database connection to %s", db->db_dir); 
#endif
    return cdb;

cnid_dbd_open_fail:
    if (cdb != NULL) {
	if (cdb->volpath != NULL) {
	    free(cdb->volpath);
	}
	free(cdb);
    }
    if (db != NULL)
	free(db);
	
    return NULL;
}

/* ---------------------- */
void cnid_dbd_close(struct _cnid_db *cdb)
{
    CNID_private *db;

    if (!cdb) {
        LOG(log_error, logtype_afpd, "cnid_close called with NULL argument !");
	return;
    }

    if ((db = cdb->_private) != NULL) {
#ifdef DEBUG 
        LOG(log_info, logtype_cnid, "closing database connection to %s", db->db_dir);
#endif  
	if (db->fd >= 0)
	    close(db->fd);
        free(db);
    }
    
    free(cdb->volpath);
    free(cdb);
    
    return;
}

/* ---------------------- */
cnid_t cnid_dbd_add(struct _cnid_db *cdb, const struct stat *st,
                const cnid_t did, char *name, const int len,
                cnid_t hint)
{
    CNID_private *db;
    struct cnid_dbd_rqst rqst;
    struct cnid_dbd_rply rply;
    cnid_t id;

    if (!cdb || !(db = cdb->_private) || !st || !name) {
        LOG(log_error, logtype_cnid, "cnid_add: Parameter error");
        errno = CNID_ERR_PARAM;
        return CNID_INVALID;
    }

    if (len > MAXPATHLEN) {
        LOG(log_error, logtype_cnid, "cnid_add: Path name is too long");
        errno = CNID_ERR_PATH;
        return CNID_INVALID;
    }

    RQST_RESET(&rqst);
    rqst.op = CNID_DBD_OP_ADD;

    if (!(cdb->flags & CNID_FLAG_NODEV)) {
        rqst.dev = st->st_dev;
    }

    rqst.ino = st->st_ino;
    rqst.type = S_ISDIR(st->st_mode)?1:0;
    rqst.did = did;
    rqst.name = name;
    rqst.namelen = len;

    if (transmit(db, &rqst, &rply) < 0) {
        errno = CNID_ERR_DB;
        return CNID_INVALID;
    }
    
    switch(rply.result) {
    case CNID_DBD_RES_OK:
        id = rply.cnid;
        break;
    case CNID_DBD_RES_ERR_MAX:
        errno = CNID_ERR_MAX;
        id = CNID_INVALID;
        break;
    case CNID_DBD_RES_ERR_DB:
    case CNID_DBD_RES_ERR_DUPLCNID:
        errno = CNID_ERR_DB;
        id = CNID_INVALID;
        break;
    default:
        abort();
    }
    return id;
}

/* ---------------------- */
cnid_t cnid_dbd_get(struct _cnid_db *cdb, const cnid_t did, char *name,
                const int len)
{
    CNID_private *db;
    struct cnid_dbd_rqst rqst;
    struct cnid_dbd_rply rply;
    cnid_t id;


    if (!cdb || !(db = cdb->_private) || !name) {
        LOG(log_error, logtype_cnid, "cnid_get: Parameter error");
        errno = CNID_ERR_PARAM;        
        return CNID_INVALID;
    }

    if (len > MAXPATHLEN) {
        LOG(log_error, logtype_cnid, "cnid_add: Path name is too long");
        errno = CNID_ERR_PATH;
        return CNID_INVALID;
    }

    RQST_RESET(&rqst);
    rqst.op = CNID_DBD_OP_GET;
    rqst.did = did;
    rqst.name = name;
    rqst.namelen = len;

    if (transmit(db, &rqst, &rply) < 0) {
        errno = CNID_ERR_DB;
        return CNID_INVALID;
    }
    
    switch(rply.result) {
    case CNID_DBD_RES_OK:
        id = rply.cnid;
        break;
    case CNID_DBD_RES_NOTFOUND:
        id = CNID_INVALID;
        break;
    case CNID_DBD_RES_ERR_DB:
        id = CNID_INVALID;
        errno = CNID_ERR_DB;
        break;
    default: 
        abort();
    }

    return id;
}

/* ---------------------- */
char *cnid_dbd_resolve(struct _cnid_db *cdb, cnid_t *id, void *buffer, u_int32_t len)
{
    CNID_private *db;
    struct cnid_dbd_rqst rqst;
    struct cnid_dbd_rply rply;
    char *name;

    if (!cdb || !(db = cdb->_private) || !id || !(*id)) {
        LOG(log_error, logtype_cnid, "cnid_resolve: Parameter error");
        errno = CNID_ERR_PARAM;                
        return NULL;
    }

    /* TODO: We should maybe also check len. At the moment we rely on the caller
       to provide a buffer that is large enough for MAXPATHLEN plus
       CNID_HEADER_LEN plus 1 byte, which is large enough for the maximum that
       can come from the database. */

    RQST_RESET(&rqst);
    rqst.op = CNID_DBD_OP_RESOLVE;
    rqst.cnid = *id;

    /* This mimicks the behaviour of the "regular" cnid_resolve. So far,
       nobody uses the content of buffer. It only provides space for the
       name in the caller. */
    rply.name = (char *)buffer + CNID_HEADER_LEN;
    rply.namelen = len - CNID_HEADER_LEN;

    if (transmit(db, &rqst, &rply) < 0) {
        errno = CNID_ERR_DB;
        *id = CNID_INVALID;
        return NULL;
    }

    switch (rply.result) {
    case CNID_DBD_RES_OK:
        *id = rply.did;
        name = rply.name;
        break;
    case CNID_DBD_RES_NOTFOUND:
        *id = CNID_INVALID;
        name = NULL;
        break;
    case CNID_DBD_RES_ERR_DB:
        errno = CNID_ERR_DB;
        *id = CNID_INVALID;
        name = NULL;
        break;
    default:
        abort();
    }

    return name;
}

/* --------------------- */
static int dbd_getstamp(CNID_private *db, void *buffer, const size_t len)
{
    struct cnid_dbd_rqst rqst;
    struct cnid_dbd_rply rply;

    memset(buffer, 0, len);
    dbd_initstamp(&rqst);

    rply.name = buffer;
    rply.namelen = len;

    if (transmit(db, &rqst, &rply) < 0) {
        errno = CNID_ERR_DB;
        return -1;
    }
    return dbd_reply_stamp(&rply);
}

/* ---------------------- */
int cnid_dbd_getstamp(struct _cnid_db *cdb, void *buffer, const int len)
{
    CNID_private *db;
    int ret;

    if (!cdb || !(db = cdb->_private) || len != ADEDLEN_PRIVSYN) {
        LOG(log_error, logtype_cnid, "cnid_getstamp: Parameter error");
        errno = CNID_ERR_PARAM;                
        return -1;
    }
    ret = dbd_getstamp(db, buffer, len);
    if (!ret) {
    	db->notfirst = 1;
    	memcpy(db->stamp, buffer, len);
    }
    return ret;
}

/* ---------------------- */
cnid_t cnid_dbd_lookup(struct _cnid_db *cdb, const struct stat *st, const cnid_t did,
                   char *name, const int len)
{
    CNID_private *db;
    struct cnid_dbd_rqst rqst;
    struct cnid_dbd_rply rply;
    cnid_t id;

    if (!cdb || !(db = cdb->_private) || !st || !name) {
        LOG(log_error, logtype_cnid, "cnid_lookup: Parameter error");
        errno = CNID_ERR_PARAM;        
        return CNID_INVALID;
    }

    if (len > MAXPATHLEN) {
        LOG(log_error, logtype_cnid, "cnid_lookup: Path name is too long");
        errno = CNID_ERR_PATH;
        return CNID_INVALID;
    }

    RQST_RESET(&rqst);
    rqst.op = CNID_DBD_OP_LOOKUP;

    if (!(cdb->flags & CNID_FLAG_NODEV)) {
        rqst.dev = st->st_dev;
    }

    rqst.ino = st->st_ino;
    rqst.type = S_ISDIR(st->st_mode)?1:0;
    rqst.did = did;
    rqst.name = name;
    rqst.namelen = len;

    if (transmit(db, &rqst, &rply) < 0) {
        errno = CNID_ERR_DB;
        return CNID_INVALID;
    }

    switch (rply.result) {
    case CNID_DBD_RES_OK:
        id = rply.cnid;
        break;
    case CNID_DBD_RES_NOTFOUND:
        id = CNID_INVALID;
        break;
    case CNID_DBD_RES_ERR_DB:
        errno = CNID_ERR_DB;
        id = CNID_INVALID;
        break;
    default:
        abort();
    }

    return id;
}

/* ---------------------- */
int cnid_dbd_update(struct _cnid_db *cdb, const cnid_t id, const struct stat *st,
                const cnid_t did, char *name, const int len)
{
    CNID_private *db;
    struct cnid_dbd_rqst rqst;
    struct cnid_dbd_rply rply;

    
    if (!cdb || !(db = cdb->_private) || !id || !st || !name) {
        LOG(log_error, logtype_cnid, "cnid_update: Parameter error");
        errno = CNID_ERR_PARAM;        
        return -1;
    }

    if (len > MAXPATHLEN) {
        LOG(log_error, logtype_cnid, "cnid_update: Path name is too long");
        errno = CNID_ERR_PATH;
        return -1;
    }

    RQST_RESET(&rqst);
    rqst.op = CNID_DBD_OP_UPDATE;
    rqst.cnid = id;
    if (!(cdb->flags & CNID_FLAG_NODEV)) {
        rqst.dev = st->st_dev;
    }
    rqst.ino = st->st_ino;
    rqst.type = S_ISDIR(st->st_mode)?1:0;
    rqst.did = did;
    rqst.name = name;
    rqst.namelen = len;

    if (transmit(db, &rqst, &rply) < 0) {
        errno = CNID_ERR_DB;
        return -1;
    }

    switch (rply.result) {
    case CNID_DBD_RES_OK:
    case CNID_DBD_RES_NOTFOUND:
        return 0;
    case CNID_DBD_RES_ERR_DB:
        errno = CNID_ERR_DB;
        return -1;
    default:
        abort();
    }
}

/* ---------------------- */
int cnid_dbd_delete(struct _cnid_db *cdb, const cnid_t id) 
{
    CNID_private *db;
    struct cnid_dbd_rqst rqst;
    struct cnid_dbd_rply rply;


    if (!cdb || !(db = cdb->_private) || !id) {
        LOG(log_error, logtype_cnid, "cnid_delete: Parameter error");
        errno = CNID_ERR_PARAM;        
        return -1;
    }

    RQST_RESET(&rqst);
    rqst.op = CNID_DBD_OP_DELETE;
    rqst.cnid = id;

    if (transmit(db, &rqst, &rply) < 0) {
        errno = CNID_ERR_DB;
        return -1;
    }

    switch (rply.result) {
    case CNID_DBD_RES_OK:
    case CNID_DBD_RES_NOTFOUND:
        return 0;
    case CNID_DBD_RES_ERR_DB:
        errno = CNID_ERR_DB;
        return -1;
    default:
        abort();
    }
}

struct _cnid_module cnid_dbd_module = {
    "dbd",
    {NULL, NULL},
    cnid_dbd_open,
    0
};

#endif /* CNID_DBD */

