/*
 *	Random supply helper
 * Copyright 2004, Clemens Fruhwirth <clemens@endorphin.org>
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int getRandom(char *buf, size_t len)
{
	int randomfd, r = 0, index = 0;

	//FIXME Run a FIPS test for the random device or include
	// PRNG if urandom not avail.
	
	randomfd = open("/dev/urandom", O_RDONLY);
	if(-1 == randomfd) {
		perror("getRandom:");
		return -EINVAL;
	}
	while(len) {
		int r;
		r = read(randomfd,buf,len);
		if (-1 == r && errno != -EINTR) {	
			perror("read: "); return -EINVAL;
		}
		len-= r; buf += r;
	}
	close(randomfd);

	return r;
}
