# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:19 wiley Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += $(INCLUDE_krb4)

lib_LIBRARIES = @EXTRA_LIB45@

EXTRA_LIBRARIES = lib45.a

lib45_a_SOURCES = get_ad_tkt.c mk_req.c 45_locl.h
