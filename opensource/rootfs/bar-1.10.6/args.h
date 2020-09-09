#ifndef __args_h__
#define __args_h__

#include "headers.h"
#include "types.h"

int parse_num(FILE *ferr, char *s, uint64 *n, uint64 min, uint64 max);
int parse_rcfile(FILE *ferr, char *filename);
int parse_rcfiles(FILE *ferr);
int parse_args(FILE *ferr, int argc, char *argv[]);

#endif
