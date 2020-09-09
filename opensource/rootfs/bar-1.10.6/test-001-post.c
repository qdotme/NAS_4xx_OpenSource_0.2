#include "config.h"

#include "headers.h"

int main(int argc, char *argv[])
{
	int in = 0;
	char buffer[1024] = { 0 };
	char *expected = "Hello World\n";
	int c = 0;

	in = STDIN_FILENO;
	for (c = 0; c < 1024; c++) buffer[c] = 0;
	read(in, buffer, strlen(expected));
	if (strcmp(buffer, expected) != 0) {
		fprintf(stderr, "*** ERROR: Unexpected string read\n");
		return(1);
	}
	sleep(5);
	for (c = 0; c < 1024; c++) buffer[c] = 0;
	read(in, buffer, strlen(expected));
	if (strcmp(buffer, expected) != 0) {
		fprintf(stderr, "*** ERROR: Unexpected string read\n");
		return(1);
	}
	close(in);
	return(0);
}

