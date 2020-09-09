dnl $Id: quota-check.m4,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
dnl Autoconf macro to check for quota support
dnl FIXME: This is in now way complete.

AC_DEFUN([AC_CHECK_QUOTA], [
	QUOTA_LIBS=""
	netatalk_cv_quotasupport="yes"
	AC_CHECK_LIB(rpcsvc, main, [QUOTA_LIBS="-lrpcsvc"])
	AC_CHECK_HEADERS([rpc/rpc.h rpc/pmap_prot.h rpcsvc/rquota.h],[],[
		QUOTA_LIBS=""
		netatalk_cv_quotasupport="no"
		AC_DEFINE(NO_QUOTA_SUPPORT, 1, [Define if quota support should not compiled])
	])

	AC_SUBST(QUOTA_LIBS)
])

