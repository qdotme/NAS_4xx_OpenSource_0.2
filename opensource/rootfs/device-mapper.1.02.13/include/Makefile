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


LN_S = ln -s

.PHONY: clean distclean all install pofile

all: .symlinks_created

.symlinks_created: .symlinks Makefile
	find . -maxdepth 2 -type l -exec $(RM) \{\} \;
	for i in `cat .symlinks`; do $(LN_S) $$i ; done
	touch $@
ifeq ("yes", "yes")
	$(LN_S) ../../kernel/ioctl/dm-ioctl.h linux
endif

distclean:
	find . -maxdepth 2 -type l -exec $(RM) \{\} \;
	$(RM) Makefile .include_symlinks .symlinks_created configure.h

pofile: all

clean:

install:

