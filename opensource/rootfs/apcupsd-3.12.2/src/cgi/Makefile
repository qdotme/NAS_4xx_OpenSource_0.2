# Network UPS Tools: clients
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

allexe = upsstats.cgi upsfstats.cgi upsimage.cgi \
	   multimon.cgi

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

# Multimon objects
MOBJS = multimon.o upsfetch.o cgilib.o

# upssstats objects
SOBJS = upsstats.o upsfetch.o cgilib.o

# upsfstats objects
FOBJS = upsfstats.o upsfetch.o cgilib.o

# upsimage objects
# The libgd-1.2 can be generated automatically when checking
# dependencies.

IOBJS = upsimage.o upsfetch.o cgilib.o
GDINCLUDE = -I$(topdir)/src/gd1.2

all-targets: $(allexe)

multimon.cgi: $(MOBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $(MOBJS) $(LIBS)

upsstats.cgi: $(SOBJS) 
	$(CXX) $(LDFLAGS) -o $@ $(SOBJS) $(LIBS)

upsfstats.cgi: $(FOBJS) 
	$(CXX) $(LDFLAGS) -o $@ $(FOBJS) $(LIBS)

upsimage.cgi: $(IOBJS)
	$(CXX)	$(LDFLAGS) -o $@ $(IOBJS) $(topdir)/src/gd1.2/libgd.a $(LIBS) -lm

clean: targetclean

distclean: targetdistclean
	$(RMF) cgiconfig.h
	$(RMF) Makefile
	$(RMF) -r CVS

install: all-targets
	@for p in $(allexe) ; do \
	    echo $(INSTALL_PROGRAM) $$p $(DESTDIR)$(cgibin); \
	    $(INSTALL_PROGRAM) $$p $(DESTDIR)$(cgibin); \
	done
	# don't overwrite any existing config file
	@if test ! -f $(DESTDIR)$(sysconfdir)/apcupsd.css; then \
	   (echo "Installing apcupsd.css..." && \
	      $(INSTALL_DATA) apcupsd.css \
		 $(DESTDIR)$(sysconfdir)/apcupsd.css); \
	else \
	    (echo "Installing apcupsd.css as $(DESTDIR)$(sysconfdir)/apcupsd.css.new" && \
	       $(INSTALL_DATA) apcupsd.css \
		 $(DESTDIR)$(sysconfdir)/apcupsd.css.new); \
	fi

	# don't overwrite any existing config file
	@if test ! -f $(DESTDIR)$(sysconfdir)/hosts.conf; then \
	   (echo "Installing hosts.conf..." && \
	      $(INSTALL_DATA) \
		 $(topdir)/platforms/etc/hosts.conf \
		 $(DESTDIR)$(sysconfdir)/hosts.conf); \
	else \
	    (echo "Installing hosts.conf as $(DESTDIR)$(sysconfdir)/hosts.conf.new" && \
	       $(INSTALL_DATA) \
		  $(topdir)/platforms/etc/hosts.conf \
		  $(DESTDIR)$(sysconfdir)/hosts.conf.new); \
	fi
	# don't overwrite any existing config file
	@if test ! -f $(DESTDIR)$(sysconfdir)/multimon.conf; then \
	   (echo "Installing multimon.conf..." && \
	       $(INSTALL_DATA) \
		  $(topdir)/platforms/etc/multimon.conf \
		  $(DESTDIR)$(sysconfdir)/multimon.conf ); \
	else \
	   (echo "Installing multimon.conf as $(DESTDIR)$(sysconfdir)/multimon.conf.new" && \
	       $(INSTALL_DATA) \
		  $(topdir)/platforms/etc/multimon.conf \
		  $(DESTDIR)$(sysconfdir)/multimon.conf.new); \
	fi

uninstall:
	@echo "Uninstalling cgi program support..."
	@for p in $(allexe) ; do \
	    $(RMF) $(DESTDIR)$(cgibin)/$$p; \
	done
	$(RMF) $(DESTDIR)$(sysconfdir)/apcupsd.css
	$(RMF) $(DESTDIR)$(sysconfdir)/hosts.conf
	$(RMF) $(DESTDIR)$(sysconfdir)/hosts.conf.new
	$(RMF) $(DESTDIR)$(sysconfdir)/multimon.conf
	$(RMF) $(DESTDIR)$(sysconfdir)/multimon.conf.new

# -----------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.

cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
cgilib.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
cgilib.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
cgilib.o: cgilib.h cgiconfig.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
cgilib.o: /usr/include/limits.h
cgilib.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gd.h
cgilib.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gdfontl.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
multimon.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
multimon.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/param.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
multimon.o: /usr/include/limits.h
multimon.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/limits.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/param.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/param.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/arch/param.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
multimon.o: upsfetch.h cgiconfig.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
multimon.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gd.h
multimon.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gdfontl.h
multimon.o: cgilib.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
upsfetch.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
upsfetch.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
upsfetch.o: cgiconfig.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
upsfetch.o: /usr/include/limits.h
upsfetch.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gd.h
upsfetch.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gdfontl.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
upsfstats.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
upsfstats.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
upsfstats.o: cgiconfig.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
upsfstats.o: /usr/include/limits.h
upsfstats.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gd.h
upsfstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gdfontl.h
upsfstats.o: cgilib.h upsfetch.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
upsimage.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
upsimage.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
upsimage.o: /usr/include/limits.h
upsimage.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
upsimage.o: cgiconfig.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gd.h
upsimage.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gdfontl.h
upsimage.o: cgilib.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdio.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/features.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/predefs.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/cdefs.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gnu/stubs.h
upsstats.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stddef.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/types.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wordsize.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/typesizes.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/libio.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/_G_config.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/wchar.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/wchar.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/gconv.h
upsstats.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/stdarg.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stdio_lim.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sys_errlist.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdlib.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/string.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/types.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/time.h
upsstats.o: cgiconfig.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/config.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/unistd.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/posix_opt.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/confname.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/getopt.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pthread.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sched.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sched.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/signal.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigset.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/pthreadtypes.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/initspin.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sigthread.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/strings.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/ctype.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/endian.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/endian.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/syslog.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/syslog.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/limits.h
upsstats.o: /usr/include/limits.h
upsstats.o: /usr/lib/gcc-lib/i386-redhat-linux/3.3.3/include/limits.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/pwd.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/errno.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/errno.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/linux/errno.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/errno.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm-generic/errno-base.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/fcntl.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/fcntl.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/setjmp.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/setjmp.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/termios.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/termios.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netdb.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/netinet/in.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/stdint.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/socket.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/uio.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/uio.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/socket.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/sockaddr.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/socket.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/sockios.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/in.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/byteswap.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/netdb.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ioctl.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctls.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctls.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/asm/ioctl.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/ioctl-types.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/ttydefaults.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/stat.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/stat.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/time.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/time.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/select.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/select.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/wait.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/sys/resource.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/resource.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitflags.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/bits/waitstatus.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include/arpa/inet.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_config.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/apc_i18n.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/version.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/defines.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/struct.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/drivers.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/nis.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/include/extern.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gd.h
upsstats.o: /opt/devel/proto/marvell/build-eabi/apcupsd-3.12.2/src/gd1.2/gdfontl.h
upsstats.o: cgilib.h upsfetch.h
