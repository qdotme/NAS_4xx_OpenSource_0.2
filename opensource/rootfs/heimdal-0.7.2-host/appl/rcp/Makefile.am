# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:18 wiley Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += $(INCLUDE_krb4)

bin_PROGRAMS = rcp

rcp_SOURCES  = rcp.c util.c

LDADD = $(LIB_roken)
