#Depending on your system, you will need to modify this makefile.

#If you do not have gcc, change the setting for CC, but you must
#use an ANSI standard C compiler (NOT the old SunOS 4.1.3 cc
#compiler; get gcc if you are still using it). 

#If the ar command fails on your system, consult the ar manpage
#for your system. 

CC = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-gcc
AR=ar
LIBS=-L./ -lgd -lm
RANLIB = /opt/devel/proto/marvell/build-eabi/staging_dir/bin/arm-linux-gnueabi-ranlib

CFLAGS = -Os -pipe -Wall
LDFLAGS = -L/opt/devel/proto/marvell/build-eabi/staging_dir/arm-linux-gnueabi/lib

lib: libgd.a

all: libgd.a gddemo giftogd webgif

gddemo: gddemo.o libgd.a gd.h gdfonts.h gdfontl.h
	$(CC) gddemo.o -o gddemo	$(LIBS)

giftogd: giftogd.o libgd.a gd.h
	$(CC) giftogd.o -o giftogd	$(LIBS) 

libgd.a: gd.o gdfontt.o gdfonts.o gdfontmb.o gdfontl.o gdfontg.o \
	gd.h gdfontt.h gdfonts.h gdfontmb.h gdfontl.h gdfontg.h
	rm -f libgd.a
	$(AR) rc libgd.a gd.o gdfontt.o gdfonts.o gdfontmb.o \
		gdfontl.o gdfontg.o
	$(RANLIB) libgd.a

webgif: webgif.o libgd.a gd.h
	$(CC) webgif.o -o webgif	$(LIBS)

clean:
	rm -f *.o *.a gddemo giftogd webgif
