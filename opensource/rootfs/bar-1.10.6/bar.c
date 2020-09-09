#include "config.h"

#include "headers.h"
#include "types.h"
#include "error.h"
#include "fd.h"
#include "io.h"
#include "display.h"
#include "args.h"

/** \mainpage Command Line Progress Bar

	Bar is a simple tool to copy a stream of data from input to output while
	displaying for the user:
	
	\li The amount of data passed
	\li The throughput of the data transfer
	
	and, if the total size of the data stream is known:

	\li The estimated time remaining
	\li What percent of the data stream has been copied
	\li A progress bar

	Bar was originally written for the purpose of estimating the amount of time
	needed to transfer large amounts (many, many gigabytes) of data across a
	network in a tar/ssh pipe.

*/

int main(int argc, char *argv[])
{
#ifdef DEBUG
	int debug = 1;

	fprintf(stderr, "Waiting for debugger to attach...\n");
	while (debug);
#endif
	if (ioInit() != 0) return(1);
	if (displayInit() != 0) return(1);
	if (parse_rcfiles(stderr) != 0) return(1);
	if (parse_args(stderr, argc, argv) != 0) return(1);
	if (ioBegin() != 0) return(1);
	if (displayBegin() != 0) return(1);

	while (!ioIsDone()) {
		ioCheck();
		if (ioRead() < 0) {
			print_error(stderr, "read error");
			break;
		}
		if (ioWrite() < 0) {
			print_error(stderr, "write error");
			break;
		}
		displayUpdate();
	}

	displayEnd();
	ioEnd();
	return(0);
}

