udevhomedir = $(udev_prefix)/lib/udev
udevhome_PROGRAMS = \
	edd_id

udevrulesdir = $(udev_prefix)/lib/udev/rules.d
dist_udevrules_DATA = \
	61-persistent-storage-edd.rules

AM_CPPFLAGS = \
	-DSYSCONFDIR=\""$(sysconfdir)"\" \
	-DUDEV_PREFIX=\""$(udev_prefix)"\" \
	-D_LIBUDEV_COMPILATION

edd_id_SOURCES = \
	edd_id.c \
	../../udev/lib/libudev.h \
	../../udev/lib/libudev.c \
	../../udev/lib/libudev-util.c

if USE_SELINUX
edd_id_LDADD = \
	$(SELINUX_LIBS)
endif

dist_man_MANS = \
	edd_id.8

distclean-local:
	rm -f Makefile.in
