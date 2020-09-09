/*
 * File descriptor manipulation
 */

#ifndef __fd_h__
#define __fd_h__

#include "headers.h"
#include "config.h"
#include "types.h"

int fdBegin(int fd);
int fdEnd(int fd);
int fdIsFile(int fd);
int fdFileSize(int fd, uint64 *size);

#endif
