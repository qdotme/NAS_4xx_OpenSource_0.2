rootlibdir = $(exec_prefix)/$(libdir_name)
rootlib_LTLIBRARIES = \
	libvolume_id.la

include_HEADERS =\
	libvolume_id.h

libvolume_id_la_SOURCES =\
	libvolume_id-private.h \
	volume_id.c \
	util.c \
	md5.h \
	md5.c \
	ext.c \
	fat.c \
	hfs.c \
	highpoint.c \
	isw_raid.c \
	lsi_raid.c \
	via_raid.c \
	silicon_raid.c \
	nvidia_raid.c \
	promise_raid.c \
	adaptec_raid.c \
	jmicron_raid.c \
	ddf_raid.c \
	iso9660.c \
	jfs.c \
	linux_raid.c \
	linux_swap.c \
	lvm.c \
	ntfs.c \
	reiserfs.c \
	udf.c \
	ufs.c \
	xfs.c \
	cramfs.c \
	hpfs.c \
	romfs.c \
	sysv.c \
	minix.c \
	gfs.c \
	luks.c \
	ocfs.c \
	vxfs.c \
	squashfs.c \
	netware.c \
	oracleasm.c

libvolume_id_la_LDFLAGS = \
	-version-info $(VOLID_LT_CURRENT):$(VOLID_LT_REVISION):$(VOLID_LT_AGE) \
	-export-symbols $(top_srcdir)/extras/volume_id/lib/exported_symbols

# move devel files to $(prefix)$(libdir_name) if needed
install-data-hook:
	rm  $(DESTDIR)$(rootlibdir)/libvolume_id.la
	if test "$(prefix)" != "$(exec_prefix)"; then \
		mkdir -p $(DESTDIR)$(prefix)/$(libdir_name); \
		mv $(DESTDIR)$(rootlibdir)/libvolume_id.a $(DESTDIR)$(prefix)/$(libdir_name)/; \
		so_img_name=$$(readlink $(DESTDIR)$(rootlibdir)/libvolume_id.so); \
		rm $(DESTDIR)$(rootlibdir)/libvolume_id.so; \
		so_img_rel_target_prefix=$$(echo $(prefix)/$(libdir_name) | sed 's,\(^/\|\)[^/][^/]*,..,g'); \
		ln -sf  $$so_img_rel_target_prefix$(exec_prefix)/$(libdir_name)/$$so_img_name \
			$(DESTDIR)$(prefix)/$(libdir_name)/libvolume_id.so; \
	fi

EXTRA_DIST = \
	exported_symbols

pkgconfigdir = $(prefix)/$(libdir_name)/pkgconfig
pkgconfig_DATA = libvolume_id.pc

distclean-local:
	rm -f Makefile.in
