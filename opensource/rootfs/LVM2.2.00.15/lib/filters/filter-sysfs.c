/*
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

#include "lib.h"
#include "filter-sysfs.h"
#include "lvm-string.h"
#include "pool.h"

#ifdef linux

#include <dirent.h>

static int _locate_sysfs_blocks(const char *proc, char *path, size_t len)
{
	char proc_mounts[PATH_MAX];
	int r = 0;
	FILE *fp;
	char *split[4], buffer[PATH_MAX + 16];

	if (!*proc) {
		log_verbose("No proc filesystem found: skipping sysfs filter");
		return 0;
	}
		
	if (lvm_snprintf(proc_mounts, sizeof(proc_mounts),
			 "%s/mounts", proc) < 0) {
		log_error("Failed to create /proc/mounts string");
		return 0;
	}

	if (!(fp = fopen(proc_mounts, "r"))) {
		log_sys_error("fopen %s", proc_mounts);
		return 0;
	}

	while (fgets(buffer, sizeof(buffer), fp)) {
		if (split_words(buffer, 4, split) == 4 &&
		    !strcmp(split[2], "sysfs")) {
			if (lvm_snprintf(path, len, "%s/%s", split[1],
					 "block") >= 0) {
				r = 1;
			}
			break;
		}
	}

	fclose(fp);
	return r;
}

/*----------------------------------------------------------------
 * We need to store a set of dev_t.
 *--------------------------------------------------------------*/
struct entry {
	struct entry *next;
	dev_t dev;
};

#define SET_BUCKETS 64
struct dev_set {
	struct pool *mem;
	const char *sys_block;
	int initialised;
	struct entry *slots[SET_BUCKETS];
};

static struct dev_set *_dev_set_create(struct pool *mem, const char *sys_block)
{
	struct dev_set *ds;

	if (!(ds = pool_zalloc(mem, sizeof(*ds))))
		return NULL;

	ds->mem = mem;
	ds->sys_block = pool_strdup(mem, sys_block);
	ds->initialised = 0;

	return ds;
}

static inline unsigned _hash_dev(dev_t dev)
{
	return (major(dev) ^ minor(dev)) & (SET_BUCKETS - 1);
}

/*
 * Doesn't check that the set already contains dev.
 */
static int _set_insert(struct dev_set *ds, dev_t dev)
{
	struct entry *e;
	unsigned h = _hash_dev(dev);

	if (!(e = pool_alloc(ds->mem, sizeof(*e))))
		return 0;

	e->next = ds->slots[h];
	e->dev = dev;
	ds->slots[h] = e;

	return 1;
}

static int _set_lookup(struct dev_set *ds, dev_t dev)
{
	unsigned h = _hash_dev(dev);
	struct entry *e;

	for (e = ds->slots[h]; e; e = e->next)
		if (e->dev == dev)
			return 1;

	return 0;
}

/*----------------------------------------------------------------
 * filter methods
 *--------------------------------------------------------------*/
static int _parse_dev(const char *file, FILE *fp, dev_t *result)
{
	unsigned major, minor;
	char buffer[64];

	if (!fgets(buffer, sizeof(buffer), fp)) {
		log_error("Empty sysfs device file: %s", file);
		return 0;
	}

	if (sscanf(buffer, "%u:%u", &major, &minor) != 2) {
		log_info("sysfs device file not correct format");
		return 0;
	}

	*result = makedev(major, minor);
	return 1;
}

static int _read_dev(const char *file, dev_t *result)
{
	int r;
	FILE *fp;

	if (!(fp = fopen(file, "r"))) {
		log_sys_error("fopen", file);
		return 0;
	}

	r = _parse_dev(file, fp, result);
	fclose(fp);

	return r;
}

/*
 * Recurse through sysfs directories, inserting any devs found.
 */
static int _read_devs(struct dev_set *ds, const char *dir)
{
        struct dirent *d;
        DIR *dr;
	char path[PATH_MAX];
	dev_t dev;
	int r = 1;

        if (!(dr = opendir(dir))) {
                log_sys_error("opendir", dir);
                return 0;
        }

        while ((d = readdir(dr))) {
                if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
			continue;

		if (lvm_snprintf(path, sizeof(path), "%s/%s", dir,
				 d->d_name) < 0) {
			log_error("sysfs path name too long: %s in %s",
				  d->d_name, dir);
			continue;
		}

		if (d->d_type == DT_DIR) {
			if (!_read_devs(ds, path)) {
				r = 0;
				break;
			}
		}

		if ((d->d_type == DT_REG && !strcmp(d->d_name, "dev")))
			if (!_read_dev(path, &dev) || !_set_insert(ds, dev)) {
				r = 0;
				break;
			}

	}

        if (closedir(dr))
                log_sys_error("closedir", dir);

	return r;
}

static int _init_devs(struct dev_set *ds)
{
	if (!_read_devs(ds, ds->sys_block)) {
		ds->initialised = -1;
		return 0;
	}

	ds->initialised = 1;

	return 1;
}


static int _accept_p(struct dev_filter *f, struct device *dev)
{
	struct dev_set *ds = (struct dev_set *) f->private;

	if (!ds->initialised)
		_init_devs(ds);

	/* Pass through if initialisation failed */
	if (ds->initialised != 1)
		return 1;

	return _set_lookup(ds, dev->dev);
}

static void _destroy(struct dev_filter *f)
{
	struct dev_set *ds = (struct dev_set *) f->private;
	pool_destroy(ds->mem);
}

struct dev_filter *sysfs_filter_create(const char *proc)
{
	char sys_block[PATH_MAX];
	struct pool *mem;
	struct dev_set *ds;
	struct dev_filter *f;

	if (!_locate_sysfs_blocks(proc, sys_block, sizeof(sys_block)))
		return NULL;

	if (!(mem = pool_create(256))) {
		log_error("sysfs pool creation failed");
		return NULL;
	}

	if (!(ds = _dev_set_create(mem, sys_block))) {
		log_error("sysfs dev_set creation failed");
		goto bad;
	}

	if (!(f = pool_zalloc(mem, sizeof(*f)))) {
		stack;
		goto bad;
	}

	f->passes_filter = _accept_p;
	f->destroy = _destroy;
	f->private = ds;
	return f;

 bad:
	pool_destroy(mem);
	return NULL;
}

#else

struct dev_filter *sysfs_filter_create(const char *proc)
{
	return NULL;
}

#endif
