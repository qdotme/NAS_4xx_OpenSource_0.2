/*
 * sysfs - extract md related information from sysfs.  Part of:
 * mdadm - manage Linux "md" devices aka RAID arrays.
 *
 * Copyright (C) 2006 Neil Brown <neilb@suse.de>
 *
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    Author: Neil Brown
 *    Email: <neilb@suse.de>
 */

#include	"mdadm.h"
#include	<dirent.h>

int load_sys(char *path, char *buf)
{
	int fd = open(path, O_RDONLY);
	int n;
	if (fd < 0)
		return -1;
	n = read(fd, buf, 1024);
	close(fd);
	if (n <=0 || n >= 1024)
		return -1;
	buf[n] = 0;
	if (buf[n-1] == '\n')
		buf[n-1] = 0;
	return 0;
}

void sysfs_free(struct mdinfo *sra)
{
	while (sra) {
		struct mdinfo *sra2 = sra->next;
		while (sra->devs) {
			struct mdinfo *d = sra->devs;
			sra->devs = d->next;
			free(d);
		}
		free(sra);
		sra = sra2;
	}
}

struct mdinfo *sysfs_read(int fd, int devnum, unsigned long options)
{
	/* Longest possible name in sysfs, mounted at /sys, is
	 *  /sys/block/md_dXXX/md/dev-XXXXX/block/dev
	 *  /sys/block/md_dXXX/md/metadata_version
	 * which is about 41 characters.  50 should do for now
	 */
	char fname[50];
	char buf[1024];
	char *base;
	char *dbase;
	struct mdinfo *sra;
	struct mdinfo *dev;
	DIR *dir;
	struct dirent *de;

	sra = malloc(sizeof(*sra));
	if (sra == NULL)
		return sra;
	sra->next = NULL;

	if (fd >= 0) {
		struct stat stb;
		mdu_version_t vers;
 		if (fstat(fd, &stb)) return NULL;
		if (ioctl(fd, RAID_VERSION, &vers) != 0)
			return NULL;
		if (major(stb.st_rdev) == MD_MAJOR)
			sprintf(sra->sys_name, "md%d", (int)minor(stb.st_rdev));
		else if (major(stb.st_rdev) == get_mdp_major())
			sprintf(sra->sys_name, "md_d%d",
				(int)minor(stb.st_rdev)>>MdpMinorShift);
		else {
			/* must be an extended-minor partition. Look at the
			 * /sys/dev/block/%d:%d link which must look like
			 * ../../block/mdXXX/mdXXXpYY
			 */
			char path[30];
			char link[200];
			char *cp;
			int n;
			sprintf(path, "/sys/dev/block/%d:%d", major(stb.st_rdev),
				minor(stb.st_rdev));
			n = readlink(path, link, sizeof(link)-1);
			if (n <= 0)
				return NULL;
			link[n] = 0;
			cp = strrchr(link, '/');
			if (cp) *cp = 0;
			cp = strchr(link, '/');
			if (cp && strncmp(cp, "/md", 3) == 0)
				strcpy(sra->sys_name, cp+1);
			else
				return NULL;
		}
	} else {
		if (devnum >= 0)
			sprintf(sra->sys_name, "md%d", devnum);
		else
			sprintf(sra->sys_name, "md_d%d",
				-1-devnum);
	}
	sprintf(fname, "/sys/block/%s/md/", sra->sys_name);
	base = fname + strlen(fname);

	sra->devs = NULL;
	if (options & GET_VERSION) {
		strcpy(base, "metadata_version");
		if (load_sys(fname, buf))
			goto abort;
		if (strncmp(buf, "none", 4) == 0) {
			sra->array.major_version =
				sra->array.minor_version = -1;
			strcpy(sra->text_version, "");
		} else if (strncmp(buf, "external:", 9) == 0) {
			sra->array.major_version = -1;
			sra->array.minor_version = -2;
			strcpy(sra->text_version, buf+9);
		} else
			sscanf(buf, "%d.%d",
			       &sra->array.major_version,
			       &sra->array.minor_version);
	}
	if (options & GET_LEVEL) {
		strcpy(base, "level");
		if (load_sys(fname, buf))
			goto abort;
		sra->array.level = map_name(pers, buf);
	}
	if (options & GET_LAYOUT) {
		strcpy(base, "layout");
		if (load_sys(fname, buf))
			goto abort;
		sra->array.layout = strtoul(buf, NULL, 0);
	}
	if (options & GET_COMPONENT) {
		strcpy(base, "component_size");
		if (load_sys(fname, buf))
			goto abort;
		sra->component_size = strtoull(buf, NULL, 0);
		/* sysfs reports "K", but we want sectors */
		sra->component_size *= 2;
	}
	if (options & GET_CHUNK) {
		strcpy(base, "chunk_size");
		if (load_sys(fname, buf))
			goto abort;
		sra->array.chunk_size = strtoul(buf, NULL, 0);
	}
	if (options & GET_CACHE) {
		strcpy(base, "stripe_cache_size");
		if (load_sys(fname, buf))
			goto abort;
		sra->cache_size = strtoul(buf, NULL, 0);
	}
	if (options & GET_MISMATCH) {
		strcpy(base, "mismatch_cnt");
		if (load_sys(fname, buf))
			goto abort;
		sra->mismatch_cnt = strtoul(buf, NULL, 0);
	}

	if (! (options & GET_DEVS))
		return sra;

	/* Get all the devices as well */
	*base = 0;
	dir = opendir(fname);
	if (!dir)
		goto abort;
	sra->array.spare_disks = 0;

	while ((de = readdir(dir)) != NULL) {
		char *ep;
		if (de->d_ino == 0 ||
		    strncmp(de->d_name, "dev-", 4) != 0)
			continue;
		strcpy(base, de->d_name);
		dbase = base + strlen(base);
		*dbase++ = '/';

		dev = malloc(sizeof(*dev));
		if (!dev)
			goto abort;
		dev->next = sra->devs;
		sra->devs = dev;
		strcpy(dev->sys_name, de->d_name);

		/* Always get slot, major, minor */
		strcpy(dbase, "slot");
		if (load_sys(fname, buf))
			goto abort;
		dev->disk.raid_disk = strtoul(buf, &ep, 10);
		if (*ep) dev->disk.raid_disk = -1;

		strcpy(dbase, "block/dev");
		if (load_sys(fname, buf))
			goto abort;
		sscanf(buf, "%d:%d", &dev->disk.major, &dev->disk.minor);

		if (options & GET_OFFSET) {
			strcpy(dbase, "offset");
			if (load_sys(fname, buf))
				goto abort;
			dev->data_offset = strtoull(buf, NULL, 0);
		}
		if (options & GET_SIZE) {
			strcpy(dbase, "size");
			if (load_sys(fname, buf))
				goto abort;
			dev->component_size = strtoull(buf, NULL, 0);
		}
		if (options & GET_STATE) {
			dev->disk.state = 0;
			strcpy(dbase, "state");
			if (load_sys(fname, buf))
				goto abort;
			if (strstr(buf, "in_sync"))
				dev->disk.state |= (1<<MD_DISK_SYNC);
			if (strstr(buf, "faulty"))
				dev->disk.state |= (1<<MD_DISK_FAULTY);
			if (dev->disk.state == 0)
				sra->array.spare_disks++;
		}
		if (options & GET_ERROR) {
			strcpy(buf, "errors");
			if (load_sys(fname, buf))
				goto abort;
			dev->errors = strtoul(buf, NULL, 0);
		}
	}
	return sra;

 abort:
	sysfs_free(sra);
	return NULL;
}

unsigned long long get_component_size(int fd)
{
	/* Find out the component size of the array.
	 * We cannot trust GET_ARRAY_INFO ioctl as it's
	 * size field is only 32bits.
	 * So look in /sys/block/mdXXX/md/component_size
	 *
	 * This returns in units of sectors.
	 */
	struct stat stb;
	char fname[50];
	int n;
	if (fstat(fd, &stb)) return 0;
	if (major(stb.st_rdev) != get_mdp_major())
		sprintf(fname, "/sys/block/md%d/md/component_size",
			(int)minor(stb.st_rdev));
	else
		sprintf(fname, "/sys/block/md_d%d/md/component_size",
			(int)minor(stb.st_rdev)>>MdpMinorShift);
	fd = open(fname, O_RDONLY);
	if (fd < 0)
		return 0;
	n = read(fd, fname, sizeof(fname));
	close(fd);
	if (n == sizeof(fname))
		return 0;
	fname[n] = 0;
	return strtoull(fname, NULL, 10) * 2;
}

int sysfs_set_str(struct mdinfo *sra, struct mdinfo *dev,
		  char *name, char *val)
{
	char fname[50];
	int n;
	int fd;
	sprintf(fname, "/sys/block/%s/md/%s/%s",
		sra->sys_name, dev?dev->sys_name:"", name);
	fd = open(fname, O_WRONLY);
	if (fd < 0)
		return -1;
	n = write(fd, val, strlen(val));
	close(fd);
	if (n != strlen(val))
		return -1;
	return 0;
}

int sysfs_set_num(struct mdinfo *sra, struct mdinfo *dev,
		  char *name, unsigned long long val)
{
	char valstr[50];
	sprintf(valstr, "%llu", val);
	return sysfs_set_str(sra, dev, name, valstr);
}

int sysfs_get_ll(struct mdinfo *sra, struct mdinfo *dev,
		       char *name, unsigned long long *val)
{
	char fname[50];
	char buf[50];
	int n;
	int fd;
	char *ep;
	sprintf(fname, "/sys/block/%s/md/%s/%s",
		sra->sys_name, dev?dev->sys_name:"", name);
	fd = open(fname, O_RDONLY);
	if (fd < 0)
		return -1;
	n = read(fd, buf, sizeof(buf));
	close(fd);
	if (n <= 0)
		return -1;
	buf[n] = 0;
	*val = strtoull(buf, &ep, 0);
	if (ep == buf || (*ep != 0 && *ep != '\n' && *ep != ' '))
		return -1;
	return 0;
}
