# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:21 wiley Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += $(INCLUDE_readline) $(INCLUDE_des) -I$(srcdir)/../lib/krb5

sbin_PROGRAMS = kadmin

libexec_PROGRAMS = kadmind

SLC = $(top_builddir)/lib/sl/slc

man_MANS = kadmin.8 kadmind.8

noinst_PROGRAMS = add_random_users

kadmin_SOURCES =				\
	ank.c					\
	cpw.c					\
	del.c					\
	del_enctype.c				\
	dump.c					\
	ext.c					\
	get.c					\
	init.c					\
	kadmin.c				\
	load.c					\
	mod.c					\
	rename.c				\
	stash.c					\
	util.c					\
	pw_quality.c				\
	random_password.c			\
	kadmin-commands.c			\
	kadmin_locl.h

$(kadmin_OBJECTS): kadmin-commands.h

CLEANFILES = kadmin-commands.h kadmin-commands.c

kadmin-commands.c kadmin-commands.h: kadmin-commands.in
	$(SLC) $(srcdir)/kadmin-commands.in

kadmind_SOURCES =				\
	kadmind.c				\
	server.c				\
	kadmin_locl.h				\
	$(version4_c)				\
	kadm_conn.c

add_random_users_SOURCES = add-random-users.c

LDADD_common = \
	$(top_builddir)/lib/hdb/libhdb.la \
	$(LIB_openldap) \
	$(top_builddir)/lib/krb5/libkrb5.la \
	$(LIB_des) \
	$(top_builddir)/lib/asn1/libasn1.la \
	$(LIB_roken) \
	$(DBLIB)

kadmind_LDADD = $(top_builddir)/lib/kadm5/libkadm5srv.la \
	$(LDADD_common) \
	$(LIB_pidfile) \
	$(LIB_dlopen)

kadmin_LDADD = \
	$(top_builddir)/lib/kadm5/libkadm5clnt.la \
	$(top_builddir)/lib/kadm5/libkadm5srv.la \
	$(top_builddir)/lib/sl/libsl.la \
	$(LIB_readline) \
	$(LDADD_common) \
	$(LIB_dlopen)

add_random_users_LDADD = \
	$(top_builddir)/lib/kadm5/libkadm5clnt.la \
	$(top_builddir)/lib/kadm5/libkadm5srv.la \
	$(LDADD_common) \
	$(LIB_dlopen)
