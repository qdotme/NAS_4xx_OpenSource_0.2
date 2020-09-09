/*
 * LUKS - Linux Unified Key Setup 
 *
 * Copyright (C) 2004-2006, Clemens Fruhwirth <clemens@endorphin.org>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "luks.h"
#include "af.h"
#include "pbkdf.h"
#include "sha1.h"
#include "random.h"
#include "XORblock.h"
#include <uuid/uuid.h>
#include <../lib/internal.h>

#define div_round_up(a,b) ({           \
	typeof(a) __a = (a);          \
	typeof(b) __b = (b);          \
	(__a - 1) / __b + 1;        \
})


struct luks_masterkey *LUKS_alloc_masterkey(int keylength)
{ 
	struct luks_masterkey *mk=malloc(sizeof(*mk) + keylength);
	mk->keyLength=keylength;
	return mk;
}

void LUKS_dealloc_masterkey(struct luks_masterkey *mk)
{
	if(NULL != mk) {
		memset(mk->key,0,mk->keyLength);
		mk->keyLength=0;
		free(mk);
	}
}

struct luks_masterkey *LUKS_generate_masterkey(int keylength)
{
	struct luks_masterkey *mk=LUKS_alloc_masterkey(keylength);
	getRandom(mk->key,keylength);
	return mk;
}

int LUKS_read_phdr(const char *device, struct luks_phdr *hdr)
{
	int devfd = 0; 
	unsigned int i; 
	int r = 0;
	char luksMagic[] = LUKS_MAGIC;
	
	devfd = open(device,O_RDONLY | O_DIRECT | O_SYNC);
	if(-1 == devfd) {
		fprintf(stderr, _("Can't open device: %s\n"), device);
		return -EINVAL; 
	}

	if(read_blockwise(devfd, hdr, sizeof(struct luks_phdr)) < sizeof(struct luks_phdr)) {
		r = -EIO;
	} else if(memcmp(hdr->magic, luksMagic, LUKS_MAGIC_L)) { /* Check magic */
		fprintf(stderr, _("%s is not a LUKS partition\n"), device);
		r = -EINVAL;
	} else if(memcmp(hdr->hashSpec, "sha1", 4)) { /* Check for SHA1 - other hashspecs are not implemented ATM */
		fputs(_("unknown hash spec in phdr"), stderr);
		r = -EINVAL;
	} else if((hdr->version = ntohs(hdr->version)) != 1) {	/* Convert every uint16/32_t item from network byte order */
		fprintf(stderr, _("unknown version %d\n"), hdr->version);
		r = -EINVAL;
	} else {
		hdr->payloadOffset      = ntohl(hdr->payloadOffset);
		hdr->keyBytes           = ntohl(hdr->keyBytes);
		hdr->mkDigestIterations = ntohl(hdr->mkDigestIterations);

		for(i = 0; i < LUKS_NUMKEYS; ++i) {
			hdr->keyblock[i].active             = ntohl(hdr->keyblock[i].active);
			hdr->keyblock[i].passwordIterations = ntohl(hdr->keyblock[i].passwordIterations);
			hdr->keyblock[i].keyMaterialOffset  = ntohl(hdr->keyblock[i].keyMaterialOffset);
			hdr->keyblock[i].stripes            = ntohl(hdr->keyblock[i].stripes);
		}
	}

	close(devfd);
	return r;
}

int LUKS_write_phdr(const char *device, struct luks_phdr *hdr)
{
	int devfd = 0; 
	unsigned int i; 
	struct luks_phdr convHdr;
	int r;
	
	devfd = open(device,O_RDWR | O_DIRECT | O_SYNC);
	if(-1 == devfd) { 
		fprintf(stderr, _("Can't open device: %s\n"), device);
		return -EINVAL;
	}

	memcpy(&convHdr, hdr, sizeof(struct luks_phdr));

	/* Convert every uint16/32_t item to network byte order */
	convHdr.version            = htons(hdr->version);
	convHdr.payloadOffset      = htonl(hdr->payloadOffset);
	convHdr.keyBytes           = htonl(hdr->keyBytes);
	convHdr.mkDigestIterations = htonl(hdr->mkDigestIterations);
	for(i = 0; i < LUKS_NUMKEYS; ++i) {
		convHdr.keyblock[i].active             = htonl(hdr->keyblock[i].active);
		convHdr.keyblock[i].passwordIterations = htonl(hdr->keyblock[i].passwordIterations);
		convHdr.keyblock[i].keyMaterialOffset  = htonl(hdr->keyblock[i].keyMaterialOffset);
		convHdr.keyblock[i].stripes            = htonl(hdr->keyblock[i].stripes);
	}

	r = write_blockwise(devfd, &convHdr, sizeof(struct luks_phdr)) < sizeof(struct luks_phdr) ? -EIO : 0;

	close(devfd);
	return r;
}	

inline int round_up_modulo(int x, int m) {
	return div_round_up(x, m) * m;
}

int LUKS_generate_phdr(struct luks_phdr *header, 
		       const struct luks_masterkey *mk, const char *cipherName,
		       const char *cipherMode, unsigned int stripes,
		       unsigned int alignPayload)
{
	unsigned int i=0;
	unsigned int blocksPerStripeSet = div_round_up(mk->keyLength*stripes,SECTOR_SIZE);
	int r;
	char luksMagic[] = LUKS_MAGIC;
	uuid_t partitionUuid;
	int currentSector;
	int alignSectors = 4096/SECTOR_SIZE;
	if (alignPayload == 0)
		alignPayload = alignSectors;

	memset(header,0,sizeof(struct luks_phdr));

	/* Set Magic */
	memcpy(header->magic,luksMagic,LUKS_MAGIC_L);
	header->version=1;
	strncpy(header->cipherName,cipherName,LUKS_CIPHERNAME_L);
	strncpy(header->cipherMode,cipherMode,LUKS_CIPHERMODE_L);

	/* This is hard coded ATM */
	strncpy(header->hashSpec,"sha1",LUKS_HASHSPEC_L);

	header->keyBytes=mk->keyLength;

	r = getRandom(header->mkDigestSalt,LUKS_SALTSIZE);
	if(r < 0) return r;

	/* Compute master key digest */
	header->mkDigestIterations = LUKS_MKD_ITER;
	PBKDF2_HMAC_SHA1(mk->key,mk->keyLength,
			 header->mkDigestSalt,LUKS_SALTSIZE,
			 header->mkDigestIterations,
			 header->mkDigest,LUKS_DIGESTSIZE);

	currentSector = round_up_modulo(LUKS_PHDR_SIZE, alignSectors);
	for(i = 0; i < LUKS_NUMKEYS; ++i) {
		header->keyblock[i].active = LUKS_KEY_DISABLED;
		header->keyblock[i].keyMaterialOffset = currentSector;
		header->keyblock[i].stripes = stripes;
		currentSector = round_up_modulo(currentSector + blocksPerStripeSet, alignSectors);
	}
	currentSector = round_up_modulo(currentSector, alignPayload);

	header->payloadOffset=currentSector;
	uuid_generate(partitionUuid);
	uuid_unparse(partitionUuid, header->uuid);

	return 0;
}

int LUKS_set_key(const char *device, unsigned int keyIndex, 
		 const char *password, size_t passwordLen, 
		 struct luks_phdr *hdr, struct luks_masterkey *mk,
		 struct setup_backend *backend)
{
	char derivedKey[hdr->keyBytes];
	char *AfKey;
	unsigned int AFEKSize;
	int r;
	
	if(hdr->keyblock[keyIndex].active != LUKS_KEY_DISABLED) {
		fprintf(stderr, _("key %d active, purge first.\n"), keyIndex);
		return -EINVAL;
	}
		
	if(hdr->keyblock[keyIndex].stripes < LUKS_STRIPES) {
	        fprintf(stderr,_("key material section %d includes too few stripes. Header manipulation?\n"),keyIndex);
	         return -EINVAL;
	}
	r = getRandom(hdr->keyblock[keyIndex].passwordSalt, LUKS_SALTSIZE);
	if(r < 0) return r;

//	assert((mk->keyLength % TWOFISH_BLOCKSIZE) == 0); FIXME

	PBKDF2_HMAC_SHA1(password,passwordLen,
			 hdr->keyblock[keyIndex].passwordSalt,LUKS_SALTSIZE,
			 hdr->keyblock[keyIndex].passwordIterations,
			 derivedKey, hdr->keyBytes);
	/*
	 * AF splitting, the masterkey stored in mk->key is splitted to AfMK
	 */
	AFEKSize = hdr->keyblock[keyIndex].stripes*mk->keyLength;
	AfKey = (char *)malloc(AFEKSize);
	if(AfKey == NULL) return -ENOMEM;
	
	r = AF_split(mk->key,AfKey,mk->keyLength,hdr->keyblock[keyIndex].stripes);
	if(r < 0) goto out;

	/* Encryption via dm */
	r = LUKS_encrypt_to_storage(AfKey,
				    AFEKSize,
				    hdr,
				    derivedKey,
				    hdr->keyBytes,
				    device,
				    hdr->keyblock[keyIndex].keyMaterialOffset,
				    backend);
	if(r < 0) {
		fprintf(stderr,"Failed to write to key storage.\n");
		goto out;
	}

	/* Mark the key as active in phdr */
	hdr->keyblock[keyIndex].active = LUKS_KEY_ENABLED;
	r = LUKS_write_phdr(device,hdr);
	if(r < 0) goto out;

	r = 0;
out:
	free(AfKey);
	return r;
}

int LUKS_open_key(const char *device, 
		  unsigned int keyIndex, 
		  const char *password, 
		  size_t passwordLen, 
		  struct luks_phdr *hdr, 
		  struct luks_masterkey *mk,
		  struct setup_backend *backend)
{
	char derivedKey[hdr->keyBytes];
	char *AfKey;
	size_t AFEKSize;
	char checkHashBuf[LUKS_DIGESTSIZE];
	int r;
	
	if(hdr->keyblock[keyIndex].active != LUKS_KEY_ENABLED) {
#ifdef LUKS_DEBUG
		fprintf(stderr, _("key %d is disabled.\n"), keyIndex);
#endif
		return -EINVAL;
	}
	
	// assert((mk->keyLength % TWOFISH_BLOCKSIZE) == 0); FIXME

	AFEKSize = hdr->keyblock[keyIndex].stripes*mk->keyLength;
	AfKey = (char *)malloc(AFEKSize);
	if(AfKey == NULL) return -ENOMEM;
	
	PBKDF2_HMAC_SHA1(password,passwordLen,
			 hdr->keyblock[keyIndex].passwordSalt,LUKS_SALTSIZE,
			 hdr->keyblock[keyIndex].passwordIterations,
			 derivedKey, hdr->keyBytes);

	r = LUKS_decrypt_from_storage(AfKey,
				      AFEKSize,
				      hdr,
				      derivedKey,
				      hdr->keyBytes,
				      device,
				      hdr->keyblock[keyIndex].keyMaterialOffset,
				      backend);
	if(r < 0) {
		fprintf(stderr,"Failed to read from key storage\n");
		goto out;
	}

	r = AF_merge(AfKey,mk->key,mk->keyLength,hdr->keyblock[keyIndex].stripes);
	if(r < 0) goto out;
	
	PBKDF2_HMAC_SHA1(mk->key,mk->keyLength,
			 hdr->mkDigestSalt,LUKS_SALTSIZE,
			 hdr->mkDigestIterations,
			 checkHashBuf,LUKS_DIGESTSIZE);

	r = (memcmp(checkHashBuf,hdr->mkDigest, LUKS_DIGESTSIZE) == 0)?0:-EPERM;
out:
	free(AfKey);
	return r;
}

int LUKS_open_any_key(const char *device, 
		      const char *password, 
		      size_t passwordLen,
		      struct luks_phdr *hdr, 
		      struct luks_masterkey **mk,
		      struct setup_backend *backend)
{
	unsigned int i;
	int r;

	r = LUKS_read_phdr(device, hdr);
	if(r < 0) 
      		return r;

	*mk=LUKS_alloc_masterkey(hdr->keyBytes);
	for(i=0; i<LUKS_NUMKEYS; i++) {
		r = LUKS_open_key(device, i, password, passwordLen, hdr, *mk, backend);
		
		if(r == 0) { 
			printf("key slot %d unlocked.\n",i);
			return i; 
		} 
		/* Do not retry for errors that are no -EPERM or -EINVAL, former meaning password wrong, latter key slot inactive */
		if ((r != -EPERM) && (r != -EINVAL)) 
			return r;
	}
	/* Warning, early returns above */
	return -EPERM;
}

/*
 * Wipe patterns according to Gutmann's Paper
 */

static void wipeSpecial(char *buffer, size_t buffer_size, unsigned int turn)
{
        unsigned int i;
	
        unsigned char write_modes[27][3] = {
                {"\x55\x55\x55"}, {"\xaa\xaa\xaa"}, {"\x92\x49\x24"},
                {"\x49\x24\x92"}, {"\x24\x92\x49"}, {"\x00\x00\x00"},
                {"\x11\x11\x11"}, {"\x22\x22\x22"}, {"\x33\x33\x33"},
                {"\x44\x44\x44"}, {"\x55\x55\x55"}, {"\x66\x66\x66"},
                {"\x77\x77\x77"}, {"\x88\x88\x88"}, {"\x99\x99\x99"},
                {"\xaa\xaa\xaa"}, {"\xbb\xbb\xbb"}, {"\xcc\xcc\xcc"},
                {"\xdd\xdd\xdd"}, {"\xee\xee\xee"}, {"\xff\xff\xff"},
                {"\x92\x49\x24"}, {"\x49\x24\x92"}, {"\x24\x92\x49"},
                {"\x6d\xb6\xdb"}, {"\xb6\xdb\x6d"}, {"\xdb\x6d\xb6"}
        };
	
        for(i = 0; i < buffer_size / 3; ++i) {
                memcpy(buffer, write_modes[turn - 3], 3);
                buffer += 3;
        }
}

static int wipe(const char *device, unsigned int from, unsigned int to)
{
	int devfd;
	char *buffer;
	unsigned int i;
	unsigned int bufLen = (to - from) * SECTOR_SIZE;
	int r = 0;
	
	devfd = open(device, O_RDWR | O_DIRECT);
	if(devfd == -1) {
		fprintf(stderr, _("Can't open device: %s\n"), device);
		return -EINVAL;
	}

	buffer = (char *) malloc(bufLen);
	if(!buffer) return -ENOMEM;

	for(i = 0; i < 39; ++i) {
		if     (i >=  0 && i <  5) getRandom(buffer, bufLen);
		else if(i >=  5 && i < 33) wipeSpecial(buffer, bufLen, i);
		else if(i >= 33 && i < 38) getRandom(buffer, bufLen);
		else if(i >= 38 && i < 39) memset(buffer, 0xFF, bufLen);

		if(write_lseek_blockwise(devfd, buffer, bufLen, from * SECTOR_SIZE) < 0) {
			r = -EIO;
			break;
		}

		fsync(devfd); fsync(devfd); sync();
	}

	free(buffer);
	close(devfd);

	return r;
}

int LUKS_del_key(const char *device, unsigned int keyIndex)
{
	struct luks_phdr hdr;
	unsigned int i, startOffset, endOffset, stripesLen;
	int r;
	
	r = LUKS_read_phdr(device, &hdr);
	if(r != 0) {
		/* placeholder */
	} else if(hdr.keyblock[keyIndex].active != LUKS_KEY_ENABLED || keyIndex >= LUKS_NUMKEYS) {
		set_error(_("Key %d not active. Can't wipe.\n"), keyIndex);
		r = -1;
	} else {
		/* secure deletion of key material */
		startOffset = hdr.keyblock[keyIndex].keyMaterialOffset;
		stripesLen = hdr.keyBytes * hdr.keyblock[keyIndex].stripes;
		endOffset = startOffset + div_round_up(stripesLen, SECTOR_SIZE);

		r = wipe(device, startOffset, endOffset);
		if(r == 0) {
			/* mark the key as inactive in header */
			hdr.keyblock[keyIndex].active = LUKS_KEY_DISABLED;
			r = LUKS_write_phdr(device, &hdr);
		}
	}

	return r;
}

int LUKS_is_last_keyslot(const char *device, unsigned int keyIndex)
{
	struct luks_phdr hdr;
	unsigned int i;
	int r;
	
	r = LUKS_read_phdr(device, &hdr);
	if(r < 0) return r;
	
	for(i = 0; i < LUKS_NUMKEYS; i++) {
		if(i != keyIndex && hdr.keyblock[i].active == LUKS_KEY_ENABLED)
			return 0;
	}
	return 1;
}


int LUKS_benchmarkt_iterations()
{
	return PBKDF2_performance_check()/2;
}
