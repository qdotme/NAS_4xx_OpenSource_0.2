# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:18 wiley Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += $(INCLUDE_readline) $(INCLUDE_des)

SLC = $(top_builddir)/lib/sl/slc

man_MANS = ktutil.8

sbin_PROGRAMS = ktutil

ktutil_SOURCES =				\
	add.c					\
	change.c				\
	copy.c					\
	get.c					\
	ktutil.c				\
	list.c					\
	purge.c					\
	remove.c				\
	ktutil-commands.c			\
	rename.c

$(ktutil_OBJECTS): ktutil-commands.h

CLEANFILES = ktutil-commands.h ktutil-commands.c

ktutil-commands.c ktutil-commands.h: ktutil-commands.in
	$(SLC) $(srcdir)/ktutil-commands.in

LDADD = \
	$(top_builddir)/lib/kadm5/libkadm5clnt.la \
	$(top_builddir)/lib/krb5/libkrb5.la \
	$(LIB_des) \
	$(top_builddir)/lib/asn1/libasn1.la \
	$(top_builddir)/lib/sl/libsl.la \
	$(LIB_readline) \
	$(LIB_roken)
