#
# Copyright (c) 2000-2001,2004 Silicon Graphics, Inc.  All Rights Reserved.
#

TOPDIR = ..
include $(TOPDIR)/include/builddefs

LTLIBRARY = libdisk.la
LT_CURRENT = 0
LT_REVISION = 0
LT_AGE = 0

CFILES = drivers.c fstype.c pttype.c
HFILES = drivers.h fstype.h pttype.h md.h xvm.h evms.h
LINUX_DRIVERS = dm.c md.c xvm.c evms.c lvm.c

ifeq ($(PKG_PLATFORM),linux)
CFILES += $(LINUX_DRIVERS)
else
LSRCFILES = $(LINUX_DRIVERS)
endif

default: $(LTLIBRARY)

include $(BUILDRULES)

install: default

install-dev: default
	$(INSTALL_LTLIB_STATIC)
