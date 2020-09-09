/*
 * Copyright (C) 2001-2004 Sistina Software, Inc. All rights reserved.  
 * Copyright (C) 2004 Red Hat, Inc. All rights reserved.
 *
 * This file is part of LVM2.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LVM_DEVICE_H
#define _LVM_DEVICE_H

#include "uuid.h"
#include <fcntl.h>

#define DEV_ACCESSED_W		0x00000001	/* Device written to? */
#define DEV_REGULAR		0x00000002	/* Regular file? */
#define DEV_ALLOCED		0x00000004	/* dbg_malloc used */

/*
 * All devices in LVM will be represented by one of these.
 * pointer comparisons are valid.
 */
struct device {
	struct list aliases;	/* struct str_list from lvm-types.h */
	dev_t dev;

	/* private */
	int fd;
	int open_count;
	uint32_t flags;
	uint64_t end;
	struct list open_list;

	char pvid[ID_LEN + 1];
};

struct device_list {
	struct list list;
	struct device *dev;
};

struct device_area {
	struct device *dev;
	uint64_t start;		/* Bytes */
	uint64_t size;		/* Bytes */
};

/*
 * All io should use these routines.
 */
int dev_get_size(struct device *dev, uint64_t *size);
int dev_get_sectsize(struct device *dev, uint32_t *size);

/* Use quiet version if device number could change e.g. when opening LV */
int dev_open(struct device *dev);
int dev_open_quiet(struct device *dev);
int dev_open_flags(struct device *dev, int flags, int append, int quiet);
int dev_close(struct device *dev);
int dev_close_immediate(struct device *dev);
void dev_close_all(void);

static inline int dev_fd(struct device *dev)
{
	return dev->fd;
}

int dev_read(struct device *dev, uint64_t offset, size_t len, void *buffer);
int dev_write(struct device *dev, uint64_t offset, size_t len, void *buffer);
int dev_append(struct device *dev, size_t len, void *buffer);
int dev_zero(struct device *dev, uint64_t offset, size_t len);
void dev_flush(struct device *dev);

struct device *dev_create_file(const char *filename, struct device *dev,
			       struct str_list *alias);

static inline const char *dev_name(const struct device *dev)
{
	return (dev) ? list_item(dev->aliases.n, struct str_list)->str :
	    "unknown device";
}

/* Return a valid device name from the alias list; NULL otherwise */
const char *dev_name_confirmed(struct device *dev, int quiet);

/* FIXME Check partition type if appropriate */

#define is_lvm_partition(a) 1

/*
static inline int is_lvm_partition(const char *name)
{
	return 1;
}
*/

#endif
