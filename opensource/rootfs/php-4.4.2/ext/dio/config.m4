dnl
dnl $Id: config.m4,v 1.1.1.1 2006/03/24 06:13:22 wiley Exp $
dnl 

PHP_ARG_ENABLE(dio, whether to enable direct I/O support,
[  --enable-dio            Enable direct I/O support])

if test "$PHP_DIO" != "no"; then
  PHP_NEW_EXTENSION(dio, dio.c, $ext_shared)
fi
