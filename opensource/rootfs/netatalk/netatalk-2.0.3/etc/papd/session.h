/*
 * $Id: session.h,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 */

#ifndef PAPD_SESSION_H
#define PAPD_SESSION_H 1

#include <atalk/atp.h>

int session( ATP atp, struct sockaddr_at *sat );

#endif /* PAPD_SESSION_H */
