# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:18 wiley Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += $(INCLUDE_krb4)

bin_PROGRAMS = afslog pagsh

afslog_SOURCES = afslog.c

pagsh_SOURCES  = pagsh.c

man_MANS = afslog.1 pagsh.1

LDADD = $(LIB_kafs) \
	$(LIB_krb4) \
	$(top_builddir)/lib/krb5/libkrb5.la \
	$(top_builddir)/lib/asn1/libasn1.la \
	$(LIB_des) \
	$(LIB_roken)
