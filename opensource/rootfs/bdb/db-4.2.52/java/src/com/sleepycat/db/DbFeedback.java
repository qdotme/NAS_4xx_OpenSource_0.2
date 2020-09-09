/*
 *  -
 *  See the file LICENSE for redistribution information.
 *
 *  Copyright (c) 1997-2003
 *  Sleepycat Software.  All rights reserved.
 *
 *  $Id: DbFeedback.java,v 1.1.1.1 2008/06/18 10:53:15 jason Exp $
 */
package com.sleepycat.db;

/**
 * @deprecated    As of Berkeley DB 4.2, replaced by {@link
 *      DbFeedbackHandler}
 */
public interface DbFeedback {
    /**
     * @deprecated      As of Berkeley DB 4.2, replaced by {@link
     *      DbFeedbackHandler#feedback(Db,int,int)
     *      DbFeedbackHandler.feedback(Db,int,int)}
     */
    public abstract void feedback(Db db, int opcode, int percent);
}
