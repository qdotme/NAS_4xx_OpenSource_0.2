#
# "$Id: Makefile,v 1.1.1.1.12.1 2009/02/03 08:27:06 wiley Exp $"
#
#   PHP Makefile for the Common UNIX Printing System (CUPS).
#
#   Copyright 1997-2003 by Easy Software Products, all rights reserved.
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
#       Hollywood, Maryland 20636-3111 USA
#
#       Voice: (301) 373-9603
#       EMail: cups-info@cups.org
#         WWW: http://www.cups.org
#

include ../../Makedefs


#
# Where to install the PHP module...
#

PHPDIR	=	`php-config --extension-dir`
OPTIONS	=	`php-config --includes` -I../.. -DCOMPILE_DL_PHPCUPS


#
# Object files...
#

OBJS	=	phpcups.o


#
# Targets in this directory...
#

TARGETS	=	phpcups.so


#
# Make all targets...
#

all:	$(TARGETS)


#
# Remove object and target files...
#

clean:
	$(RM) $(OBJS) $(TARGETS)


#
# Update dependencies (without system header dependencies...)
#

depend:
	makedepend -Y -I../.. -fDependencies $(OBJS:.o=.c) >/dev/null 2>&1


#
# Install object and target files...
#

install:	all
	echo Installing $(TARGETS) in $(PHPDIR)
	$(INSTALL_DIR) $(PHPDIR)
	cp $(TARGETS) $(PHPDIR)


#
# phpcups.so
#

phpcups.so:	$(OBJS) ../../Makedefs
	echo Linking $@...
	$(DSO) $(DSOFLAGS) -o $@ $(OBJS) -L../../cups -lcups


#
# Dependencies...
#

include Dependencies


#
# End of "$Id: Makefile,v 1.1.1.1.12.1 2009/02/03 08:27:06 wiley Exp $".
#
