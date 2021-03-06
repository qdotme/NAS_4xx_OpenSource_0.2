#
# Copyright (C) 2001-2004 Sistina Software, Inc. All rights reserved.
# Copyright (C) 2004-2005 Red Hat, Inc. All rights reserved.
#
# This file is part of the device-mapper userspace tools.
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions
# of the GNU Lesser General Public License v.2.1.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

srcdir = .
top_srcdir = ..

interface = ioctl

SOURCES =\
	datastruct/bitset.c \
	datastruct/hash.c \
	libdm-common.c \
	libdm-file.c \
	libdm-deptree.c \
	libdm-string.c \
	mm/dbg_malloc.c \
	mm/pool.c \
	$(interface)/libdm-iface.c

INCLUDES = -I$(interface)

LIB_STATIC = $(interface)/libdevmapper.a

ifeq ("so","dylib")
  LIB_SHARED = $(interface)/libdevmapper.dylib
else
  LIB_SHARED = $(interface)/libdevmapper.so
endif

CFLAGS += -DDEVICE_UID=0 -DDEVICE_GID=0 \
	  -DDEVICE_MODE=0600

include ../make.tmpl

.PHONY: install_dynamic install_static install_include \
	install_fs install_ioctl install_ioctl_static \
	install_pkgconfig

INSTALL_TYPE = install_dynamic

ifeq ("no", "yes")
  INSTALL_TYPE += install_static
endif

ifeq ("no", "yes")
  INSTALL_TYPE += install_pkgconfig
endif

install: $(INSTALL_TYPE) install_include

install_include:
	$(INSTALL) -D $(OWNER) $(GROUP) -m 444 libdevmapper.h \
		$(includedir)/libdevmapper.h

install_dynamic: install_ioctl
	$(LN_S) -f libdevmapper.$(LIB_SUFFIX).$(LIB_VERSION) \
		$(libdir)/libdevmapper.$(LIB_SUFFIX)

install_static: install_ioctl_static
	$(LN_S) -f libdevmapper.a.$(LIB_VERSION) $(libdir)/libdevmapper.a

install_fs: fs/libdevmapper.$(LIB_SUFFIX)
	$(INSTALL) -D $(OWNER) $(GROUP) -m 555 $(STRIP) $< \
		$(libdir)/libdevmapper.$(LIB_SUFFIX).$(LIB_VERSION)

install_ioctl: ioctl/libdevmapper.$(LIB_SUFFIX)
	$(INSTALL) -D $(OWNER) $(GROUP) -m 555 $(STRIP) $< \
		$(libdir)/libdevmapper.$(LIB_SUFFIX).$(LIB_VERSION)

install_pkgconfig:
	$(INSTALL) -D $(OWNER) $(GROUP) -m 444 libdevmapper.pc \
		$(usrlibdir)/pkgconfig/devmapper.pc

install_ioctl_static: ioctl/libdevmapper.a
	$(INSTALL) -D $(OWNER) $(GROUP) -m 555 $(STRIP) $< \
		$(libdir)/libdevmapper.a.$(LIB_VERSION)

.PHONY: distclean_lib distclean

distclean_lib: 
	$(RM) libdm-common.h libdevmapper.pc

distclean: distclean_lib

