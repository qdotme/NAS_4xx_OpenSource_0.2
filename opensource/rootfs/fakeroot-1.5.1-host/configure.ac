dnl Process this file with autoconf to produce a configure script.
AC_INIT([fakeroot],[FAKEROOT_VERSION],[schizo@debian.org],[fakeroot])
AC_PREREQ(2.59)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE
AC_CONFIG_HEADERS([config.h])
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL
AC_PROG_CC

AC_ARG_WITH([ipc],
	    AS_HELP_STRING([--with-ipc@<:@=IPCTYPE@:>@],
			   [method of IPC to use: either sysv (default) or tcp]),
	    [ac_cv_use_ipc=$withval],
	    [ac_cv_use_ipc=sysv])

AC_CACHE_CHECK([which IPC method to use],
	       [ac_cv_use_ipc],
	       [ac_cv_use_ipc=sysv])

AC_ARG_WITH([dbformat],
	    AS_HELP_STRING([--with-dbformat@<:@=DBFORMAT@:>@],
			   [database format to use: either inode (default) or path]),
	    [ac_cv_dbformat=$withval],
	    [ac_cv_dbformat=inode])

AC_CACHE_CHECK([which database format to use],
	       [ac_cv_dbformat],
	       [ac_cv_dbformat=inode])

AH_TEMPLATE([FAKEROOT_DB_PATH], [store path in the database instead of inode and device])
if test $ac_cv_dbformat = "path"; then
AC_DEFINE_UNQUOTED(FAKEROOT_DB_PATH, [1])
fi

dnl Checks for programs.

dnl Checks for libraries.
dnl Replace `main' with a function in -ldl:
AC_CHECK_LIB(dl, dlopen)
AH_TEMPLATE([FAKEROOT_FAKENET], [use TCP instead of SysV IPC])
if test $ac_cv_use_ipc = "tcp"; then
AC_DEFINE_UNQUOTED(FAKEROOT_FAKENET, [TCP])
AC_CHECK_LIB(pthread, pthread_self)
AC_CHECK_LIB(socket, connect)
signal=HUP
else
signal=TERM
fi

AC_SUBST(signal)

dnl Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h unistd.h features.h sys/feature_tests.h pthread.h stdint.h inttypes.h grp.h endian.h sys/sysmacros.h sys/socket.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_CHECK_TYPE(mode_t, int)
AC_CHECK_TYPE(off_t, long)
AC_CHECK_TYPE(size_t, unsigned)

AH_TEMPLATE([FAKEROOT_ATTR], [for packed])
if test -n "$GCC";
then
AC_DEFINE_UNQUOTED(FAKEROOT_ATTR(x), [__attribute__ ((x))])
fi

dnl The parameters to the wrapped functions have to match
dnl those in the system header files. I don't know how to
dnl really get the names of the paramters, but this seems to work.
AC_MSG_CHECKING([for second and third argument of readlink])
readlink_buf_arg=unknown
readlink_bufsize_arg=unknown
for second in void char; do
  for third in size_t int; do
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <unistd.h>
      int readlink(const char *path, $second *buf, $third bufsiz);]], [[puts("hello, world");]])],[readlink_buf_arg=$second
       readlink_bufsize_arg=$third
      ],[])
  done
done
AC_MSG_RESULT([$readlink_buf_arg, $readlink_bufsize_arg])
AH_TEMPLATE([READLINK_BUF_TYPE], [type of readlink buf])
AH_TEMPLATE([READLINK_BUFSIZE_TYPE], [type of readlink bufsize])
AC_DEFINE_UNQUOTED(READLINK_BUF_TYPE, $readlink_buf_arg)
AC_DEFINE_UNQUOTED(READLINK_BUFSIZE_TYPE, $readlink_bufsize_arg)

AC_MSG_CHECKING([for first argument of setgroups])
setgroups_size_arg=unknown
for first in size_t int; do
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#define _BSD_SOURCE
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <unistd.h>
#ifdef HAVE_GRP_H
#include <grp.h>
#endif
  int setgroups($first size, const gid_t *list);]], [[puts("hello, world");]])],[setgroups_size_arg=$first],[])
done
AC_MSG_RESULT([$setgroups_size_arg])
AH_TEMPLATE([SETGROUPS_SIZE_TYPE], [type of setgroups size])
AC_DEFINE_UNQUOTED(SETGROUPS_SIZE_TYPE, $setgroups_size_arg)


AH_TEMPLATE([HAVE_SEMUN_DEF], [have the semun union])
AC_MSG_CHECKING([for union semun])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
       # include <sys/types.h>
       # include <sys/ipc.h>
       # include <sys/sem.h>
  ]], [[  
       union semun s;
  ]])],[AC_DEFINE(HAVE_SEMUN_DEF)
   AC_MSG_RESULT([yes])  
  ],[ AC_MSG_RESULT([no])
  ])

AH_TEMPLATE([XMKNOD_FRTH_ARG], [fourth argument of __xmknod])
dnl glibc uses `* dev' as fourth argument of __xmknod.
dnl Although the test below should probably be more general
dnl (not just __xmknod, but also mknod etc), at the moment this
dnl seems enough, as probably glibc is the only that does this.
AC_MSG_CHECKING([for type of arg of __xmknod])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
       #include <unistd.h>
  ]], [[  
       int __xmknod  ( int ver, 
                       const char *pathname ,  
                       mode_t  mode ,  dev_t dev);
  ]])],[
   AC_DEFINE(XMKNOD_FRTH_ARG,)
   AC_MSG_RESULT([no extra *])
  ],[ 
   AC_DEFINE(XMKNOD_FRTH_ARG,[*])
   AC_MSG_RESULT([needs *])  
  
  ])

AH_TEMPLATE([INITGROUPS_SECOND_ARG], [second argument of initgroups])
dnl FreeBSD 4.7 uses int instead of gid_t
AC_MSG_CHECKING([for type of arg of initgroups])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
       #include <sys/types.h>
       #include <unistd.h>
  ]], [[  
       int initgroups ( const char *user, gid_t group );
  ]])],[
   AC_DEFINE(INITGROUPS_SECOND_ARG, gid_t)
   AC_MSG_RESULT([gid_t])
  ],[ 
   AC_DEFINE(INITGROUPS_SECOND_ARG, int)
   AC_MSG_RESULT([not gid_t; will assume int])  
  ])

AH_TEMPLATE([SETREUID_ARG], [argument of setreuid])
dnl OpenBSD 2.8 uses int instead of uid_t
AC_MSG_CHECKING([for type of arg of setreuid])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
       #include <sys/types.h>
       #include <unistd.h>
  ]], [[  
       int setreuid ( uid_t ruid, uid_t euid );
  ]])],[
   AC_DEFINE(SETREUID_ARG, gid_t)
   AC_MSG_RESULT([gid_t])
  ],[ 
   AC_DEFINE(SETREUID_ARG, int)
   AC_MSG_RESULT([not uid_t; will assume int])  
  ])

AH_TEMPLATE([SETREGID_ARG], [argument of setregid])
dnl OpenBSD 2.8 uses int instead of gid_t
AC_MSG_CHECKING([for type of arg of setregid])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
       #include <sys/types.h>
       #include <unistd.h>
  ]], [[  
       int setreuid ( gid_t rgid, gid_t egid );
  ]])],[
   AC_DEFINE(SETREGID_ARG, gid_t)
   AC_MSG_RESULT([gid_t])
  ],[ 
   AC_DEFINE(SETREGID_ARG, int)
   AC_MSG_RESULT([not gid_t; will assume int])  
  ])

AH_TEMPLATE([STAT_SECOND_ARG], [second argument of stat])
dnl Tru64 or something uses stat * instead of struct stat *
AC_MSG_CHECKING([for type of second arg to stat])
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
       #include <sys/stat.h>
       #include <sys/types.h>
       #include <unistd.h>
  ]], [[  
       int stat ( const char *file_name, struct stat *buf);
  ]])],[
   AC_DEFINE(STAT_SECOND_ARG, struct stat *)
   AC_MSG_RESULT([struct stat *])
  ],[ 
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
       #include <sys/stat.h>
       #include <sys/types.h>
       #include <unistd.h>
  ]], [[  
       int stat ( const char *file_name, stat *buf);
  ]])],[
   AC_DEFINE(STAT_SECOND_ARG, stat *)
   AC_MSG_RESULT([stat *])
  ],[ 
  AC_MSG_ERROR(cannot determine second stat argument)
  ])
])

for field in d_off d_type; do
AC_MSG_CHECKING([for ${field} in struct dirent])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
       #include <sys/types.h>
       #include <dirent.h>
  ]], [[  

       struct dirent d;
       d.${field}=0
  ]])],[AC_DEFINE_UNQUOTED(STAT_HAS_${field},1)
   AC_MSG_RESULT([yes])  
  ],[ AC_MSG_RESULT([no])
  ])
done

dnl find out how stat() etc are called. On linux systems, we really
dnl need to wrap __xstat(), on Solaris _stat, on Digital Unix stat() (IIRC)

:>fakerootconfig.h.tmp

for SEARCH in %stat f%stat l%stat %stat64 f%stat64 l%stat64 %mknod; do
  FUNC=`echo $SEARCH|sed -e 's/.*%//'`
  PRE=`echo $SEARCH|sed -e 's/%.*//'`
  FOUND=
  for WRAPPED in __${PRE}x${FUNC} __${PRE}${FUNC}13 ${PRE}${FUNC}; do
    AC_CHECK_FUNCS($WRAPPED,FOUND=$WRAPPED)
dnl
dnl to unconditionally define only the _* functions, comment out the 2 lines above,
dnl and uncomment the 2 lines below.
dnl
dnl  for WRAPPED in _${PRE}${FUNC}; do
dnl    FOUND=$WRAPPED
    if test -n "$FOUND"; then
      PF=`echo ${PRE}${FUNC}| tr '[[:lower:]]' '[[:upper:]]'` 
      DEFINE_WRAP=`echo wrap_${PRE}${FUNC}| tr '[[:lower:]]' '[[:upper:]]'` 
      DEFINE_NEXT=`echo wrap_${FOUND}| tr '[[:lower:]]' '[[:upper:]]'` 
      DEFINE_ARG=`echo wrap_${FOUND}| tr '[[:lower:]]' '[[:upper:]]'` 
      AC_DEFINE_UNQUOTED(WRAP_${PF}, $FOUND)
      AC_DEFINE_UNQUOTED(WRAP_${PF}_QUOTE, "$FOUND")
      AC_DEFINE_UNQUOTED(TMP_${PF}, tmp_$FOUND)
      AC_DEFINE_UNQUOTED(NEXT_${PF}_NOARG, next_$FOUND)
      if test __"${PRE}x${FUNC}" != "${WRAPPED}"; then
        DEF_BEGIN=""
      else
        DEF_BEGIN="a,"
      fi
      if test "${FUNC}" = "mknod"; then
        DEF_END=",d"
      else
        DEF_END=""
      fi
      dnl no matter what I do, the resulting define looks like
      dnl #define macro (a,b,c) (a,b,c)
      dnl with a space in between "macro" and "(". To prevent this,
      dnl I leave the space here, and remove it later with sed 
      dnl at (end of configure.in)
      dnl AC_DEFINE_UNQUOTED(NEXT_${PF}(a,b,c${DEF_END}), 
      dnl 		     next_$FOUND(${DEF_BEGIN}b,c${DEF_END}))
      dnl AC_DEFINE_UNQUOTED(${PF}[_ARG(a,b,c${DEF_END})], 
      dnl 		     (${DEF_BEGIN}b,c${DEF_END}))

      dnl Anyway the trickery above also leads to automake problems
      dnl (tries to remake config.h.in, but fails). So, as a way
      dnl out, Yann DIRSON wrote this:
      {
       echo "#define NEXT_${PF}(a,b,c${DEF_END}) next_$FOUND(${DEF_BEGIN}b,c${DEF_END})"
       echo "#define ${PF}_ARG(a,b,c${DEF_END}) (${DEF_BEGIN}b,c${DEF_END})"
      } >>fakerootconfig.h.tmp

      break
    fi
  done      
done

if test -r fakerootconfig.h
then
       if test "`diff fakerootconfig.h fakerootconfig.h.tmp`" = ""
       then
               AC_MSG_RESULT([fakerootconfig.h not changed])
               rm fakerootconfig.h.tmp
       else
               AC_MSG_RESULT([recreated fakerootconfig.h])
               rm fakerootconfig.h ; mv fakerootconfig.h.tmp fakerootconfig.h
       fi
else
       AC_MSG_RESULT([created fakerootconfig.h])
       mv fakerootconfig.h.tmp fakerootconfig.h
fi

dnl This should really be done intelligently.

case $target_cpu:$target_os in
	(alpha*:linux*|ia64*:linux*)
      		libcpath="/lib/libc.so.6.1"
		;;
	(*:linux*)
	        libcpath="/lib/libc.so.6"
		;;
	(*:k*bsd*-gnu)
	        libcpath="/lib/libc.so.0.1"
		;;
	(*:freebsd*)
		libcpath="/usr/lib/libc.so.4"
		;;
	(*:netbsd*)
		libcpath="/usr/lib/libc.so.12"
		;;
	(*:openbsd*)
		libcpath="/usr/lib/libc.so.25.2"
		;;
	(*:hpux*)
		libcpath="/usr/lib/hpux32/libc.so.1"
		;;
	(*:osf*)
		libcpath="/shlib/libc.so"
		;;
	(*:solaris*)
		libcpath="/lib/libc.so.1"
		;;
	(*)
		AC_MSG_WARN([don't know where libc is for $target_os on
			     $target_cpu, setting to /lib/libc.so])
		libcpath="/lib/libc.so"
		;;
esac

AC_DEFINE_UNQUOTED([LIBCPATH], "$libcpath", [path to libc shared object])

dnl Checks for library functions.
AC_CHECK_FUNCS(strdup strstr getresuid setresuid getresgid setresgid setfsuid setfsgid)

dnl kludge
AH_VERBATIM([WRAP_STAT],
[/* Stuff.  */
#define WRAP_STAT  __astat 
#define WRAP_STAT_QUOTE  __astat 
#define TMP_STAT  __astat 
#define NEXT_STAT_NOARG  next___astat

#define WRAP_LSTAT_QUOTE  __astat 
#define WRAP_LSTAT  __astat 
#define TMP_LSTAT  __astat 
#define NEXT_LSTAT_NOARG  next___astat 

#define WRAP_FSTAT_QUOTE  __astat 
#define WRAP_FSTAT  __astat 
#define TMP_FSTAT  __astat 
#define NEXT_FSTAT_NOARG  next___astat 

#define WRAP_STAT64_QUOTE  __astat64 
#define WRAP_STAT64  __astat64 
#define TMP_STAT64  __astat64 
#define NEXT_STAT64_NOARG  next___astat64 

#define WRAP_LSTAT64_QUOTE  __astat64 
#define WRAP_LSTAT64  __astat64 
#define TMP_LSTAT64  __astat64 
#define NEXT_LSTAT64_NOARG  next___astat64 

#define WRAP_FSTAT64_QUOTE  __astat64
#define WRAP_FSTAT64  __astat64
#define TMP_FSTAT64  __astat64
#define NEXT_FSTAT64_NOARG  next___astat64 

#define WRAP_MKNOD_QUOTE  __amknod 
#define WRAP_MKNOD  __amknod 
#define TMP_MKNOD  __amknod 
#define NEXT_MKNOD_NOARG  next___amknod 
])
dnl kludge end

dnl check for b0rked Solaris (and other shells) and find one that works
AC_MSG_CHECKING(for a working shell...)
for i in /usr/xpg6/bin/sh /usr/xpg4/bin/sh /usr/ccs/bin/sh /bin/sh /usr/bin/sh /bin/ksh /bin/bash /usr/local/bin/bash ; do
    $i -c 'if ! false ; then echo $(echo ok) ; fi' >config.$$ 2>/dev/null
    j=`cat config.$$`
    rm -f config.$$
    if test "x$j" = "xok" ; then
        SHELL=$i
        AC_SUBST(SHELL)
        break
    fi
done
AC_MSG_RESULT($SHELL)
if test "x$SHELL" = "x" ; then
    AC_MSG_ERROR(no SUS compliant shell found - on Solaris, install SUNWxcu4)
fi

case "$target_cpu:$target_os" in
	(alpha*:linux*)
	AH_TEMPLATE([STUPID_ALPHA_HACK], [stat-struct conversion hackery])
	AC_MSG_CHECKING([if we need to do stat-struct conversion hackery])
	AC_EGREP_CPP([3:3],[
#include <sys/stat.h>
_STAT_VER:_STAT_VER_GLIBC2_3_4
],
       [AC_MSG_RESULT([yes]); AC_DEFINE(STUPID_ALPHA_HACK)
CPPFLAGS="$CPPFLAGS -I\$(srcdir)/statconv/glibc/linux/alpha"
],
       [AC_MSG_RESULT([no]);])
        ;;
esac

AC_CONFIG_FILES([Makefile scripts/Makefile doc/Makefile doc/es/Makefile doc/fr/Makefile doc/sv/Makefile test/Makefile])
AC_OUTPUT

dnl Local variables:
dnl mode: m4
dnl End:
