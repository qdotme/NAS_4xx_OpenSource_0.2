dnl
dnl $Id: config.m4,v 1.1.1.1 2006/03/24 06:13:19 wiley Exp $
dnl

PHP_ARG_ENABLE(wddx,whether to enable WDDX support,
[  --enable-wddx           Enable WDDX support.])

if test "$PHP_WDDX" != "no"; then
  if test "$ext_shared" != "yes" && test "$enable_xml" = "no"; then
    AC_MSG_WARN(Activating XML)
    enable_xml=yes
  fi
  AC_DEFINE(HAVE_WDDX, 1, [ ])
  PHP_NEW_EXTENSION(wddx, wddx.c, $ext_shared)
fi
