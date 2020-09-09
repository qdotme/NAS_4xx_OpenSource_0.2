#include "config.h"

#include "headers.h"
#include "error.h"

int get_errno(void)
{
	return(errno);
}

void clear_errno(void)
{
	errno = 0;
}

void print_error(FILE *ferr, char *fmt, ...)
{
	va_list ap;
	char msg[256] = { 0 };

	va_start(ap, fmt);
	(void)vsprintf(msg, fmt, ap);
	va_end(ap);

	fprintf(ferr, "*** ERROR: ");
	if (errno != 0) {
		fprintf(ferr, "[%d]: %s\n", errno, strerror(errno));
		fprintf(ferr, "           ");
	}
	fprintf(ferr, "%s\n", msg);
}

void print_esup(FILE *ferr, char *fmt, ...)
{
	va_list ap;
	char msg[256] = { 0 };

	va_start(ap, fmt);
	(void)vsprintf(msg, fmt, ap);
	va_end(ap);

	fprintf(ferr, "           %s\n", msg);
}

