/*
 * volume_id - reads filesystem label and uuid
 *
 * Copyright (C) 2005 Kay Sievers <kay.sievers@vrfy.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "libvolume_id.h"
#include "libvolume_id-private.h"

struct hpfs_super
{
	uint8_t		magic[4];
	uint8_t		version;
} PACKED;

#define HPFS_SUPERBLOCK_OFFSET			0x2000

int volume_id_probe_hpfs(struct volume_id *id, uint64_t off, uint64_t size)
{
	struct hpfs_super *hs;

	info("probing at offset 0x%llx\n", (unsigned long long) off);

	hs = (struct hpfs_super *) volume_id_get_buffer(id, off + HPFS_SUPERBLOCK_OFFSET, 0x200);
	if (hs == NULL)
		return -1;

	if (memcmp(hs->magic, "\x49\xe8\x95\xf9", 4) == 0) {
		sprintf(id->type_version, "%u", hs->version);

		volume_id_set_usage(id, VOLUME_ID_FILESYSTEM);
		id->type = "hpfs";
		return 0;
	}

	return -1;
}
