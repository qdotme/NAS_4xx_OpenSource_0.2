/*
 * $Id: list.h,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $
 *
 * Copyright (c) 1990,1992 Regents of The University of Michigan.
 * All Rights Reserved. See COPYRIGHT.
 */

struct list {
    void	*l_data;
    struct list	*l_next,
		*l_prev;
};
