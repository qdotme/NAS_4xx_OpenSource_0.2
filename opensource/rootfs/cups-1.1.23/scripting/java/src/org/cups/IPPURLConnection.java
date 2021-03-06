package org.cups;

/**
 * @version 1.00 06-NOV-2003
 * @author  Easy Software Products
 *
 *   Internet Printing Protocol definitions for the Common UNIX Printing
 *   System (CUPS).
 *
 *   Copyright 1997-2003 by Easy Software Products.
 *
 *   These coded instructions, statements, and computer programs are the
 *   property of Easy Software Products and are protected by Federal
 *   copyright law.  Distribution and use rights are outlined in the file
 *   "LICENSE.txt" which should have been included with this file.  If this
 *   file is missing or damaged please contact Easy Software Products
 *   at:
 *
 *       Attn: CUPS Licensing Information
 *       Easy Software Products
 *       44141 Airport View Drive, Suite 204
 *       Hollywood, Maryland 20636-3111 USA
 *
 *       Voice: (301) 373-9603
 *       EMail: cups-info@cups.org
 *         WWW: http://www.cups.org
 */

/**
 * Implementation of the URLConnection interface.
 *
 * @author	TDB
 * @version	1.0
 * @since	JDK1.3
 */

import java.util.*;
import java.io.*;
import java.net.*;

public class IPPURLConnection extends URLConnection
{

  /**
   * Constructor.
   */
  public IPPURLConnection( URL url )
  {
    super(url);
  }

  /**
   * Determine if using proxy.
   *
   * @return	<code>boolean</code>	Always <code>false</code> for now.
   */
  public boolean usingProxy()
  {
    return(false);
  }

  /**
   * Not used.
   */
  public void connect()
  {
    return;
  }

  /**
   * Not used.
   */
  public void disconnect()
  {
    return;
  }

}  // end of class
