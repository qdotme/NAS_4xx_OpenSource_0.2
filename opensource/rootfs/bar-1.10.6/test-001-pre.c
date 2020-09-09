#include "config.h"

#include "headers.h"

int main(int argc, char *argv[])
{
	int out = 0;
	char *buffer = "Hello World\n";
#ifdef DEBUG
	int debug = 1;

	while (debug);
#endif

	out = STDOUT_FILENO;
	write(out, buffer, strlen(buffer));
	sleep(5);
	write(out, buffer, strlen(buffer));
	close(out);

	return(0);
}

