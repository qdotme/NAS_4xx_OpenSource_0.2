# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:19 wiley Exp $

include $(top_srcdir)/Makefile.am.common

if do_roken_rename
ES = snprintf.c strlcpy.c
endif

AM_CPPFLAGS += $(ROKEN_RENAME)

YFLAGS = -d

lib_LTLIBRARIES = libcom_err.la
libcom_err_la_LDFLAGS = -version-info 2:1:1

bin_PROGRAMS = compile_et

include_HEADERS = com_err.h com_right.h

compile_et_SOURCES = compile_et.c compile_et.h parse.y lex.l

libcom_err_la_SOURCES = error.c com_err.c roken_rename.h $(ES)

CLEANFILES = lex.c parse.c parse.h

$(compile_et_OBJECTS): parse.h parse.c ## XXX broken automake 1.4s

compile_et_LDADD = \
	libcom_err.la \
	$(LIB_roken) \
	$(LEXLIB)

snprintf.c:
	$(LN_S) $(srcdir)/../roken/snprintf.c .
strlcpy.c:
	$(LN_S) $(srcdir)/../roken/strlcpy.c .
