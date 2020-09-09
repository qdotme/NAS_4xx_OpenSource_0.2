#
# (C) Copyright 2002
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

OBJECTS := crc32.o  fw_env.o  fw_env_main.o
SOURCES := crc32.c  fw_env.c  fw_env_main.c
HEADERS := fw_env.h

COMPILER = gcc

CFLAGS := -I/project/seagate/proto/marvell_5182/build-eabi/staging_dir/arm-linux-gnueabi/include -DUSE_HOSTCC

OBJS = $(SOURCES:.c=.o)

all:	fw_printenv


fw_printenv:	$(OBJS) $(HEADERS)
	gcc -Wall $(CFLAGS) $(OBJS) -o fw_printenv
	gcc -Wall $(CFLAGS) $(OBJS) -o fw_setenv

clean:
	rm -f *.o
	rm -f fw_printenv 

#########################################################################

#########################################################################
