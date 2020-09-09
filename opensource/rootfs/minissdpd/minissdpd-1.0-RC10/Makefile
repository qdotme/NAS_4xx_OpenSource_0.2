# $Id: Makefile,v 1.1 2007/11/14 08:25:54 wiley Exp $
# MiniUPnP project
# author: Thomas Bernard
# website: http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
# for use with GNU Make (gmake)
# install with :
# $ PREFIX=/tmp/dummylocation make install
# or
# $ INSTALLPREFIX=/usr/local make install
# or
# make install (miniupnpd will be put in /usr/sbin)
#
CFLAGS = -Wall -g -D_GNU_SOURCE
CC = gcc
RM = rm -f
INSTALL = install

#EXECUTABLES = minissdpd testminissdpd listifaces
EXECUTABLES = minissdpd testminissdpd
MINISSDPDOBJS = minissdpd.o openssdpsocket.o daemonize.o
TESTMINISSDPDOBJS = testminissdpd.o

ALLOBJS = $(MINISSDPDOBJS) $(TESTMINISSDPDOBJS)

INSTALLPREFIX ?= $(PREFIX)/usr
SBININSTALLDIR = $(INSTALLPREFIX)/sbin

.PHONY:	all clean install depend

all:	$(EXECUTABLES)

clean:
	$(RM) $(ALLOBJS) $(EXECUTABLES)

install:	minissdpd
	$(INSTALL) -d $(SBININSTALLDIR)
	$(INSTALL) minissdpd $(SBININSTALLDIR)
	$(INSTALL) -d $(PREFIX)/etc/init.d
	$(INSTALL) minissdpd.init.d.script $(PREFIX)/etc/init.d/minissdpd

minissdpd: $(MINISSDPDOBJS)
	$(CC) $(CFLAGS) -o $@ $(MINISSDPDOBJS)

testminissdpd:	$(TESTMINISSDPDOBJS)

depend:
	makedepend -f$(MAKEFILE_LIST) -Y \
	$(ALLOBJS:.o=.c) 2>/dev/null

# DO NOT DELETE

minissdpd.o: openssdpsocket.h daemonize.h
openssdpsocket.o: openssdpsocket.h
daemonize.o: daemonize.h
