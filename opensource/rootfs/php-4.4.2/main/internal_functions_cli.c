/* -*- C -*-
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
   | Authors: Andi Gutmans <andi@zend.com>                                |
   |          Zeev Suraski <zeev@zend.com>                                |
   +----------------------------------------------------------------------+
 */


/* $Id: internal_functions.c.in,v 1.1.1.1 2006/03/24 06:13:15 wiley Exp $ */

#include "php.h"
#include "php_main.h"
#include "zend_modules.h"
#include "internal_functions_registry.h"
#include "zend_compile.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "ext/openssl/php_openssl.h"
#include "ext/pcre/php_pcre.h"
#include "ext/zlib/php_zlib.h"
#include "ext/ctype/php_ctype.h"
#include "ext/mbstring/mbstring.h"
#include "ext/mysql/php_mysql.h"
#include "ext/overload/php_overload.h"
#include "ext/pcntl/php_pcntl.h"
#include "ext/posix/php_posix.h"
#include "ext/session/php_session.h"
#include "ext/standard/php_standard.h"
#include "ext/tokenizer/php_tokenizer.h"
#include "ext/xml/php_xml.h"


zend_module_entry *php_builtin_extensions[] = {
	phpext_xml_ptr,
	phpext_tokenizer_ptr,
	phpext_standard_ptr,
	phpext_session_ptr,
	phpext_posix_ptr,
	phpext_pcntl_ptr,
	phpext_overload_ptr,
	phpext_mysql_ptr,
	phpext_mbstring_ptr,
	phpext_ctype_ptr,
	phpext_zlib_ptr,
	phpext_pcre_ptr,
	phpext_openssl_ptr,

};

#define EXTCOUNT (sizeof(php_builtin_extensions)/sizeof(zend_module_entry *))
	

int php_startup_internal_extensions(void)
{
	return php_startup_extensions(php_builtin_extensions, EXTCOUNT);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
