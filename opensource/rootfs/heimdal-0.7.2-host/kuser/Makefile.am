# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:21 wiley Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += $(INCLUDE_krb4) $(INCLUDE_des) -I$(srcdir)/../lib/krb5

man_MANS = kinit.1 klist.1 kdestroy.1 kgetcred.1

bin_PROGRAMS = kinit klist kdestroy kgetcred

noinst_PROGRAMS = kverify kdecode_ticket generate-requests copy_cred_cache
noinst_MANS = copy_cred_cache.1

kinit_LDADD = \
	$(LIB_kafs) \
	$(top_builddir)/lib/krb5/libkrb5.la \
	$(LIB_krb4) \
	$(LIB_des) \
	$(top_builddir)/lib/asn1/libasn1.la \
	$(LIB_roken)

kdestroy_LDADD	= $(kinit_LDADD)

klist_LDADD	= $(kinit_LDADD)

LDADD = \
	$(top_builddir)/lib/krb5/libkrb5.la \
	$(LIB_des) \
	$(top_builddir)/lib/asn1/libasn1.la \
	$(LIB_roken)

# make sure install-exec-hook doesn't have any commands in Makefile.am.common
install-exec-hook:
	(cd $(DESTDIR)$(bindir) && rm -f kauth && $(LN_S) kinit kauth)
