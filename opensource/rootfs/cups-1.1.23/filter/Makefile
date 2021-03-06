#
# "$Id: Makefile,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $"
#
#   Filter makefile for the Common UNIX Printing System (CUPS).
#
#   Copyright 1997-2005 by Easy Software Products.
#
#   These coded instructions, statements, and computer programs are the
#   property of Easy Software Products and are protected by Federal
#   copyright law.  Distribution and use rights are outlined in the file
#   "LICENSE.txt" which should have been included with this file.  If this
#   file is missing or damaged please contact Easy Software Products
#   at:
#
#       Attn: CUPS Licensing Information
#       Easy Software Products
#       44141 Airport View Drive, Suite 204
#       Hollywood, Maryland 20636 USA
#
#       Voice: (301) 373-9600
#       EMail: cups-info@cups.org
#         WWW: http://www.cups.org
#
#   This file is subject to the Apple OS-Developed Software exception.
#

include ../Makedefs

FILTERS	=	gziptoany hpgltops texttops pstops imagetops imagetoraster \
		rastertodymo rastertoepson rastertohp
TARGETS	=	$(FILTERS)

HPGLOBJS =	hpgl-attr.o hpgl-config.o hpgl-main.o hpgl-prolog.o \
		hpgl-char.o hpgl-input.o hpgl-polygon.o hpgl-vector.o
IMAGEOBJS =	image-bmp.o image-colorspace.o image-gif.o image-jpeg.o \
		image-photocd.o image-pix.o image-png.o image-pnm.o \
		image-sgi.o image-sgilib.o image-sun.o image-tiff.o \
		image-zoom.o image.o raster.o
FORMOBJS =	form-attr.o form-main.o form-ps.o form-text.o form-tree.o
OBJS	=	$(HPGLOBJS) $(IMAGEOBJS) $(FORMOBJS) \
		gziptoany.o imagetops.o imagetoraster.o common.o pstops.o \
		rastertodymo.o rastertoepson.o rastertohp.o \
		texttops.o textcommon.o

#
# Make all targets...
#

all:	$(TARGETS) $(LIBCUPSIMAGE) libcupsimage.a


#
# Clean all object files...
#

clean:
	$(RM) $(OBJS) $(TARGETS) $(LIBCUPSIMAGE) libcupsimage.a
	$(RM) `basename $(LIBCUPSIMAGE) .2` libcupsimage.dylib


#
# Update dependencies (without system header dependencies...)
#

depend:
	makedepend -Y -I.. -fDependencies $(OBJS:.o=.c) >/dev/null 2>&1


#
# Install all targets...
#

install:	all installhdrs
	$(INSTALL_DIR) $(SERVERBIN)/filter
	for file in $(TARGETS); do \
		$(INSTALL_BIN) $$file $(SERVERBIN)/filter; \
	done
	$(INSTALL_DIR) $(LIBDIR)
	$(INSTALL_LIB) $(LIBCUPSIMAGE) $(LIBDIR)
	-if test $(LIBCUPSIMAGE) = "libcupsimage.so.2" -o $(LIBCUPSIMAGE) = "libcupsimage.sl.2"; then \
		$(RM) $(LIBDIR)/`basename $(LIBCUPSIMAGE) .2`; \
		$(LN) $(LIBCUPSIMAGE) $(LIBDIR)/`basename $(LIBCUPSIMAGE) .2`; \
	fi
	-if test $(LIBCUPSIMAGE) = "libcupsimage.2.dylib"; then \
		$(STRIP) -x $(LIBDIR)/$(LIBCUPSIMAGE); \
		$(RM) $(LIBDIR)/libcupsimage.dylib; \
		$(LN) $(LIBCUPSIMAGE) $(LIBDIR)/libcupsimage.dylib; \
	fi
	-if test $(LIBCUPSIMAGE) != "libcupsimage.a"; then \
		$(INSTALL_LIB) libcupsimage.a $(LIBDIR); \
		$(RANLIB) $(LIBDIR)/libcupsimage.a; \
	fi

installhdrs:
	$(INSTALL_DIR) $(INCLUDEDIR)/cups
	$(INSTALL_DATA) image.h $(INCLUDEDIR)/cups
	$(INSTALL_DATA) raster.h $(INCLUDEDIR)/cups


#
# formtops
#

formtops:	$(FORMOBJS) common.o ../Makedefs ../cups/$(LIBCUPS)
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ $(FORMOBJS) common.o $(LIBS) -lm


#
# gziptoany
#

gziptoany:	gziptoany.o ../Makedefs ../cups/$(LIBCUPS)
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ gziptoany.o $(LIBZ) $(LIBS)


#
# hpgltops
#

hpgltops:	$(HPGLOBJS) common.o ../Makedefs ../cups/$(LIBCUPS)
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ $(HPGLOBJS) common.o $(LIBS) -lm


#
# libcupsimage.so.2, libcupsimage.sl.2
#

libcupsimage.so.2 libcupsimage.sl.2:	$(IMAGEOBJS) ../Makedefs
	echo Linking $@...
	$(DSO) $(DSOFLAGS) -o $@ $(IMAGEOBJS) $(DSOLIBS) -lm
	$(RM) `basename $@ .2`
	$(LN) $@ `basename $@ .2`


#
# libcupsimage.2.dylib
#

libcupsimage.2.dylib:	$(IMAGEOBJS) ../Makedefs
	echo Linking $@...
	$(DSO) $(DSOFLAGS) -o $@  \
		-install_name $(libdir)/$@ \
		-current_version 2.1.0 \
		-compatibility_version 2.0.0 \
		$(IMAGEOBJS) $(DSOLIBS) -L../cups $(LINKCUPS) -lm -lcc_dynamic
	$(RM) libcupsimage.dylib
	$(LN) $@ libcupsimage.dylib


#
# libcupsimage_s.a
#

libcupsimage_s.a:	$(IMAGEOBJS) ../Makedefs
	echo Linking $@...
	$(DSO) $(DSOFLAGS) -Wl,-berok -o libcupsimage_s.o $(IMAGEOBJS) \
		$(DSOLIBS) -lm
	$(RM) $@
	$(AR) $(ARFLAGS) $@ libcupsimage_s.o


#
# libcupsimage.la
#

libcupsimage.la:       $(IMAGEOBJS) ../Makedefs
	echo Linking $@...
	$(DSO) $(DSOFLAGS) -o $@ $(IMAGEOBJS:.o=.lo) $(DSOLIBS) \
		-rpath $(LIBDIR) -version-info 2:1


#
# libcupsimage.a
#

libcupsimage.a:	$(IMAGEOBJS) ../Makedefs
	echo Archiving $@...
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $(IMAGEOBJS)
	$(RANLIB) $@


#
# testimage
#

testimage:	testimage.o libcupsimage.a ../Makedefs
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ testimage.o libcupsimage.a \
		$(IMGLIBS) $(DSOLIBS) $(LIBS)


#
# imagetops
#

imagetops:	imagetops.o common.o $(LIBCUPSIMAGE) ../Makedefs \
		../cups/$(LIBCUPS)
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ imagetops.o common.o $(LINKCUPSIMAGE) \
		$(IMGLIBS) $(LIBS)


#
# imagetoraster
#

imagetoraster:	imagetoraster.o common.o $(LIBCUPSIMAGE) ../Makedefs \
		../cups/$(LIBCUPS)
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ imagetoraster.o common.o $(LINKCUPSIMAGE) \
		$(IMGLIBS) $(LIBS)


#
# pstops
#

pstops:	pstops.o common.o ../Makedefs ../cups/$(LIBCUPS)
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ pstops.o common.o $(LIBS)


#
# rastertodymo
#

rastertodymo:	rastertodymo.o ../Makedefs ../cups/$(LIBCUPS) $(LIBCUPSIMAGE)
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ rastertodymo.o $(LINKCUPSIMAGE) $(IMGLIBS) $(LIBS)


#
# rastertoepson
#

rastertoepson:	rastertoepson.o ../Makedefs ../cups/$(LIBCUPS) $(LIBCUPSIMAGE)
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ rastertoepson.o $(LINKCUPSIMAGE) $(IMGLIBS) $(LIBS)


#
# rastertohp
#

rastertohp:	rastertohp.o ../Makedefs ../cups/$(LIBCUPS) $(LIBCUPSIMAGE)
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ rastertohp.o $(LINKCUPSIMAGE) $(IMGLIBS) $(LIBS)


#
# texttops
#

texttops:	texttops.o textcommon.o common.o ../Makedefs \
		../cups/$(LIBCUPS)
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ texttops.o textcommon.o common.o $(LIBS)


#
# Dependencies...
#

include Dependencies


#
# End of "$Id: Makefile,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $".
#
