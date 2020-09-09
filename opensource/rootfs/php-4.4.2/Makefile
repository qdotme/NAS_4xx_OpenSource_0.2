srcdir = /opt/devel/proto/marvell/build-eabi/php-4.4.2
builddir = /opt/devel/proto/marvell/build-eabi/php-4.4.2
top_srcdir = /opt/devel/proto/marvell/build-eabi/php-4.4.2
top_builddir = /opt/devel/proto/marvell/build-eabi/php-4.4.2
EGREP = grep -E
SED = /bin/sed
SHLIB_SUFFIX_NAME = so
SAPI_CLI_PATH = sapi/cli/php
BUILD_CLI = $(LIBTOOL) --mode=link $(CC) -export-dynamic $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) $(EXTRA_LDFLAGS_PROGRAM) $(LDFLAGS) $(PHP_RPATHS) $(PHP_GLOBAL_OBJS) $(PHP_CLI_OBJS) $(EXTRA_LIBS) $(ZEND_EXTRA_LIBS) -o $(SAPI_CLI_PATH)
INSTALL_CLI = $(mkinstalldirs) $(INSTALL_ROOT)$(bindir); $(INSTALL) -m 0755 $(SAPI_CLI_PATH) $(INSTALL_ROOT)$(bindir)/$(program_prefix)php$(program_suffix)
SERVLET_CLASSPATH =
SAPI_CGI_PATH = sapi/cgi/php
BUILD_CGI = $(LIBTOOL) --mode=link $(CC) -export-dynamic $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) $(EXTRA_LDFLAGS_PROGRAM) $(LDFLAGS) $(PHP_RPATHS) $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS) $(EXTRA_LIBS) $(ZEND_EXTRA_LIBS) -o $(SAPI_CGI_PATH)
OPENSSL_SHARED_LIBADD =
PCRE_SHARED_LIBADD =
ZLIB_SHARED_LIBADD =
MBSTRING_SHARED_LIBADD =
MYSQL_SHARED_LIBADD =
MYSQL_MODULE_TYPE = builtin
MYSQL_LIBS =
MYSQL_INCLUDE =
SESSION_SHARED_LIBADD =
PHP_CLI_TARGET = $(SAPI_CLI_PATH)
PHP_SAPI_OBJS = sapi/cgi/cgi_main.lo sapi/cgi/getopt.lo main/internal_functions.lo
PHP_CLI_OBJS = sapi/cli/php_cli.lo sapi/cli/getopt.lo main/internal_functions_cli.lo
PHP_GLOBAL_OBJS = ext/openssl/openssl.lo ext/pcre/php_pcre.lo ext/zlib/zlib.lo ext/zlib/zlib_fopen_wrapper.lo ext/ctype/ctype.lo ext/mbstring/mbstring.lo ext/mbstring/php_unicode.lo ext/mbstring/php_mbregex.lo ext/mbstring/mbregex/mbregex.lo ext/mbstring/libmbfl/filters/html_entities.lo ext/mbstring/libmbfl/filters/mbfilter_7bit.lo ext/mbstring/libmbfl/filters/mbfilter_ascii.lo ext/mbstring/libmbfl/filters/mbfilter_base64.lo ext/mbstring/libmbfl/filters/mbfilter_big5.lo ext/mbstring/libmbfl/filters/mbfilter_byte2.lo ext/mbstring/libmbfl/filters/mbfilter_byte4.lo ext/mbstring/libmbfl/filters/mbfilter_cp1251.lo ext/mbstring/libmbfl/filters/mbfilter_cp1252.lo ext/mbstring/libmbfl/filters/mbfilter_cp866.lo ext/mbstring/libmbfl/filters/mbfilter_cp932.lo ext/mbstring/libmbfl/filters/mbfilter_cp936.lo ext/mbstring/libmbfl/filters/mbfilter_euc_cn.lo ext/mbstring/libmbfl/filters/mbfilter_euc_jp.lo ext/mbstring/libmbfl/filters/mbfilter_euc_jp_win.lo ext/mbstring/libmbfl/filters/mbfilter_euc_kr.lo ext/mbstring/libmbfl/filters/mbfilter_euc_tw.lo ext/mbstring/libmbfl/filters/mbfilter_htmlent.lo ext/mbstring/libmbfl/filters/mbfilter_hz.lo ext/mbstring/libmbfl/filters/mbfilter_iso2022_kr.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_1.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_10.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_13.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_14.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_15.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_16.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_2.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_3.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_4.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_5.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_6.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_7.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_8.lo ext/mbstring/libmbfl/filters/mbfilter_iso8859_9.lo ext/mbstring/libmbfl/filters/mbfilter_jis.lo ext/mbstring/libmbfl/filters/mbfilter_koi8r.lo ext/mbstring/libmbfl/filters/mbfilter_qprint.lo ext/mbstring/libmbfl/filters/mbfilter_armscii8.lo ext/mbstring/libmbfl/filters/mbfilter_sjis.lo ext/mbstring/libmbfl/filters/mbfilter_ucs2.lo ext/mbstring/libmbfl/filters/mbfilter_ucs4.lo ext/mbstring/libmbfl/filters/mbfilter_uhc.lo ext/mbstring/libmbfl/filters/mbfilter_utf16.lo ext/mbstring/libmbfl/filters/mbfilter_utf32.lo ext/mbstring/libmbfl/filters/mbfilter_utf7.lo ext/mbstring/libmbfl/filters/mbfilter_utf7imap.lo ext/mbstring/libmbfl/filters/mbfilter_utf8.lo ext/mbstring/libmbfl/filters/mbfilter_uuencode.lo ext/mbstring/libmbfl/mbfl/mbfilter.lo ext/mbstring/libmbfl/mbfl/mbfilter_8bit.lo ext/mbstring/libmbfl/mbfl/mbfilter_pass.lo ext/mbstring/libmbfl/mbfl/mbfilter_wchar.lo ext/mbstring/libmbfl/mbfl/mbfl_convert.lo ext/mbstring/libmbfl/mbfl/mbfl_encoding.lo ext/mbstring/libmbfl/mbfl/mbfl_filter_output.lo ext/mbstring/libmbfl/mbfl/mbfl_ident.lo ext/mbstring/libmbfl/mbfl/mbfl_language.lo ext/mbstring/libmbfl/mbfl/mbfl_memory_device.lo ext/mbstring/libmbfl/mbfl/mbfl_string.lo ext/mbstring/libmbfl/mbfl/mbfl_allocators.lo ext/mbstring/libmbfl/nls/nls_de.lo ext/mbstring/libmbfl/nls/nls_en.lo ext/mbstring/libmbfl/nls/nls_ja.lo ext/mbstring/libmbfl/nls/nls_kr.lo ext/mbstring/libmbfl/nls/nls_neutral.lo ext/mbstring/libmbfl/nls/nls_ru.lo ext/mbstring/libmbfl/nls/nls_uni.lo ext/mbstring/libmbfl/nls/nls_zh.lo ext/mbstring/libmbfl/nls/nls_hy.lo ext/mbstring/libmbfl/nls/nls_tr.lo ext/mysql/php_mysql.lo ext/mysql/libmysql/libmysql.lo ext/mysql/libmysql/errmsg.lo ext/mysql/libmysql/net.lo ext/mysql/libmysql/violite.lo ext/mysql/libmysql/password.lo ext/mysql/libmysql/my_init.lo ext/mysql/libmysql/my_lib.lo ext/mysql/libmysql/my_static.lo ext/mysql/libmysql/my_malloc.lo ext/mysql/libmysql/my_realloc.lo ext/mysql/libmysql/my_create.lo ext/mysql/libmysql/my_delete.lo ext/mysql/libmysql/my_tempnam.lo ext/mysql/libmysql/my_open.lo ext/mysql/libmysql/mf_casecnv.lo ext/mysql/libmysql/my_read.lo ext/mysql/libmysql/my_write.lo ext/mysql/libmysql/errors.lo ext/mysql/libmysql/my_error.lo ext/mysql/libmysql/my_getwd.lo ext/mysql/libmysql/my_div.lo ext/mysql/libmysql/mf_pack.lo ext/mysql/libmysql/my_messnc.lo ext/mysql/libmysql/mf_dirname.lo ext/mysql/libmysql/mf_fn_ext.lo ext/mysql/libmysql/mf_wcomp.lo ext/mysql/libmysql/typelib.lo ext/mysql/libmysql/safemalloc.lo ext/mysql/libmysql/my_alloc.lo ext/mysql/libmysql/mf_format.lo ext/mysql/libmysql/mf_path.lo ext/mysql/libmysql/mf_unixpath.lo ext/mysql/libmysql/my_fopen.lo ext/mysql/libmysql/mf_loadpath.lo ext/mysql/libmysql/my_pthread.lo ext/mysql/libmysql/my_thr_init.lo ext/mysql/libmysql/thr_mutex.lo ext/mysql/libmysql/mulalloc.lo ext/mysql/libmysql/string.lo ext/mysql/libmysql/default.lo ext/mysql/libmysql/my_compress.lo ext/mysql/libmysql/array.lo ext/mysql/libmysql/my_once.lo ext/mysql/libmysql/list.lo ext/mysql/libmysql/my_net.lo ext/mysql/libmysql/dbug.lo ext/mysql/libmysql/strmov.lo ext/mysql/libmysql/strxmov.lo ext/mysql/libmysql/strnmov.lo ext/mysql/libmysql/strmake.lo ext/mysql/libmysql/strend.lo ext/mysql/libmysql/strfill.lo ext/mysql/libmysql/is_prefix.lo ext/mysql/libmysql/int2str.lo ext/mysql/libmysql/str2int.lo ext/mysql/libmysql/strinstr.lo ext/mysql/libmysql/strcont.lo ext/mysql/libmysql/strcend.lo ext/mysql/libmysql/bchange.lo ext/mysql/libmysql/bmove.lo ext/mysql/libmysql/bmove_upp.lo ext/mysql/libmysql/longlong2str.lo ext/mysql/libmysql/strtoull.lo ext/mysql/libmysql/strtoll.lo ext/mysql/libmysql/charset.lo ext/mysql/libmysql/ctype.lo ext/overload/overload.lo ext/pcntl/pcntl.lo ext/pcntl/php_signal.lo ext/posix/posix.lo ext/session/session.lo ext/session/mod_files.lo ext/session/mod_mm.lo ext/session/mod_user.lo regex/regcomp.lo regex/regexec.lo regex/regerror.lo regex/regfree.lo ext/standard/array.lo ext/standard/base64.lo ext/standard/basic_functions.lo ext/standard/browscap.lo ext/standard/crc32.lo ext/standard/crypt.lo ext/standard/cyr_convert.lo ext/standard/datetime.lo ext/standard/dir.lo ext/standard/dl.lo ext/standard/dns.lo ext/standard/exec.lo ext/standard/file.lo ext/standard/filestat.lo ext/standard/flock_compat.lo ext/standard/formatted_print.lo ext/standard/fsock.lo ext/standard/head.lo ext/standard/html.lo ext/standard/image.lo ext/standard/info.lo ext/standard/iptc.lo ext/standard/lcg.lo ext/standard/link.lo ext/standard/mail.lo ext/standard/math.lo ext/standard/md5.lo ext/standard/metaphone.lo ext/standard/microtime.lo ext/standard/pack.lo ext/standard/pageinfo.lo ext/standard/parsedate.lo ext/standard/quot_print.lo ext/standard/rand.lo ext/standard/reg.lo ext/standard/soundex.lo ext/standard/string.lo ext/standard/scanf.lo ext/standard/syslog.lo ext/standard/type.lo ext/standard/uniqid.lo ext/standard/url.lo ext/standard/url_scanner.lo ext/standard/var.lo ext/standard/versioning.lo ext/standard/assert.lo ext/standard/strnatcmp.lo ext/standard/levenshtein.lo ext/standard/incomplete_class.lo ext/standard/url_scanner_ex.lo ext/standard/ftp_fopen_wrapper.lo ext/standard/http_fopen_wrapper.lo ext/standard/php_fopen_wrapper.lo ext/standard/credits.lo ext/standard/css.lo ext/standard/var_unserializer.lo ext/standard/ftok.lo ext/standard/aggregation.lo ext/standard/sha1.lo ext/tokenizer/tokenizer.lo ext/xml/xml.lo ext/xml/expat/xmlparse.lo ext/xml/expat/xmlrole.lo ext/xml/expat/xmltok.lo TSRM/TSRM.lo TSRM/tsrm_strtok_r.lo TSRM/tsrm_virtual_cwd.lo main/main.lo main/snprintf.lo main/spprintf.lo main/php_sprintf.lo main/safe_mode.lo main/fopen_wrappers.lo main/alloca.lo main/php_scandir.lo main/php_ini.lo main/SAPI.lo main/rfc1867.lo main/php_content_types.lo main/strlcpy.lo main/strlcat.lo main/mergesort.lo main/reentrancy.lo main/php_variables.lo main/php_ticks.lo main/streams.lo main/network.lo main/php_open_temporary_file.lo main/php_logos.lo main/output.lo main/memory_streams.lo main/user_streams.lo Zend/zend_language_parser.lo Zend/zend_language_scanner.lo Zend/zend_ini_parser.lo Zend/zend_ini_scanner.lo Zend/zend_alloc.lo Zend/zend_compile.lo Zend/zend_constants.lo Zend/zend_dynamic_array.lo Zend/zend_execute_API.lo Zend/zend_highlight.lo Zend/zend_llist.lo Zend/zend_opcode.lo Zend/zend_operators.lo Zend/zend_ptr_stack.lo Zend/zend_stack.lo Zend/zend_variables.lo Zend/zend.lo Zend/zend_API.lo Zend/zend_extensions.lo Zend/zend_hash.lo Zend/zend_list.lo Zend/zend_indent.lo Zend/zend_builtin_functions.lo Zend/zend_sprintf.lo Zend/zend_ini.lo Zend/zend_qsort.lo Zend/zend_multibyte.lo Zend/zend_strtod.lo Zend/zend_execute.lo
PHP_MODULES =
EXT_LIBS =
abs_builddir = /opt/devel/proto/marvell/build-eabi/php-4.4.2
abs_srcdir = /opt/devel/proto/marvell/build-eabi/php-4.4.2
php_abs_top_builddir = /opt/devel/proto/marvell/build-eabi/php-4.4.2
php_abs_top_srcdir = /opt/devel/proto/marvell/build-eabi/php-4.4.2
bindir = /usr/bin
exec_prefix = /usr
program_prefix =
program_suffix =
includedir = ${prefix}/include
libdir = ${exec_prefix}/lib/php
mandir = /usr/man
phplibdir = /opt/devel/proto/marvell/build-eabi/php-4.4.2/modules
phptempdir = /opt/devel/proto/marvell/build-eabi/php-4.4.2/libs
prefix = /usr
localstatedir = /var
datadir = /usr/share
sysconfdir = /etc
AWK = gawk
CC = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-gcc
CFLAGS = $(CFLAGS_CLEAN) -prefer-non-pic -static
CFLAGS_CLEAN = -Os -pipe -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CONFIGURE_COMMAND = './configure' '--target=arm-linux' '--host=arm-linux' '--build=i386-pc-linux-gnu' '--prefix=/usr' '--exec-prefix=/usr' '--bindir=/usr/bin' '--sbindir=/usr/sbin' '--libexecdir=/usr/lib' '--sysconfdir=/etc' '--datadir=/usr/share' '--localstatedir=/var' '--mandir=/usr/man' '--infodir=/usr/info' '--with-config-file-path=/etc' '--enable-shared' '--disable-static' '--disable-ipv6' '--without-pear' '--enable-xml' '--with-pcre-regex=/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi' '--with-openssl=/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi' '--enable-discard-path' '--enable-inline-optimization' '--enable-memory-limit' '--disable-debug' '--with-zlib-dir=/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi' '--enable-pcntl' '--enable-mbstring=all'
CPP = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-cpp
CPPFLAGS = -I/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include
CXX = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-g++
CXXFLAGS = -prefer-non-pic -static
CXXFLAGS_CLEAN =
CXX_PHP_COMPILE = $(LIBTOOL) --mode=compile $(CXX_COMPILE) -c $<
DEBUG_CFLAGS =
EXTENSION_DIR = /usr/lib/php/extensions/no-debug-non-zts-20020429
EXTRA_LDFLAGS = -L/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/lib
EXTRA_LDFLAGS_PROGRAM = -L/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/lib
EXTRA_LIBS = -lcrypt -lcrypt -lz -lpcre -lssl -lcrypto -lm -lcrypt -lcrypt
ZEND_EXTRA_LIBS =
INCLUDES = -I/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/include -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/mbregex -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/expat -I$(top_builddir)/TSRM -I$(top_builddir)/Zend
EXTRA_INCLUDES =
INCLUDE_PATH = .:/inc:
INSTALL_IT = @echo "Installing PHP CGI into: $(INSTALL_ROOT)$(bindir)/"; $(INSTALL) -m 0755 $(SAPI_CGI_PATH) $(INSTALL_ROOT)$(bindir)/$(program_prefix)php$(program_suffix)
LEX = flex
LEX_OUTPUT_ROOT = lex.yy
LFLAGS =
LIBTOOL = $(SHELL) $(top_builddir)/libtool --silent --preserve-dup-deps
LN_S = ln -s
NATIVE_RPATHS = -Wl,-rpath,/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/lib
PEAR_INSTALLDIR =
PHP_BUILD_DATE = 2009-03-25
PHP_COMPILE = $(LIBTOOL) --mode=compile $(COMPILE) -c $<
PHP_LDFLAGS = -L/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/lib
PHP_LIBS =
OVERALL_TARGET = $(SAPI_CGI_PATH)
PHP_RPATHS = -R /opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/lib
PHP_SAPI = cgi
PHP_VERSION = 4.4.2
PROG_SENDMAIL =
RE2C = exit 0;
SHELL = /bin/sh
SHARED_LIBTOOL = $(LIBTOOL)
WARNING_LEVEL =
PHP_FRAMEWORKS =
PHP_FRAMEWORKPATH =
YACC = bison -y
SHLIB_SUFFIX_NAME = so
all_targets = $(OVERALL_TARGET) $(PHP_MODULES) $(PHP_CLI_TARGET)
install_targets = install-sapi install-build install-headers install-programs
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
			$(top_builddir)/$(SAPI_CLI_PATH) -d 'open_basedir=' -d 'safe_mode=0' -d 'output_buffering=0' $(top_srcdir)/run-tests.php $(TESTS); \
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
	egrep define'.*include/php' $(top_srcdir)/configure|sed 's/.*>//'|xargs rm -f
	find . -name Makefile | xargs rm -f

.PHONY: all clean install distclean test
.NOEXPORT:
$(SAPI_CLI_PATH): $(PHP_GLOBAL_OBJS) $(PHP_CLI_OBJS)
	$(BUILD_CLI)

install-cli: $(SAPI_CLI_PATH)
	@echo "Installing PHP CLI binary:        $(INSTALL_ROOT)$(bindir)/"
	@$(INSTALL_CLI)
	@echo "Installing PHP CLI man page:      $(INSTALL_ROOT)$(mandir)/man1/"
	@$(mkinstalldirs) $(INSTALL_ROOT)$(mandir)/man1
	@$(INSTALL_DATA) sapi/cli/php.1 $(INSTALL_ROOT)$(mandir)/man1/$(program_prefix)php$(program_suffix).1
$(SAPI_CGI_PATH): $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS)
	$(BUILD_CGI)

/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/parsedate.c: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/parsedate.y

/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/var_unserializer.c: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/var_unserializer.re
	@(cd $(top_srcdir); $(RE2C) -b -o ext/standard/var_unserializer.c ext/standard/var_unserializer.re)

/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/url_scanner_ex.c: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/url_scanner_ex.re
	@(cd $(top_srcdir); $(RE2C) -b -o ext/standard/url_scanner_ex.c	ext/standard/url_scanner_ex.re)

ext/standard/info.lo: ext/standard/../../main/build-defs.h
Zend/zend_language_parser.h:
ext/tokenizer/tokenizer.lo: Zend/zend_language_parser.h

#
# Build environment install
#

phpincludedir = $(includedir)/php
phpbuilddir = $(libdir)/build

BUILD_FILES = \
	scripts/phpize.m4 \
	build/mkdep.awk \
	build/scan_makefile_in.awk \
	build/libtool.m4 \
	Makefile.global \
	acinclude.m4 \
	ltmain.sh

BUILD_FILES_EXEC = \
	build/shtool \
	config.guess \
	config.sub

bin_SCRIPTS = phpize php-config
man_PAGES = phpize php-config

install-build:
	@echo "Installing build environment:     $(INSTALL_ROOT)$(phpbuilddir)/"
	@$(mkinstalldirs) $(INSTALL_ROOT)$(phpbuilddir) $(INSTALL_ROOT)$(bindir) && \
	(cd $(top_srcdir) && \
	$(INSTALL) $(BUILD_FILES_EXEC) $(INSTALL_ROOT)$(phpbuilddir) && \
	$(INSTALL_DATA) $(BUILD_FILES) $(INSTALL_ROOT)$(phpbuilddir))

HEADER_DIRS = \
	/ \
	Zend/ \
	TSRM/ \
	include/ \
	main/ \
	regex/ \
	ext/iconv/ \
	ext/pgsql/ \
	ext/standard/ \
	ext/session/ \
	ext/xml/ \
	ext/xml/expat/ \
	ext/mbstring/ \
	ext/mbstring/libmbfl/ \
	ext/mbstring/libmbfl/mbfl/

install-headers:
	-@for i in $(HEADER_DIRS); do \
		i=`$(top_srcdir)/build/shtool path -d $$i`; \
		paths="$$paths $(INSTALL_ROOT)$(phpincludedir)/$$i"; \
	done; \
	$(mkinstalldirs) $$paths && \
	echo "Installing header files:          $(INSTALL_ROOT)$(phpincludedir)/" && \
	for i in $(HEADER_DIRS); do \
		if test -f "$(top_srcdir)/$$i"; then \
			$(INSTALL_DATA) $(top_srcdir)/$$i $(INSTALL_ROOT)$(phpincludedir)/$$i; \
		elif test -f "$(top_builddir)/$$i"; then \
			$(INSTALL_DATA) $(top_builddir)/$$i $(INSTALL_ROOT)$(phpincludedir)/$$i; \
		else \
			(cd $(top_srcdir)/$$i && $(INSTALL_DATA) *.h $(INSTALL_ROOT)$(phpincludedir)/$$i; \
			cd $(top_builddir)/$$i && $(INSTALL_DATA) *.h $(INSTALL_ROOT)$(phpincludedir)/$$i) 2>/dev/null || true; \
		fi \
	done; \
	cd $(top_srcdir)/sapi/embed && $(INSTALL_DATA) *.h $(INSTALL_ROOT)$(phpincludedir)/main

install-programs: scripts/phpize scripts/php-config
	@echo "Installing helper programs:       $(INSTALL_ROOT)$(bindir)/"
	@for prog in $(bin_SCRIPTS); do \
		echo "  program: $(program_prefix)$${prog}$(program_suffix)"; \
		$(INSTALL) -m 755 scripts/$${prog} $(INSTALL_ROOT)$(bindir)/$(program_prefix)$${prog}$(program_suffix); \
	done
	@echo "Installing man pages:             $(INSTALL_ROOT)$(mandir)/man1/"
	@$(mkinstalldirs) $(INSTALL_ROOT)$(mandir)/man1
	@for page in $(man_PAGES); do \
		echo "  page: $(program_prefix)$${page}$(program_suffix).1"; \
		$(INSTALL_DATA) scripts/man1/$${page}.1 $(INSTALL_ROOT)$(mandir)/man1/$(program_prefix)$${page}$(program_suffix).1; \
	done
	
scripts/phpize: /opt/devel/proto/marvell/build-eabi/php-4.4.2/scripts/phpize.in $(top_builddir)/config.status
	(CONFIG_FILES=$@ CONFIG_HEADERS= $(top_builddir)/config.status)

scripts/php-config: /opt/devel/proto/marvell/build-eabi/php-4.4.2/scripts/php-config.in $(top_builddir)/config.status
	(CONFIG_FILES=$@ CONFIG_HEADERS= $(top_builddir)/config.status)

#
# Zend
#

Zend/zend_language_scanner.lo: Zend/zend_language_parser.h
Zend/zend_ini_scanner.lo: Zend/zend_ini_parser.h

Zend/zend_language_scanner.c: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_language_scanner.l
	$(LEX) -Pzend -S/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/flex.skl -o$@ -i /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_language_scanner.l

Zend/zend_language_parser.h: Zend/zend_language_parser.c
Zend/zend_language_parser.c: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_language_parser.y
	$(YACC) -p zend -v -d /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_language_parser.y -o $@

Zend/zend_ini_parser.h: Zend/zend_ini_parser.c
Zend/zend_ini_parser.c: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_ini_parser.y
	$(YACC) -p ini_ -v -d /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_ini_parser.y -o $@

Zend/zend_ini_scanner.c: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_ini_scanner.l
	$(LEX) -Pini_ -S/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/flex.skl -o$@ -i /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_ini_scanner.l

Zend/zend_indent.lo Zend/zend_highlight.lo Zend/zend_compile.lo: Zend/zend_language_parser.h
sapi/cgi/cgi_main.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cgi/cgi_main.c
	$(CC)  -Isapi/cgi/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cgi/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cgi/cgi_main.c -o sapi/cgi/cgi_main.o  && echo > $@
sapi/cgi/getopt.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cgi/getopt.c
	$(CC)  -Isapi/cgi/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cgi/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cgi/getopt.c -o sapi/cgi/getopt.o  && echo > $@
ext/openssl/openssl.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/openssl/openssl.c
	$(CC)  -Iext/openssl/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/openssl/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/openssl/openssl.c -o ext/openssl/openssl.o  && echo > $@
ext/pcre/php_pcre.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/pcre/php_pcre.c
	$(CC) -DEXPORT= -DNEWLINE=10 -DSUPPORT_UTF8 -DSUPPORT_UCP -DLINK_SIZE=2 -DPOSIX_MALLOC_THRESHOLD=10 -DMATCH_LIMIT=10000000 -Iext/pcre/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/pcre/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/pcre/php_pcre.c -o ext/pcre/php_pcre.o  && echo > $@
ext/zlib/zlib.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/zlib/zlib.c
	$(CC)  -Iext/zlib/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/zlib/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/zlib/zlib.c -o ext/zlib/zlib.o  && echo > $@
ext/zlib/zlib_fopen_wrapper.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/zlib/zlib_fopen_wrapper.c
	$(CC)  -Iext/zlib/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/zlib/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/zlib/zlib_fopen_wrapper.c -o ext/zlib/zlib_fopen_wrapper.o  && echo > $@
ext/ctype/ctype.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/ctype/ctype.c
	$(CC)  -Iext/ctype/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/ctype/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/ctype/ctype.c -o ext/ctype/ctype.o  && echo > $@
ext/mbstring/mbstring.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/mbstring.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/mbstring.c -o ext/mbstring/mbstring.o  && echo > $@
ext/mbstring/php_unicode.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/php_unicode.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/php_unicode.c -o ext/mbstring/php_unicode.o  && echo > $@
ext/mbstring/php_mbregex.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/php_mbregex.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/php_mbregex.c -o ext/mbstring/php_mbregex.o  && echo > $@
ext/mbstring/mbregex/mbregex.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/mbregex/mbregex.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/mbregex/mbregex.c -o ext/mbstring/mbregex/mbregex.o  && echo > $@
ext/mbstring/libmbfl/filters/html_entities.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/html_entities.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/html_entities.c -o ext/mbstring/libmbfl/filters/html_entities.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_7bit.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_7bit.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_7bit.c -o ext/mbstring/libmbfl/filters/mbfilter_7bit.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_ascii.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_ascii.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_ascii.c -o ext/mbstring/libmbfl/filters/mbfilter_ascii.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_base64.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_base64.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_base64.c -o ext/mbstring/libmbfl/filters/mbfilter_base64.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_big5.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_big5.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_big5.c -o ext/mbstring/libmbfl/filters/mbfilter_big5.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_byte2.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_byte2.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_byte2.c -o ext/mbstring/libmbfl/filters/mbfilter_byte2.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_byte4.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_byte4.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_byte4.c -o ext/mbstring/libmbfl/filters/mbfilter_byte4.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_cp1251.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_cp1251.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_cp1251.c -o ext/mbstring/libmbfl/filters/mbfilter_cp1251.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_cp1252.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_cp1252.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_cp1252.c -o ext/mbstring/libmbfl/filters/mbfilter_cp1252.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_cp866.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_cp866.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_cp866.c -o ext/mbstring/libmbfl/filters/mbfilter_cp866.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_cp932.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_cp932.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_cp932.c -o ext/mbstring/libmbfl/filters/mbfilter_cp932.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_cp936.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_cp936.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_cp936.c -o ext/mbstring/libmbfl/filters/mbfilter_cp936.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_euc_cn.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_euc_cn.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_euc_cn.c -o ext/mbstring/libmbfl/filters/mbfilter_euc_cn.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_euc_jp.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_euc_jp.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_euc_jp.c -o ext/mbstring/libmbfl/filters/mbfilter_euc_jp.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_euc_jp_win.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_euc_jp_win.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_euc_jp_win.c -o ext/mbstring/libmbfl/filters/mbfilter_euc_jp_win.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_euc_kr.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_euc_kr.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_euc_kr.c -o ext/mbstring/libmbfl/filters/mbfilter_euc_kr.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_euc_tw.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_euc_tw.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_euc_tw.c -o ext/mbstring/libmbfl/filters/mbfilter_euc_tw.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_htmlent.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_htmlent.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_htmlent.c -o ext/mbstring/libmbfl/filters/mbfilter_htmlent.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_hz.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_hz.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_hz.c -o ext/mbstring/libmbfl/filters/mbfilter_hz.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso2022_kr.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso2022_kr.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso2022_kr.c -o ext/mbstring/libmbfl/filters/mbfilter_iso2022_kr.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_1.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_1.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_1.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_1.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_10.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_10.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_10.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_10.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_13.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_13.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_13.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_13.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_14.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_14.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_14.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_14.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_15.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_15.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_15.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_15.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_16.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_16.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_16.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_16.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_2.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_2.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_2.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_2.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_3.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_3.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_3.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_3.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_4.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_4.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_4.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_4.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_5.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_5.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_5.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_5.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_6.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_6.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_6.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_6.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_7.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_7.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_7.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_7.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_8.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_8.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_8.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_8.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_iso8859_9.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_9.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_iso8859_9.c -o ext/mbstring/libmbfl/filters/mbfilter_iso8859_9.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_jis.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_jis.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_jis.c -o ext/mbstring/libmbfl/filters/mbfilter_jis.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_koi8r.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_koi8r.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_koi8r.c -o ext/mbstring/libmbfl/filters/mbfilter_koi8r.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_qprint.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_qprint.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_qprint.c -o ext/mbstring/libmbfl/filters/mbfilter_qprint.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_armscii8.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_armscii8.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_armscii8.c -o ext/mbstring/libmbfl/filters/mbfilter_armscii8.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_sjis.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_sjis.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_sjis.c -o ext/mbstring/libmbfl/filters/mbfilter_sjis.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_ucs2.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_ucs2.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_ucs2.c -o ext/mbstring/libmbfl/filters/mbfilter_ucs2.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_ucs4.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_ucs4.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_ucs4.c -o ext/mbstring/libmbfl/filters/mbfilter_ucs4.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_uhc.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_uhc.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_uhc.c -o ext/mbstring/libmbfl/filters/mbfilter_uhc.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_utf16.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_utf16.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_utf16.c -o ext/mbstring/libmbfl/filters/mbfilter_utf16.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_utf32.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_utf32.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_utf32.c -o ext/mbstring/libmbfl/filters/mbfilter_utf32.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_utf7.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_utf7.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_utf7.c -o ext/mbstring/libmbfl/filters/mbfilter_utf7.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_utf7imap.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_utf7imap.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_utf7imap.c -o ext/mbstring/libmbfl/filters/mbfilter_utf7imap.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_utf8.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_utf8.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_utf8.c -o ext/mbstring/libmbfl/filters/mbfilter_utf8.o  && echo > $@
ext/mbstring/libmbfl/filters/mbfilter_uuencode.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_uuencode.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/filters/mbfilter_uuencode.c -o ext/mbstring/libmbfl/filters/mbfilter_uuencode.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfilter.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfilter.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfilter.c -o ext/mbstring/libmbfl/mbfl/mbfilter.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfilter_8bit.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfilter_8bit.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfilter_8bit.c -o ext/mbstring/libmbfl/mbfl/mbfilter_8bit.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfilter_pass.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfilter_pass.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfilter_pass.c -o ext/mbstring/libmbfl/mbfl/mbfilter_pass.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfilter_wchar.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfilter_wchar.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfilter_wchar.c -o ext/mbstring/libmbfl/mbfl/mbfilter_wchar.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfl_convert.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_convert.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_convert.c -o ext/mbstring/libmbfl/mbfl/mbfl_convert.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfl_encoding.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_encoding.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_encoding.c -o ext/mbstring/libmbfl/mbfl/mbfl_encoding.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfl_filter_output.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_filter_output.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_filter_output.c -o ext/mbstring/libmbfl/mbfl/mbfl_filter_output.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfl_ident.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_ident.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_ident.c -o ext/mbstring/libmbfl/mbfl/mbfl_ident.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfl_language.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_language.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_language.c -o ext/mbstring/libmbfl/mbfl/mbfl_language.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfl_memory_device.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_memory_device.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_memory_device.c -o ext/mbstring/libmbfl/mbfl/mbfl_memory_device.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfl_string.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_string.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_string.c -o ext/mbstring/libmbfl/mbfl/mbfl_string.o  && echo > $@
ext/mbstring/libmbfl/mbfl/mbfl_allocators.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_allocators.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/mbfl/mbfl_allocators.c -o ext/mbstring/libmbfl/mbfl/mbfl_allocators.o  && echo > $@
ext/mbstring/libmbfl/nls/nls_de.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_de.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_de.c -o ext/mbstring/libmbfl/nls/nls_de.o  && echo > $@
ext/mbstring/libmbfl/nls/nls_en.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_en.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_en.c -o ext/mbstring/libmbfl/nls/nls_en.o  && echo > $@
ext/mbstring/libmbfl/nls/nls_ja.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_ja.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_ja.c -o ext/mbstring/libmbfl/nls/nls_ja.o  && echo > $@
ext/mbstring/libmbfl/nls/nls_kr.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_kr.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_kr.c -o ext/mbstring/libmbfl/nls/nls_kr.o  && echo > $@
ext/mbstring/libmbfl/nls/nls_neutral.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_neutral.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_neutral.c -o ext/mbstring/libmbfl/nls/nls_neutral.o  && echo > $@
ext/mbstring/libmbfl/nls/nls_ru.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_ru.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_ru.c -o ext/mbstring/libmbfl/nls/nls_ru.o  && echo > $@
ext/mbstring/libmbfl/nls/nls_uni.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_uni.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_uni.c -o ext/mbstring/libmbfl/nls/nls_uni.o  && echo > $@
ext/mbstring/libmbfl/nls/nls_zh.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_zh.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_zh.c -o ext/mbstring/libmbfl/nls/nls_zh.o  && echo > $@
ext/mbstring/libmbfl/nls/nls_hy.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_hy.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_hy.c -o ext/mbstring/libmbfl/nls/nls_hy.o  && echo > $@
ext/mbstring/libmbfl/nls/nls_tr.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_tr.c
	$(CC)  -DHAVE_CONFIG_H -Iext/mbstring/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mbstring/libmbfl/nls/nls_tr.c -o ext/mbstring/libmbfl/nls/nls_tr.o  && echo > $@
ext/mysql/php_mysql.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/php_mysql.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/php_mysql.c -o ext/mysql/php_mysql.o  && echo > $@
ext/mysql/libmysql/libmysql.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/libmysql.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/libmysql.c -o ext/mysql/libmysql/libmysql.o  && echo > $@
ext/mysql/libmysql/errmsg.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/errmsg.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/errmsg.c -o ext/mysql/libmysql/errmsg.o  && echo > $@
ext/mysql/libmysql/net.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/net.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/net.c -o ext/mysql/libmysql/net.o  && echo > $@
ext/mysql/libmysql/violite.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/violite.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/violite.c -o ext/mysql/libmysql/violite.o  && echo > $@
ext/mysql/libmysql/password.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/password.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/password.c -o ext/mysql/libmysql/password.o  && echo > $@
ext/mysql/libmysql/my_init.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_init.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_init.c -o ext/mysql/libmysql/my_init.o  && echo > $@
ext/mysql/libmysql/my_lib.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_lib.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_lib.c -o ext/mysql/libmysql/my_lib.o  && echo > $@
ext/mysql/libmysql/my_static.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_static.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_static.c -o ext/mysql/libmysql/my_static.o  && echo > $@
ext/mysql/libmysql/my_malloc.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_malloc.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_malloc.c -o ext/mysql/libmysql/my_malloc.o  && echo > $@
ext/mysql/libmysql/my_realloc.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_realloc.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_realloc.c -o ext/mysql/libmysql/my_realloc.o  && echo > $@
ext/mysql/libmysql/my_create.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_create.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_create.c -o ext/mysql/libmysql/my_create.o  && echo > $@
ext/mysql/libmysql/my_delete.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_delete.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_delete.c -o ext/mysql/libmysql/my_delete.o  && echo > $@
ext/mysql/libmysql/my_tempnam.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_tempnam.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_tempnam.c -o ext/mysql/libmysql/my_tempnam.o  && echo > $@
ext/mysql/libmysql/my_open.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_open.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_open.c -o ext/mysql/libmysql/my_open.o  && echo > $@
ext/mysql/libmysql/mf_casecnv.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_casecnv.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_casecnv.c -o ext/mysql/libmysql/mf_casecnv.o  && echo > $@
ext/mysql/libmysql/my_read.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_read.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_read.c -o ext/mysql/libmysql/my_read.o  && echo > $@
ext/mysql/libmysql/my_write.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_write.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_write.c -o ext/mysql/libmysql/my_write.o  && echo > $@
ext/mysql/libmysql/errors.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/errors.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/errors.c -o ext/mysql/libmysql/errors.o  && echo > $@
ext/mysql/libmysql/my_error.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_error.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_error.c -o ext/mysql/libmysql/my_error.o  && echo > $@
ext/mysql/libmysql/my_getwd.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_getwd.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_getwd.c -o ext/mysql/libmysql/my_getwd.o  && echo > $@
ext/mysql/libmysql/my_div.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_div.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_div.c -o ext/mysql/libmysql/my_div.o  && echo > $@
ext/mysql/libmysql/mf_pack.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_pack.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_pack.c -o ext/mysql/libmysql/mf_pack.o  && echo > $@
ext/mysql/libmysql/my_messnc.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_messnc.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_messnc.c -o ext/mysql/libmysql/my_messnc.o  && echo > $@
ext/mysql/libmysql/mf_dirname.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_dirname.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_dirname.c -o ext/mysql/libmysql/mf_dirname.o  && echo > $@
ext/mysql/libmysql/mf_fn_ext.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_fn_ext.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_fn_ext.c -o ext/mysql/libmysql/mf_fn_ext.o  && echo > $@
ext/mysql/libmysql/mf_wcomp.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_wcomp.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_wcomp.c -o ext/mysql/libmysql/mf_wcomp.o  && echo > $@
ext/mysql/libmysql/typelib.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/typelib.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/typelib.c -o ext/mysql/libmysql/typelib.o  && echo > $@
ext/mysql/libmysql/safemalloc.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/safemalloc.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/safemalloc.c -o ext/mysql/libmysql/safemalloc.o  && echo > $@
ext/mysql/libmysql/my_alloc.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_alloc.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_alloc.c -o ext/mysql/libmysql/my_alloc.o  && echo > $@
ext/mysql/libmysql/mf_format.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_format.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_format.c -o ext/mysql/libmysql/mf_format.o  && echo > $@
ext/mysql/libmysql/mf_path.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_path.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_path.c -o ext/mysql/libmysql/mf_path.o  && echo > $@
ext/mysql/libmysql/mf_unixpath.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_unixpath.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_unixpath.c -o ext/mysql/libmysql/mf_unixpath.o  && echo > $@
ext/mysql/libmysql/my_fopen.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_fopen.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_fopen.c -o ext/mysql/libmysql/my_fopen.o  && echo > $@
ext/mysql/libmysql/mf_loadpath.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_loadpath.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mf_loadpath.c -o ext/mysql/libmysql/mf_loadpath.o  && echo > $@
ext/mysql/libmysql/my_pthread.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_pthread.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_pthread.c -o ext/mysql/libmysql/my_pthread.o  && echo > $@
ext/mysql/libmysql/my_thr_init.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_thr_init.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_thr_init.c -o ext/mysql/libmysql/my_thr_init.o  && echo > $@
ext/mysql/libmysql/thr_mutex.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/thr_mutex.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/thr_mutex.c -o ext/mysql/libmysql/thr_mutex.o  && echo > $@
ext/mysql/libmysql/mulalloc.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mulalloc.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/mulalloc.c -o ext/mysql/libmysql/mulalloc.o  && echo > $@
ext/mysql/libmysql/string.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/string.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/string.c -o ext/mysql/libmysql/string.o  && echo > $@
ext/mysql/libmysql/default.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/default.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/default.c -o ext/mysql/libmysql/default.o  && echo > $@
ext/mysql/libmysql/my_compress.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_compress.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_compress.c -o ext/mysql/libmysql/my_compress.o  && echo > $@
ext/mysql/libmysql/array.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/array.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/array.c -o ext/mysql/libmysql/array.o  && echo > $@
ext/mysql/libmysql/my_once.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_once.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_once.c -o ext/mysql/libmysql/my_once.o  && echo > $@
ext/mysql/libmysql/list.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/list.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/list.c -o ext/mysql/libmysql/list.o  && echo > $@
ext/mysql/libmysql/my_net.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_net.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/my_net.c -o ext/mysql/libmysql/my_net.o  && echo > $@
ext/mysql/libmysql/dbug.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/dbug.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/dbug.c -o ext/mysql/libmysql/dbug.o  && echo > $@
ext/mysql/libmysql/strmov.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strmov.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strmov.c -o ext/mysql/libmysql/strmov.o  && echo > $@
ext/mysql/libmysql/strxmov.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strxmov.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strxmov.c -o ext/mysql/libmysql/strxmov.o  && echo > $@
ext/mysql/libmysql/strnmov.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strnmov.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strnmov.c -o ext/mysql/libmysql/strnmov.o  && echo > $@
ext/mysql/libmysql/strmake.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strmake.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strmake.c -o ext/mysql/libmysql/strmake.o  && echo > $@
ext/mysql/libmysql/strend.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strend.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strend.c -o ext/mysql/libmysql/strend.o  && echo > $@
ext/mysql/libmysql/strfill.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strfill.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strfill.c -o ext/mysql/libmysql/strfill.o  && echo > $@
ext/mysql/libmysql/is_prefix.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/is_prefix.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/is_prefix.c -o ext/mysql/libmysql/is_prefix.o  && echo > $@
ext/mysql/libmysql/int2str.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/int2str.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/int2str.c -o ext/mysql/libmysql/int2str.o  && echo > $@
ext/mysql/libmysql/str2int.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/str2int.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/str2int.c -o ext/mysql/libmysql/str2int.o  && echo > $@
ext/mysql/libmysql/strinstr.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strinstr.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strinstr.c -o ext/mysql/libmysql/strinstr.o  && echo > $@
ext/mysql/libmysql/strcont.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strcont.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strcont.c -o ext/mysql/libmysql/strcont.o  && echo > $@
ext/mysql/libmysql/strcend.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strcend.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strcend.c -o ext/mysql/libmysql/strcend.o  && echo > $@
ext/mysql/libmysql/bchange.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/bchange.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/bchange.c -o ext/mysql/libmysql/bchange.o  && echo > $@
ext/mysql/libmysql/bmove.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/bmove.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/bmove.c -o ext/mysql/libmysql/bmove.o  && echo > $@
ext/mysql/libmysql/bmove_upp.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/bmove_upp.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/bmove_upp.c -o ext/mysql/libmysql/bmove_upp.o  && echo > $@
ext/mysql/libmysql/longlong2str.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/longlong2str.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/longlong2str.c -o ext/mysql/libmysql/longlong2str.o  && echo > $@
ext/mysql/libmysql/strtoull.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strtoull.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strtoull.c -o ext/mysql/libmysql/strtoull.o  && echo > $@
ext/mysql/libmysql/strtoll.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strtoll.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/strtoll.c -o ext/mysql/libmysql/strtoll.o  && echo > $@
ext/mysql/libmysql/charset.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/charset.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/charset.c -o ext/mysql/libmysql/charset.o  && echo > $@
ext/mysql/libmysql/ctype.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/ctype.c
	$(CC) -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql -Iext/mysql/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/mysql/libmysql/ctype.c -o ext/mysql/libmysql/ctype.o  && echo > $@
ext/overload/overload.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/overload/overload.c
	$(CC)  -Iext/overload/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/overload/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/overload/overload.c -o ext/overload/overload.o  && echo > $@
ext/pcntl/pcntl.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/pcntl/pcntl.c
	$(CC)  -Iext/pcntl/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/pcntl/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/pcntl/pcntl.c -o ext/pcntl/pcntl.o  && echo > $@
ext/pcntl/php_signal.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/pcntl/php_signal.c
	$(CC)  -Iext/pcntl/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/pcntl/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/pcntl/php_signal.c -o ext/pcntl/php_signal.o  && echo > $@
ext/posix/posix.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/posix/posix.c
	$(CC)  -Iext/posix/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/posix/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/posix/posix.c -o ext/posix/posix.o  && echo > $@
ext/session/session.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/session.c
	$(CC)  -Iext/session/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/session.c -o ext/session/session.o  && echo > $@
ext/session/mod_files.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/mod_files.c
	$(CC)  -Iext/session/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/mod_files.c -o ext/session/mod_files.o  && echo > $@
ext/session/mod_mm.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/mod_mm.c
	$(CC)  -Iext/session/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/mod_mm.c -o ext/session/mod_mm.o  && echo > $@
ext/session/mod_user.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/mod_user.c
	$(CC)  -Iext/session/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/session/mod_user.c -o ext/session/mod_user.o  && echo > $@
regex/regcomp.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/regcomp.c
	$(CC)  -Iregex/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/regcomp.c -o regex/regcomp.o  && echo > $@
regex/regexec.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/regexec.c
	$(CC)  -Iregex/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/regexec.c -o regex/regexec.o  && echo > $@
regex/regerror.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/regerror.c
	$(CC)  -Iregex/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/regerror.c -o regex/regerror.o  && echo > $@
regex/regfree.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/regfree.c
	$(CC)  -Iregex/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/regex/regfree.c -o regex/regfree.o  && echo > $@
ext/standard/array.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/array.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/array.c -o ext/standard/array.o  && echo > $@
ext/standard/base64.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/base64.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/base64.c -o ext/standard/base64.o  && echo > $@
ext/standard/basic_functions.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/basic_functions.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/basic_functions.c -o ext/standard/basic_functions.o  && echo > $@
ext/standard/browscap.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/browscap.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/browscap.c -o ext/standard/browscap.o  && echo > $@
ext/standard/crc32.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/crc32.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/crc32.c -o ext/standard/crc32.o  && echo > $@
ext/standard/crypt.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/crypt.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/crypt.c -o ext/standard/crypt.o  && echo > $@
ext/standard/cyr_convert.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/cyr_convert.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/cyr_convert.c -o ext/standard/cyr_convert.o  && echo > $@
ext/standard/datetime.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/datetime.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/datetime.c -o ext/standard/datetime.o  && echo > $@
ext/standard/dir.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/dir.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/dir.c -o ext/standard/dir.o  && echo > $@
ext/standard/dl.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/dl.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/dl.c -o ext/standard/dl.o  && echo > $@
ext/standard/dns.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/dns.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/dns.c -o ext/standard/dns.o  && echo > $@
ext/standard/exec.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/exec.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/exec.c -o ext/standard/exec.o  && echo > $@
ext/standard/file.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/file.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/file.c -o ext/standard/file.o  && echo > $@
ext/standard/filestat.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/filestat.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/filestat.c -o ext/standard/filestat.o  && echo > $@
ext/standard/flock_compat.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/flock_compat.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/flock_compat.c -o ext/standard/flock_compat.o  && echo > $@
ext/standard/formatted_print.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/formatted_print.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/formatted_print.c -o ext/standard/formatted_print.o  && echo > $@
ext/standard/fsock.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/fsock.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/fsock.c -o ext/standard/fsock.o  && echo > $@
ext/standard/head.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/head.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/head.c -o ext/standard/head.o  && echo > $@
ext/standard/html.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/html.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/html.c -o ext/standard/html.o  && echo > $@
ext/standard/image.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/image.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/image.c -o ext/standard/image.o  && echo > $@
ext/standard/info.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/info.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/info.c -o ext/standard/info.o  && echo > $@
ext/standard/iptc.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/iptc.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/iptc.c -o ext/standard/iptc.o  && echo > $@
ext/standard/lcg.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/lcg.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/lcg.c -o ext/standard/lcg.o  && echo > $@
ext/standard/link.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/link.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/link.c -o ext/standard/link.o  && echo > $@
ext/standard/mail.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/mail.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/mail.c -o ext/standard/mail.o  && echo > $@
ext/standard/math.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/math.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/math.c -o ext/standard/math.o  && echo > $@
ext/standard/md5.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/md5.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/md5.c -o ext/standard/md5.o  && echo > $@
ext/standard/metaphone.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/metaphone.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/metaphone.c -o ext/standard/metaphone.o  && echo > $@
ext/standard/microtime.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/microtime.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/microtime.c -o ext/standard/microtime.o  && echo > $@
ext/standard/pack.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/pack.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/pack.c -o ext/standard/pack.o  && echo > $@
ext/standard/pageinfo.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/pageinfo.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/pageinfo.c -o ext/standard/pageinfo.o  && echo > $@
ext/standard/parsedate.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/parsedate.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/parsedate.c -o ext/standard/parsedate.o  && echo > $@
ext/standard/quot_print.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/quot_print.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/quot_print.c -o ext/standard/quot_print.o  && echo > $@
ext/standard/rand.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/rand.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/rand.c -o ext/standard/rand.o  && echo > $@
ext/standard/reg.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/reg.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/reg.c -o ext/standard/reg.o  && echo > $@
ext/standard/soundex.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/soundex.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/soundex.c -o ext/standard/soundex.o  && echo > $@
ext/standard/string.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/string.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/string.c -o ext/standard/string.o  && echo > $@
ext/standard/scanf.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/scanf.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/scanf.c -o ext/standard/scanf.o  && echo > $@
ext/standard/syslog.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/syslog.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/syslog.c -o ext/standard/syslog.o  && echo > $@
ext/standard/type.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/type.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/type.c -o ext/standard/type.o  && echo > $@
ext/standard/uniqid.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/uniqid.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/uniqid.c -o ext/standard/uniqid.o  && echo > $@
ext/standard/url.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/url.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/url.c -o ext/standard/url.o  && echo > $@
ext/standard/url_scanner.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/url_scanner.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/url_scanner.c -o ext/standard/url_scanner.o  && echo > $@
ext/standard/var.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/var.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/var.c -o ext/standard/var.o  && echo > $@
ext/standard/versioning.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/versioning.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/versioning.c -o ext/standard/versioning.o  && echo > $@
ext/standard/assert.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/assert.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/assert.c -o ext/standard/assert.o  && echo > $@
ext/standard/strnatcmp.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/strnatcmp.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/strnatcmp.c -o ext/standard/strnatcmp.o  && echo > $@
ext/standard/levenshtein.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/levenshtein.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/levenshtein.c -o ext/standard/levenshtein.o  && echo > $@
ext/standard/incomplete_class.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/incomplete_class.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/incomplete_class.c -o ext/standard/incomplete_class.o  && echo > $@
ext/standard/url_scanner_ex.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/url_scanner_ex.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/url_scanner_ex.c -o ext/standard/url_scanner_ex.o  && echo > $@
ext/standard/ftp_fopen_wrapper.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ftp_fopen_wrapper.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ftp_fopen_wrapper.c -o ext/standard/ftp_fopen_wrapper.o  && echo > $@
ext/standard/http_fopen_wrapper.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/http_fopen_wrapper.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/http_fopen_wrapper.c -o ext/standard/http_fopen_wrapper.o  && echo > $@
ext/standard/php_fopen_wrapper.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/php_fopen_wrapper.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/php_fopen_wrapper.c -o ext/standard/php_fopen_wrapper.o  && echo > $@
ext/standard/credits.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/credits.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/credits.c -o ext/standard/credits.o  && echo > $@
ext/standard/css.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/css.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/css.c -o ext/standard/css.o  && echo > $@
ext/standard/var_unserializer.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/var_unserializer.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/var_unserializer.c -o ext/standard/var_unserializer.o  && echo > $@
ext/standard/ftok.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ftok.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ftok.c -o ext/standard/ftok.o  && echo > $@
ext/standard/aggregation.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/aggregation.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/aggregation.c -o ext/standard/aggregation.o  && echo > $@
ext/standard/sha1.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/sha1.c
	$(CC)  -Iext/standard/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/standard/sha1.c -o ext/standard/sha1.o  && echo > $@
ext/tokenizer/tokenizer.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/tokenizer/tokenizer.c
	$(CC)  -Iext/tokenizer/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/tokenizer/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/tokenizer/tokenizer.c -o ext/tokenizer/tokenizer.o  && echo > $@
ext/xml/xml.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/xml.c
	$(CC) -DBYTEORDER=1234 -Iext/xml/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/xml.c -o ext/xml/xml.o  && echo > $@
ext/xml/expat/xmlparse.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/expat/xmlparse.c
	$(CC) -DBYTEORDER=1234 -Iext/xml/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/expat/xmlparse.c -o ext/xml/expat/xmlparse.o  && echo > $@
ext/xml/expat/xmlrole.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/expat/xmlrole.c
	$(CC) -DBYTEORDER=1234 -Iext/xml/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/expat/xmlrole.c -o ext/xml/expat/xmlrole.o  && echo > $@
ext/xml/expat/xmltok.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/expat/xmltok.c
	$(CC) -DBYTEORDER=1234 -Iext/xml/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/ext/xml/expat/xmltok.c -o ext/xml/expat/xmltok.o  && echo > $@
sapi/cli/php_cli.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cli/php_cli.c
	$(CC)  -Isapi/cli/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cli/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cli/php_cli.c -o sapi/cli/php_cli.o  && echo > $@
sapi/cli/getopt.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cli/getopt.c
	$(CC)  -Isapi/cli/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cli/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/sapi/cli/getopt.c -o sapi/cli/getopt.o  && echo > $@
TSRM/TSRM.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/TSRM/TSRM.c
	$(CC)  -ITSRM/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/TSRM/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/TSRM/TSRM.c -o TSRM/TSRM.o  && echo > $@
TSRM/tsrm_strtok_r.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/TSRM/tsrm_strtok_r.c
	$(CC)  -ITSRM/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/TSRM/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/TSRM/tsrm_strtok_r.c -o TSRM/tsrm_strtok_r.o  && echo > $@
TSRM/tsrm_virtual_cwd.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/TSRM/tsrm_virtual_cwd.c
	$(CC)  -ITSRM/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/TSRM/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/TSRM/tsrm_virtual_cwd.c -o TSRM/tsrm_virtual_cwd.o  && echo > $@
main/main.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/main.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/main.c -o main/main.o  && echo > $@
main/snprintf.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/snprintf.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/snprintf.c -o main/snprintf.o  && echo > $@
main/spprintf.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/spprintf.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/spprintf.c -o main/spprintf.o  && echo > $@
main/php_sprintf.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_sprintf.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_sprintf.c -o main/php_sprintf.o  && echo > $@
main/safe_mode.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/safe_mode.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/safe_mode.c -o main/safe_mode.o  && echo > $@
main/fopen_wrappers.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/fopen_wrappers.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/fopen_wrappers.c -o main/fopen_wrappers.o  && echo > $@
main/alloca.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/alloca.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/alloca.c -o main/alloca.o  && echo > $@
main/php_scandir.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_scandir.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_scandir.c -o main/php_scandir.o  && echo > $@
main/php_ini.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_ini.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_ini.c -o main/php_ini.o  && echo > $@
main/SAPI.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/SAPI.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/SAPI.c -o main/SAPI.o  && echo > $@
main/rfc1867.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/rfc1867.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/rfc1867.c -o main/rfc1867.o  && echo > $@
main/php_content_types.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_content_types.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_content_types.c -o main/php_content_types.o  && echo > $@
main/strlcpy.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/strlcpy.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/strlcpy.c -o main/strlcpy.o  && echo > $@
main/strlcat.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/strlcat.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/strlcat.c -o main/strlcat.o  && echo > $@
main/mergesort.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/mergesort.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/mergesort.c -o main/mergesort.o  && echo > $@
main/reentrancy.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/reentrancy.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/reentrancy.c -o main/reentrancy.o  && echo > $@
main/php_variables.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_variables.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_variables.c -o main/php_variables.o  && echo > $@
main/php_ticks.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_ticks.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_ticks.c -o main/php_ticks.o  && echo > $@
main/streams.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/streams.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/streams.c -o main/streams.o  && echo > $@
main/network.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/network.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/network.c -o main/network.o  && echo > $@
main/php_open_temporary_file.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_open_temporary_file.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_open_temporary_file.c -o main/php_open_temporary_file.o  && echo > $@
main/php_logos.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_logos.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/php_logos.c -o main/php_logos.o  && echo > $@
main/output.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/output.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/output.c -o main/output.o  && echo > $@
main/memory_streams.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/memory_streams.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/memory_streams.c -o main/memory_streams.o  && echo > $@
main/user_streams.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/user_streams.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/main/user_streams.c -o main/user_streams.o  && echo > $@
main/internal_functions.lo: main/internal_functions.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c main/internal_functions.c -o main/internal_functions.o  && echo > $@
main/internal_functions_cli.lo: main/internal_functions_cli.c
	$(CC)  -Imain/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/main/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c main/internal_functions_cli.c -o main/internal_functions_cli.o  && echo > $@
Zend/zend_language_parser.lo: Zend/zend_language_parser.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c Zend/zend_language_parser.c -o Zend/zend_language_parser.o  && echo > $@
Zend/zend_language_scanner.lo: Zend/zend_language_scanner.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c Zend/zend_language_scanner.c -o Zend/zend_language_scanner.o  && echo > $@
Zend/zend_ini_parser.lo: Zend/zend_ini_parser.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c Zend/zend_ini_parser.c -o Zend/zend_ini_parser.o  && echo > $@
Zend/zend_ini_scanner.lo: Zend/zend_ini_scanner.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c Zend/zend_ini_scanner.c -o Zend/zend_ini_scanner.o  && echo > $@
Zend/zend_alloc.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_alloc.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_alloc.c -o Zend/zend_alloc.o  && echo > $@
Zend/zend_compile.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_compile.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_compile.c -o Zend/zend_compile.o  && echo > $@
Zend/zend_constants.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_constants.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_constants.c -o Zend/zend_constants.o  && echo > $@
Zend/zend_dynamic_array.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_dynamic_array.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_dynamic_array.c -o Zend/zend_dynamic_array.o  && echo > $@
Zend/zend_execute_API.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_execute_API.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_execute_API.c -o Zend/zend_execute_API.o  && echo > $@
Zend/zend_highlight.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_highlight.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_highlight.c -o Zend/zend_highlight.o  && echo > $@
Zend/zend_llist.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_llist.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_llist.c -o Zend/zend_llist.o  && echo > $@
Zend/zend_opcode.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_opcode.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_opcode.c -o Zend/zend_opcode.o  && echo > $@
Zend/zend_operators.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_operators.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_operators.c -o Zend/zend_operators.o  && echo > $@
Zend/zend_ptr_stack.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_ptr_stack.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_ptr_stack.c -o Zend/zend_ptr_stack.o  && echo > $@
Zend/zend_stack.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_stack.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_stack.c -o Zend/zend_stack.o  && echo > $@
Zend/zend_variables.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_variables.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_variables.c -o Zend/zend_variables.o  && echo > $@
Zend/zend.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend.c -o Zend/zend.o  && echo > $@
Zend/zend_API.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_API.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_API.c -o Zend/zend_API.o  && echo > $@
Zend/zend_extensions.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_extensions.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_extensions.c -o Zend/zend_extensions.o  && echo > $@
Zend/zend_hash.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_hash.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_hash.c -o Zend/zend_hash.o  && echo > $@
Zend/zend_list.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_list.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_list.c -o Zend/zend_list.o  && echo > $@
Zend/zend_indent.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_indent.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_indent.c -o Zend/zend_indent.o  && echo > $@
Zend/zend_builtin_functions.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_builtin_functions.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_builtin_functions.c -o Zend/zend_builtin_functions.o  && echo > $@
Zend/zend_sprintf.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_sprintf.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_sprintf.c -o Zend/zend_sprintf.o  && echo > $@
Zend/zend_ini.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_ini.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_ini.c -o Zend/zend_ini.o  && echo > $@
Zend/zend_qsort.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_qsort.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_qsort.c -o Zend/zend_qsort.o  && echo > $@
Zend/zend_multibyte.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_multibyte.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_multibyte.c -o Zend/zend_multibyte.o  && echo > $@
Zend/zend_strtod.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_strtod.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_strtod.c -o Zend/zend_strtod.o  && echo > $@
Zend/zend_execute.lo: /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_execute.c
	$(CC)  -IZend/ -I/opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/ $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -c /opt/devel/proto/marvell/build-eabi/php-4.4.2/Zend/zend_execute.c -o Zend/zend_execute.o  && echo > $@
