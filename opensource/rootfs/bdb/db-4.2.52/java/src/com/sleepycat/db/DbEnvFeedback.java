/*
 *  -
 *  See the file LICENSE for redistribution information.
 *
 *  Copyright (c) 1999-2003
 *  Sleepycat Software.  All rights reserved.
 *
 *  $Id: DbEnvFeedback.java,v 1.1.1.1 2008/06/18 10:53:15 jason Exp $
 */
package com.sleepycat.db;

/**
 * @deprecated    As of Berkeley DB 4.2, replaced by {@link
 *      DbEnvFeedbackHandler}
 */
public interface DbEnvFeedback {
    /**
     * @param  env
     * @param  opcode
     * @param  percent
     * @deprecated      As of Berkeley DB 4.2, replaced by {@link
     *      DbEnvFeedbackHandler#feedback(DbEnv,int,int)
     *      DbEnvFeedbackHandler.feedback(DbEnv,int,int)}
     */
    public abstract void feedback(DbEnv env, int opcode, int percent);
}
