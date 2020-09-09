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


#ifndef CONNECTION_INCLUDED
#define CONNECTION_INCLUDED


/**
 * A <b>Connection</b> class represent a connection to a SQL database system.
 *
 * A Connection is used to execute SQL statements and return the result. 
 * There are three ways to execute a SQL statement: Connection_execute() 
 * is used to execute SQL statements that does not return a result set. 
 * Such statements are INSERT, UPDATE or DELETE.  Connection_executeQuery() 
 * is used to execute a SQL SELECT statement and return the result set. 
 * These methods can only handle values which can be expressed as C-strings. 
 * If you need to handle binary data, such as inserting a blob value into the
 * database, use a PreparedStatement object to execute the SQL statement. 
 * The factory method Connection_prepareStatement() is used to obtain a 
 * PreparedStatement object. 
 *
 * If an error occurred during execution, the method
 * Connection_getLastError() can be used to obtain a string describing
 * the error. The method Connection_executeQuery() will return an
 * empty ResultSet (not NULL) if the SQL statement did not return any
 * values.  NULL is <i>only</i> returned if an error occurred. A
 * ResultSet lives until the next call to Connection_executeQuery() or
 * until the Connection is returned to the Connection Pool.
 *
 * Any SQL statement that changes the database (basically, any SQL
 * command other than SELECT) will automatically start a transaction
 * if one is not already in effect. Automatically started transactions
 * are committed at the conclusion of the command.
 *
 * Transactions can also be started manually using the
 * Connection_beginTransaction() method. Such transactions usually
 * persist until the next call to Connection_commit() or
 * Connection_rollback(). A transaction will also ROLLBACK if the
 * database is closed or if an error occurs. Nested transactions are not 
 * allowed.
 *
 * @see ResultSet.h PreparedStatement.h SQLException.h
 * @version \$Id: Connection.h,v 1.1 2008/11/12 13:25:56 wiley Exp $
 * @file
 */


#define T Connection_T
typedef struct T *T;

//<< Start filter-out

/**
 * Create a new Connection.
 * @param pool The parent connection pool
 * @param error Connection error or NULL if no error was found
 * @return A new Connection object or NULL on error
 */
T Connection_new(void *pool, char **error);


/**
 * Destroy a Connection and release allocated resources.
 * @param C A Connection object reference
 */
void Connection_free(T *C);


/**
 * Set if this Connection is available and not already in use.
 * @param C A Connection object
 * @param isAvailable true if this Connection is available otherwise false
 */
void Connection_setAvailable(T C, int isAvailable);


/**
 * Get the availablity of this Connection.
 * @param C A Connection object
 * @return true if this Connection is available otherwise false
 */
int Connection_isAvailable(T C);


/**
 * Return the last time this Connection was accessed from the Connection Pool.
 * The time is returned as the number of seconds since midnight, January 1, 
 * 1970 GMT. Actions that your application takes, such as calling the public
 * methods of this class does not affect this time.
 * @param C A Connection object
 * @return The last time (seconds) this Connection was accessed
 */
long Connection_getLastAccessedTime(T C);


/**
 * Return true if this Connection is in a transaction that has not
 * been committed.
 * @param C A Connection object
 * @return true if this Connection is in a transaction otherwise false
 */
int Connection_isInTransaction(T C);

//>> End filter-out

/** @name Properties */
//@{

/**
 * Sets the number of milliseconds the Connection should wait for a
 * SQL statement to finish if the database is busy. If the limit is
 * exceeded, then the <code>execute</code> methods will return
 * immediately with an error. The default timeout is <code>3
 * seconds</code>.
 * @param C A Connection object
 * @param ms The query timeout limit in milliseconds; zero means
 * there is no limit
 */
void Connection_setQueryTimeout(T C, int ms);


/**
 * Retrieves the number of milliseconds the Connection will wait for a
 * SQL statement object to execute.
 * @param C A Connection object
 * @return The query timeout limit in milliseconds; zero means there
 * is no limit
 */
int Connection_getQueryTimeout(T C);


/**
 * Sets the limit for the maximum number of rows that any ResultSet
 * object can contain. If the limit is exceeded, the excess rows 
 * are silently dropped.
 * @param C A Connection object
 * @param max the new max rows limit; 0 means there is no limit
 */
void Connection_setMaxRows(T C, int max);


/**
 * Retrieves the maximum number of rows that a ResultSet object
 * produced by this Connection object can contain. If this limit is
 * exceeded, the excess rows are silently dropped.
 * @param C A Connection object
 * @return the new max rows limit; 0 means there is no limit
 */
int Connection_getMaxRows(T C);


/**
 * Returns this Connection URL
 * @param C A Connection object
 * @return This Connection URL
 * @see URL.h
 */
URL_T Connection_getURL(T C);

//@}

/**
 * Ping the database server and returns true if this Connection is 
 * alive, otherwise false in which case the Connection should be closed. 
 * @param C A Connection object
 * @return true if Connection is connected to a database server 
 * otherwise false
 */
int Connection_ping(T C);


/**
 * Close any ResultSet and PreparedStatements in the Connection. 
 * Normally it is not necessary to call this method, but for some
 * implementation (SQLite) it <i>may, in some situations,</i> be 
 * necessary to call this if PreparedStatement_executeQuery() was used.
 * @param C A Connection object
 */
void Connection_clear(T C);


/**
 * Return connection to the connection pool. The same as calling 
 * ConnectionPool_returnConnection() on a connection.
 * @param C A Connection object
 */
void Connection_close(T C);


/**
 * Start a transaction. 
 * @param C A Connection object
 * @return true on success otherwise false
 * @exception SQLException if a database error occurs
 * @see SQLException.h
 */
int Connection_beginTransaction(T C);


/**
 * Makes all changes made since the previous commit/rollback permanent
 * and releases any database locks currently held by this Connection
 * object.
 * @param C A Connection object
 * @return true on sucess otherwise false
 * @exception SQLException if a database error occurs
 * @see SQLException.h
 */
int Connection_commit(T C);


/**
 * Undoes all changes made in the current transaction and releases any
 * database locks currently held by this Connection object.
 * @param C A Connection object
 * @return true on sucess otherwise false
 * @exception SQLException if a database error occurs
 * @see SQLException.h
 */
int Connection_rollback(T C);


/**
 * Returns the value for the most recent INSERT statement into a 
 * table with an AUTO_INCREMENT or INTEGER PRIMARY KEY column.
 * @param C A Connection object
 * @return The value of the rowid from the last insert statement
 */
long long int Connection_lastRowId(T C);


/**
 * Returns the number of rows that was inserted, deleted or modified
 * by the last Connection_execute() statement. If used with a
 * transaction, this method should be called <i>before</i> commit is
 * executed, otherwise 0 is returned.
 * @param C A Connection object
 * @return The number of rows changed by the last (DIM) SQL statement
 */
long long int Connection_rowsChanged(T C);


/**
 * Executes the given SQL statement, which may be an INSERT, UPDATE,
 * or DELETE statement or an SQL statement that returns nothing, such
 * as an SQL DDL statement. Several SQL statements can be used in the
 * sql parameter string, each separated with the <i>;</i> SQL
 * statement separator character. <b>Note</b>, calling this method
 * clears any previous ResultSets associated with the Connection.
 * @param C A Connection object
 * @param sql A SQL statement
 * @return true on success otherwise false on error and the method
 * Connection_getLastError() can be used to obtain the error string.
 * @exception SQLException if a database error occurs
 * @see SQLException.h
 */
int Connection_execute(T C, const char *sql, ...);


/**
 * Executes the given SQL statement, which returns a single ResultSet
 * object. You may <b>only</b> use one SQL statement with this method.
 * This is different from the behavior of Connection_execute() which
 * executes all SQL statements in its input string. If the sql
 * parameter string contains more than one SQL statement, only the
 * first statement is executed, the others are silently ignored.
 * A ResultSet "lives" only until the next call to
 * Connection_executeQuery(), Connection_execute() or until the 
 * Connection is returned to the Connection Pool. <i>This means that 
 * Result Sets cannot be saved between queries</i>.
 * @param C A Connection object
 * @param sql A SQL statement
 * @return A ResultSet object that contains the data produced by the
 * given query. If there was an error, NULL is returned and the method
 * Connection_getLastError() can be used to obtain the error string.
 * @exception SQLException if a database error occurs
 * @see ResultSet.h
 * @see SQLException.h
 */
ResultSet_T Connection_executeQuery(T C, const char *sql, ...);


/**
 * Creates a PreparedStatement object for sending parameterized SQL 
 * statements to the database. The <code>sql</code> parameter may 
 * contain IN parameter placeholders. An IN placeholder is specified 
 * with a '?' character in the sql string. The placeholders are 
 * then replaced with actual values by using the PreparedStatement's 
 * setXXX methods. Only <i>one</i> SQL statement may be used in the sql 
 * parameter, this in difference to Connection_execute() which may 
 * take several statements. A PreparedStatement "lives" until the 
 * Connection is returned to the Connection Pool. 
 * @param C A Connection object
 * @param sql a single SQL statement that may contain one or more '?' 
 * IN parameter placeholders
 * @return a new PreparedStatement object containing the pre-compiled
 * SQL statement or NULL if a database error occurred. If NULL was 
 * returned, the method Connection_getLastError() can be used to obtain
 * the error string.
 * @exception SQLException if a database error occurs
 * @see PreparedStatement.h
 * @see SQLException.h
 */
PreparedStatement_T Connection_prepareStatement(T C, const char *sql);


/**
 * This method can be used to obtain a string describing the last
 * error that occurred. 
 * @param C A Connection object
 * @return A string explaining the last error
 */
const char *Connection_getLastError(T C);


/** @name class methods */
//@{

/**
 * <b>Class method</b>, test if the specified database system is 
 * supported by this library. Clients may pass a full Connection URL, 
 * for example using URL_toString(), or for convenience only the protocol
 * part of the URL. E.g. "mysql" or "sqlite".
 * @param url a database url string
 * @return true if supported otherwise false
 */
int Connection_isSupported(const char *url);

// @}

#undef T
#endif
