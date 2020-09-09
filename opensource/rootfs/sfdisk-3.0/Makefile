# Makefile for standalone sfdisk
# Copyright (C) 2001 Erik Andersen <andersen@codepoet.org>
#
# This package was ripped out of the util-linux package and contains
# sfdisk version 3.0 Copyright (C) 1995  Andries E. Brouwer (aeb@cwi.nl)
# which is licensed under the terms of the GNU General Public License

PROG      := sfdisk


# If you want a static binary, turn this on.
DOSTATIC = false

# Set the following to `true' to make a debuggable build.
# Leave this set to `false' for production use.
DODEBUG = false

# If you want large file summit support, turn this on.
# This has no effect if you don't have a kernel with lfs
# support, and a system with libc-2.1.3 or later.
# LFS allows support for files larger than 2GB!
DOLFS = true


# If you are running a cross compiler, you may want to set this
# to something more interesting, like "powerpc-linux-".
CROSS = #../../staging_dir/usr/bin/i386-uclibc-
CC = $(CROSS)gcc
AR = $(CROSS)ar
STRIPTOOL = $(CROSS)strip

WARNINGS=-Wall -Wmissing-prototypes -Wstrict-prototypes #-Wshadow
CFLAGS=-fomit-frame-pointer


# A nifty macro to make testing gcc features easier
check_gcc=$(shell if $(CC) $(1) -S -o /dev/null -xc /dev/null > /dev/null 2>&1; \
	then echo "$(1)"; else echo "$(2)"; fi)

# use '-Os' optimization if available, else use -O2
OPTIMIZATION=
OPTIMIZATION=${call check_gcc,-Os,-O2}

ifeq ($(strip $(DOLFS)),true)
    # For large file summit support
    CFLAGS+=-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
endif
ifeq ($(strip $(DODEBUG)),true)
    CFLAGS  += $(WARNINGS) -g -D_GNU_SOURCE
    LDFLAGS += -Wl,-warn-common
    STRIP    =
else
    CFLAGS  += $(WARNINGS) $(OPTIMIZATION) -fomit-frame-pointer -D_GNU_SOURCE
    LDFLAGS += -s -Wl,-warn-common
    STRIP    = $(STRIPTOOL) --remove-section=.note --remove-section=.comment $(PROG)
endif
ifeq ($(strip $(DOSTATIC)),true)
    LDFLAGS += --static
endif

ifndef $(PREFIX)
    PREFIX = `pwd`/_install
endif

SOURCE=$(PROG).c i386_sys_types.c partname.c get_blocks.c
OBJS=$(patsubst %.c,%.o, $(SOURCE))

all: $(OBJS) $(PROG)

$(PROG): $(OBJS)
	$(CC) -s $(LDFLAGS) $(OBJS) -o $(PROG)
	$(STRIPTOOL) --remove-section=.note --remove-section=.comment $(PROG)

$(OBJS): %.o: %.c Makefile 
	$(CC) $(CFLAGS) -c $*.c -o $*.o

clean:
	rm -f *.o core $(PROG)

