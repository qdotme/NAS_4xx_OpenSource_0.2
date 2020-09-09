/*
 * Copyright (C) 2008 Tildeslash Ltd. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED


/**
 * Global defines, macros and types
 *
 * @version \$Id: Config.h,v 1.1 2008/11/12 13:25:56 wiley Exp $
 * @file
 */


#include <assert.h>
#include <errno.h>
#include "xconfig.h"

#include "Mem.h"
#include "Str.h"
#include "Util.h"
#include "SQLException.h"
#include "AssertException.h"


/**
 * The libzdb URL
 */
#define LIBZDB_URL	"http://www.tildeslash.com/libzdb/"


/**
 * Version, copyright and contact information
 */
#define ABOUT   "Zild Database Library, version " VERSION \
                ". Copyright (C) 2008 Tildeslash Ltd. " LIBZDB_URL


/* ----------------------------------- Error, Exceptions and report macros */


/**
 * The standard abort routine
 */
#define ABORT	Util_abort


/**
 * The standard debug routine
 */
#define DEBUG	if (ZBDEBUG) Util_debug


/**
 * The standard maximum length for a checked error message 
 */
#define ERROR_SIZE      1024


/* --------------------------------------------- SQL standard value macros */


/**
 * Standard millisecond timeout value for a database call. 
 */
#define SQL_DEFAULT_TIMEOUT 3000


/**
 * The default maximum number of database connections
 */
#define SQL_DEFAULT_MAX_CONNECTIONS 20


/**
 * The initial number of database connections
 */
#define SQL_DEFAULT_INIT_CONNECTIONS 5


/**
 * The standard sweep interval in seconds for a ConnectionPool reaper thread
 */
#define SQL_DEFAULT_SWEEP_INTERVAL 60


/**
 * Default Connection timeout in seconds, used by reaper to remove
 * inactive connections
 */
#define SQL_DEFAULT_CONNECTION_TIMEOUT 30


/**
 * Default TCP/IP Connection timeout in seconds, used when connecting to
 * a database server over a TCP/IP connection
 */
#define SQL_DEFAULT_TCP_TIMEOUT 3


/**
 * MySQL default server port number
 */
#define MYSQL_DEFAULT_PORT 3306


/**
 * PostgreSQL default server port number
 */
#define POSTGRESQL_DEFAULT_PORT 5432


/* ------------------------------------------ General Purpose value macros */


/**
 * Standard String length
 */
#define STRLEN 256


/**
 * Boolean truth value
 */
#define true 1


/**
 * Boolean false value
 */
#define false 0


/** 
 * Microseconds per second 
*/
#define USEC_PER_SEC 1000000L


/** 
 * Microseconds per millisecond 
 */
#define USEC_PER_MSEC 1000L


/* ------------------------------------- General Purpose functional macros */


#define IS(a,b) ((a&&b)?Str_isEqual(a, b):0)
#define STRERROR strerror(errno)

/* SQLite timed retry macro */
#define EXEC_SQLITE(status, action, timeout) \
        do { long s = (timeout * USEC_PER_MSEC); long t = s/5; \
        do { status = (action); } while ((status == SQLITE_BUSY) \
        && (t <= s) && ((Util_usleep(t), (t+=(s/5)))));} while (0)


/* ------------------------------------------------------ Type definitions */


/**
 * The internal 8-bit char type
 */
typedef unsigned char uchar_t;


/**
 * The internal 32 bits integer type
 */
typedef  unsigned int uint32_t;


/* -------------------------------------------------------------- Globals  */


/**
 * Abort handler callback
 */
extern void(*AbortHandler)(const char *error);


/**
 * Library Debug flag. If set to true, emit debug output 
 */
extern int ZBDEBUG;


#endif


