/*
 * $Id: asingle.h,v 1.1.1.1 2008/06/18 10:55:40 jason Exp $
 */

#ifndef _ASINGLE_H
#define _ASINGLE_H 1

/* Forward Declarations */
struct FHeader;

int single_open(char *singlefile, int flags, struct FHeader *fh, int options);
int single_close(int readflag);
int single_header_read(struct FHeader *fh, int version);
int single_header_test(void);
int single_read(int fork, char *buffer, int length);

#endif /* _ASINGLE_H */
