SUBDIRS						=	Posix Linux
INCLUDES						=	-I$(top_srcdir)/include/
LDADD							=	$(AUTOIPD_EXTRA_OBJECTS) $(top_builddir)/src/lib/howl/libhowl.la $(PLATFORM_LIBS)
bin_PROGRAMS				=	autoipd
autoipd_SOURCES			=	autoip.c autoip.h
EXTRA_autoipd_SOURCES	=	Linux/linux_autoip.c Posix/posix_main.c
autoipd_DEPENDENCIES		=	linux_autoip.o posix_main.o
