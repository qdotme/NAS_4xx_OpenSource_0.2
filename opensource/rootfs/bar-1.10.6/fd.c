#include "config.h"

#include "headers.h"
#include "types.h"
#include "error.h"
#include "fd.h"

int set_fl(int fd, int flags)
{
	int val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		print_error(stderr, "fcntl() failed");
		return(1);
	}
	val |= flags;
	if (fcntl(fd, F_SETFL, val) < 0) {
		print_error(stderr, "fcntl() failed");
		return(1);
	}
	return(0);
}

int clr_fl(int fd, int flags)
{
	int val;

	if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
		print_error(stderr, "fcntl() failed");
		return(1);
	}
	val &= ~flags;
	if (fcntl(fd, F_SETFL, val) < 0) {
		print_error(stderr, "fcntl() failed");
		return(1);
	}
	return(0);
}

int fdBegin(int fd)
{
	return(set_fl(fd, O_NONBLOCK));
}

int fdEnd(int fd)
{
	return(clr_fl(fd, O_NONBLOCK));
}

int fdIsFile(int fd)
{
	struct stat st;

	if (fstat(fd, &st) != 0) {
		print_error(stderr, "fstat() failed");
		return(0);
	}
	if (S_ISREG(st.st_mode)) return(1);
	return(0);
}

int fdFileSize(int fd, uint64 *size)
{
	struct stat st;

	if (size == 0) {
		print_error(stderr, "size == NULL");
		return(1);
	}
	if (fstat(fd, &st) != 0) {
		print_error(stderr, "fstat() failed");
		return(1);
	}
	*size = (uint64)st.st_size;
	return(0);
}

