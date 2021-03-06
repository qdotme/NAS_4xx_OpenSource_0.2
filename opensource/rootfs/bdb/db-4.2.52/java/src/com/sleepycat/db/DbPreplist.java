/*
 *  -
 *  See the file LICENSE for redistribution information.
 *
 *  Copyright (c) 1999-2003
 *  Sleepycat Software.  All rights reserved.
 *
 *  $Id: DbPreplist.java,v 1.1.1.1 2008/06/18 10:53:15 jason Exp $
 */
package com.sleepycat.db;

/**
 *  The DbPreplist object is used to encapsulate a single prepared,
 *  but not yet resolved, transaction.</p>
 */
public class DbPreplist {
    /**
     *  The global transaction ID for the transaction. The global
     *  transaction ID is the one specified when the transaction was
     *  prepared. The application is responsible for ensuring
     *  uniqueness among global transaction IDs.
     *</ul>
     *
     */
    public byte[] gid;

    /**
     *  The transaction handle for the transaction.
     *</ul>
     *
     */
    public DbTxn txn;


    /**
     *  Create a new DbPreplist, this is only done by the DbEnv class.
     *  <p>
     *
     *  A global transaction ID is the one specified when the
     *  transaction was prepared. The application is responsible for
     *  ensuring uniqueness among global transaction IDs.
     *
     * @param  txn  The transaction handle for the transaction.
     * @param  gid  An array of all the global transaction IDs which
     *      must be resolved for this transaction.
     */
    DbPreplist(DbTxn txn, byte[] gid) {
        this.txn = txn;
        this.gid = gid;
    }
}
