dnl
dnl $Id: config.m4,v 1.1.1.1 2006/03/24 06:13:22 wiley Exp $
dnl

AC_ARG_WITH(dbase,[],[enable_dbase=$withval])

PHP_ARG_ENABLE(dbase,whether to enable dbase support,
[  --enable-dbase          Enable the bundled dbase library])

if test "$PHP_DBASE" = "yes"; then
  AC_DEFINE(DBASE,1,[ ])
  PHP_NEW_EXTENSION(dbase, dbf_head.c dbf_rec.c dbf_misc.c dbf_ndx.c dbase.c, $ext_shared)
fi
