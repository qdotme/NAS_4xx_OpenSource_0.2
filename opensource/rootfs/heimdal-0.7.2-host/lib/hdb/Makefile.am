# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:19 wiley Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += -I../asn1 -I$(srcdir)/../asn1 $(INCLUDE_des)

BUILT_SOURCES = \
	$(foo:.x=.c)	\
	hdb_err.c \
	hdb_err.h

foo = \
	asn1_Event.x \
	asn1_GENERATION.x \
	asn1_HDBFlags.x \
	asn1_Key.x \
	asn1_Salt.x \
	asn1_hdb_entry.x

CLEANFILES = $(BUILT_SOURCES) $(foo) hdb_asn1.h asn1_files

noinst_PROGRAMS = convert_db
LDADD = libhdb.la \
	$(LIB_openldap) \
	../krb5/libkrb5.la \
	../asn1/libasn1.la \
	$(LIB_des) \
	$(LIB_roken) \
	$(LIB_ldopen)

if OPENLDAP_MODULE

ldap_so = hdb_ldap.la
hdb_ldap_la_SOURCES = hdb-ldap.c
hdb_ldap_la_LDFLAGS = -module

else

ldap = hdb-ldap.c

endif


lib_LTLIBRARIES = libhdb.la $(ldap_so)
libhdb_la_LDFLAGS = -version-info 8:0:1

hdb_source =					\
	common.c				\
	db.c					\
	db3.c					\
	$(ldap)					\
	hdb.c					\
	keys.c					\
	keytab.c				\
	mkey.c					\
	ndbm.c					\
	print.c

libhdb_la_SOURCES =				\
	$(hdb_source)				\
	$(BUILT_SOURCES)

AM_CPPFLAGS += $(INCLUDE_openldap)

include_HEADERS = hdb.h hdb_err.h hdb_asn1.h hdb-protos.h hdb-private.h

libhdb_la_LIBADD = \
	../krb5/libkrb5.la \
	../asn1/libasn1.la \
	../roken/libroken.la \
	$(LIB_openldap) \
	$(LIB_dlopen) \
	$(DBLIB) \
	$(LIB_NDBM)

$(libhdb_la_OBJECTS): $(srcdir)/hdb-protos.h $(srcdir)/hdb-private.h

CHECK_SYMBOLS = -lib hdb -asn1compile -com_err \
	hdb_ _hdb HDBFlags2int initialize_hdb_error_table

$(srcdir)/hdb-protos.h:
	cd $(srcdir); perl ../../cf/make-proto.pl -q -P comment -o hdb-protos.h $(hdb_source) || rm -f hdb-protos.h

$(srcdir)/hdb-private.h:
	cd $(srcdir); perl ../../cf/make-proto.pl -q -P comment -p hdb-private.h $(hdb_source) || rm -f hdb-private.h

$(foo) hdb_asn1.h: asn1_files

asn1_files: ../asn1/asn1_compile$(EXEEXT) $(srcdir)/hdb.asn1
	../asn1/asn1_compile$(EXEEXT) $(srcdir)/hdb.asn1 hdb_asn1

$(libhdb_la_OBJECTS): hdb_asn1.h hdb_err.h

$(convert_db_OBJECTS): hdb_asn1.h hdb_err.h

# to help stupid solaris make

hdb_err.h: hdb_err.et
