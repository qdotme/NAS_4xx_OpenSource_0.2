/*-
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 1996-2003
 *	Sleepycat Software.  All rights reserved.
 *
 * $Id: hmac.h,v 1.1.1.1 2008/06/18 10:53:13 jason Exp $
 */

#ifndef	_DB_HMAC_H_
#define	_DB_HMAC_H_

/*
 * Algorithm specific information.
 */
/*
 * SHA1 checksumming
 */
typedef struct {
	u_int32_t	state[5];
	u_int32_t	count[2];
	unsigned char	buffer[64];
} SHA1_CTX;

/*
 * AES assumes the SHA1 checksumming (also called MAC)
 */
#define	DB_MAC_MAGIC	"mac derivation key magic value"
#define	DB_ENC_MAGIC	"encryption and decryption key value magic"

#include "dbinc_auto/hmac_ext.h"
#endif /* !_DB_HMAC_H_ */
