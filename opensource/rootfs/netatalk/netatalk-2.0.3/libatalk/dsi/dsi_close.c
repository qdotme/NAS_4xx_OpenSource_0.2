/*
 * $Id: dsi_close.c,v 1.1.1.1 2008/06/18 10:55:40 jason Exp $
 *
 * Copyright (c) 1997 Adrian Sun (asun@zoology.washington.edu)
 * All rights reserved. See COPYRIGHT.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <atalk/dsi.h>
#include <netatalk/endian.h>

void dsi_close(DSI *dsi)
{
  /* server generated. need to set all the fields. */
  if (!dsi->asleep) {
      dsi->header.dsi_flags = DSIFL_REQUEST;
      dsi->header.dsi_command = DSIFUNC_CLOSE;
      dsi->header.dsi_requestID = htons(dsi_serverID(dsi));
      dsi->header.dsi_code = dsi->header.dsi_reserved = htonl(0);
      dsi->cmdlen = 0; 
      dsi_send(dsi);
      dsi->proto_close(dsi);
  }
  free(dsi);
}
