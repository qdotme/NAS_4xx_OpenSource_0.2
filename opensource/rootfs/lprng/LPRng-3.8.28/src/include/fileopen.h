/***************************************************************************
 * LPRng - An Extended Print Spooler System
 *
 * Copyright 1988-2003, Patrick Powell, San Diego, CA
 *     papowell@lprng.com
 * See LICENSE for conditions of use.
 * $Id: fileopen.h,v 1.1.1.1 2006/04/06 06:25:56 wiley Exp $
 ***************************************************************************/



#ifndef _FILEOPEN_H_
#define _FILEOPEN_H_ 1

/*****************************************************************
 * File open functions
 * These perform extensive checking for permissions and types
 *  see fileopen.c for details
 *****************************************************************/

/* PROTOTYPES */
int Checkread( const char *file, struct stat *statb );
int Checkwrite( const char *file, struct stat *statb, int rw, int create,
	int nodelay );
int Checkwrite_timeout(int timeout,
	const char *file, struct stat *statb, int rw, int create, int nodelay );

#endif
