#
# Makefile for lib/ss
#

srcdir = .
top_srcdir = ../..

top_builddir = ../..
my_dir = lib/ss
INSTALL = /usr/bin/install -c
DLOPEN_LIB = -ldl


# Beginning of file MCONFIG

all::

check::

SHELL = /bin/sh

COMPRESS_EXT = gz bz2 bz Z

prefix = /usr
root_prefix = /usr
exec_prefix = /usr
root_bindir = /usr/bin
root_sbindir = /usr/sbin
root_libdir = ${exec_prefix}/lib
bindir = /usr/bin
sbindir = /usr/sbin
libdir = ${exec_prefix}/lib
datadir= ${prefix}/share
localedir = $(datadir)/locale
root_sysconfdir= ${prefix}/etc
includedir = ${prefix}/include
mandir = /usr/man
man1dir = $(mandir)/man1
man3dir = $(mandir)/man3
man5dir = $(mandir)/man5
man8dir = $(mandir)/man8
infodir = ${prefix}/info
datadir = ${prefix}/share



CC = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-gcc
BUILD_CC = gcc
DEFS = -DLOCALEDIR=\"$(localedir)\" -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DENABLE_HTREE=1 -DENABLE_SWAPFS=1 -DPACKAGE=\"e2fsprogs\" -DVERSION=\"0.14.1\" -DHAVE_LONG_LONG=1 -DHAVE_LONG_DOUBLE=1 -DHAVE_WCHAR_T=1 -DHAVE_WINT_T=1 -DHAVE_INTTYPES_H_WITH_UINTMAX=1 -DHAVE_STDINT_H_WITH_UINTMAX=1 -DHAVE_INTMAX_T=1 -DHAVE_POSIX_PRINTF=1 -DHAVE_ALLOCA_H=1 -DHAVE_ALLOCA=1 -DHAVE_STDLIB_H=1 -DHAVE_UNISTD_H=1 -DHAVE_GETPAGESIZE=1 -DINTDIV0_RAISES_SIGFPE=0 -DHAVE_UNSIGNED_LONG_LONG=1 -DHAVE_UINTMAX_T=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_STDINT_H=1 -DHAVE_ARGZ_H=1 -DHAVE_LIMITS_H=1 -DHAVE_LOCALE_H=1 -DHAVE_NL_TYPES_H=1 -DHAVE_MALLOC_H=1 -DHAVE_STDDEF_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_UNISTD_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_ASPRINTF=1 -DHAVE_FWPRINTF=1 -DHAVE_GETCWD=1 -DHAVE_GETEGID=1 -DHAVE_GETEUID=1 -DHAVE_GETGID=1 -DHAVE_GETUID=1 -DHAVE_MEMPCPY=1 -DHAVE_MUNMAP=1 -DHAVE_PUTENV=1 -DHAVE_SETENV=1 -DHAVE_SETLOCALE=1 -DHAVE_SNPRINTF=1 -DHAVE_STPCPY=1 -DHAVE_STRCASECMP=1 -DHAVE_STRDUP=1 -DHAVE_STRTOUL=1 -DHAVE_TSEARCH=1 -DHAVE_WCSLEN=1 -DHAVE___ARGZ_COUNT=1 -DHAVE___ARGZ_STRINGIFY=1 -DHAVE___ARGZ_NEXT=1 -DHAVE___FSETLOCKING=1 -DHAVE_DECL__SNPRINTF=0 -DHAVE_DECL__SNWPRINTF=0 -DHAVE_DECL_FEOF_UNLOCKED=1 -DHAVE_DECL_FGETS_UNLOCKED=0 -DHAVE_DECL_GETC_UNLOCKED=1 -DHAVE_ICONV=1 -DICONV_CONST= -DHAVE_LANGINFO_CODESET=1 -DHAVE_LC_MESSAGES=1 -DHAVE_STDLIB_H=1 -DHAVE_UNISTD_H=1 -DHAVE_STDARG_H=1 -DHAVE_STDINT_H=1 -DHAVE_ERRNO_H=1 -DHAVE_MALLOC_H=1 -DHAVE_MNTENT_H=1 -DHAVE_PATHS_H=1 -DHAVE_DIRENT_H=1 -DHAVE_GETOPT_H=1 -DHAVE_SETJMP_H=1 -DHAVE_SIGNAL_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_LINUX_FD_H=1 -DHAVE_LINUX_MAJOR_H=1 -DHAVE_SYS_IOCTL_H=1 -DHAVE_SYS_PRCTL_H=1 -DHAVE_SYS_QUEUE_H=1 -DHAVE_SYS_SOCKET_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_WAIT_H=1 -DHAVE_SYS_RESOURCE_H=1 -DHAVE_NETINET_IN_H=1 -DHAVE_SYS_MOUNT_H=1 -DHAVE_NET_IF_H=1 -DHAVE_VPRINTF=1 -DHAVE_RECLEN_DIRENT=1 -DHAVE_TYPE_SSIZE_T=1 -DHAVE_LSEEK64_PROTOTYPE=1 -DSIZEOF_SHORT=2 -DSIZEOF_INT=4 -DSIZEOF_LONG=4 -DSIZEOF_LONG_LONG=8 -DHAVE_INTTYPES_H=1 -DHAVE_INTPTR_T=1 -DHAVE_CHFLAGS=1 -DHAVE_GETRUSAGE=1 -DHAVE_LLSEEK=1 -DHAVE_LSEEK64=1 -DHAVE_OPEN64=1 -DHAVE_FSTAT64=1 -DHAVE_STRTOULL=1 -DHAVE_STRCASECMP=1 -DHAVE_SRANDOM=1 -DHAVE_FCHOWN=1 -DHAVE_MALLINFO=1 -DHAVE_FDATASYNC=1 -DHAVE_STRNLEN=1 -DHAVE_STRPTIME=1 -DHAVE_SYSCONF=1 -DHAVE_PATHCONF=1 -DHAVE_POSIX_MEMALIGN=1 -DHAVE_MEMALIGN=1 -DHAVE_VALLOC=1 -DHAVE___SECURE_GETENV=1 -DHAVE_PRCTL=1 -DHAVE_DLOPEN=1 -DHAVE_EXT2_IOCTLS=1 
CFLAGS = -Os -pipe
CPPFLAGS =  -I$(top_builddir)/lib -I$(top_srcdir)/lib -I/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include
INTL_FLAGS = 
ALL_CFLAGS = $(CPPFLAGS) $(DEFS) $(USE_WFLAGS) $(CFLAGS) $(XTRA_CFLAGS) \
	$(INTL_FLAGS) $(LINUX_INCLUDE) 
LDFLAGS = 
ALL_LDFLAGS = $(LDFLAGS)
BUILD_CFLAGS = 
BUILD_LDFLAGS = 
RM = /bin/rm
LN = /bin/ln
LN_S = ln -s
MV = /bin/mv
CP = /bin/cp
CHMOD = /bin/chmod
AR = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-ar
AWK = gawk
SED = /bin/sed
PERL = /usr/bin/perl
RANLIB = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-ranlib
STRIP = strip
LD = $(PURE) /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-gcc
ARUPD = $(AR) r
ARGEN = $(AR) rc
LDCONFIG = /sbin/ldconfig
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
MKINSTALLDIRS = $(top_builddir)/config/mkinstalldirs

#
# Library definitions
#
DEVMAPPER_LIBS = 
STATIC_DEVMAPPER_LIBS = 

LIB = $(top_builddir)/lib
LIBSS = $(LIB)/libss.so
LIBCOM_ERR = $(LIB)/libcom_err.so
LIBE2P = $(LIB)/libe2p.so
LIBEXT2FS = $(LIB)/libext2fs.so
LIBUUID = $(LIB)/libuuid.so  
LIBBLKID = $(LIB)/libblkid.so $(DEVMAPPER_LIBS)
LIBINTL = 
DEPLIBUUID = $(LIB)/libuuid.so

STATIC_LIBSS = $(LIB)/libss.a
STATIC_LIBCOM_ERR = $(LIB)/libcom_err.a
STATIC_LIBE2P = $(LIB)/libe2p.a
STATIC_LIBEXT2FS = $(LIB)/libext2fs.a
STATIC_LIBUUID = $(LIB)/libuuid.a  
STATIC_LIBBLKID = $(LIB)/libblkid.a $(STATIC_DEVMAPPER_LIBS)
DEPSTATIC_LIBUUID = $(LIB)/libuuid.a

PROFILED_LIBSS = $(LIB)/libss
PROFILED_LIBCOM_ERR = $(LIB)/libcom_err
PROFILED_LIBE2P = $(LIB)/libe2p
PROFILED_LIBEXT2FS = $(LIB)/libext2fs
PROFILED_LIBUUID = $(LIB)/libuuid  
PROFILED_LIBBLKID = $(LIB)/libblkid
DEPPROFILED_LIBUUID = $(LIB)/libuuid

# An include directive pointing to a directory holding enough linux-like
# include files to satisfy some programs here
LINUX_INCLUDE=

#
# A fast substitution command for fixing up man pages, shell scripts, etc.
#
SUBST_CONF=$(top_builddir)/util/subst.conf
SUBSTITUTE= $(top_builddir)/util/subst -f $(SUBST_CONF)
SUBSTITUTE_UPTIME= $(top_builddir)/util/subst -t -f $(SUBST_CONF)
DEP_SUBSTITUTE= $(top_builddir)/util/subst $(SUBST_CONF)

$(top_builddir)/util/subst:
	cd $(top_builddir)/util ; $(MAKE) subst

#
# Warning flags
#
# Run make gcc-wall to do a build with warning messages.
#
#
WFLAGS=		-ansi -D_POSIX_SOURCE -pedantic $(WFLAGS_EXTRA) \
			-Wall -W -Wwrite-strings -Wpointer-arith \
			-Wcast-qual -Wcast-align  \
			-Wstrict-prototypes -Wmissing-prototypes \
			-Wformat-security  -Wformat-nonliteral \
			-Wmissing-format-attribute \
			-Wnested-externs -Winline -DNO_INLINE_FUNCS -Wshadow \
			-UENABLE_NLS

gcc-wall-new:
	(make USE_WFLAGS="$(WFLAGS)" > /dev/null) 2>&1 | sed -f $(top_srcdir)/util/gcc-wall-cleanup 

gcc-wall:
	make clean > /dev/null
	make gcc-wall-new

#
# Installation user and groups
#
BINGRP=		bin
BINOWN=		bin
BINMODE=	555
INCGRP=		bin
INCOWN=		bin
INCMODE=	444
LIBOWN=		bin
LIBGRP=		bin
LIBMODE=	444
MANGRP=		bin
MANOWN=		bin
MANMODE=	444

#
# Autoconf magic...
#

DEP_LIB_MAKEFILES = $(top_srcdir)/lib/Makefile.elf-lib  \
	$(top_srcdir)/lib/Makefile.bsd-lib $(top_srcdir)/lib/Makefile.darwin-lib \
	$(top_srcdir)/lib/Makefile.solaris-lib $(top_srcdir)/lib/Makefile.checker \
	$(top_srcdir)/lib/Makefile.profile

$(top_builddir)/config.status: $(top_srcdir)/configure
	cd $(top_builddir); ./config.status --recheck

$(top_builddir)/MCONFIG: $(top_srcdir)/MCONFIG.in $(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=MCONFIG ./config.status

$(top_builddir)/lib/substitute_sh: $(top_srcdir)/lib/substitute_sh.in \
		$(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=lib/substitute_sh ./config.status

$(top_builddir)/util/subst.conf: $(top_srcdir)/util/subst.conf.in \
		$(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=util/subst.conf ./config.status

Makefile: $(srcdir)/Makefile.in $(top_builddir)/MCONFIG \
		$(DEP_MAKEFILE) $(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=$(my_dir)/Makefile ./config.status

#$(top_srcdir)/configure: $(top_srcdir)/configure.in
#	cd $(top_srcdir) && autoconf

#
# Make depend magic...
#

.depend: Makefile $(SRCS) $(top_srcdir)/depfix.sed $(top_srcdir)/wordwrap.pl
	if test -n "$(SRCS)" ; then \
		$(CC) -M $(ALL_CFLAGS) $(SRCS) | \
			$(SED) -f $(top_srcdir)/depfix.sed \
			    -e 's; $(srcdir)/; $$(srcdir)/;g' \
			    -e 's; $(top_srcdir)/; $$(top_srcdir)/;g' \
			    -e 's; $(top_builddir)/; $$(top_builddir)/;g' \
			    -e 's; \./; ;g' \
			    -e '/^#/d' \
			    -e '/^ *\\$$/d' | \
			$(PERL) $(top_srcdir)/wordwrap.pl > .depend; \
	else :; fi

depend:: .depend
	if test -n "$(SRCS)" ; then \
		sed -e '/^# +++ Dependency line eater +++/,$$d' \
			< $(srcdir)/Makefile.in | cat - .depend \
			> $(srcdir)/Makefile.in.new; \
	if cmp -s $(srcdir)/Makefile.in $(srcdir)/Makefile.in.new ; then \
		$(RM) $(srcdir)/Makefile.in.new ; \
	else \
		$(MV) $(srcdir)/Makefile.in $(srcdir)/Makefile.in.old; \
		$(MV) $(srcdir)/Makefile.in.new $(srcdir)/Makefile.in; \
	fi ; else :; fi

# End of file MCONFIG

LIBRARY= libss
LIBDIR= ss

ELF_VERSION = 2.0
ELF_SO_VERSION = 2
ELF_IMAGE = libss
ELF_MYDIR = ss
ELF_INSTALL_DIR = $(root_libdir)
ELF_OTHER_LIBS = -L../.. -lcom_err $(DLOPEN_LIB)

BSDLIB_VERSION = 1.0
BSDLIB_IMAGE = libss
BSDLIB_MYDIR = ss
BSDLIB_INSTALL_DIR = $(root_libdir)

TAGS=etags
COMPILE_ET=../et/compile_et --build-tree
MK_CMDS=_SS_DIR_OVERRIDE=. ./mk_cmds

# -I.. is so that ss/ss_err.h works
# -I$(srcdir)/.. is so that ss/ss.h works
# -I$(srcdir)/../et is so com_err.h works
XTRA_CFLAGS= -I$(srcdir)/../et

.c.o:
	@echo "	CC $<"
	@$(CC) $(ALL_CFLAGS) -c $<
#	@$(CC) $(ALL_CFLAGS) -g -pg -o profiled/$*.o -c $<
#	@$(CC) $(ALL_CFLAGS) -checker -g -o checker/$*.o -c $<
	@$(CC) $(ALL_CFLAGS) -DSHARED_ELF_LIB -fPIC -o elfshared/$*.o -c $<
#	@$(CC) $(ALL_CFLAGS) $(BSDLIB_PIC_FLAG) -o pic/$*.o -c $<

# for the library

LIB=	libss.a

# with ss_err.o first, ss_err.h should get rebuilt first too.  should not
# be relying on this, though.
OBJS=	ss_err.o \
	std_rqs.o \
	invocation.o help.o \
	execute_cmd.o listen.o parse.o error.o prompt.o \
	request_tbl.o list_rqs.o pager.o requests.o \
	data.o get_readline.o

SRCS=	$(srcdir)/invocation.c $(srcdir)/help.c \
	$(srcdir)/execute_cmd.c $(srcdir)/listen.c $(srcdir)/parse.c \
	$(srcdir)/error.c $(srcdir)/prompt.c $(srcdir)/request_tbl.c \
	$(srcdir)/list_rqs.c $(srcdir)/pager.c $(srcdir)/requests.c \
	$(srcdir)/data.c $(srcdir)/get_readline.c

all:: mk_cmds


all:: subdirs $(LIBRARY).a

install-shlibs::

uninstall-shlibs::

real-subdirs::

subdirs:: Makefile
	@$(MAKE) -s real-subdirs
	@touch subdirs

clean::
	@$(RM) -f subdirs

$(LIBRARY).a: $(OBJS)
	@echo "	GEN_LIB $@"
	@(if test -r $@; then $(RM) -f $@.bak && $(MV) $@ $@.bak; fi)
	@$(ARGEN) $@ $(OBJS)
	-@$(RANLIB) $@
	@$(RM) -f ../$@
	@$(LN) $@ ../$@


#
# This is a Makefile stub which handles the creation of Linux ELF shared
# libraries.
#
# In order to use this stub, the following makefile variables must be defined.
# 
# ELF_VERSION = 1.0
# ELF_SO_VERSION = 1
# ELF_IMAGE = libce
# ELF_MYDIR = et
# ELF_INSTALL_DIR = $(SHLIBDIR)
# ELF_OTHER_LIBS = -lc

all:: image

real-subdirs:: Makefile
	@echo "	MKDIR elfshared"
	@mkdir -p elfshared

ELF_LIB = $(ELF_IMAGE).so.$(ELF_VERSION)
ELF_SONAME = $(ELF_IMAGE).so.$(ELF_SO_VERSION)

image:		$(ELF_LIB)

$(ELF_LIB): $(OBJS)
	@echo "	GEN_ELF_SOLIB $(ELF_LIB)"
	@(cd elfshared; $(CC) --shared -o $(ELF_LIB) $(LDFLAGS) \
		-Wl,-soname,$(ELF_SONAME) $(OBJS) $(ELF_OTHER_LIBS))
	@$(MV) elfshared/$(ELF_LIB) .
	@$(RM) -f ../$(ELF_LIB) ../$(ELF_IMAGE).so ../$(ELF_SONAME)
	@$(LN) $(ELF_LIB) ../$(ELF_LIB)
	@$(LN) ../$(ELF_LIB) ../$(ELF_IMAGE).so
	@$(LN) ../$(ELF_LIB) ../$(ELF_SONAME)

installdirs-elf-lib::
	@echo "	MKINSTALLDIRS $(ELF_INSTALL_DIR) $(libdir)"
	@$(MKINSTALLDIRS) $(DESTDIR)$(ELF_INSTALL_DIR) \
		$(DESTDIR)$(libdir)

installdirs:: installdirs-elf-lib

install-shlibs install:: $(ELF_LIB) installdirs-elf-lib
	@echo "	INSTALL-ELF-LIB $(ELF_INSTALL_DIR)/$(ELF_LIB)"
	@$(INSTALL_PROGRAM) $(ELF_LIB) $(DESTDIR)$(ELF_INSTALL_DIR)/$(ELF_LIB)
	@echo "	SYMLINK $(ELF_INSTALL_DIR)/$(ELF_SONAME)"
	@$(LN_S) -f $(ELF_LIB) $(DESTDIR)$(ELF_INSTALL_DIR)/$(ELF_SONAME)
	@echo "	SYMLINK $(libdir)/$(ELF_IMAGE).so"
	@$(LN_S) -f $(ELF_INSTALL_DIR)/$(ELF_SONAME) \
		$(DESTDIR)$(libdir)/$(ELF_IMAGE).so
	@echo "	LDCONFIG"
	@-$(LDCONFIG)

install-strip: install
	@echo "	STRIP-LIB $(ELF_INSTALL_DIR)/$(ELF_LIB)"
	@$(STRIP) --strip-unneeded --remove-section=.comment \
		--remove-section=.note $(DESTDIR)$(ELF_INSTALL_DIR)/$(ELF_LIB)

install-shlibs-strip: install-shlibs
	@echo "	STRIP-LIB $(ELF_INSTALL_DIR)/$(ELF_LIB)"
	@$(STRIP) --strip-unneeded --remove-section=.comment \
		--remove-section=.note $(DESTDIR)$(ELF_INSTALL_DIR)/$(ELF_LIB)

uninstall-shlibs uninstall::
	$(RM) -f $(DESTDIR)$(ELF_INSTALL_DIR)/$(ELF_LIB) \
		$(DESTDIR)$(ELF_INSTALL_DIR)/$(ELF_SONAME) \
		$(DESTDIR)$(libdir)/$(ELF_IMAGE).so
	-$(LDCONFIG)

clean::
	$(RM) -rf elfshared
	$(RM) -f $(ELF_LIB)
	$(RM) -f ../$(ELF_LIB) ../$(ELF_IMAGE).so ../$(ELF_SONAME)




CODE= $(SRCS) $(MKCMDSFILES)

MKCMDSOBJS=	mk_cmds.o utils.o options.o ct.tab.o cmd_tbl.lex.o

MKCMDSFILES=	mk_cmds.c utils.c options.c ct.y cmd_tbl.lex.l

MKCMDSCSRCS=	mk_cmds.c utils.c options.c ct.tab.c cmd_tbl.lex.c


HFILES=	ss.h ss_internal.h
SHARE_FILES= ct_c.awk ct_c.sed

INSTALL_HFILES=	ss.h 

# for 'tags' and dependencies

CFILES=	$(SRCS) $(MKCMDSCSRCS) test_ss.c

# for building archives

FILES=	$(SRCS) $(MKCMDSFILES) $(HFILES) \
	ss_err.et std_rqs.ct Makefile \
	test_ss.c ss 

all::	libss.a ss.pc # libss_p.a lint

std_rqs.c: std_rqs.ct mk_cmds
	@echo "	MK_CMDS $@"
	@DIR=$(srcdir) $(MK_CMDS) $(srcdir)/std_rqs.ct

ss_err.c ss_err.h: ss_err.et
	@echo "	COMPILE_ET ss_err.et"
	@$(COMPILE_ET) $(srcdir)/ss_err.et

ct.tab.c ct.tab.h: ct.y
	$(RM) -f ct.tab.* y.*
	$(YACC) -d $(srcdir)/ct.y
	$(MV) -f y.tab.c ct.tab.c
	$(MV) -f y.tab.h ct.tab.h

#libss.o:	$(OBJS)
#	$(LD) -r -s -o $@ $(OBJS)
#	$(CHMOD) -x $@

mk_cmds: $(DEP_SUBSTITUTE) $(srcdir)/mk_cmds.sh.in
	@echo "	SUBST $@"
	@$(SUBSTITUTE) $(srcdir)/mk_cmds.sh.in mk_cmds
	@$(CHMOD) +x mk_cmds

ss.pc: $(srcdir)/ss.pc.in $(top_builddir)/config.status
	@echo "	CONFIG.STATUS $@"
	@cd $(top_builddir); CONFIG_FILES=lib/ss/ss.pc ./config.status

installdirs::
	@echo "	MKINSTALLDIRS $(libdir) $(includedir)/ss $(datadir)/ss $(bindir)"
	@$(MKINSTALLDIRS) $(DESTDIR)$(libdir) \
		$(DESTDIR)$(includedir)/ss $(DESTDIR)$(datadir)/ss \
		$(DESTDIR)$(bindir) $(DESTDIR)$(libdir)/pkgconfig

install:: libss.a $(INSTALL_HFILES) installdirs ss_err.h mk_cmds ss.pc
	@echo "	INSTALL_DATA $(DESTDIR)$(libdir)/libss.a"
	@$(INSTALL_DATA) libss.a $(DESTDIR)$(libdir)/libss.a
	@-$(RANLIB) $(DESTDIR)$(libdir)/libss.a
	@$(CHMOD) $(LIBMODE) $(DESTDIR)$(libdir)/libss.a
	@$(RM) -f $(DESTDIR)$(includedir)/ss/*
	@for i in $(INSTALL_HFILES); do \
		echo "	INSTALL_DATA $(DESTDIR)$(includedir)/ss/$$i"; \
		$(INSTALL_DATA) $(srcdir)/$$i $(DESTDIR)$(includedir)/ss/$$i; \
	done
	@echo "	INSTALL_DATA $(includedir)/ss/ss_err.h"
	@$(INSTALL_DATA) ss_err.h $(DESTDIR)$(includedir)/ss/ss_err.h
	@for i in $(SHARE_FILES); do \
		echo "	INSTALL_DATA $(DESTDIR)$(datadir)/ss/$$i"; \
		$(INSTALL_DATA) $(srcdir)/$$i $(DESTDIR)$(datadir)/ss/$$i; \
	done
	@echo "	INSTALL $(bindir)/mk_cmds"
	@$(INSTALL) mk_cmds $(DESTDIR)$(bindir)/mk_cmds
	@echo "	INSTALL_DATA $(man1dir)/mk_cmds.1"
	@$(INSTALL_DATA) $(srcdir)/mk_cmds.1 $(DESTDIR)$(man1dir)/mk_cmds.1
	@echo "	INSTALL_DATA $(libdir)/pkgconfig/ss.pc"
	@$(INSTALL_DATA) ss.pc $(DESTDIR)$(libdir)/pkgconfig/ss.pc

uninstall::
	$(RM) -f $(DESTDIR)$(libdir)/libss.a $(DESTDIR)$(bindir)/mk_cmds \
		$(DESTDIR)$(libdir)/pkgconfig/ss.pc \
		$(DESTDIR)$(man1dir)/mk_cmds.1 
	$(RM) -rf $(DESTDIR)$(includedir)/ss $(DESTDIR)$(datadir)/ss

clean::
	$(RM) -f ../libss.a libss.a mk_cmds ss_err.h ss_err.c std_rqs.c \
		*.o *~ \#* *.bak core 

mostlyclean:: clean
distclean:: clean
	$(RM) -f .depend Makefile ss.pc \
		$(srcdir)/TAGS $(srcdir)/Makefile.in.old

#
# Hack to parallel makes recognize dependencies correctly.
#
$(top_builddir)/lib/ss/ss_err.h: ss_err.h

$(OBJS): subdirs

# +++ Dependency line eater +++
# 
# Makefile dependencies follow.  This must be the last section in
# the Makefile.in file
#
invocation.o: $(srcdir)/invocation.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
help.o: $(srcdir)/help.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
execute_cmd.o: $(srcdir)/execute_cmd.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
listen.o: $(srcdir)/listen.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
parse.o: $(srcdir)/parse.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
error.o: $(srcdir)/error.c $(top_srcdir)/lib/et/com_err.h \
 $(srcdir)/ss_internal.h $(srcdir)/ss.h $(top_builddir)/lib/ss/ss_err.h \
 $(top_srcdir)/lib/et/com_err.h
prompt.o: $(srcdir)/prompt.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
request_tbl.o: $(srcdir)/request_tbl.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
list_rqs.o: $(srcdir)/list_rqs.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
pager.o: $(srcdir)/pager.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
requests.o: $(srcdir)/requests.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
data.o: $(srcdir)/data.c $(srcdir)/ss_internal.h $(srcdir)/ss.h \
 $(top_builddir)/lib/ss/ss_err.h $(top_srcdir)/lib/et/com_err.h
get_readline.o: $(srcdir)/get_readline.c $(srcdir)/ss_internal.h \
 $(srcdir)/ss.h $(top_builddir)/lib/ss/ss_err.h \
 $(top_srcdir)/lib/et/com_err.h
