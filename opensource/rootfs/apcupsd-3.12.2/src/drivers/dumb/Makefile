# Makefile template for apcupsd
#
# Copyright (C) 1999-2002 Riccardo Facchetti <riccardo@master.oasi.gpa.it>
#

# Default variables

# General rules for Makefile(s) subsystem.
# In this file we will put everything that need to be
# shared betweek all the Makefile(s).
# This file must be included at the beginning of every Makefile
#
# Copyright (C) 1999-2002 Riccardo Facchetti <riccardo@master.oasi.gpa.it>

#
# package version
PACKAGE = apcupsd
DISTNAME = redhat
DISTVER = (Tettnang)
VERSION = 3.12.2

#
# programs needed by compilation
CP = /bin/cp
MV = /bin/mv
ECHO = /bin/echo
RM = /bin/rm
RMF = $(RM) -f
LN = /bin/ln
SED = /bin/sed
MAKE = /usr/bin/gmake
SHELL = /bin/sh
RANLIB = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-ranlib
AR = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-ar
LIBTOOL = /usr/bin/libtool
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
MKINSTALLDIRS = /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/autoconf/mkinstalldirs
ETAGS = :
CTAGS = :
GENCAT = 
MSGFMT = /usr/bin/msgfmt


# Files and directories (paths)
#
# Commond prefix for machine-independent installed files.
prefix = /usr

# Commond prefix for machine-dependent installed files.
exec_prefix = /usr

# system configuration directory.
sysconfdir = /etc/apcupsd

# cgi-bin install directory.
cgibin = /etc/apcupsd

# Ultrix 2.2 make doesn't expand the value of VPATH.
VPATH = /usr/lib:/usr/local/lib

# source directory where this Makefile is placed.
srcdir = .

# Absolute top srcdir
abssrcdir = /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2

# Directory in which to install.
sbindir = /usr/sbin

# Directory for pid files.
piddir = /var/run

# Manual extension
manext = 8

# Manual directory
mandir=/usr/man


# Compilation macros.
CC = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-gcc
CXX = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-g++
DEFS =  $(LOCALDEFS)
CPPFLAGS = -I/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include

# Libraries
APCLIBS = $(topdir)/src/lib/libapc.a
DRVLIBS = $(topdir)/src/drivers/libdrivers.a
win32 = $(topdir)/src/win32/winmain.o $(topdir)/src/win32/winlib.a $(topdir)/src/win32/winres.res
INTLLIBS = 
WINAPC = $()
# Hack or die: seems that cygwin doesnn't like -lgdi32 and -luser32 in deps.
WINLIBS = 


# Made INCFLAGS relative to the topdir and hardcoded into the Makefiles
# For GDINCLUDE see src/cgi/Makefile.in
INCFLAGS = $(GDINCLUDE) -I$(topdir)/include $(EXTRAINCS)
CFLAGS = -Os -pipe -Wall $(CPPFLAGS)  $(INCFLAGS)
LDFLAGS = -L/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/lib
LIBS = $(DRVLIBS) $(APCLIBS) $(INTLLIBS) -lpthread 

# Flags for windows programs
WLDFLAGS = 
# TOP source directory.
topdir = /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2
top_builddir = $(topdir)

# Include the default make targets: to be put before the all-targets: rule.

# General targets for Makefile(s) subsystem.
# In this file we will put everything that need to be
# shared betweek all the Makefile(s).
# This file must be included at the beginning of every Makefile
#
# Copyright (C) 1999-2002 Riccardo Facchetti <riccardo@master.oasi.gpa.it>

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
.SUFFIXES: .o .lo .c .cpp .h .po .gmo .mo .cat .msg .pox
.MAIN: all
.PHONY: all install uninstall install- install-apcupsd install-powerflute \
		install-cgi clean realclean distclean mostlyclean clobber

all: all-subdirs all-targets

all-subdirs:
	@if test ! x"$(subdirs)" = x; then \
	    for file in . ${subdirs}; \
	    do \
		(cd $$file && if test "$$file" != "."; then $(MAKE) DESTDIR=$(DESTDIR) all; fi); \
		  if test "$$?" != "0"; then \
		     break; \
		  fi; \
	    done; \
	fi

# Standard compilation targets
dummy:

.c.o:
	$(CXX) -c $(CFLAGS) $(DEFS) $<

.c.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) $<

.cpp.o:
	$(CXX) -c $(CFLAGS) $(DEFS) $<

.po.pox:
	$(MAKE) DESTDIR=$(DESTDIR) $(PACKAGE).pot
	$(MSGMERGE) $< $(srcdir)/$(PACKAGE).pot -o $*.pox
 
.po.mo:
	$(MSGFMT) -o $@ $<
 
.po.gmo:
	file=$(srcdir)/`echo $* | sed 's,.*/,,'`.gmo \
	  && rm -f $$file && $(GMSGFMT) -o $$file $<
 
.po.cat:
	sed -f $(topdir)/src/intl/po2msg.sed < $< > $*.msg \
	  && rm -f $@ && $(GENCAT) $@ $*.msg					    

# Library targets
$(topdir)/src/lib/libapc.a: $(topdir)/src/lib/*.[ch]
	@(cd $(topdir)/src/lib && $(MAKE))

$(topdir)/src/drivers/libdrivers.a: $(topdir)/src/drivers/*.[ch]
	@(cd $(topdir)/src/drivers && $(MAKE))

$(topdir)/src/drivers/apcsmart/libapcsmart.a: $(topdir)/src/drivers/apcsmart/*.[ch]
	@(cd $(topdir)/src/drivers/apcsmart && $(MAKE))

$(topdir)/src/drivers/dumb/libdumb.a: $(topdir)/src/drivers/dumb/*.[ch]
	@(cd $(topdir)/src/drivers/dumb && $(MAKE))

$(topdir)/src/drivers/net/libnet.a: $(topdir)/src/drivers/net/*.[ch]
	@(cd $(topdir)/src/drivers/net && $(MAKE))

$(topdir)/src/drivers/usb/libusb.a: $(topdir)/src/drivers/usb/*.[ch]
	@(cd $(topdir)/src/drivers/usb && $(MAKE))

$(topdir)/src/drivers/snmp/libdumb.a: $(topdir)/src/drivers/snmp/*.[ch]
	@(cd $(topdir)/src/drivers/snmp && $(MAKE))

$(topdir)/src/drivers/dumb/libtest.a: $(topdir)/src/drivers/test/*.[ch]
	@(cd $(topdir)/src/drivers/test && $(MAKE))

$(topdir)/src/intl/libintl.a: $(topdir)/src/intl/*.[ch]
	@(cd $(topdir)/src/intl && $(MAKE))

$(topdir)/src/gd1.2/libgd.a: $(topdir)/src/gd1.2/*.[ch]
	@(cd $(topdir)/src/gd1.2 && $(MAKE))

$(topdir)/src/win32/winmain.o: $(topdir)/src/win32/winmain.cpp \
	     $(topdir)/src/win32/winups.h
	@(cd $(topdir)/src/win32 && $(MAKE) DESTDIR=$(DESTDIR) winmain.o)

$(topdir)/src/win32/winlib.a: $(topdir)/src/win32/*.[ch] \
	     $(topdir)/src/win32/*.cpp
	@(cd $(topdir)/src/win32 && $(MAKE) DESTDIR=$(DESTDIR) winlib.a)

$(topdir)/src/win32/winres.res: $(topdir)/src/win32/winres.rc \
	     $(topdir)/src/win32/apcupsd.ico \
	     $(topdir)/src/win32/winres.h \
	     $(topdir)/src/win32/online.ico \
	     $(topdir)/src/win32/onbatt.ico
	@(cd $(topdir)/src/win32 && $(MAKE) winres.res)

# Makefile subsystem targets
$(topdir)/autoconf/variables.mak: $(topdir)/autoconf/variables.mak.in
	@(cd $(topdir) && \
	SINGLE_MAKEFILE=yes \
	CONFIG_FILES=./autoconf/variables.mak \
	CONFIG_HEADERS= $(SHELL) ./config.status)

Makefiles:
	@(cd $(topdir) && \
	$(SHELL) ./config.status)

Makefile: $(srcdir)/Makefile.in $(topdir)/config.status \
			$(topdir)/autoconf/variables.mak $(topdir)/autoconf/targets.mak
	@$(abssrcdir)/autoconf/rebuild-makefile.sh $(abssrcdir)
	@echo "You can ignore any makedepend error messages"
	@$(MAKE) DESTDIR=$(DESTDIR) single-depend

# Configuration targets

configure:  $(topdir)/autoconf/configure.in $(topdir)/autoconf/aclocal.m4 \
			$(topdir)/autoconf/acconfig.h $(topdir)/autoconf/config.h.in
	cd $(topdir);
	$(RMF) config.cache config.log config.out config.status include/config.h
	autoconf --prepend-include=$(topdir)/autoconf \
	autoconf/configure.in > configure
	chmod 755 configure

$(topdir)/autoconf/config.h.in: $(topdir)/autoconf/configure.in \
		$(topdir)/autoconf/acconfig.h
#	cd $(srcdir);
#	autoheader --prepend-include=$(srcdir)/autoconf \
#	autoconf/configure.in > autoconf/config.h.in
#	chmod 644 autoconf/config.h.in

$(topdir)/config.status:
	@if test -x $(topdir)/config.status; then \
		(cd $(topdir) && \
		$(SHELL) ./config.status --recheck); \
	else \
		(cd $(topdir) && \
		$(SHELL) ./configure); \
	fi

clean-subdirs:
	@if test ! x"$(subdirs)" = x; then \
		for file in . ${subdirs}; \
		do \
			(cd $$file && if test "$$file" != "."; then $(MAKE) clean; fi); \
		done; \
	fi

distclean-subdirs:
	@if test ! x"$(subdirs)" = x; then \
		for file in . ${subdirs}; \
		do \
			(cd $$file && if test "$$file" != "."; then $(MAKE) distclean; fi); \
		done; \
	fi

targetclean: clean-subdirs
	$(RMF) *.o *.lo *.a core core.* .*~ *~ *.bak
	$(RMF) *.exe *.res *.cgi
	$(RMF) 1 2 3 4 ID TAGS *.orig $(allexe)

targetdistclean: clean distclean-subdirs

mostlyclean: clean
realclean: distclean
clobber: distclean

# Semi-automatic generation of dependencies:
# Use gcc if possible because X11 `makedepend' doesn't work on all systems
# and it also includes system headers.
# Also test for the presence of source files: if not present, do nothing.
depend:
	@if test "`$(topdir)/autoconf/has-c-files.sh`" = "no"; then \
		$(ECHO) "Nothing to do for depend."; \
	else \
		$(MAKE) real-depend; \
	fi
	@if test ! x"$(subdirs)" = x; then \
		for file in . ${subdirs}; \
		do \
			(cd $$file && if test "$$file" != "."; then $(MAKE) depend; fi); \
		done; \
	fi

single-depend:
	@if test "`$(topdir)/autoconf/has-c-files.sh`" = "no"; then \
		$(ECHO) "Nothing to do for depend."; \
	else \
		$(MAKE) real-depend; \
	fi

real-depend:
	@$(RMF) Makefile.bak
	@if test `basename "$(CXX)"` = "g++" ; then \
	   $(MV) Makefile Makefile.bak; \
	   $(SED) "/^# DO NOT DELETE THIS LINE/,$$ d" Makefile.bak > Makefile; \
	   $(ECHO) "# DO NOT DELETE THIS LINE -- make depend depends on it." >> Makefile; \
	   $(CXX) -S -M $(CPPFLAGS) $(INCFLAGS) *.c >> Makefile; \
	else \
	   makedepend -- $(CFLAGS) -- $(INCFLAGS) *.c; \
	fi 
	@if test -f Makefile ; then \
	    $(RMF) Makefile.bak; \
	else \
	   $(MV) Makefile.bak Makefile; \
	   echo -e "Something went wrong with the make depend!\n\a\a\a\a"; \
	fi

install-subdirs:
	@if test ! x"$(subdirs)" = x; then \
	   for file in . ${subdirs}; \
	   do \
	       (cd $$file && if test "$$file" != "."; then $(MAKE) DESTDIR=$(DESTDIR) install; fi); \
	   done; \
	fi

uninstall-subdirs:
	@if test ! x"$(subdirs)" = x; then \
	   for file in . ${subdirs}; \
	   do \
	      (cd $$file && if test "$$file" != "."; then $(MAKE) DESTDIR=$(DESTDIR) uninstall; fi); \
	   done; \
	fi

indent:
	(cd $(topdir) && \
		find . \( -name '*.c' -o -name '*.h' -o -name '*.cpp' \) \
			-exec ./scripts/format_code {} \;)

TAGS:
	(cd $(topdir) && $(ETAGS) `find . \( -name \*.c -o -name \*.h \)`)
tags:
	(cd $(topdir) && $(CTAGS) `find . \( -name \*.c -o -name \*.h \)`)

apclib = libdumb.a

apclibobjs = dumboper.o dumbsetup.o

all-targets: $(apclib)

$(apclib): $(apclibobjs)
	ar rc $(apclib) $(apclibobjs)
	/opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-ranlib $(apclib)

clean: targetclean

distclean: targetdistclean
	$(RMF) Makefile

install:

uninstall:

# -----------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

dumboper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
dumboper.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
dumboper.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
dumboper.o: /usr/include/limits.h
dumboper.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
dumboper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
dumboper.o: dumb.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
dumbsetup.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
dumbsetup.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
dumbsetup.o: /usr/include/limits.h
dumbsetup.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
dumbsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
dumbsetup.o: dumb.h
