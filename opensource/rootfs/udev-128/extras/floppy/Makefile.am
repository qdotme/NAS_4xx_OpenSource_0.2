udevhomedir = $(udev_prefix)/lib/udev
udevhome_PROGRAMS = \
	create_floppy_devices

AM_CPPFLAGS = \
	-DSYSCONFDIR=\""$(sysconfdir)"\" \
	-DUDEV_PREFIX=\""$(udev_prefix)"\" \
	-D_LIBUDEV_COMPILATION

create_floppy_devices_SOURCES = \
	create_floppy_devices.c \
	../../udev/lib/libudev.h \
	../../udev/lib/libudev.c \
	../../udev/lib/libudev-util.c \
	../../udev/udev_utils.c

if USE_SELINUX
create_floppy_devices_LDADD = \
	$(SELINUX_LIBS)
endif

dist_man_MANS = \
	create_floppy_devices.8

distclean-local:
	rm -f Makefile.in
