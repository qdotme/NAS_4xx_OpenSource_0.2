srcdir = /opt/devel/proto/marvell/build-eabi/sqlite3-0.5
builddir = /opt/devel/proto/marvell/build-eabi/sqlite3-0.5
top_srcdir = /opt/devel/proto/marvell/build-eabi/sqlite3-0.5
top_builddir = /opt/devel/proto/marvell/build-eabi/sqlite3-0.5
SHLIB_SUFFIX_NAME = so
shared_objects_sqlite3 = php_sqlite3.lo
SQLITE3_SHARED_LIBADD = -Wl,-rpath,/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/usr/lib -L/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/usr/lib -lsqlite3
PHP_MODULES = $(phplibdir)/sqlite3.la
all_targets = $(PHP_MODULES)
install_targets = install-modules
prefix = /usr
exec_prefix = /usr
libdir = ${exec_prefix}/lib
prefix = /usr
phplibdir = /opt/devel/proto/marvell/build-eabi/sqlite3-0.5/modules
PHP_COMPILE =
CC = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-gcc
CFLAGS = -Os -pipe -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS_CLEAN = $(CFLAGS)
CPP = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-cpp
CPPFLAGS = -I/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include -L/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/usr/include -DHAVE_CONFIG_H
CXX = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-g++
DEFS =
EXTENSION_DIR = /usr/lib/php4
EXTRA_LDFLAGS =
EXTRA_LIBS =
INCLUDES = -I/usr/include/php -I/usr/include/php/main -I/usr/include/php/Zend -I/usr/include/php/TSRM -I/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/usr/include
LEX =
LEX_OUTPUT_ROOT =
LFLAGS =
LDFLAGS = -L/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/lib -L/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/usr/lib
SHARED_LIBTOOL =
LIBTOOL = $(SHELL) $(top_builddir)/libtool
SHELL = /bin/sh
AWK = gawk
RE2C = exit 0;
mkinstalldirs = $(top_srcdir)/build/shtool mkdir -p
INSTALL = $(top_srcdir)/build/shtool install -c
INSTALL_DATA = $(INSTALL) -m 644

DEFS = -DPHP_ATOM_INC -I$(top_builddir)/include -I$(top_builddir)/main -I$(top_srcdir)
COMMON_FLAGS = $(DEFS) $(INCLUDES) $(EXTRA_INCLUDES) $(CPPFLAGS) $(PHP_FRAMEWORKPATH)


all: $(all_targets) 
	@echo
	@echo "Build complete."
	@echo "(It is safe to ignore warnings about tempnam and tmpnam)."
	@echo
	
build-modules: $(PHP_MODULES)

libphp4.la: $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS)
	$(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(EXTRA_CFLAGS) -rpath $(phptempdir) $(EXTRA_LDFLAGS) $(LDFLAGS) $(PHP_RPATHS) $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS) $(EXTRA_LIBS) $(ZEND_EXTRA_LIBS) -o $@
	-@$(LIBTOOL) --silent --mode=install cp libphp4.la $(phptempdir)/libphp4.la >/dev/null 2>&1

libs/libphp4.bundle: $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS)
	$(CC) $(MH_BUNDLE_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) $(PHP_GLOBAL_OBJS:.lo=.o) $(PHP_SAPI_OBJS:.lo=.o) $(PHP_FRAMEWORKS) $(EXTRA_LIBS) $(ZEND_EXTRA_LIBS) -o $@ && cp $@ libs/libphp4.so

install: $(all_targets) $(install_targets)

install-sapi: $(OVERALL_TARGET)
	@echo "Installing PHP SAPI module:       $(PHP_SAPI)"
	-@$(mkinstalldirs) $(INSTALL_ROOT)$(bindir)
	-@if test ! -r $(phptempdir)/libphp4.$(SHLIB_SUFFIX_NAME); then \
		for i in 0.0.0 0.0 0; do \
			if test -r $(phptempdir)/libphp4.$(SHLIB_SUFFIX_NAME).$$i; then \
				$(LN_S) $(phptempdir)/libphp4.$(SHLIB_SUFFIX_NAME).$$i $(phptempdir)/libphp4.$(SHLIB_SUFFIX_NAME); \
				break; \
			fi; \
		done; \
	fi
	@$(INSTALL_IT)

install-modules: build-modules
	@test -d modules && \
	$(mkinstalldirs) $(INSTALL_ROOT)$(EXTENSION_DIR)
	@echo "Installing shared extensions:     $(INSTALL_ROOT)$(EXTENSION_DIR)/"
	@rm -f modules/*.la >/dev/null 2>&1
	@$(INSTALL) modules/* $(INSTALL_ROOT)$(EXTENSION_DIR)

install-tester:
	@echo "Installing regression tester:     $(INSTALL_ROOT)$(PEAR_INSTALLDIR)/"
	@$(mkinstalldirs) $(INSTALL_ROOT)$(PEAR_INSTALLDIR)
	@$(INSTALL) -m 755 $(top_srcdir)/run-tests.php $(INSTALL_ROOT)$(PEAR_INSTALLDIR)

install-su: install-pear install-tester

test: 
	-@if test -x $(SAPI_CLI_PATH) && test ! -z $(SAPI_CLI_PATH); then \
		TEST_PHP_EXECUTABLE=$(top_builddir)/$(SAPI_CLI_PATH) \
		TEST_PHP_SRCDIR=$(top_srcdir) \
		CC="$(CC)" \
			$(top_builddir)/$(SAPI_CLI_PATH) -n -d 'open_basedir=' -d 'safe_mode=0' -d 'output_buffering=0' $(top_srcdir)/run-tests.php $(TESTS); \
	else \
		echo "ERROR: Cannot run tests without CLI sapi."; \
	fi

clean:
	find . -name \*.lo | xargs rm -f
	find . -name \*.o | xargs rm -f
	find . -name \*.la | xargs rm -f
	find . -name \*.a | xargs rm -f
	find . -name \*.so | xargs rm -f
	find . -name .libs -a -type d|xargs rm -rf
	rm -f libphp4.la $(SAPI_CLI_PATH) $(OVERALL_TARGET) modules/* libs/*

distclean: clean
	rm -f config.cache config.log config.status Makefile.objects Makefile.fragments libtool main/php_config.h stamp-h php4.spec sapi/apache/libphp4.module buildmk.stamp
	egrep define'.*include/php' configure|sed 's/.*>//'|xargs rm -f
	find . -name Makefile | xargs rm -f

.PHONY: all clean install distclean test
.NOEXPORT:
php_sqlite3.lo: /opt/devel/proto/marvell/build-eabi/sqlite3-0.5/php_sqlite3.c
	$(LIBTOOL) --mode=compile $(CC)  -I. -I/opt/devel/proto/marvell/build-eabi/sqlite3-0.5 $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -prefer-pic -c /opt/devel/proto/marvell/build-eabi/sqlite3-0.5/php_sqlite3.c -o php_sqlite3.lo 
$(phplibdir)/sqlite3.la: ./sqlite3.la
	$(LIBTOOL) --mode=install cp ./sqlite3.la $(phplibdir)

./sqlite3.la: $(shared_objects_sqlite3) $(SQLITE3_SHARED_DEPENDENCIES)
	$(LIBTOOL) --mode=link $(CC) $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) $(LDFLAGS) -o $@ -export-dynamic -avoid-version -prefer-pic -module -rpath $(phplibdir) $(EXTRA_LDFLAGS) $(shared_objects_sqlite3) $(SQLITE3_SHARED_LIBADD)

