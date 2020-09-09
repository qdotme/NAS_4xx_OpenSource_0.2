#!/bin/bash

#
# Build U-Boot image when `mkimage' tool is available.
#

MKIMAGE=$(type -path "${CROSS_COMPILE}mkimage")

if [ -z "${MKIMAGE}" ]; then
	MKIMAGE=/opt/devel/proto/marvell/build-eabi/u-boot-3.4.4/tools/mkimage
	if [ -z "${MKIMAGE}" ]; then
		# Doesn't exist
		echo '"mkimage" command not found - U-Boot images will not be built' >&2
		exit 0;
	fi
fi

# Call "mkimage" to create U-Boot image
${MKIMAGE} "$@"
