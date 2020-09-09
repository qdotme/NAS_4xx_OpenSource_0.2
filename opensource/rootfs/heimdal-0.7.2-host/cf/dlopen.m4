dnl
dnl $Id: dlopen.m4,v 1.1.1.1 2007/01/11 02:33:18 wiley Exp $
dnl

AC_DEFUN([rk_DLOPEN], [
	AC_FIND_FUNC_NO_LIBS(dlopen, dl)
	AM_CONDITIONAL(HAVE_DLOPEN, test "$ac_cv_funclib_dlopen" != no)
])
