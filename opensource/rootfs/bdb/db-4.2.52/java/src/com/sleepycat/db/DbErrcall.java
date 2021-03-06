/*
 *  -
 *  See the file LICENSE for redistribution information.
 *
 *  Copyright (c) 1997-2003
 *  Sleepycat Software.  All rights reserved.
 *
 *  $Id: DbErrcall.java,v 1.1.1.1 2008/06/18 10:53:15 jason Exp $
 */
package com.sleepycat.db;

/**
 * @deprecated    As of Berkeley DB 4.2, replaced by {@link
 *      DbErrorHandler}
 */
public interface DbErrcall {
    /**
     * @deprecated     As of Berkeley DB 4.2, replaced by {@link
     *      DbErrorHandler#error(String,String)}
     */
    public abstract void errcall(String prefix, String buffer);
}
