# Makefile.am for libatalk/util/

noinst_LTLIBRARIES = libunicode.la

CFLAGS = -I$(top_srcdir)/sys @CFLAGS@ @ICONV_CFLAGS@

SUBDIRS = charsets

LIBUNICODE_DEPS = charsets/libcharsets.la 


libunicode_la_SOURCES = \
	util_unistr.c	\
	iconv.c		\
	charcnv.c	\
	utf8.c

libunicode_la_LIBADD = $(LIBUNICODE_DEPS)

noinst_HEADERS = ucs2_casetable.h precompose.h byteorder.h

LIBS=@ICONV_LIBS@
