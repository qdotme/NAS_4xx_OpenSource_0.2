/*
 *  -
 *  See the file LICENSE for redistribution information.
 *
 *  Copyright (c) 1997-2003
 *  Sleepycat Software.  All rights reserved.
 *
 *  $Id: DbEnvFeedbackHandler.java,v 1.1.1.1 2008/06/18 10:53:15 jason Exp $
 */
package com.sleepycat.db;

/**
 *  The DbEnvFeedbackHandler interface is used by the
 *  DbEnv.setFeedback method. This interface defines the
 *  application-specific function to be called to to report Berkeley
 *  DB operation progress.
 */
public interface DbEnvFeedbackHandler {
    /**
     *  The DbEnvFeedbackHandler interface is used by the
     *  DbEnv.setFeedback method. This interface defines the
     *  application-specific function to be called to to report
     *  Berkeley DB operation progress. <p>
     *
     *  </p>
     *
     * @param  dbenv    a reference to the enclosing database
     *      environment.
     * @param  opcode   an operation code. The <b>opcode</b> parameter
     *      may take on any of the following values:
     *      <ul>
     *        <li> {@link com.sleepycat.db.Db#DB_RECOVER
     *        Db.DB_RECOVER}<p>
     *
     *        The environment is being recovered. </li>
     *      </ul>
     *
     * @param  percent  the percent of the operation that has been
     *      completed, specified as an integer value between 0 and
     *      100.
     */
    public abstract void feedback(DbEnv dbenv, int opcode, int percent);
}
