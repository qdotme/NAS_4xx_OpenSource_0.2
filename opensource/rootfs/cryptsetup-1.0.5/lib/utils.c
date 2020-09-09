#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <errno.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "libcryptsetup.h"
#include "internal.h"


struct safe_allocation {
	size_t	size;
	char	data[1];
};

static char *error;

void set_error_va(const char *fmt, va_list va)
{
	int bufsize;

	bufsize = fmt ? (strlen(fmt) + 1) : 0;
	if (bufsize < 128)
		bufsize = 128;

	if (error)
		free(error);
	if (!fmt) {
		error = NULL;
		return;
	}

	error = malloc(bufsize);

	for(;;) {
		int n;

		n = vsnprintf(error, bufsize, fmt, va);

		if (n >= 0 && n < bufsize)
			break;

		if (n >= 0)
			bufsize = n + 1;
		else
			bufsize *= 2;

		error = realloc(error, bufsize);
	}
}

void set_error(const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	set_error_va(fmt, va);
	va_end(va);
}

const char *get_error(void)
{
	return error;
}

void *safe_alloc(size_t size)
{
	struct safe_allocation *alloc;

	if (!size)
		return NULL;

	alloc = malloc(size + offsetof(struct safe_allocation, data));
	if (!alloc)
		return NULL;

	alloc->size = size;

	return &alloc->data;
}

void safe_free(void *data)
{
	struct safe_allocation *alloc;

	if (!data)
		return;

	alloc = data - offsetof(struct safe_allocation, data);

	memset(data, 0, alloc->size);

	alloc->size = 0x55aa55aa;
	free(alloc);
}

void *safe_realloc(void *data, size_t size)
{
	void *new_data;

	new_data = safe_alloc(size);

	if (new_data && data) {
		struct safe_allocation *alloc;

		alloc = data - offsetof(struct safe_allocation, data);

		if (size > alloc->size)
			size = alloc->size;

		memcpy(new_data, data, size);
	}

	safe_free(data);
	return new_data;
}

char *safe_strdup(const char *s)
{
	char *s2 = safe_alloc(strlen(s) + 1);

	if (!s2)
		return NULL;

	return strcpy(s2, s);
}

/* Credits go to Michal's padlock patches for this alignment code */

static void *aligned_malloc(char **base, int size, int alignment) 
{
	char *ptr;

	ptr  = malloc(size + alignment);
	if(ptr == NULL) return NULL;

	*base = ptr;
	if(alignment > 1 && ((long)ptr & (alignment - 1))) {
		ptr += alignment - ((long)(ptr) & (alignment - 1));
	}
	return ptr;
}

static int sector_size(int fd) 
{
	int bsize;
	if (ioctl(fd,BLKSSZGET, &bsize) < 0)
		return -EINVAL;
	else
		return bsize;
}

int sector_size_for_device(const char *device)
{
	int fd = open(device, O_RDONLY);
	int r;
	if(fd < 0)
		return -EINVAL;
	r = sector_size(fd);
	close(fd);
	return r;
}

ssize_t write_blockwise(int fd, const void *orig_buf, size_t count) 
{
	char *padbuf; char *padbuf_base;
	char *buf = (char *)orig_buf;
	int r;
	int hangover; int solid; int bsize;

	if ((bsize = sector_size(fd)) < 0)
		return bsize;

	hangover = count % bsize;
	solid = count - hangover;

	padbuf = aligned_malloc(&padbuf_base, bsize, bsize);
	if(padbuf == NULL) return -ENOMEM;

	while(solid) {
		memcpy(padbuf, buf, bsize);
		r = write(fd, padbuf, bsize);
		if(r < 0 || r != bsize) goto out;

		solid -= bsize;
		buf += bsize;
	}
	if(hangover) {
		r = read(fd,padbuf,bsize);
		if(r < 0 || r != bsize) goto out;

		lseek(fd,-bsize,SEEK_CUR);
		memcpy(padbuf,buf,hangover);

		r = write(fd,padbuf, bsize);
		if(r < 0 || r != bsize) goto out;
		buf += hangover;
	}
 out:
	free(padbuf_base);
	return (buf-(char *)orig_buf)?(buf-(char *)orig_buf):r;

}

ssize_t read_blockwise(int fd, void *orig_buf, size_t count) {
	char *padbuf; char *padbuf_base;
	char *buf = (char *)orig_buf;
	int r;
	int step;
	int bsize;

	if ((bsize = sector_size(fd)) < 0)
		return bsize;

	padbuf = aligned_malloc(&padbuf_base, bsize, bsize);
	if(padbuf == NULL) return -ENOMEM;

	while(count) {
		r = read(fd,padbuf,bsize);
		if(r < 0 || r != bsize) {
			fprintf(stderr, "read failed in read_blockwise.\n");
			goto out;
		}
		step = count<bsize?count:bsize;
		memcpy(buf,padbuf,step);
		buf += step;
		count -= step;
	}
 out:
	free(padbuf_base); 
	return (buf-(char *)orig_buf)?(buf-(char *)orig_buf):r;
}

/* 
 * Combines llseek with blockwise write. write_blockwise can already deal with short writes
 * but we also need a function to deal with short writes at the start. But this information
 * is implicitly included in the read/write offset, which can not be set to non-aligned 
 * boundaries. Hence, we combine llseek with write.
 */
   
ssize_t write_lseek_blockwise(int fd, const char *buf, size_t count, off_t offset) {
	int bsize = sector_size(fd);
	const char *orig_buf = buf;
	char frontPadBuf[bsize];
	int frontHang = offset % bsize;
	int r;

	if (bsize < 0)
		return bsize;

	lseek(fd, offset - frontHang, SEEK_SET);
	if(offset % bsize) {
		int innerCount = count<bsize?count:bsize;

		r = read(fd,frontPadBuf,bsize);
		if(r < 0) return -1;

		memcpy(frontPadBuf+frontHang, buf, innerCount);

		r = write(fd,frontPadBuf,bsize);
		if(r < 0) return -1;

		buf += innerCount;
		count -= innerCount;
	}
	if(count <= 0) return buf - orig_buf;

	return write_blockwise(fd, buf, count);
}
