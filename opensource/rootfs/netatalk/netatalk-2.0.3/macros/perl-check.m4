dnl Autoconf macro to check for the existence of Perl
dnl $Id: perl-check.m4,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $

AC_DEFUN([AC_PROG_PERL], [
	AC_REQUIRE([AC_EXEEXT])dnl
	test "x$PERL" = x && AC_PATH_PROG(PERL, perl$EXEEXT, perl$EXEEXT)
	test "x$PERL" = x && AC_MSG_ERROR([no acceptable Perl found in \$PATH])
])

AC_SUBST(PERL)
