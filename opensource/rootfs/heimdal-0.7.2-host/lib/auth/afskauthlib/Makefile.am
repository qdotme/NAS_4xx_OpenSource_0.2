# $Id: Makefile.am,v 1.1.1.1 2007/01/11 02:33:19 wiley Exp $

include $(top_srcdir)/Makefile.am.common

AM_CPPFLAGS += $(INCLUDE_krb4)

DEFS = @DEFS@

foodir = $(libdir)
foo_DATA = afskauthlib.so

SUFFIXES += .c .o

SRCS = verify.c
OBJS = verify.o

CLEANFILES = $(foo_DATA) $(OBJS) so_locations

afskauthlib.so: $(OBJS)
	$(LIBTOOL) --mode=link $(CC) -shared -o $@ $(OBJS) $(L) $(LDFLAGS)

.c.o:
	$(CC) $(DEFS) $(DEFAULT_AM_CPPFLAGS) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS) \
	-c `test -f '$<' || echo '$(srcdir)/'`$<

KAFS = $(top_builddir)/lib/kafs/libkafs.la

if KRB5
L = \
	$(KAFS)	\
	$(top_builddir)/lib/krb5/libkrb5.la	\
	$(top_builddir)/lib/asn1/libasn1.la	\
	$(LIB_krb4)				\
	$(LIB_des)				\
	$(top_builddir)/lib/roken/libroken.la	\
	-lc

else

L = \
	$(KAFS)	\
	$(LIB_krb4)				\
	$(LIB_des)				\
	$(top_builddir)/lib/roken/libroken.la	\
	-lc
endif

$(OBJS): $(top_builddir)/include/config.h
