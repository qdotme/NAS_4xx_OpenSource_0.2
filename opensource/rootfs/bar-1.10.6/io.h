/*
 * I/O and buffering
 */

#ifndef __io_h__
#define __io_h__

#include "headers.h"
#include "types.h"

/** I/O */
struct _io {
	/** Input file descriptor. */
	int in;
	/** Output file descriptor. */
	int out;
	/** Flag: Input source is ready to be read. */
	int in_ready;
	/** Flag: Output source is ready to be written to. */
	int out_ready;
	/** Flag: End of input has been reached. */
	int eof_in;
	/** Flag: End of output has been reached. */
	int eof_out;
	/** The size of the I/O buffer. */
	size_t buffer_size;
	/** A pointer to the I/O buffer. */
	char *buffer;
	/** The location of the start of the ring buffer. */
	size_t buffer_head;
	/** The length of the ring buffer. */
	size_t buffer_used;
	/** The number of bytes read the last time I/O was performed. */
	ssize_t last_read;
	/** The number of bytes written the last time I/O was performed. */
	ssize_t last_write;
	/** The total number of bytes read. */
	uint64 total_read;
	/** The total number of bytes written. */
	uint64 total_write;
	/** The total size of the input stream, if known. */
	uint64 total_size;
	/** Flag: Whether or not the total size of the input stream is known. */
	int total_size_known;
	/** The number of microseconds to wait for a change in I/O state. */
	uint32 timeout;
	/** The current time, used for throttling input. */
	time_t current_time;
	/** The maximum number of bytes per second that we're allowed to read. */
	uint64 throttle;
	/** The number if bytes read so far for this second. */
	uint64 throttle_count;
	/** The size of a block in bytes, used for calculating total_size and
	 * buffer_size. */
	uint64 block_size;
	};

typedef struct _io IO;

extern IO io;

int ioInit(void);
int ioBegin(void);
int ioEnd(void);
void ioCheck(void);
int ioRead(void);
int ioWrite(void);
int ioIsDone(void);

#endif
