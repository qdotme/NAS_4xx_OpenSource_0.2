#
# Copyright (C) 2001-2004 Sistina Software, Inc. All rights reserved.
# Copyright (C) 2004 Red Hat, Inc. All rights reserved.
#
# This file is part of the LVM2.
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions
# of the GNU General Public License v.2.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

srcdir = .
top_srcdir = ..


SOURCES =\
	archiver.c \
	dumpconfig.c \
	lvchange.c \
	lvcreate.c \
	lvdisplay.c \
	lvextend.c \
	lvmchange.c \
	lvmcmdline.c \
	lvmdiskscan.c \
	lvreduce.c \
	lvremove.c \
	lvrename.c \
	lvresize.c \
	lvscan.c \
	pvchange.c \
	pvcreate.c \
	pvdisplay.c \
	pvmove.c \
	pvremove.c \
	pvscan.c \
	reporter.c \
	toollib.c \
	vgcfgbackup.c \
	vgcfgrestore.c \
	vgchange.c \
	vgck.c \
	vgcreate.c \
	vgconvert.c \
	vgdisplay.c \
	vgexport.c \
	vgextend.c \
	vgimport.c \
	vgmerge.c \
	vgmknodes.c \
	vgreduce.c \
	vgremove.c \
	vgrename.c \
	vgscan.c \
	vgsplit.c

TARGETS =\
	.commands \
	lvm

INSTALL_TARGETS = install_tools_dynamic
INSTALL_CMDLIB_TARGETS = install_cmdlib_dynamic

ifeq ("no", "yes")
  TARGETS += lvm.static
  INSTALL_TARGETS += install_tools_static
  INSTALL_CMDLIB_TARGETS += install_cmdlib_static
endif

LVMLIBS = -llvm

CLEAN_TARGETS = liblvm2cmd.so liblvm2cmd.a lvm lvm.static

ifeq ("no", "yes")
	TARGETS += liblvm2cmd.so
	INSTALL_TARGETS += $(INSTALL_CMDLIB_TARGETS)
endif

ifeq ("yes", "yes")
	LVMLIBS += -ldevmapper
endif

include ../make.tmpl

LD_FLAGS += -L../../device-mapper.1.02.13/lib/ioctl

lvm: $(OBJECTS) lvm.o $(top_srcdir)/lib/liblvm.a
	$(CC) -o $@ $(OBJECTS) lvm.o $(LD_FLAGS) $(LVMLIBS) $(LIBS) -rdynamic

lvm.static: $(OBJECTS) lvm.o $(top_srcdir)/lib/liblvm.a
	$(CC) -o $@ $(OBJECTS) lvm.o -static $(LD_FLAGS) $(LVMLIBS) $(LIBS) \
		-rdynamic

liblvm2cmd.a: $(top_srcdir)/lib/liblvm.a $(OBJECTS)
	cat $(top_srcdir)/lib/liblvm.a > $@
	$(AR) rs $@ $(OBJECTS)

liblvm2cmd.so: liblvm2cmd.a $(LD_DEPS)
	$(CC) -o liblvm2cmd.so $(SOFLAG) $(CLDFLAGS) \
		$(CLDWHOLEARCHIVE) liblvm2cmd.a $(CLDNOWHOLEARCHIVE)

.commands: commands.h cmdnames.h Makefile
	$(CC) -E -P cmdnames.h 2> /dev/null | \
		egrep -v '^ *(|#.*|dumpconfig|help|pvdata|version) *$$' > .commands

.PHONY: install_cmdlib_dynamic install_cmdlib_static \
	install_tools_dynamic install_tools_static

install_cmdlib_dynamic: liblvm2cmd.so
	$(INSTALL) -D $(OWNER) $(GROUP) -m 555 $(STRIP) liblvm2cmd.so \
		$(libdir)/liblvm2cmd.so.$(LIB_VERSION)
	$(LN_S) -f liblvm2cmd.so.$(LIB_VERSION) $(libdir)/liblvm2cmd.so
	$(INSTALL) -D $(OWNER) $(GROUP) -m 444 lvm2cmd.h \
		$(includedir)/lvm2cmd.h

install_cmdlib_static: liblvm2cmd.a
	$(INSTALL) -D $(OWNER) $(GROUP) -m 555 $(STRIP) liblvm2cmd.a \
		$(libdir)/liblvm2cmd.a.$(LIB_VERSION)
	$(LN_S) -f liblvm2cmd.a.$(LIB_VERSION) $(libdir)/liblvm2cmd.a
	$(INSTALL) -D $(OWNER) $(GROUP) -m 444 lvm2cmd.h \
		$(includedir)/lvm2cmd.h

install_tools_dynamic: lvm .commands
	$(INSTALL) -D $(OWNER) $(GROUP) -m 555 $(STRIP) lvm \
		$(sbindir)/lvm
	@echo Creating symbolic links for individual commands in $(sbindir)
	@( \
		for v in `cat .commands`; do \
			cd $(sbindir); \
			$(LN_S) -f lvm $$v; \
		done; \
	)

install_tools_static: lvm.static
	$(INSTALL) -D $(OWNER) $(GROUP) -m 555 $(STRIP) lvm.static \
		$(staticdir)/lvm.static

install: $(INSTALL_TARGETS)

