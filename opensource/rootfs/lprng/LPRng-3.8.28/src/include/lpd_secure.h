/***************************************************************************
 * LPRng - An Extended Print Spooler System
 *
 * Copyright 1988-2003, Patrick Powell, San Diego, CA
 *     papowell@lprng.com
 * See LICENSE for conditions of use.
 * $Id: lpd_secure.h,v 1.1.1.1 2006/04/06 06:25:56 wiley Exp $
 ***************************************************************************/



#ifndef _LPD_SECURE_H_
#define _LPD_SECURE_H_ 1

/* PROTOTYPES */
int Receive_secure( int *sock, char *input );
int Do_secure_work( char *jobsize, int from_server,
	char *tempfile, struct line_list *header_info );
struct security *Fix_receive_auth( char *name, struct line_list *info );
int Check_secure_perms( struct line_list *options, int from_server,
	char *error, int errlen );

#endif
