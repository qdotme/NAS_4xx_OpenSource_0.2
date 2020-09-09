/*
   +----------------------------------------------------------------------+
   | PHP Version 4                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2006 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Jason Greene <jason@inetgurus.net>                           |
   +----------------------------------------------------------------------+
*/

/* $Id: php_signal.h,v 1.1.1.1 2006/03/24 06:13:19 wiley Exp $ */

#include <signal.h>
#ifndef PHP_SIGNAL_H
#define PHP_SIGNAL_H

typedef void Sigfunc(int);
Sigfunc *php_signal(int signo, Sigfunc *func, int restart);

#endif
