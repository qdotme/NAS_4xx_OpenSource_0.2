# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:17 wiley Exp $

include $(top_srcdir)/Makefile.am.common

if KCM
kcm_dir = kcm
endif

SUBDIRS	= include lib kuser kdc admin kadmin kpasswd $(kcm_dir) appl doc tools

## ACLOCAL = @ACLOCAL@ -I cf
ACLOCAL_AMFLAGS = -I cf

EXTRA_DIST = Makefile.am.common krb5.conf
