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
   | Authors: Rasmus Lerdorf <rasmus@php.net>                             |
   |          Jim Winstead <jimw@php.net>                                 |
   |          Hartmut Holzgraefe <hholzgra@php.net>                       |
   +----------------------------------------------------------------------+
*/

/* $Id: php_fopen_wrappers.h,v 1.1.1.1 2006/03/24 06:13:23 wiley Exp $ */

#ifndef PHP_FOPEN_WRAPPERS_H
#define PHP_FOPEN_WRAPPERS_H

php_stream *php_stream_url_wrap_http(php_stream_wrapper *wrapper, char *path, char *mode, int options, char **opened_path, php_stream_context *context STREAMS_DC TSRMLS_DC);
php_stream *php_stream_url_wrap_ftp(php_stream_wrapper *wrapper, char *path, char *mode, int options, char **opened_path, php_stream_context *context STREAMS_DC TSRMLS_DC);
extern php_stream_wrapper php_stream_http_wrapper;
extern php_stream_wrapper php_stream_ftp_wrapper;
extern php_stream_wrapper php_stream_php_wrapper;

#endif
