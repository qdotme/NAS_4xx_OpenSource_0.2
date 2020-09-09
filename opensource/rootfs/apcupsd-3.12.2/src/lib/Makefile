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

apclib = libapc.a

apclibobjs =  \
	     apcconfig.o apcerror.o \
	     apcevents.o apcexec.o \
	     apcfile.o apclibnis.o \
	     apclock.o apclog.o apcsignal.o \
	     apcstatus.o asys.o \
	     newups.o wincompat.o

all-targets: $(apclib)

$(apclib): $(apclibobjs)
	ar rc $(apclib) $(apclibobjs)
	/opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-ranlib $(apclib)

install:

uninstall:

clean: targetclean

distclean: targetdistclean
	$(RMF) Makefile CVS

# -----------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

apcconfig.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apcconfig.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apcconfig.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apcconfig.o: /usr/include/limits.h
apcconfig.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apcconfig.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apcerror.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apcerror.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apcerror.o: /usr/include/limits.h
apcerror.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apcerror.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apcevents.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apcevents.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apcevents.o: /usr/include/limits.h
apcevents.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apcevents.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apcexec.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apcexec.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apcexec.o: /usr/include/limits.h
apcexec.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apcexec.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apcfile.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apcfile.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apcfile.o: /usr/include/limits.h
apcfile.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apcfile.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apclibnis.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apclibnis.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apclibnis.o: /usr/include/limits.h
apclibnis.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apclibnis.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
apclist.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apclist.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apclist.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apclist.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apclist.o: /usr/include/limits.h
apclist.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apclist.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apclist.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apclist.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apclist.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apclist.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apclist.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apclist.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apclist.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apclist.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
apclock.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apclock.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apclock.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apclock.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apclock.o: /usr/include/limits.h
apclock.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apclock.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apclock.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apclock.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apclock.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apclock.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apclock.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apclock.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apclock.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apclock.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
apclog.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apclog.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apclog.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apclog.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apclog.o: /usr/include/limits.h
apclog.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apclog.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apclog.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apclog.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apclog.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apclog.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apclog.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apclog.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apclog.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apclog.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apcsignal.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apcsignal.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apcsignal.o: /usr/include/limits.h
apcsignal.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apcsignal.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
apcstatus.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
apcstatus.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
apcstatus.o: /usr/include/limits.h
apcstatus.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
apcstatus.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
asys.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
asys.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
asys.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
asys.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
asys.o: /usr/include/limits.h
asys.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
asys.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
asys.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
asys.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
asys.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
asys.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
asys.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
asys.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
asys.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
asys.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
getopt.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
getopt.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
getopt.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
getopt1.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
getopt1.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
getopt1.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
inet_pton.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
inet_pton.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
inet_pton.o: /usr/include/limits.h
inet_pton.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/param.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/limits.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/param.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/param.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/arch/param.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/nameser.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/bitypes.h
inet_pton.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/nameser_compat.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
localtime_r.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
localtime_r.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
localtime_r.o: /usr/include/limits.h
localtime_r.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
localtime_r.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
newups.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
newups.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
newups.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
newups.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
newups.o: /usr/include/limits.h
newups.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
newups.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
newups.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
newups.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
newups.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
newups.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
newups.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
newups.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
newups.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
newups.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
wincompat.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
wincompat.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
wincompat.o: /usr/include/limits.h
wincompat.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
wincompat.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
