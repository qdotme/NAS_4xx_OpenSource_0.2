#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <libdevmapper.h>

#include "libcryptsetup.h"
#include "internal.h"

#define DEVICE_DIR	"/dev"

#define	CRYPT_TARGET	"crypt"

static void set_dm_error(int level, const char *file, int line,
                         const char *f, ...)
{
	va_list va;

	if (level > 3)
		return;

	va_start(va, f);
	set_error_va(f, va);
	va_end(va);
}

static int dm_init(void)
{
	dm_log_init(set_dm_error);
	return 1;	/* unsafe memory */
}

static void dm_exit(void)
{
	dm_log_init(NULL);
	dm_lib_release();
}

static void flush_dm_workqueue(void)
{
	/* 
	 * Unfortunately this is the only way to trigger libdevmapper's
	 * update_nodes function 
	 */ 
	dm_exit(); 
	dm_init();
}

static char *__lookup_dev(char *path, dev_t dev)
{
	struct dirent *entry;
	struct stat st;
	char *ptr;
	char *result = NULL;
	DIR *dir;
	int space;

	path[PATH_MAX - 1] = '\0';
	ptr = path + strlen(path);
	*ptr++ = '/';
	*ptr = '\0';
	space = PATH_MAX - (ptr - path);

	dir = opendir(path);
	if (!dir)
		return NULL;

	while((entry = readdir(dir))) {
		if (entry->d_name[0] == '.' &&
		    (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' &&
		                                  entry->d_name[2] == '\0')))
			continue;

		strncpy(ptr, entry->d_name, space);
		if (lstat(path, &st) < 0)
			continue;

		if (S_ISDIR(st.st_mode)) {
			result = __lookup_dev(path, dev);
			if (result)
				break;
		} else if (S_ISBLK(st.st_mode)) {
			if (st.st_rdev == dev) {
				result = strdup(path);
				break;
			}
		}
	}

	closedir(dir);

	return result;
}

static char *lookup_dev(const char *dev)
{
	uint32_t major, minor;
	char buf[PATH_MAX + 1];

	if (sscanf(dev, "%" PRIu32 ":%" PRIu32, &major, &minor) != 2)
		return NULL;

	strncpy(buf, DEVICE_DIR, PATH_MAX);
	buf[PATH_MAX] = '\0';

	return __lookup_dev(buf, makedev(major, minor));
}

static char *get_params(struct crypt_options *options, const char *key)
{
	char *params;
	char *hexkey;
	int i;

	hexkey = safe_alloc(options->key_size * 2 + 1);
	if (!hexkey) {
		set_error("Memory allocation problem");
		return NULL;
	}

	for(i = 0; i < options->key_size; i++)
		sprintf(&hexkey[i * 2], "%02x", (unsigned char)key[i]);

	params = safe_alloc(strlen(hexkey) + strlen(options->cipher) +
	                    strlen(options->device) + 64);
	if (!params) {
		set_error("Memory allocation problem");
		goto out;
	}

	sprintf(params, "%s %s %" PRIu64 " %s %" PRIu64,
	        options->cipher, hexkey, options->skip,
	        options->device, options->offset);

out:
	safe_free(hexkey);

	return params;
}

static int dm_create_device(int reload, struct crypt_options *options,
                            const char *key)
{
	struct dm_task *dmt = NULL;
	struct dm_task *dmt_query = NULL;
	struct dm_info dmi;
	char *params = NULL;
	int r = -EINVAL;

	params = get_params(options, key);
	if (!params)
		goto out_no_removal;
	if (!(dmt = dm_task_create(reload ? DM_DEVICE_RELOAD
	                                  : DM_DEVICE_CREATE)))
		goto out;
	if (!dm_task_set_name(dmt, options->name))
		goto out;
	if (options->flags & CRYPT_FLAG_READONLY && !dm_task_set_ro(dmt))
                goto out;
	if (!dm_task_add_target(dmt, 0, options->size, CRYPT_TARGET, params))
		goto out;
	if (!dm_task_run(dmt))
		goto out;

	if (reload) {
		dm_task_destroy(dmt);
		if (!(dmt = dm_task_create(DM_DEVICE_RESUME)))
			goto out;
		if (!dm_task_set_name(dmt, options->name))
			goto out;
		if (!dm_task_run(dmt))
			goto out;
	}

	if (!dm_task_get_info(dmt, &dmi))
		goto out;
	if (dmi.read_only)
		options->flags |= CRYPT_FLAG_READONLY;

	r = 0;
	
out:
	if (r < 0 && !reload) {
		char *error = (char *)get_error();
		if (error)
			error = strdup(error);
		if (dmt)
			dm_task_destroy(dmt);

		if (!(dmt = dm_task_create(DM_DEVICE_REMOVE)))
			goto out_restore_error;
		if (!dm_task_set_name(dmt, options->name))
			goto out_restore_error;
		if (!dm_task_run(dmt))
			goto out_restore_error;

out_restore_error:
		set_error("%s", error);
		if (error)
			free(error);
	}

out_no_removal:
	if (params)
		safe_free(params);
	if (dmt)
		dm_task_destroy(dmt);
	if(dmt_query)
		dm_task_destroy(dmt_query);
	flush_dm_workqueue();
	return r;
}

static int dm_query_device(int details, struct crypt_options *options,
                           char **key)
{
	struct dm_task *dmt;
	struct dm_info dmi;
	uint64_t start, length;
	char *target_type, *params;
	void *next = NULL;
	int r = -EINVAL;

	if (!(dmt = dm_task_create(details ? DM_DEVICE_TABLE
	                                   : DM_DEVICE_STATUS)))
		goto out;
	if (!dm_task_set_name(dmt, options->name))
		goto out;
	r = -ENODEV;
	if (!dm_task_run(dmt))
		goto out;

	r = -EINVAL;
	if (!dm_task_get_info(dmt, &dmi))
		goto out;

	if (!dmi.exists) {
		r = -ENODEV;
		goto out;
	}

	next = dm_get_next_target(dmt, next, &start, &length,
	                          &target_type, &params);
	if (!target_type || strcmp(target_type, CRYPT_TARGET) != 0 ||
	    start != 0 || next)
		goto out;

	options->hash = NULL;
	options->cipher = NULL;
	options->offset = 0;
	options->skip = 0;
	options->size = length;
	if (details) {
		char *cipher, *key_, *device, *tmp;
		uint64_t val64;

		set_error("Invalid dm table");

		cipher = strsep(&params, " ");
		key_ = strsep(&params, " ");
		if (!params)
			goto out;

		val64 = strtoull(params, &params, 10);
		if (*params != ' ')
			goto out;
		params++;
		options->skip = val64;

		device = strsep(&params, " ");
		if (!params)
			goto out;

		val64 = strtoull(params, &params, 10);
		if (*params)
			goto out;
		options->offset = val64;

		options->cipher = strdup(cipher);
		options->key_size = strlen(key_) / 2;
		if (key) {
			char buffer[3];
			char *endp;
			int i;

			*key = safe_alloc(options->key_size);
			if (!*key) {
				set_error("Out of memory");
				r = -ENOMEM;
				goto out;
			}

			buffer[2] = '\0';
			for(i = 0; i < options->key_size; i++) {
				memcpy(buffer, &key_[i * 2], 2);
				(*key)[i] = strtoul(buffer, &endp, 16);
				if (endp != &buffer[2]) {
					safe_free(key);
					*key = NULL;
					goto out;
				}
			}
		}
		memset(key_, 0, strlen(key_));
		options->device = lookup_dev(device);

		set_error(NULL);
	}

	r = (dmi.open_count > 0);

out:
	if (dmt)
		dm_task_destroy(dmt);
	if (r >= 0) {
		if (options->device)
			options->flags |= CRYPT_FLAG_FREE_DEVICE;
		if (options->cipher)
			options->flags |= CRYPT_FLAG_FREE_CIPHER;
		options->flags &= ~CRYPT_FLAG_READONLY;
		if (dmi.read_only)
			options->flags |= CRYPT_FLAG_READONLY;
	} else {
		if (options->device) {
			free((char *)options->device);
			options->device = NULL;
			options->flags &= ~CRYPT_FLAG_FREE_DEVICE;
		}
		if (options->cipher) {
			free((char *)options->cipher);
			options->cipher = NULL;
			options->flags &= ~CRYPT_FLAG_FREE_CIPHER;
		}
	}
	return r;
}

static int dm_remove_device(struct crypt_options *options)
{
	struct dm_task *dmt;
	int r = -EINVAL;

	if (!(dmt = dm_task_create(DM_DEVICE_REMOVE)))
		goto out;
	if (!dm_task_set_name(dmt, options->name))
		goto out;
	if (!dm_task_run(dmt))
		goto out;

	r = 0;

out:	
	if (dmt)
		dm_task_destroy(dmt);
	flush_dm_workqueue();
	return r;
}


static const char *dm_get_dir(void)
{
	return dm_dir();
}

struct setup_backend setup_libdevmapper_backend = {
	.name = "dm-crypt",
	.init = dm_init,
	.exit = dm_exit,
	.create = dm_create_device,
	.status = dm_query_device,
	.remove = dm_remove_device,
	.dir = dm_get_dir
};
