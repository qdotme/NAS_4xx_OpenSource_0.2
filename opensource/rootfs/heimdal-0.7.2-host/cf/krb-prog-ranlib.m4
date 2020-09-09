dnl $Id: krb-prog-ranlib.m4,v 1.1.1.1 2007/01/11 02:33:18 wiley Exp $
dnl
dnl
dnl Also look for EMXOMF for OS/2
dnl

AC_DEFUN([AC_KRB_PROG_RANLIB],
[AC_CHECK_PROGS(RANLIB, ranlib EMXOMF, :)])
