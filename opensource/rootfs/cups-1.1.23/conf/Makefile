#
# "$Id: Makefile,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $"
#
#   Configuration file makefile for the Common UNIX Printing System (CUPS).
#
#   Copyright 1993-2005 by Easy Software Products.
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

#
# Config files...
#

KEEP	=	classes.conf client.conf cupsd.conf printers.conf
REPLACE	=	mime.convs mime.types


#
# Make everything...
#

all:


#
# Clean all config and object files...
#

clean:


#
# Install files...
#

install:
	$(INSTALL_DIR) $(SERVERROOT)
	for file in $(KEEP); do \
		if test -r $(SERVERROOT)/$$file ; then \
			$(INSTALL_DATA) $$file $(SERVERROOT)/$$file.N ; \
		else \
			$(INSTALL_DATA) $$file $(SERVERROOT) ; \
		fi ; \
	done
	for file in $(REPLACE); do \
		if test -r $(SERVERROOT)/$$file ; then \
			$(MV) $(SERVERROOT)/$$file $(SERVERROOT)/$$file.O ; \
		fi ; \
		$(INSTALL_DATA) $$file $(SERVERROOT) ; \
	done
	-if test x$(PAMDIR) != x$(BUILDROOT); then \
		$(INSTALL_DIR) $(PAMDIR); \
		$(INSTALL_DATA) $(PAMFILE) $(PAMDIR)/cups; \
	fi


#
# End of "$Id: Makefile,v 1.1.1.1.12.1 2009/02/03 08:27:04 wiley Exp $".
#
