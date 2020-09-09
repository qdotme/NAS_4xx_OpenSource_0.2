# Makefile.in generated automatically by automake 1.4-p6 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

#
# To override any of the following variables, you can set the veriable in your
# environment before calling make.  Be sure that make will honor environmental
# overrides, usually by passing the -e flag to make or by setting MAKEFLAGS in
# your environment.
#
# Ex:
#
# 	$ ./configure
# 	$ CFLAGS="-g -Wall" make -e
# 	$ make install
#

# CFLAGS =


##############################################################################
#
# A list of sourceforge compile-farm hosts
#
# Last update: Thu Mar  2 12:13:07 EST 2006
#


SHELL = /bin/sh

srcdir = .
top_srcdir = .

prefix = /usr
exec_prefix = /usr

bindir = /usr/bin
sbindir = /usr/sbin
libexecdir = /usr/lib
datadir = /usr/share
sysconfdir = /etc
sharedstatedir = ${prefix}/com
localstatedir = /var
libdir = ${exec_prefix}/lib
infodir = /usr/info
mandir = /usr/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/bar
pkglibdir = $(libdir)/bar
pkgincludedir = $(includedir)/bar

top_builddir = .

ACLOCAL = aclocal-1.4
AUTOCONF = autoconf
AUTOMAKE = automake-1.4
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,^,arm-linux-,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_alias = i386-pc-linux-gnu
build_triplet = i386-pc-linux-gnu
host_alias = arm-linux
host_triplet = arm-unknown-linux-gnu
target_alias = arm-linux
target_triplet = arm-unknown-linux-gnu
AWK = gawk
CC = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-gcc
DEFAULT_BUFFER_SIZE = 
DEFAULT_DISPLAY_TWIDDLE = 
DEFAULT_PAGE_SIZE = 
DEFAULT_SW_MINUS_ONE = 
HAVE_LIB = @HAVE_LIB@
LIB = @LIB@
LN_S = ln -s
LTLIB = @LTLIB@
MAKEINFO = makeinfo
PACKAGE = bar
VERSION = 1.10.6

CFLAGS = -O2
# CFLAGS = -g -Wall

# LDFLAGS =
LDFLAGS = -s

bin_PROGRAMS =  bar

noinst_PROGRAMS =  test-types test-001-pre test-001-post
noinst_HEADERS =  headers.h types.h error.h fd.h io.h display.h args.h

TESTS =  test-types test-001 test-002
nodist_TESTS = 
BUILT_SOURCES = 
CLEANFILES = *~ test-file-002.1.bin test-file-002.2.bin test-file-003.incomplete.bin test-file-003.1.bin test-file-003.2.bin test-pipe-003.1 test-pipe-003.2 test-pipe-003.3 test-file-004.incomplete.bin test-file-004.bin test-file-004.1.bin test-file-004.2.bin

DISTCLEANFILES = ./deps/*.P ./log.test-*
EXTRA_DIST =  test-001 test-002 test-003 test-004 itest $(man_MANS) autogen debian/changelog debian/control debian/copyright debian/dirs debian/install debian/rules sourceforge.mk TROUBLESHOOTING troubleshooter

man_MANS =  bar.1
test_types_SOURCES = \
	test-types.c \
	#

test_001_pre_SOURCES = \
	test-001-pre.c \
	#

test_001_post_SOURCES = \
	test-001-post.c \
	#

bar_SOURCES = \
	error.c \
	fd.c \
	io.c \
	display.c \
	args.c \
	bar.c \
	#


##########################
#
# IBM Power5 Architecture:
#
##########################

# * openpower-linux1: SuSE Enterprise Linux 9, running on an IBM OpenPower 720 (e-Series) host.
SOURCEFORGE_COMPILE_FARM_HOSTS =  x86-linux2 x86-openbsd1 x86-solaris1 x86-linux1 x86-freebsd1 x86-netbsd1 amd64-linux1 amd64-linux2 alpha-linux1 ppc-osx1 ppc-osx2 sparc-solaris1 openpower-linux1

#
#------------------------------------------------------------------------------
#

FILE_DIR = ../dist
WEB_DIR = ../www
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = config.h
CONFIG_CLEAN_FILES =  bar.1 doxygen.conf
PROGRAMS =  $(bin_PROGRAMS) $(noinst_PROGRAMS)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I.
CPPFLAGS = -I/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include
LIBS = 
bar_OBJECTS =  error.o fd.o io.o display.o args.o bar.o
bar_LDADD = $(LDADD)
bar_DEPENDENCIES = 
bar_LDFLAGS = 
test_types_OBJECTS =  test-types.o
test_types_LDADD = $(LDADD)
test_types_DEPENDENCIES = 
test_types_LDFLAGS = 
test_001_pre_OBJECTS =  test-001-pre.o
test_001_pre_LDADD = $(LDADD)
test_001_pre_DEPENDENCIES = 
test_001_pre_LDFLAGS = 
test_001_post_OBJECTS =  test-001-post.o
test_001_post_LDADD = $(LDADD)
test_001_post_DEPENDENCIES = 
test_001_post_LDFLAGS = 
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
man1dir = $(mandir)/man1
MANS = $(man_MANS)

NROFF = nroff
HEADERS =  $(noinst_HEADERS)

DIST_COMMON =  ./stamp-h.in AUTHORS COPYING ChangeLog INSTALL \
Makefile.am Makefile.in TODO aclocal.m4 bar.1.in config.guess \
config.h.in config.sub configure configure.in doxygen.conf.in \
install-sh missing mkinstalldirs


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
SOURCES = $(bar_SOURCES) $(test_types_SOURCES) $(test_001_pre_SOURCES) $(test_001_post_SOURCES)
OBJECTS = $(bar_OBJECTS) $(test_types_OBJECTS) $(test_001_pre_OBJECTS) $(test_001_post_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) $(srcdir)/sourceforge.mk
	cd $(top_srcdir) && $(AUTOMAKE) --foreign --include-deps Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(ACLOCAL_M4):  configure.in 
	cd $(srcdir) && $(ACLOCAL)

config.status: $(srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck
$(srcdir)/configure: $(srcdir)/configure.in $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && $(AUTOCONF)

config.h: stamp-h
	@if test ! -f $@; then \
		rm -f stamp-h; \
		$(MAKE) stamp-h; \
	else :; fi
stamp-h: $(srcdir)/config.h.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=config.h \
	     $(SHELL) ./config.status
	@echo timestamp > stamp-h 2> /dev/null
$(srcdir)/config.h.in: $(srcdir)/stamp-h.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/stamp-h.in; \
		$(MAKE) $(srcdir)/stamp-h.in; \
	else :; fi
$(srcdir)/stamp-h.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOHEADER)
	@echo timestamp > $(srcdir)/stamp-h.in 2> /dev/null

mostlyclean-hdr:

clean-hdr:

distclean-hdr:
	-rm -f config.h

maintainer-clean-hdr:
bar.1: $(top_builddir)/config.status bar.1.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status
doxygen.conf: $(top_builddir)/config.status doxygen.conf.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

mostlyclean-binPROGRAMS:

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

distclean-binPROGRAMS:

maintainer-clean-binPROGRAMS:

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	done

mostlyclean-noinstPROGRAMS:

clean-noinstPROGRAMS:
	-test -z "$(noinst_PROGRAMS)" || rm -f $(noinst_PROGRAMS)

distclean-noinstPROGRAMS:

maintainer-clean-noinstPROGRAMS:

.c.o:
	$(COMPILE) -c $<

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

bar: $(bar_OBJECTS) $(bar_DEPENDENCIES)
	@rm -f bar
	$(LINK) $(bar_LDFLAGS) $(bar_OBJECTS) $(bar_LDADD) $(LIBS)

test-types: $(test_types_OBJECTS) $(test_types_DEPENDENCIES)
	@rm -f test-types
	$(LINK) $(test_types_LDFLAGS) $(test_types_OBJECTS) $(test_types_LDADD) $(LIBS)

test-001-pre: $(test_001_pre_OBJECTS) $(test_001_pre_DEPENDENCIES)
	@rm -f test-001-pre
	$(LINK) $(test_001_pre_LDFLAGS) $(test_001_pre_OBJECTS) $(test_001_pre_LDADD) $(LIBS)

test-001-post: $(test_001_post_OBJECTS) $(test_001_post_DEPENDENCIES)
	@rm -f test-001-post
	$(LINK) $(test_001_post_LDFLAGS) $(test_001_post_OBJECTS) $(test_001_post_LDADD) $(LIBS)

install-man1:
	$(mkinstalldirs) $(DESTDIR)$(man1dir)
	@list='$(man1_MANS)'; \
	l2='$(man_MANS)'; for i in $$l2; do \
	  case "$$i" in \
	    *.1*) list="$$list $$i" ;; \
	  esac; \
	done; \
	for i in $$list; do \
	  if test -f $(srcdir)/$$i; then file=$(srcdir)/$$i; \
	  else file=$$i; fi; \
	  ext=`echo $$i | sed -e 's/^.*\\.//'`; \
	  inst=`echo $$i | sed -e 's/\\.[0-9a-z]*$$//'`; \
	  inst=`echo $$inst | sed '$(transform)'`.$$ext; \
	  echo " $(INSTALL_DATA) $$file $(DESTDIR)$(man1dir)/$$inst"; \
	  $(INSTALL_DATA) $$file $(DESTDIR)$(man1dir)/$$inst; \
	done

uninstall-man1:
	@list='$(man1_MANS)'; \
	l2='$(man_MANS)'; for i in $$l2; do \
	  case "$$i" in \
	    *.1*) list="$$list $$i" ;; \
	  esac; \
	done; \
	for i in $$list; do \
	  ext=`echo $$i | sed -e 's/^.*\\.//'`; \
	  inst=`echo $$i | sed -e 's/\\.[0-9a-z]*$$//'`; \
	  inst=`echo $$inst | sed '$(transform)'`.$$ext; \
	  echo " rm -f $(DESTDIR)$(man1dir)/$$inst"; \
	  rm -f $(DESTDIR)$(man1dir)/$$inst; \
	done
install-man: $(MANS)
	@$(NORMAL_INSTALL)
	$(MAKE) $(AM_MAKEFLAGS) install-man1
uninstall-man:
	@$(NORMAL_UNINSTALL)
	$(MAKE) $(AM_MAKEFLAGS) uninstall-man1

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES) config.h.in $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)config.h.in$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags config.h.in $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	-rm -rf $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) zxf $(distdir).tar.gz
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	dc_install_base=`cd $(distdir)/=inst && pwd`; \
	cd $(distdir)/=build \
	  && ../configure --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) dist
	-rm -rf $(distdir)
	@banner="$(distdir).tar.gz is ready for distribution"; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$dashes"; \
	echo "$$banner"; \
	echo "$$dashes"
dist: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
dist-all: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
distdir: $(DISTFILES)
	-rm -rf $(distdir)
	mkdir $(distdir)
	-chmod 777 $(distdir)
	$(mkinstalldirs) $(distdir)/debian
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
check-TESTS: $(TESTS)
	@failed=0; all=0; \
	srcdir=$(srcdir); export srcdir; \
	for tst in $(TESTS); do \
	  if test -f $$tst; then dir=.; \
	  else dir="$(srcdir)"; fi; \
	  if $(TESTS_ENVIRONMENT) $$dir/$$tst; then \
	    all=`expr $$all + 1`; \
	    echo "PASS: $$tst"; \
	  elif test $$? -ne 77; then \
	    all=`expr $$all + 1`; \
	    failed=`expr $$failed + 1`; \
	    echo "FAIL: $$tst"; \
	  fi; \
	done; \
	if test "$$failed" -eq 0; then \
	  banner="All $$all tests passed"; \
	else \
	  banner="$$failed of $$all tests failed"; \
	fi; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$dashes"; \
	echo "$$banner"; \
	echo "$$dashes"; \
	test "$$failed" -eq 0
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
	$(MAKE) $(AM_MAKEFLAGS) check-TESTS
check: check-am
installcheck-am:
installcheck: installcheck-am
all-recursive-am: config.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

install-exec-am: install-binPROGRAMS
install-exec: install-exec-am

install-data-am: install-man
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-binPROGRAMS uninstall-man
uninstall: uninstall-am
all-am: Makefile $(PROGRAMS) $(MANS) $(HEADERS) config.h
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(bindir) $(DESTDIR)$(mandir)/man1


mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*
	-test -z "$(DISTCLEANFILES)" || rm -f $(DISTCLEANFILES)

maintainer-clean-generic:
	-test -z "$(BUILT_SOURCES)" || rm -f $(BUILT_SOURCES)
mostlyclean-am:  mostlyclean-hdr mostlyclean-binPROGRAMS \
		mostlyclean-noinstPROGRAMS mostlyclean-compile \
		mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-hdr clean-binPROGRAMS clean-noinstPROGRAMS \
		clean-compile clean-tags clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-hdr distclean-binPROGRAMS \
		distclean-noinstPROGRAMS distclean-compile \
		distclean-tags distclean-generic clean-am

distclean: distclean-am
	-rm -f config.status

maintainer-clean-am:  maintainer-clean-hdr maintainer-clean-binPROGRAMS \
		maintainer-clean-noinstPROGRAMS \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am
	-rm -f config.status

.PHONY: mostlyclean-hdr distclean-hdr clean-hdr maintainer-clean-hdr \
mostlyclean-binPROGRAMS distclean-binPROGRAMS clean-binPROGRAMS \
maintainer-clean-binPROGRAMS uninstall-binPROGRAMS install-binPROGRAMS \
mostlyclean-noinstPROGRAMS distclean-noinstPROGRAMS \
clean-noinstPROGRAMS maintainer-clean-noinstPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile install-man1 uninstall-man1 install-man \
uninstall-man tags mostlyclean-tags distclean-tags clean-tags \
maintainer-clean-tags distdir check-TESTS info-am info dvi-am dvi check \
check-am installcheck-am installcheck all-recursive-am install-exec-am \
install-exec install-data-am install-data install-am install \
uninstall-am uninstall all-redirect all-am all installdirs \
mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


clean-doc:
	rm -fr ./doc

bin_pkgs: dist
	#
	# Generate debain package file
	#
	[ -x /usr/bin/dpkg-source ] || exit 1
	[ -x /usr/bin/dpkg-buildpackage ] || exit 1
	[ -x /usr/bin/fakeroot ] || exit 1
	[ ! -z "$(distdir)" ] || exit 1
	-rm -fr $(distdir)
	$(MAKE) distdir
	dpkg-source -b $(distdir)
	cd $(distdir) && dpkg-buildpackage -rfakeroot
	rm -fr $(distdir)
	#
	# Generate redhat package file (from debian, via alien)
	#
	[ -x /usr/bin/alien ] || exit 1
	fakeroot alien --to-rpm $(PACKAGE)_$(VERSION)_*.deb
	#
	# Generate binary tgz package
	#
	fakeroot alien --to-tgz $(PACKAGE)_$(VERSION)_*.deb

#
# Create doxygen output
#
doc: doxygen.conf *.h *.c configure configure.in Makefile Makefile.am
	$(MAKE) clean-doc
	mkdir ./doc
	doxygen ./doxygen.conf

#
# Update files in ../dist/
# Update files in ../www/
#
update-dist: dist $(man_MANS) doc bin_pkgs
	[ ! -e $(FILE_DIR) ] || rm -fr $(FILE_DIR)
	-mkdir -p $(FILE_DIR)
	chmod 0755 $(FILE_DIR)
	-rm ./ChangeLog
	ln debian/changelog ./ChangeLog
	for file in ChangeLog COPYING INSTALL; do \
		cat $${file} \
		> $(FILE_DIR)/$(PACKAGE)-$(VERSION)-$${file} ; \
	done
	cp ./$(PACKAGE)-$(VERSION).tar.gz $(FILE_DIR)/
	cp ./$(PACKAGE)_$(VERSION)_*.deb $(FILE_DIR)/
	cp ./$(PACKAGE)_$(VERSION)_*.changes $(FILE_DIR)/
	cp ./$(PACKAGE)_$(VERSION).tar.gz $(FILE_DIR)/
	cp ./$(PACKAGE)_$(VERSION).dsc $(FILE_DIR)/
	cp ./$(PACKAGE)-$(VERSION)*.rpm $(FILE_DIR)/
	for file in $(man_MANS); do \
		cat $${file} \
			| groff -man -Tascii \
			| sed \
				-e 's,_,,g' \
				-e 's,.,,g' \
			> $(FILE_DIR)/$(PACKAGE)-$(VERSION)-man.txt \
			; \
	done
	for file in $(man_MANS); do \
		cat $${file} \
			| groff -man -Tdvi \
			> $(FILE_DIR)/$(PACKAGE)-$(VERSION)-man.dvi \
			; \
		dvipdf $(FILE_DIR)/$(PACKAGE)-$(VERSION)-man.dvi \
			$(FILE_DIR)/$(PACKAGE)-$(VERSION)-man.pdf \
			; \
		rm -f $(FILE_DIR)/$(PACKAGE)-$(VERSION)-man.dvi \
			; \
	done
	-rm -fr $(WEB_DIR)/doxygen/html
	(cd ./doc && tar -cf - html) \
	| (cd $(WEB_DIR)/doxygen && tar -xf -)
	-rm -fr $(WEB_DIR)/doxygen/$(VERSION)
	mv $(WEB_DIR)/doxygen/html \
		$(WEB_DIR)/doxygen/$(VERSION)
	-rm -f $(WEB_DIR)/template-02-$(VERSION).html
	cat $(WEB_DIR)/template-02.html \
	| sed "s,@VERS@,$(VERSION),g" \
	>> $(WEB_DIR)/template-02-$(VERSION).html
	-rm -f $(WEB_DIR)/index.html
	cat $(WEB_DIR)/template-01.html >> $(WEB_DIR)/index.html
	cat $(WEB_DIR)/template-02-*.html >> $(WEB_DIR)/index.html
	cat $(WEB_DIR)/template-03.html >> $(WEB_DIR)/index.html

update-web: doc $(man_MANS)
	ssh -l michaelpeek shell.sourceforge.net \
		"cd www/$(PACKAGE)/htdocs && rm -fr *"
	cd $(WEB_DIR) && tar -cvf - * \
	| ssh -l michaelpeek shell.sourceforge.net \
		"cd www/$(PACKAGE)/htdocs && tar -xvf -"

update: update-dist update-web

###############################################################################
#
# Automated Sourceforge Compilefarm Testing
#
###############################################################################

$(PACKAGE)-$(VERSION).tar.gz: $(DIST_COMMON) $(SOURCES)
	$(MAKE) dist

update-sourceforge-compilefarm: .update-sourceforge-compilefarm
.update-sourceforge-compilefarm: $(PACKAGE)-$(VERSION).tar.gz
	tar -cvf - ./$(PACKAGE)-$(VERSION).tar.gz \
	| ssh -l michaelpeek shell.cf.sourceforge.net \
		"tar -xvf -"
	touch $@

test-sourceforge-compilefarm-generic: .test-sourceforge-compilefarm-$(HOST)
.test-sourceforge-compilefarm-$(HOST): .update-sourceforge-compilefarm
	-ssh -l michaelpeek shell.cf.sourceforge.net \
		"test-project \"$(HOST)\" \"$(PACKAGE)\" \"$(VERSION)\"" 2>&1 \
		| tee log.sourceforge.$(HOST).txt \
		| sed 's,^,[$(HOST)]: ,g'

test-sourceforge-compilefarm: .update-sourceforge-compilefarm
	for host in $(SOURCEFORGE_COMPILE_FARM_HOSTS); do \
		$(MAKE) test-sourceforge-compilefarm-generic HOST="$${host}" ; \
	done ; \
	#

test-sourceforge: .update-sourceforge-compilefarm
	-rm -f log.sourceforge.*
	$(MAKE) test-sourceforge-compilefarm 2>&1 | tee log.sourceforge.txt

###############################################################################

beep:
	@printf ''
	@sleep 1
	@printf ''
	@sleep 1
	@printf ''

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
