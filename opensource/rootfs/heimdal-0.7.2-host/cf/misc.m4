
dnl $Id: misc.m4,v 1.1.1.1 2007/01/11 02:33:18 wiley Exp $
dnl
AC_DEFUN([upcase],[`echo $1 | tr abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ`])dnl
AC_DEFUN([rk_LIBOBJ],[AC_LIBOBJ([$1])])dnl
AC_DEFUN([rk_CONFIG_HEADER],[AH_TOP([#ifndef RCSID
#define RCSID(msg) \
static /**/const char *const rcsid[] = { (const char *)rcsid, "@(#)" msg }
#endif

/* Maximum values on all known systems */
#define MaxHostNameLen (64+4)
#define MaxPathLen (1024+4)

])])