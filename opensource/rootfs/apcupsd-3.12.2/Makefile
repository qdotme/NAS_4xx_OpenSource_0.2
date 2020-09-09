# main Makefile for apcupsd
#
# Copyright (C) 1999-2003 Riccardo Facchetti <riccardo@master.oasi.gpa.it>
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

#
# this is a list of all subdirectories that contain Makefiles
# Warning: "autoconf" must always be the last.
# Warning: "platforms" must always be next to last.
#
subdirs = src include doc platforms autoconf

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

all-targets: Makefile

install: install-directories install-subdirs

install-directories: install-dircgi
	$(SHELL) $(MKINSTALLDIRS) $(DESTDIR)$(sbindir)	
	$(SHELL) $(MKINSTALLDIRS) $(DESTDIR)$(mandir)  
	@if [ ! -d $(DESTDIR)$(sysconfdir) ] ; then \
	      $(SHELL) $(MKINSTALLDIRS) $(DESTDIR)$(sysconfdir); \
	      chmod 755 $(DESTDIR)$(sysconfdir); \
	fi

install-dir: dummy

install-dircgi:
	@$(SHELL) $(MKINSTALLDIRS)  $(DESTDIR)$(cgibin)

uninstall: uninstall-subdirs

example_clean:
	(cd examples; $(MAKE) clean)

clean: example_clean targetclean
	find . -type f -size 0 -exec $(RMF) {} \;

example_distclean: example_clean
	(cd examples; $(MAKE) distclean)

distclean: example_distclean targetdistclean
	$(RMF) TAGS tags
	$(RMF) config.cache config.status config.log config.out stamp-config
	$(RMF) Makefile
	rm -rf CVS

# -----------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- make depend depends on it.
