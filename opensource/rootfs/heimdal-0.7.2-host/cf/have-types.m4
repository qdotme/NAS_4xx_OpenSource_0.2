dnl
dnl $Id: have-types.m4,v 1.1.1.1 2007/01/11 02:33:18 wiley Exp $
dnl

AC_DEFUN([AC_HAVE_TYPES], [
for i in $1; do
        AC_HAVE_TYPE($i)
done
if false;then
	AC_CHECK_FUNCS($1)
fi
])
