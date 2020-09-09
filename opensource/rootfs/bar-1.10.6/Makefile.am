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
CFLAGS = -O2
# CFLAGS = -g -Wall

# LDFLAGS =
LDFLAGS = -s

bin_PROGRAMS =
noinst_PROGRAMS =
noinst_HEADERS =
TESTS =
nodist_TESTS = 
BUILT_SOURCES =
CLEANFILES = *~
DISTCLEANFILES = ./deps/*.P
EXTRA_DIST =
man_MANS =

noinst_HEADERS += headers.h

noinst_PROGRAMS += test-types
test_types_SOURCES = \
	test-types.c \
	#
TESTS += test-types

# noinst_PROGRAMS += test-args
# test_args_SOURCES = \
# 	error.c \
# 	fd.c \
# 	io.c \
# 	display.c \
# 	args.c \
# 	test-args.c \
# 	#
# TESTS += test-args

noinst_PROGRAMS += test-001-pre
test_001_pre_SOURCES = \
	test-001-pre.c \
	#

noinst_PROGRAMS += test-001-post
test_001_post_SOURCES = \
	test-001-post.c \
	#

TESTS += test-001
EXTRA_DIST += test-001

TESTS += test-002
EXTRA_DIST += test-002
CLEANFILES += test-file-002.1.bin test-file-002.2.bin

# TESTS += test-003
EXTRA_DIST += test-003
CLEANFILES += test-file-003.incomplete.bin
CLEANFILES += test-file-003.1.bin
CLEANFILES += test-file-003.2.bin
CLEANFILES += test-pipe-003.1
CLEANFILES += test-pipe-003.2
CLEANFILES += test-pipe-003.3

# TESTS += test-004
EXTRA_DIST += test-004
CLEANFILES += test-file-004.incomplete.bin
CLEANFILES += test-file-004.bin
CLEANFILES += test-file-004.1.bin
CLEANFILES += test-file-004.2.bin

EXTRA_DIST += itest

DISTCLEANFILES += ./log.test-*

bin_PROGRAMS += bar
noinst_HEADERS += types.h
noinst_HEADERS += error.h
noinst_HEADERS += fd.h
noinst_HEADERS += io.h
noinst_HEADERS += display.h
noinst_HEADERS += args.h
bar_SOURCES = \
	error.c \
	fd.c \
	io.c \
	display.c \
	args.c \
	bar.c \
	#

man_MANS += bar.1
EXTRA_DIST += $(man_MANS)

# EXTRA_DIST += PORTING

EXTRA_DIST += autogen

EXTRA_DIST += debian/changelog
EXTRA_DIST += debian/control
EXTRA_DIST += debian/copyright
EXTRA_DIST += debian/dirs
EXTRA_DIST += debian/install
EXTRA_DIST += debian/rules
EXTRA_DIST += sourceforge.mk

EXTRA_DIST += TROUBLESHOOTING
EXTRA_DIST += troubleshooter

include sourceforge.mk

#
#------------------------------------------------------------------------------
#

FILE_DIR = ../dist
WEB_DIR = ../www

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
