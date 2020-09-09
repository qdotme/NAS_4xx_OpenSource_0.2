AC_INIT(README)
AM_INIT_AUTOMAKE(howl,1.0.0)
AM_CONFIG_HEADER(include/howl_config.h)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_C_BIGENDIAN
AM_PROG_LIBTOOL
HOWL_LIBRARY_VERSION=0:0:0
MDNSRESPONDER_LIBRARY_VERSION=0:0:0
case "$host" in
	*-*-darwin*)
		SRC_SUBDIRS="lib"
		LIB_SUBDIRS="howl"
		HOWL_LIB_SUBDIRS=MacOSX
		HOWL_LIB_OBJECTS='macosx_salt.lo macosx_socket.lo macosx_time.lo macosx_mdns_stub.lo'
		PLATFORM_LIBS="-framework CoreFoundation"
		HOWL_MAN_PAGES=
		;;
	*-*-linux*)
		SRC_SUBDIRS="lib mDNSResponder autoipd nifd"
		LIB_SUBDIRS="howl mDNSResponder"
		HOWL_LIB_SUBDIRS="Posix NotOSX"
		HOWL_LIB_OBJECTS='posix_salt.lo posix_socket.lo posix_time.lo posix_signal.lo posix_interface.lo notosx_mdns_stub.lo'
		MDNSRESPONDER_LIB_SUBDIRS="Posix"
		MDNSRESPONDER_LIB_OBJECTS='posix_mdns.lo'
		AUTOIPD_EXTRA_OBJECTS='$(top_builddir)/src/autoipd/linux_autoip.lo $(top_builddir)/src/autoipd/posix_main.lo'
		PLATFORM_LIBS=-lpthread
		HOWL_MAN_PAGES='mDNSResponder.8 autoipd.8 nifd.8'
		;;
	*-*-solaris*)
		SRC_SUBDIRS="lib mDNSResponder"
		LIB_SUBDIRS="howl mDNSResponder"
		HOWL_LIB_SUBDIRS="Posix NotOSX"
		HOWL_LIB_OBJECTS='posix_salt.lo posix_socket.lo posix_time.lo posix_signal.lo posix_interface.lo notosx_mdns_stub.lo'
		MDNSRESPONDER_LIB_SUBDIRS="Posix"
		MDNSRESPONDER_LIB_OBJECTS='posix_mdns.lo'
		CFLAGS=
		PLATFORM_LIBS="-lsocket -lnsl -lsched -lrt -lresolv -lpthread"
		HOWL_MAN_PAGES='mDNSResponder.8'
		;;
	*-*-cygwin*)
		SRC_SUBDIRS="lib mDNSResponder"
		LIB_SUBDIRS="howl mDNSResponder"
		HOWL_LIB_SUBDIRS="Posix NotOSX"
		HOWL_LIB_OBJECTS='posix_salt.lo posix_socket.lo posix_time.lo posix_signal.lo posix_interface.lo notosx_mdns_stub.lo'
		MDNSRESPONDER_LIB_SUBDIRS="Posix"
		MDNSRESPONDER_LIB_OBJECTS='posix_mdns.lo'
		CFLAGS=
		PLATFORM_LIBS=
		HOWL_MAN_PAGES='mDNSResponder.8'
		;;
	*)
		SRC_SUBDIRS="lib mDNSResponder"
		LIB_SUBDIRS="howl mDNSResponder"
		HOWL_LIB_SUBDIRS="Posix NotOSX"
		HOWL_LIB_OBJECTS='posix_salt.lo posix_socket.lo posix_time.lo posix_signal.lo posix_interface.lo notosx_mdns_stub.lo'
		MDNSRESPONDER_LIB_SUBDIRS="Posix"
		MDNSRESPONDER_LIB_OBJECTS='posix_mdns.lo'
		CFLAGS=-pthread
		PLATFORM_LIBS=
		HOWL_MAN_PAGES='mDNSResponder.8'
		;;
esac
AC_SUBST([HOWL_LIBRARY_VERSION])
AC_SUBST([MDNSRESPONDER_LIBRARY_VERSION])
AC_SUBST([HOWL_RELEASE])
AC_SUBST([SRC_SUBDIRS])
AC_SUBST([LIB_SUBDIRS])
AC_SUBST([HOWL_LIB_SUBDIRS])
AC_SUBST([HOWL_LIB_OBJECTS])
AC_SUBST([MDNSRESPONDER_LIB_SUBDIRS])
AC_SUBST([MDNSRESPONDER_LIB_OBJECTS])
AC_SUBST([AUTOIPD_EXTRA_OBJECTS])
AC_SUBST([PLATFORM_LIBS])
AC_SUBST([HOWL_MAN_PAGES])
AC_OUTPUT(
				Makefile											\
				include/Makefile								\
				include/salt/Makefile						\
				include/corby/Makefile						\
				include/discovery/Makefile					\
				include/rendezvous/Makefile				\
				src/Makefile									\
				src/lib/Makefile								\
				src/lib/howl/Makefile						\
				src/lib/howl/MacOSX/Makefile				\
				src/lib/howl/Posix/Makefile				\
				src/lib/howl/Win32/Makefile				\
				src/lib/howl/NotOSX/Makefile				\
				src/lib/mDNSResponder/Makefile			\
				src/lib/mDNSResponder/Posix/Makefile	\
				src/lib/mDNSResponder/Win32/Makefile	\
				src/mDNSResponder/Makefile					\
				src/mDNSResponder/Posix/Makefile			\
				src/mDNSResponder/Win32/Makefile			\
				src/autoipd/Makefile							\
				src/autoipd/Posix/Makefile					\
				src/autoipd/Linux/Makefile					\
				src/nifd/Makefile								\
				test/Makefile									\
				test/step/Makefile							\
				samples/Makefile								\
				samples/console/Makefile					\
				samples/console/publish/Makefile			\
				samples/console/browse/Makefile			\
				samples/console/resolve/Makefile			\
				samples/console/query/Makefile			\
				samples/Win32/Makefile						\
				samples/Win32/IEBar/Makefile				\
				docs/Makefile									\
				etc/Makefile									\
				howl.pc
			)
