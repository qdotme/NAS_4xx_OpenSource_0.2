# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:19 wiley Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += $(INCLUDE_krb4) $(INCLUDE_des) -I$(srcdir)/../lib/krb5

libexec_PROGRAMS = kcm

kcm_SOURCES =		\
	acl.c		\
	acquire.c	\
	cache.c		\
	client.c	\
	config.c	\
	connect.c	\
	cursor.c	\
	events.c	\
	glue.c		\
	headers.h	\
	kcm_locl.h	\
	kcm_protos.h	\
	log.c		\
	main.c		\
	protocol.c	\
	renew.c

$(srcdir)/kcm_protos.h:
	cd $(srcdir); perl ../cf/make-proto.pl -o kcm_protos.h -q -P comment $(kcm_SOURCES) || rm -f kcm_protos.h

$(kcm_OBJECTS): $(srcdir)/kcm_protos.h

man_MANS = kcm.8

LDADD = $(top_builddir)/lib/hdb/libhdb.la \
	$(LIB_openldap) \
	$(top_builddir)/lib/krb5/libkrb5.la \
	$(LIB_krb4) \
	$(LIB_des) \
	$(top_builddir)/lib/asn1/libasn1.la \
	$(LIB_roken) \
	$(LIB_door_create) \
	$(LIB_pidfile)


