/*
 * prompt.c: Routines for retrieving and setting a prompt.
 *
 * $Header: /var/cvs/proto/marvell_5182/package/e2fsprogs/e2fsprogs-1.39/lib/ss/prompt.c,v 1.1.1.1 2007/03/20 02:30:58 jason Exp $
 * $Locker:  $
 *
 * Copyright 1987, 1988 by MIT Student Information Processing Board
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose is hereby granted, provided that
 * the names of M.I.T. and the M.I.T. S.I.P.B. not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  M.I.T. and the
 * M.I.T. S.I.P.B. make no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#include <stdio.h>
#include "ss_internal.h"

void ss_set_prompt(int sci_idx, char *new_prompt)
{
     ss_info(sci_idx)->prompt = new_prompt;
}

char *ss_get_prompt(int sci_idx)
{
     return(ss_info(sci_idx)->prompt);
}
