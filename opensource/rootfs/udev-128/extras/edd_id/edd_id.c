/*
 * edd_id - naming of BIOS disk devices via EDD
 *
 * Copyright (C) 2005 John Hull <John_Hull@Dell.com>
 * Copyright (C) 2005 Kay Sievers <kay.sievers@vrfy.org>
 *
 *	This program is free software; you can redistribute it and/or modify it
 *	under the terms of the GNU General Public License as published by the
 *	Free Software Foundation version 2 of the License.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <stdint.h>

#include "../../udev/udev.h"

static void log_fn(struct udev *udev, int priority,
		   const char *file, int line, const char *fn,
		   const char *format, va_list args)
{
	vsyslog(priority, format, args);
}

int main(int argc, char *argv[])
{
	struct udev *udev;
	const char *node = NULL;
	int i;
	int export = 0;
	uint32_t disk_id;
	uint16_t mbr_valid;
	struct dirent *dent;
	int disk_fd;
	int sysfs_fd;
	DIR *dir = NULL;
	int rc = 1;
	char match[NAME_MAX] = "";

		udev = udev_new();
	if (udev == NULL)
		goto exit;

	logging_init("edd_id");
	udev_set_log_fn(udev, log_fn);

	for (i = 1 ; i < argc; i++) {
		char *arg = argv[i];

		if (strcmp(arg, "--export") == 0) {
			export = 1;
		} else
			node = arg;
	}
	if (node == NULL) {
		err(udev, "no node specified\n");
		fprintf(stderr, "no node specified\n");
		goto exit;
	}

	/* check for kernel support */
	dir = opendir("/sys/firmware/edd");
	if (dir == NULL) {
		info(udev, "no kernel EDD support\n");
		fprintf(stderr, "no kernel EDD support\n");
		rc = 2;
		goto exit;
	}

	disk_fd = open(node, O_RDONLY);
	if (disk_fd < 0) {
		info(udev, "unable to open '%s'\n", node);
		fprintf(stderr, "unable to open '%s'\n", node);
		rc = 3;
		goto closedir;
	}

	/* check for valid MBR signature */
	if (lseek(disk_fd, 510, SEEK_SET) < 0) {
		info(udev, "seek to MBR validity failed '%s'\n", node);
		rc = 4;
		goto close;
	}
	if (read(disk_fd, &mbr_valid, sizeof(mbr_valid)) != sizeof(mbr_valid)) {
		info(udev, "read MBR validity failed '%s'\n", node);
		rc = 5;
		goto close;
	}
	if (mbr_valid != 0xAA55) {
		fprintf(stderr, "no valid MBR signature '%s'\n", node);
		info(udev, "no valid MBR signature '%s'\n", node);
		rc=6;
		goto close;
	}

	/* read EDD signature */
	if (lseek(disk_fd, 440, SEEK_SET) < 0) {
		info(udev, "seek to signature failed '%s'\n", node);
		rc = 7;
		goto close;
	}
	if (read(disk_fd, &disk_id, sizeof(disk_id)) != sizeof(disk_id)) {
		info(udev, "read signature failed '%s'\n", node);
		rc = 8;
		goto close;
	}
	/* all zero is invalid */
	info(udev, "read id 0x%08x from '%s'\n", disk_id, node);
	if (disk_id == 0) {
		fprintf(stderr, "no EDD signature '%s'\n", node);
		info(udev, "'%s' signature is zero\n", node);
		rc = 9;
		goto close;
	}

	/* lookup signature in sysfs to determine the name */
	for (dent = readdir(dir); dent != NULL; dent = readdir(dir)) {
		char file[UTIL_PATH_SIZE];
		char sysfs_id_buf[256];
		uint32_t sysfs_id;
		ssize_t size;

		if (dent->d_name[0] == '.')
			continue;

		snprintf(file, sizeof(file), "/sys/firmware/edd/%s/mbr_signature", dent->d_name);
		file[sizeof(file)-1] = '\0';

		sysfs_fd = open(file, O_RDONLY);
		if (sysfs_fd < 0) {
			info(udev, "unable to open sysfs '%s'\n", file);
			continue;
		}

		size = read(sysfs_fd, sysfs_id_buf, sizeof(sysfs_id_buf)-1);
		close(sysfs_fd);
		if (size <= 0) {
			info(udev, "read sysfs '%s' failed\n", file);
			continue;
		}
		sysfs_id_buf[size] = '\0';
		info(udev, "read '%s' from '%s'\n", sysfs_id_buf, file);
		sysfs_id = strtoul(sysfs_id_buf, NULL, 16);

		/* look for matching value, that appears only once */
		if (disk_id == sysfs_id) {
			if (match[0] == '\0') {
				/* store id */
				util_strlcpy(match, dent->d_name, sizeof(match));
			} else {
				/* error, same signature for another device */
				info(udev, "'%s' does not have a unique signature\n", node);
				fprintf(stderr, "'%s' does not have a unique signature\n", node);
				rc = 10;
				goto exit;
			}
		}
	}

	if (match[0] != '\0') {
		if (export)
			printf("ID_EDD=%s\n", match);
		else
			printf("%s\n", match);
		rc = 0;
	}

close:
	close(disk_fd);
closedir:
	closedir(dir);
exit:
	udev_unref(udev);
	logging_close();
	return rc;
}
