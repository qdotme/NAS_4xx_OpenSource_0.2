# $Id: Makefile,v 1.1 2007/11/14 07:24:11 wiley Exp $
# MiniUPnP Project
# http://miniupnp.free.fr/
# (c) 2005-2007 Thomas Bernard
# to install use :
# $ PREFIX=/tmp/dummylocation make install
# or
# $ INSTALLPREFIX=/usr/local make install
# or 
# make install (will go to /usr/bin, /usr/lib, etc...)
#CC = gcc
#AR = gar
#CFLAGS = -O -Wall -g -DDEBUG
CFLAGS = -O -Wall -DNDEBUG
INSTALL = install
#following libs are needed on Solaris
#LDLIBS=-lsocket -lnsl -lresolv

SRCS = igd_desc_parse.c miniupnpc.c minixml.c minisoap.c miniwget.c \
       upnpc.c upnpcommands.c upnpreplyparse.c testminixml.c \
       minixmlvalid.c testupnpreplyparse.c minissdpc.c

LIBOBJS = miniwget.o minixml.o igd_desc_parse.o minisoap.o \
          miniupnpc.o upnpreplyparse.o upnpcommands.o minissdpc.o

OBJS = $(patsubst %.c,%.o,$(SRCS))

# HEADERS to install
HEADERS = miniupnpc.h miniwget.h upnpcommands.h igd_desc_parse.h \
          upnpreplyparse.h
LIBRARY = libminiupnpc.a
SHAREDLIBRARY = libminiupnpc.so
SONAME = $(SHAREDLIBRARY).1
EXECUTABLES = upnpc-static upnpc-shared

INSTALLPREFIX ?= $(PREFIX)/usr
INSTALLDIRINC = $(INSTALLPREFIX)/include/miniupnpc
INSTALLDIRLIB = $(INSTALLPREFIX)/lib

.PHONY:	install clean depend all installpythonmodule

all: $(LIBRARY) $(EXECUTABLES)

pythonmodule: $(LIBRARY) miniupnpcmodule.c setup.py
	python setup.py build
	touch $@

installpythonmodule: pythonmodule
	python setup.py install

validateminixml: minixmlvalid
	@echo "minixml validation test"
	./minixmlvalid
	touch $@

clean:
	$(RM) $(LIBRARY) $(SHAREDLIBRARY) $(EXECUTABLES) $(OBJS)

install: $(LIBRARY) $(SHAREDLIBRARY)
	$(INSTALL) -d $(INSTALLDIRINC)
	$(INSTALL) --mode=644 $(HEADERS) $(INSTALLDIRINC)
	$(INSTALL) -d $(INSTALLDIRLIB)
	$(INSTALL) --mode=644 $(LIBRARY) $(INSTALLDIRLIB)
	$(INSTALL) --mode=644 $(SHAREDLIBRARY) $(INSTALLDIRLIB)/$(SONAME)
	ln --force --symbolic $(SONAME) $(INSTALLDIRLIB)/$(SHAREDLIBRARY)

cleaninstall:
	$(RM) -r $(INSTALLDIRINC)
	$(RM) $(INSTALLDIRLIB)/$(LIBRARY)
	$(RM) $(INSTALLDIRLIB)/$(SHAREDLIBRARY)

depend:
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2>/dev/null

$(LIBRARY): $(LIBOBJS)
	$(AR) crs $@ $?

$(SHAREDLIBRARY): $(LIBOBJS)
	$(CC) -shared -Wl,-soname,$(SONAME) -o $@ $^

upnpc-static: upnpc.o $(LIBRARY)
	$(CC) -o $@ $^

upnpc-shared: upnpc.o $(SHAREDLIBRARY)
	$(CC) -o $@ $^

testminixml: minixml.o igd_desc_parse.o testminixml.o

minixmlvalid: minixml.o minixmlvalid.o

testupnpreplyparse: testupnpreplyparse.o minixml.o upnpreplyparse.o

# DO NOT DELETE THIS LINE -- make depend depends on it.

igd_desc_parse.o: igd_desc_parse.h
miniupnpc.o: miniupnpc.h declspec.h igd_desc_parse.h minissdpc.h miniwget.h
miniupnpc.o: minisoap.h minixml.h upnpcommands.h upnpreplyparse.h
minixml.o: minixml.h
minisoap.o: minisoap.h
miniwget.o: miniupnpc.h declspec.h igd_desc_parse.h
upnpc.o: miniwget.h declspec.h miniupnpc.h igd_desc_parse.h upnpcommands.h
upnpc.o: upnpreplyparse.h
upnpcommands.o: upnpcommands.h upnpreplyparse.h declspec.h miniupnpc.h
upnpcommands.o: igd_desc_parse.h
upnpreplyparse.o: upnpreplyparse.h minixml.h
testminixml.o: minixml.h igd_desc_parse.h
minixmlvalid.o: minixml.h
testupnpreplyparse.o: upnpreplyparse.h
minissdpc.o: minissdpc.h miniupnpc.h declspec.h igd_desc_parse.h
