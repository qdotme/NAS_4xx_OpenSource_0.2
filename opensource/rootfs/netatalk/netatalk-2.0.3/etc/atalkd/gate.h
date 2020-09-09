/*
 * $Id: gate.h,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (c) 1990,1993 Regents of The University of Michigan.
 * All Rights Reserved. See COPYRIGHT.
 */

struct gate {
    struct gate		*g_next,
			*g_prev;
    int			g_state;
    struct interface	*g_iface;
    struct rtmptab	*g_rt;
    struct sockaddr_at	g_sat;
};
