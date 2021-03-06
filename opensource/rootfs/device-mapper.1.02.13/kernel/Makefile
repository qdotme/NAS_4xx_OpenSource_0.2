#
# Copyright (C) 2001-2004 Sistina Software, Inc. All rights reserved.
# Copyright (C) 2004 Red Hat, Inc. All rights reserved.
#
# This file is part of the device-mapper userspace tools.
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions
# of the GNU General Public License v.2.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

SHELL = /bin/sh

srcdir = .
top_srcdir = ..

interface = ioctl
kerneldir = 
kernelvsn = UNKNOWN
tmpdir=/tmp/kerndiff
LN_S = ln -s


FS=dmfs-error.c dmfs-lv.c dmfs-root.c dmfs-status.c \
   dmfs-super.c dmfs-suspend.c dmfs-table.c dmfs.h

COMMON=dm-linear.c dm-stripe.c \
       dm-snapshot.c kcopyd.c \
       dm-table.c dm-target.c dm.c dm.h dm-snapshot.h \
       dm-exception-store.c kcopyd.h \
       dm-io.c dm-io.h dm-log.c dm-log.h dm-daemon.c dm-daemon.h dm-raid1.c

IOCTL=dm-ioctl.c

common_hdrs=common/device-mapper.h
ioctl_hdr=ioctl/dm-ioctl.h


KERNELTAR=/usr/src/linux-$(kernelvsn).tar
fs=$(patsubst %,fs/%,$(FS))
common=$(patsubst %,common/%,$(COMMON))
ioctl=$(patsubst %,ioctl/%,$(IOCTL))

.PHONY: install clean distclean all symlinks patches patches-clean

all: symlinks

symlinks:
	for i in $(common) $(fs) $(ioctl); do \
		if [ -L $(kerneldir)/drivers/md/`basename $$i` ] ; \
			then $(RM) $(kerneldir)/drivers/md/`basename $$i`; \
		fi; \
	done
	for i in $(common) $($(interface)) ; do \
		$(LN_S) `pwd`/$$i $(kerneldir)/drivers/md ; \
	done
	for i in $(common_hdrs) $(ioctl_hdr) ; do \
		if [ -L $(kerneldir)/include/linux/`basename $$i` ] ; \
			then $(RM) \
			     $(kerneldir)/include/linux/`basename $$i`; \
		fi; \
	done

	for i in $(common_hdrs) ; do \
		$(LN_S) `pwd`/$$i $(kerneldir)/include/linux ; \
	done
	if [ "$(interface)" == "ioctl" ] ; then \
		$(LN_S) `pwd`/$(ioctl_hdr) $(kerneldir)/include/linux ; \
	fi

patches:
	if [ ! -e $(KERNELTAR) ] ; then \
		echo "Can't find kernel source tarball $(KERNELTAR)" ; \
		exit 1; \
	fi
	if [ ! -e $(tmpdir) ] ; then mkdir $(tmpdir); fi
	if [ ! -d $(tmpdir) ] ; then \
		echo "Working directory $(tmpdir) missing" ; \
		exit 1; \
	fi

	( \
		cd $(tmpdir); \
		tar xf $(KERNELTAR) ; \
		cp -al linux linux-$(kernelvsn) ; \
	)

	for i in $(common) $($(interface)) ; do \
		$(LN_S) `pwd`/$$i $(tmpdir)/linux/drivers/md ; \
	done

	for i in $(common_hdrs) ; do \
		$(LN_S) `pwd`/$$i $(tmpdir)/linux/include/linux ; \
	done

	if [ "$(interface)" == "ioctl" ] ; then \
		$(LN_S) `pwd`/$(ioctl_hdr) $(tmpdir)/linux/include/linux ; \
	fi

	for i in ../patches/common/linux-$(kernelvsn)* \
		 ../patches/$(interface)/linux-$(kernelvsn)* ; do \
		patch -d $(tmpdir)/linux -p1 -i `pwd`/$$i ; \
	done

	( \
		cd $(tmpdir); \
		diff -ruN linux-$(kernelvsn) linux ; \
		exit 0; \
	) > ../patches/linux-$(kernelvsn)-devmapper-$(interface).patch

	$(RM) -r $(tmpdir)

patches-clean:
	$(RM) -r $(tmpdir)
	$(RM) ../patches/linux-$(kernelvsn)-devmapper-$(interface).patch

install:

clean:

distclean:
	$(RM) Makefile

