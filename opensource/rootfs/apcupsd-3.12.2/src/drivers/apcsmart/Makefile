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

apclib = libapcsmart.a

apclibobjs = smartoper.o smartsetup.o smarteeprom.o smartsetup2.o smart.o

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

smart.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
smart.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
smart.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
smart.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
smart.o: /usr/include/limits.h
smart.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
smart.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
smart.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
smart.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
smart.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
smart.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
smart.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
smart.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
smart.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
smart.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
smart.o: apcsmart.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
smarteeprom.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
smarteeprom.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
smarteeprom.o: /usr/include/limits.h
smarteeprom.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
smarteeprom.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
smarteeprom.o: apcsmart.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
smartoper.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
smartoper.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
smartoper.o: /usr/include/limits.h
smartoper.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
smartoper.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
smartoper.o: apcsmart.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
smartsetup.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
smartsetup.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
smartsetup.o: /usr/include/limits.h
smartsetup.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
smartsetup.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
smartsetup.o: apcsmart.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
smartsetup2.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
smartsetup2.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
smartsetup2.o: /usr/include/limits.h
smartsetup2.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
smartsetup2.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
smartsetup2.o: apcsmart.h
