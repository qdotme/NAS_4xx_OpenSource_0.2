# getopt.m4 serial 6
dnl Copyright (C) 2002, 2003, 2004 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

# The getopt module assume you want GNU getopt, with getopt_long etc,
# rather than vanilla POSIX getopt.  This means your your code should
# always include <getopt.h> for the getopt prototypes.

AC_DEFUN([gl_GETOPT_SUBSTITUTE],
[
  GETOPT_H=getopt.h
  AC_LIBOBJ([getopt])
  AC_LIBOBJ([getopt1])
  AC_DEFINE([__GETOPT_PREFIX], [[rpl_]],
    [Define to rpl_ if the getopt replacement functions and variables
     should be used.])
  AC_SUBST([GETOPT_H])
])

AC_DEFUN([gl_GETOPT],
[
  gl_PREREQ_GETOPT

  if test -z "$GETOPT_H"; then
    GETOPT_H=
    AC_CHECK_HEADERS([getopt.h], [], [GETOPT_H=getopt.h])
    AC_CHECK_FUNCS([getopt_long_only], [], [GETOPT_H=getopt.h])

    dnl BSD getopt_long uses an incompatible method to reset option processing,
    dnl and (as of 2004-10-15) mishandles optional option-arguments.
    AC_CHECK_DECL([optreset], [GETOPT_H=getopt.h], [], [#include <getopt.h>])

    if test -n "$GETOPT_H"; then
      gl_GETOPT_SUBSTITUTE
    fi
  fi
])

# Prerequisites of lib/getopt*.
AC_DEFUN([gl_PREREQ_GETOPT], [:])
