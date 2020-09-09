#include "config.h"

#include "headers.h"
#include "error.h"
#include "fd.h"
#include "io.h"

/** I/O state data. */
IO io;

/** Initialize I/O state data.
 * \return 0 for success.  (This function does not fail.)
 */
int ioInit(void)
{
	/** Initialize the input file descriptor to standard input. */
	io.in = STDIN_FILENO;
	/** Initialize the output file descriptor to standard output. */
	io.out = STDOUT_FILENO;
	/** Set the initial end-of-file flags to false (or zero). */
	io.eof_in = 0;
	io.eof_out = 0;
	/** Initialize the ring buffer. */
	io.buffer_used = 0;
	io.buffer_head = 0;
	io.buffer_size = DEFAULT_BUFFER_SIZE;
	io.buffer = 0;
	/** Initialize the byte-counters. */
	io.last_read = 0;
	io.last_write = 0;
	io.total_size = 0;
	io.total_size_known = 0;
	io.total_read = 0;
	io.total_write = 0;
	/** Initialize the I/O timeout to 1 millisecond. */
	io.timeout = 1; /* 1/1000000 second */
	/** Initialize the current time and the throttle counts. */
	io.current_time = time(0);
	io.throttle_count = 1;
	io.throttle = MAX_UINT64;
	/** Initialize the default block size to 512 bytes. */
	io.block_size = 512; /* 0.5k default */
	return(0);
}

#ifdef HAVE_SIGNAL_H
/** A signal handler for control-C.
 *
 * \param signo The signal number.
 */
static void sig_int(int signo)
{
	/** Return I/O streams to a default state before exiting. */
	fdEnd(io.in);
	fdEnd(io.out);
	exit(1);
}
#endif

#ifdef HAVE_SYSCONF
#	if HAVE_DECL__SC_PAGE_SIZE == 1
#		define PAGESIZE _SC_PAGE_SIZE
#	else
#		if HAVE_DECL__SC_PAGESIZE == 1
#			define PAGESIZE _SC_PAGESIZE
#		endif
#	endif
#	ifndef PAGESIZE
#		warning I dont know how to retrieve the size of a page using sysconf.
#		warning Assuming page size is DEFAULT_PAGE_SIZE bytes.
#		undef HAVE_SYSCONF
#	endif
#endif

/** Prepare I/O
 *
 * \return 0 for success, non-zero for failure.
 */
int ioBegin(void)
{
	/** If memalign() or posix_memalign() is available, then allocate memory on
	 * a page boundary.  If sysconf() is available, find out what the page size
	 * is for this machine, otherwise assume that a page is 8192 bytes.
	 */
#if defined(USE_MEMALIGN) \
	&& (defined(HAVE_POSIX_MEMALIGN) || defined(HAVE_MEMALIGN))
	long page_size = 0;

#	ifdef HAVE_SYSCONF
	page_size = sysconf(PAGESIZE);
#	else
	page_size = DEFAULT_PAGE_SIZE;
#	endif
#endif

	/** Allocate memory for the ring buffer. */
#undef USED_MEMALIGN
#ifdef USE_MEMALIGN
#	if defined(HAVE_POSIX_MEMALIGN)
#		define USED_MEMALIGN
	if (
		/** Incorporating patch provided by Doncho N. Gunchev
		 * Closes bug: [ 1158420 ] glibc detects invalid free on exit (Fedora Core 3)
		 * */
		posix_memalign((void*)&io.buffer, page_size, sizeof(char) * io.buffer_size) != 0
		)
	{
		print_error(stderr, "Memory allocation failed");
		return(1);
	}
/*
#	elif defined(HAVE_MEMALIGN)
#		if defined(USE_MEMALIGN_BOUNDARY_SIZE)
#			define USED_MEMALIGN
			io.buffer = (char *)memalign(page_size, sizeof(char) * io.buffer_size);
#		elif defined(USE_MEMALIGN_SIZE_BOUNDARY)
#			define USED_MEMALIGN
			io.buffer = (char *)memalign(sizeof(char) * io.buffer_size, page_size);
#		endif
*/
#	endif
#endif
#ifndef USED_MEMALIGN
	io.buffer = (char *)malloc(sizeof(char) * io.buffer_size);
#endif
	if (io.buffer == 0) {
		print_error(stderr, "Memory allocation failed");
		return(1);
	}
	/** If we have signal(), then set up a signal handler to catch control-C's.
	 */
#ifdef HAVE_SIGNAL_H
	if (signal(SIGINT, sig_int) == SIG_ERR) {
		print_error(stderr, "Could not install SIGINT signal handler");
		print_error(stderr, "Control-C events will not reset non-blocking I/O");
	}
#endif
	/** Prepare the I/O file descriptors. */
	fdBegin(io.in);
	fdBegin(io.out);
	return(0);
}

/** Shut down I/O
 *
 * \return 0 for success.  (This function does not fail.)
 */
int ioEnd(void)
{
	/** Free the I/O buffer. */
	free(io.buffer);
	io.buffer = 0;
	io.buffer_used = 0;
	/** Return the file descriptors to their previous state. */
	fdEnd(io.in);
	fdEnd(io.out);
	return(0);
}

/** Check to see if I/O is ready. */
void ioCheck(void)
{
	static fd_set rset, wset;
	static struct timeval to;
	
	to.tv_sec = 0;
	/** Wait for up to io.timeout milliseconds for a change in I/O state. */
	to.tv_usec = io.timeout;
	io.in_ready = 1;
	io.out_ready = 1;
	FD_ZERO(&rset);
	FD_ZERO(&wset);
	if (io.buffer_used != io.buffer_size) FD_SET(io.in, &rset);
	if (io.buffer_used) FD_SET(io.out, &wset);
	/** Check to see if I/O is ready. */
	if (select( ((io.in<io.out)?io.out:io.in)+1, 
		&rset, &wset, 0, &to) == -1) {
		if (errno != EINTR) {
			print_error(stderr, "select() failed");
			return;
		}
	}
	/** Set the I/O ready flags in I/O state data. */
	if (!FD_ISSET(io.in, &rset))
		io.in_ready = 0;
	if (!FD_ISSET(io.out, &wset))
		io.out_ready = 0;
}

/** Read input
 *
 * \return 0 for success, non-zero for failure.
 */
int ioRead(void)
{
	int num_read = 0;
	int end = 0;
	int avail = 0;
#ifdef USE_IOVEC
	int num = 0;
	struct iovec vec[2] = { { 0 } };
#endif

	/** If we've reached out throttle count for this second, return. */
	if ((io.current_time == time(0))
		&& (io.throttle_count >= io.throttle)) {
		return(0);
	}
	if (io.current_time != time(0)) {
		io.throttle_count = 0;
		io.current_time = time(0);
	}
	/** If there is nothing to read yet, return. */
	if (!io.in_ready) return(0);
	io.last_read = 0;
	/** If we've reached the end of input, return. */
	if (io.eof_in) return(0);
	/** If the buffer is full, return. */
	if (io.buffer_used == io.buffer_size) return(0);
	/** Calculate the location and size of the free space in the buffer. */
	end = (io.buffer_used + io.buffer_head) % io.buffer_size;
	avail = io.buffer_size - io.buffer_used;
	/** If the free space in the buffer is greater than the number of bytes left
	 * to be read in this second, throttle the input.
	 */
	if ((io.throttle - io.throttle_count) < avail)
		avail = io.throttle - io.throttle_count;
#ifdef USE_IOVEC
	/** If using iovec I/O, set up vec[] with the proper information and then
	 * read using iovec I/O.
	 */
	vec[num  ].iov_base = io.buffer + end;
	if (end+avail <= io.buffer_size)
		vec[num++].iov_len  = avail;
	else {
		vec[num++].iov_len  = io.buffer_size - end;
		vec[num  ].iov_base = io.buffer;
		vec[num++].iov_len  = avail - (io.buffer_size - end);
	}
	num_read = readv(io.in, vec, num);
#else
	/** If not using iovec, then read input into the next available chunk of
	 * linear buffer space.
	 */
	if (end+avail <= io.buffer_size) {
		num_read = read(io.in,
			io.buffer + end,
			avail);
	}
	else {
		num_read = read(io.in,
			io.buffer + end,
			io.buffer_size - end);
	}
#endif
	/** Check for end of file. */
	if (num_read == 0) {
		io.eof_in = 1;
		return(0);
	}
	/** Check for an error. */
	if (num_read < 0) return(num_read);
	/** Update I/O byte counters. */
	io.buffer_used += num_read;
	io.last_read = num_read;
	io.total_read += num_read;
	io.throttle_count += num_read;
	return(0);
}

/** Write data
 *
 * \return 0 for success, non-zero for failure.
 */
int ioWrite(void)
{
	int num_written = 0;
#ifdef USE_IOVEC
	int num = 0;
	struct iovec vec[2] = { { 0 } };
#endif

	/** If output is not ready, return. */
	if (!io.out_ready) return(0);
	io.last_write = 0;
#ifdef USE_IOVEC
	/** If using iovec I/O, set up vec[] with the correct information and then
	 * write.
	 */
	vec[num  ].iov_base = io.buffer + io.buffer_head;
	if (io.buffer_head + io.buffer_used <= io.buffer_size)
		vec[num++].iov_len  = io.buffer_used;
	else {
		vec[num++].iov_len  = io.buffer_size - io.buffer_head;
		vec[num  ].iov_base = io.buffer;
		vec[num++].iov_len  = io.buffer_used - (io.buffer_size - io.buffer_head);
	}
	num_written = writev(io.out, vec, num);
#else
	/** If not using iovec, then write the next chunk of linear data from the
	 * buffer.
	 */
	if (io.buffer_head + io.buffer_used <= io.buffer_size) {
		num_written = write(io.out,
			io.buffer + io.buffer_head,
			io.buffer_used);
	}
	else {
		num_written = write(io.out,
			io.buffer + io.buffer_head,
			io.buffer_size - io.buffer_head);
	}
#endif
	/** Check for end of file. */
	if (num_written == 0) {
		io.eof_out = 1;
		return(0);
	}
	/** Check for an error. */
	if (num_written < 0) return(num_written);
	/** Update the I/O byte counters. */
	io.buffer_used -= num_written;
	io.buffer_head = (io.buffer_head + num_written) % io.buffer_size;
	io.last_write = num_written;
	io.total_write += num_written;
	return(0);
}

/** Check to see if we're done copying input to output
 *
 * \return 1 if done, 0 otherwise.
 */
int ioIsDone(void)
{
	if ((io.eof_in && (io.buffer_used == 0)) || io.eof_out) return(1);
	return(0);
}

