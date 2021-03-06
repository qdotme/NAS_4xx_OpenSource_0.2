/*
 * $Id: route.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (c) 1990,1996 Regents of The University of Michigan.
 * All Rights Reserved. See COPYRIGHT.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/route.h>
#include <sys/ioctl.h>
#include <netatalk/at.h>

#include "rtmp.h"
#include "route.h"

#ifndef BSD4_4
int route( message, dst, gate, flags )
    int			message;
    struct sockaddr	*dst, *gate;
    int			flags;
{
#ifdef TRU64
    struct ortentry	rtent;
#else /* TRU64 */
    struct rtentry	rtent;
#endif /* TRU64 */

    memset( &rtent, 0, sizeof( struct rtentry ));
    rtent.rt_dst = *dst;
    rtent.rt_gateway = *gate;
    rtent.rt_flags = flags;
    return( ioctl( rtfd, message, &rtent ));
}

#else /* BSD4_4 */

struct sockaddr_m {
    u_char	sam_len;
    u_char	sam_family;
    u_short	sam_pad;
    u_short	sam_mask;
    u_short	sam_pad2;
} mask = { sizeof( struct sockaddr_m ), 0, 0, 0xffff, 0 };

struct rt_msg_at {
    struct rt_msghdr	rtma_rtm;
    struct sockaddr_at	rtma_dst;
    struct sockaddr_at	rtma_gate;
    struct sockaddr_m	rtma_mask;
} rtma;

route( message, dst, gate, flags )
    int			message;
    struct sockaddr_at	*dst, *gate;
    int			flags;
{
    int			rc;

    memset( &rtma, 0, sizeof( struct rt_msg_at ));
    rtma.rtma_rtm.rtm_msglen = sizeof( struct rt_msg_at );
    rtma.rtma_rtm.rtm_version = RTM_VERSION;
    rtma.rtma_rtm.rtm_type = message;
    rtma.rtma_rtm.rtm_pid = getpid();
    rtma.rtma_rtm.rtm_addrs = RTA_DST|RTA_GATEWAY;
    if ( flags & RTF_HOST ) {
	rtma.rtma_rtm.rtm_msglen = sizeof( struct rt_msg_at ) -
		sizeof( struct sockaddr_m );
    } else {
	rtma.rtma_rtm.rtm_msglen = sizeof( struct rt_msg_at );
	rtma.rtma_rtm.rtm_addrs |= RTA_NETMASK;
	rtma.rtma_mask = mask;
    }

    rtma.rtma_rtm.rtm_flags = flags;
    rtma.rtma_dst = *dst;
    rtma.rtma_gate = *gate;
    if (( rc = write( rtfd, &rtma, rtma.rtma_rtm.rtm_msglen )) !=
	    rtma.rtma_rtm.rtm_msglen ) {
	return( -1 );
    }
    return( 0 );
}
#endif /* BSD4_4 */
