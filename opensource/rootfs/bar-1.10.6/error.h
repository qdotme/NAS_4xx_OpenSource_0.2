#ifndef __error_h__
#define __error_h__

#include "headers.h"

int get_errno(void);
void clear_errno(void);
void print_error(FILE *ferr, char *fmt, ...);
void print_esup(FILE *ferr, char *fmt, ...);

#endif
