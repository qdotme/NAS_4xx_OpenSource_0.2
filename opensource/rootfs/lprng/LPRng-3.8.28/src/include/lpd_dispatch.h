/***************************************************************************
 * LPRng - An Extended Print Spooler System
 *
 * Copyright 1988-2003, Patrick Powell, San Diego, CA
 *     papowell@lprng.com
 * See LICENSE for conditions of use.
 * $Id: lpd_dispatch.h,v 1.1.1.1 2006/04/06 06:25:56 wiley Exp $
 ***************************************************************************/



#ifndef _LPD_DISPATCH_H_
#define _LPD_DISPATCH_H_ 1

/* PROTOTYPES */
void Dispatch_input(int *talk, char *input, const char *from_addr );
void Service_all( struct line_list *args );
void Service_connection( struct line_list *args );
void Service_lpd( int talk, const char *from_addr );

#endif
