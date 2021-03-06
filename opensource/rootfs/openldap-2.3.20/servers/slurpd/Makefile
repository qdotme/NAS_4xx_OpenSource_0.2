# $OpenLDAP: pkg/ldap/build/top.mk,v 1.93.2.8 2006/01/03 22:16:01 kurt Exp $
## This work is part of OpenLDAP Software <http://www.openldap.org/>.
##
## Copyright 1998-2006 The OpenLDAP Foundation.
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted only as authorized by the OpenLDAP
## Public License.
##
## A copy of this license is available in the file LICENSE in the
## top-level directory of the distribution or, alternatively, at
##---------------------------------------------------------------------------
#
# Top-level Makefile template
#

PACKAGE= OpenLDAP
VERSION= 2.3.20
RELEASEDATE= 2006/02/18


SHELL = /bin/sh

top_builddir = ../../

srcdir = .
top_srcdir = ../..

prefix = /usr
exec_prefix = /usr
ldap_subdir = /openldap

bindir = /usr/bin
datadir = ${prefix}/share$(ldap_subdir)
includedir = /usr/include
infodir = ${prefix}/info
libdir = /usr/lib
libexecdir = ${exec_prefix}/libexec
localstatedir = ${prefix}/var
mandir = ${prefix}/man
moduledir = ${exec_prefix}/libexec$(ldap_subdir)
sbindir = /usr/sbin
sharedstatedir = ${prefix}/com
sysconfdir = ${prefix}/etc$(ldap_subdir)
schemadir = $(sysconfdir)/schema

PLAT = UNIX
EXEEXT = 
OBJEXT = o

BUILD_LIBS_DYNAMIC = static

SHTOOL = $(top_srcdir)/build/shtool

INSTALL = $(SHTOOL) install -c
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644
INSTALL_SCRIPT = $(INSTALL)

STRIP = -s

LINT = lint
5LINT = 5lint

MKDEP = $(top_srcdir)/build/mkdep $(MKDEPFLAG) \
	-d "$(srcdir)" -c "$(MKDEP_CC)" -m "$(MKDEP_CFLAGS)"
MKDEP_CC	= /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-gcc
MKDEP_CFLAGS = -M

MKVERSION = $(top_srcdir)/build/mkversion -v "$(VERSION)"

LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIBRELEASE = 2.3
LIBVERSION = 2:8:2
LTVERSION = -release $(LIBRELEASE) -version-info $(LIBVERSION)

# libtool --only flag for libraries: platform specific
NT_LTONLY_LIB = # --only-$(BUILD_LIBS_DYNAMIC)
LTONLY_LIB = $(UNIX_LTONLY_LIB)

# libtool --only flag for modules: depends on linkage of module
# The BUILD_MOD macro is defined in each backend Makefile.in file
LTONLY_yes = --tag=disable-shared
LTONLY_mod = --tag=disable-static
LTONLY_MOD = $(LTONLY_$(BUILD_MOD))

# platform-specific libtool flags
NT_LTFLAGS_LIB = -no-undefined -avoid-version -rpath $(libdir)
NT_LTFLAGS_MOD = -no-undefined -avoid-version -rpath $(moduledir)
UNIX_LTFLAGS_LIB = $(LTVERSION) -rpath $(libdir)
UNIX_LTFLAGS_MOD = $(LTVERSION) -rpath $(moduledir)

# libtool flags
LTFLAGS     = $(UNIX_LTFLAGS)
LTFLAGS_LIB = $(UNIX_LTFLAGS_LIB)
LTFLAGS_MOD = $(UNIX_LTFLAGS_MOD)

# LIB_DEFS defined in liblber and libldap Makefile.in files.
# MOD_DEFS defined in backend Makefile.in files.

# platform-specific LINK_LIBS defined in various Makefile.in files.
# LINK_LIBS referenced in library and module link commands.
LINK_LIBS = $(MOD_LIBS) $(UNIX_LINK_LIBS)

LTSTATIC = -static

LTLINK   = $(LIBTOOL) --mode=link \
	$(CC) $(LTSTATIC) $(LT_CFLAGS) $(LDFLAGS) $(LTFLAGS)

LTCOMPILE_LIB = $(LIBTOOL) $(LTONLY_LIB) --mode=compile \
	$(CC) $(LT_CFLAGS) $(LT_CPPFLAGS) $(LIB_DEFS) -c

LTLINK_LIB = $(LIBTOOL) $(LTONLY_LIB) --mode=link \
	$(CC) $(LT_CFLAGS) $(LDFLAGS) $(LTFLAGS_LIB)

LTCOMPILE_MOD = $(LIBTOOL) $(LTONLY_MOD) --mode=compile \
	$(CC) $(LT_CFLAGS) $(LT_CPPFLAGS) $(MOD_DEFS) -c

LTLINK_MOD = $(LIBTOOL) $(LTONLY_MOD) --mode=link \
	$(CC) $(LT_CFLAGS) $(LDFLAGS) $(LTFLAGS_MOD)

LTINSTALL = $(LIBTOOL) --mode=install $(INSTALL) 
LTFINISH = $(LIBTOOL) --mode=finish

# Misc UNIX commands used in build environment
AR = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-ar
BASENAME = basename
CAT = cat
CHMOD = chmod
DATE = date
ECHO = $(SHTOOL) echo
HOSTNAME = $(SHTOOL) echo -e "%h%d"
LN = $(SHTOOL) mkln
LN_H = $(LN)
LN_S = $(LN) -s
MAKEINFO = ${SHELL} /opt/devel/proto/marvell/build-eabi/openldap-2.3.20/build/missing --run makeinfo
MKDIR = $(SHTOOL) mkdir -p
MV = $(SHTOOL) move
PWD = pwd
RANLIB = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-ranlib
RM = rm -f
SED = sed
SUBST = $(SHTOOL) subst

# For manual pages
# MANCOMPRESS=@MANCOMPRESS@
# MANCOMPRESSSUFFIX=@MANCOMPRESSSUFFIX@
MANCOMPRESS=$(CAT)
MANCOMPRESSSUFFIX=

INCLUDEDIR= $(top_srcdir)/include
LDAP_INCPATH= -I$(LDAP_INCDIR) -I$(INCLUDEDIR)
LDAP_LIBDIR= $(top_builddir)/libraries

LUTIL_LIBS = 
LDBM_LIBS = 
LTHREAD_LIBS =  -pthread

LDAP_LIBLBER_LA = $(LDAP_LIBDIR)/liblber/liblber.la
LDAP_LIBLDAP_LA = $(LDAP_LIBDIR)/libldap/libldap.la
LDAP_LIBLDAP_R_LA = $(LDAP_LIBDIR)/libldap_r/libldap_r.la

LDAP_LIBREWRITE_A = $(LDAP_LIBDIR)/librewrite/librewrite.a
LDAP_LIBLUNICODE_A = $(LDAP_LIBDIR)/liblunicode/liblunicode.a
LDAP_LIBLUTIL_A = $(LDAP_LIBDIR)/liblutil/liblutil.a

LDAP_L = $(LDAP_LIBLUTIL_A) \
	$(LDAP_LIBLDAP_LA) $(LDAP_LIBLBER_LA)
SLURPD_L = $(LDAP_LIBLUTIL_A) \
	$(LDAP_LIBLDAP_R_LA) $(LDAP_LIBLBER_LA)
SLAPD_L = $(LDAP_LIBLUNICODE_A) $(LDAP_LIBREWRITE_A) \
	$(SLURPD_L)

WRAP_LIBS = 
# AutoConfig generated 
AC_CC	= /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-gcc
AC_CFLAGS = -g -O2
AC_DEFS = -I/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/usr/include # -DHAVE_CONFIG_H
AC_LDFLAGS = -L/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/usr/lib -L/opt/devel/proto/marvell/build-eabi/openldap-2.3.20/libraries/liblutil
AC_LIBS = -lresolv 

KRB4_LIBS = 
KRB5_LIBS = 
KRB_LIBS =  
SASL_LIBS = 
TLS_LIBS = -lssl -lcrypto
AUTH_LIBS = 
SECURITY_LIBS = $(SASL_LIBS) $(KRB_LIBS) $(TLS_LIBS) $(AUTH_LIBS)

MODULES_CPPFLAGS = 
MODULES_LDFLAGS = 
MODULES_LIBS = 
SLAPD_PERL_LDFLAGS = 

SLAPD_SQL_LDFLAGS = 
SLAPD_SQL_INCLUDES = 
SLAPD_SQL_LIBS = 

SLAPD_LIBS =      
SLURPD_LIBS = 

# Our Defaults
CC = $(AC_CC)
DEFS = $(LDAP_INCPATH) $(XINCPATH) $(XDEFS) $(AC_DEFS) $(DEFINES)
CFLAGS = $(AC_CFLAGS) $(DEFS)
LDFLAGS = $(LDAP_LIBPATH) $(AC_LDFLAGS) $(XLDFLAGS)
LIBS = $(XLIBS) $(XXLIBS) $(AC_LIBS) $(XXXLIBS)

LT_CFLAGS = $(AC_CFLAGS)
LT_CPPFLAGS = $(DEFS)

all:		all-common all-local FORCE
install:	install-common install-local FORCE
clean:		clean-common clean-local FORCE
veryclean:	veryclean-common veryclean-local FORCE
depend:		depend-common depend-local FORCE

# empty common rules
all-common:
install-common:
clean-common:
veryclean-common:	clean-common FORCE
depend-common:
lint-common:
lint5-common:

# empty local rules
all-local:
install-local:
clean-local:
veryclean-local:	clean-local FORCE
depend-local:
lint-local:
lint5-local:

veryclean: FORCE
	$(RM) Makefile
	$(RM) -r .libs

Makefile: Makefile.in $(top_srcdir)/build/top.mk

pathtest:
	$(SHTOOL) --version

# empty rule for forcing rules
FORCE:

##---------------------------------------------------------------------------

# Makefile.in for slurpd
# $OpenLDAP: pkg/ldap/servers/slurpd/Makefile.in,v 1.36.2.3 2006/01/03 22:16:26 kurt Exp $
## This work is part of OpenLDAP Software <http://www.openldap.org/>.
##
## Copyright 1998-2006 The OpenLDAP Foundation.
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted only as authorized by the OpenLDAP
## Public License.
##
## A copy of this license is available in the file LICENSE in the
## top-level directory of the distribution or, alternatively, at
## <http://www.OpenLDAP.org/license.html>.

PROGRAMS = slurpd
XPROGRAMS = sslurpd

XSRCS	= version.c

NT_SRCS	= nt_svc.c
NT_OBJS = nt_svc.o ../../libraries/liblutil/slapdmsg.res

SRCS	=	admin.c args.c ch_malloc.c config.c \
		fm.c globals.c ldap_op.c lock.c main.c re.c \
		reject.c replica.c replog.c ri.c rq.c sanity.c st.c \
		$(UNIX_SRCS)
OBJS	=	admin.o args.o ch_malloc.o config.o \
		fm.o globals.o ldap_op.o lock.o main.o re.o \
		reject.o replica.o replog.o ri.o rq.o sanity.o st.o \
		$(UNIX_OBJS)

LDAP_INCDIR= ../../include       
LDAP_LIBDIR= ../../libraries

BUILD_OPT = "--enable-slurpd"
BUILD_SRV = no

all-local-srv: 	$(PROGRAMS)

# $(LTHREAD_LIBS) must be last!
XLIBS = $(SLURPD_L)
XXLIBS = $(SLURPD_LIBS) $(SECURITY_LIBS) $(LUTIL_LIBS)
XXXLIBS = $(LTHREAD_LIBS)

slurpd:	version.o
	$(LTLINK) -o $@ $(OBJS) version.o $(LIBS)

sslurpd:	version.o
	$(LTLINK) -static -o $@ $(OBJS) version.o $(LIBS)

version.c: Makefile
	@-$(RM) $@
	$(MKVERSION) -s -n Versionstr slurpd > $@

version.o: version.c $(OBJS) $(SLURPD_L)

install-local-srv: FORCE
	-$(MKDIR) $(DESTDIR)$(libexecdir)
	@-$(INSTALL) -m 700 -d $(DESTDIR)$(localstatedir)/openldap-slurp
	@(								\
	    for prg in $(PROGRAMS); do					\
		$(LTINSTALL) $(INSTALLFLAGS) $(STRIP) -m 755 $$prg$(EXEEXT)	\
		    $(DESTDIR)$(libexecdir);				\
	    done							\
	)

# $OpenLDAP: pkg/ldap/build/srv.mk,v 1.15.2.3 2006/01/03 22:16:01 kurt Exp $
## This work is part of OpenLDAP Software <http://www.openldap.org/>.
##
## Copyright 1998-2006 The OpenLDAP Foundation.
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted only as authorized by the OpenLDAP
## Public License.
##
## A copy of this license is available in the file LICENSE in the
## top-level directory of the distribution or, alternatively, at
## <http://www.OpenLDAP.org/license.html>.
##---------------------------------------------------------------------------
#
# Makefile Template for Servers
#

all-common: all-$(BUILD_SRV)
all-no lint-no 5lint-no depend-no install-no:
	@echo "run configure with $(BUILD_OPT) to make $(PROGRAMS)"

clean-common: clean-srv FORCE
veryclean-common: veryclean-srv FORCE

lint-common: lint-$(BUILD_SRV)

5lint-common: 5lint-$(BUILD_SRV)

depend-common: depend-$(BUILD_SRV)

install-common: install-$(BUILD_SRV)

all-local-srv:
all-yes: all-local-srv FORCE

install-local-srv:
install-yes: install-local-srv FORCE

lint-local-srv:
lint-yes: lint-local-srv FORCE
	$(LINT) $(DEFS) $(DEFINES) $(SRCS)

5lint-local-srv:
5lint-yes: 5lint-local-srv FORCE
	$(5LINT) $(DEFS) $(DEFINES) $(SRCS)

clean-local-srv:
clean-srv: 	clean-local-srv FORCE
	$(RM) $(PROGRAMS) $(XPROGRAMS) $(XSRCS) *.o a.out core .libs/* *.exe

depend-local-srv:
depend-yes: depend-local-srv FORCE
	$(MKDEP) $(DEFS) $(DEFINES) $(SRCS)

veryclean-local-srv:
veryclean-srv: 	clean-srv veryclean-local-srv

Makefile: $(top_srcdir)/build/srv.mk
