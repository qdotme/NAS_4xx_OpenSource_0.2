#
# "$Id: Makefile,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $"
#
#   CGI makefile for the Common UNIX Printing System (CUPS).
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

include ../Makedefs

CGIS	=	admin.cgi classes.cgi jobs.cgi printers.cgi
TARGETS	=	libcgi.a $(CGIS)
LIBOBJS	=	html.o ipp-var.o template.o var.o
OBJS	=	$(LIBOBJS) admin.o classes.o jobs.o printers.o


#
# Make all targets...
#

all:	$(TARGETS)


#
# Clean all object files...
#

clean:
	$(RM) $(OBJS) $(TARGETS)


#
# Update dependencies (without system header dependencies...)
#

depend:
	makedepend -Y -I.. -fDependencies $(OBJS:.o=.c) >/dev/null 2>&1


#
# Install all targets...
#

install:	all
	$(INSTALL_DIR) $(SERVERBIN)/cgi-bin
	for file in $(CGIS); do \
		$(INSTALL_BIN) $$file $(SERVERBIN)/cgi-bin; \
	done


#
# libcgi.a
#

libcgi.a:	$(LIBOBJS)
	echo Archiving $@...
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $(LIBOBJS)
	$(RANLIB) $@


#
# admin.cgi
#

admin.cgi:	admin.o ../Makedefs ../cups/$(LIBCUPS) libcgi.a
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ admin.o libcgi.a $(LIBS)


#
# classes.cgi
#

classes.cgi:	classes.o ../Makedefs ../cups/$(LIBCUPS) libcgi.a
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ classes.o libcgi.a $(LIBS)


#
# jobs.cgi
#

jobs.cgi:	jobs.o ../Makedefs ../cups/$(LIBCUPS) libcgi.a
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ jobs.o libcgi.a $(LIBS)


#
# printers.cgi
#

printers.cgi:	printers.o ../Makedefs ../cups/$(LIBCUPS) libcgi.a
	echo Linking $@...
	$(CC) $(LDFLAGS) -o $@ printers.o libcgi.a $(LIBS)


#
# Dependencies...
#

include Dependencies


#
# End of "$Id: Makefile,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $".
#
