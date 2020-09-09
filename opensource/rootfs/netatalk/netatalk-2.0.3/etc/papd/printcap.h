/*
 * $Id: printcap.h,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 */

#ifndef PAPD_PRINTCAP_H
#define PAPD_PRINTCAP_H 1

#include <sys/cdefs.h>

int getprent __P(( register char *, register char *, register int ));
int pnchktc __P(( char * ));
int pgetflag __P(( char * ));
void endprent __P(( void ));
int pgetent __P(( char *, char *, char * ));
int pgetnum __P(( char * ));
int pnamatch __P(( char * ));

#endif /* PAPD_PRINTCAP_H */
