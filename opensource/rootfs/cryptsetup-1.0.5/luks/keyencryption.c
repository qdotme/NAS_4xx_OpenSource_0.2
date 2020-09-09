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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "luks.h"
#include "../lib/libcryptsetup.h"
#include "../lib/internal.h"
#include "../lib/blockdev.h"

#define div_round_up(a,b) ({           \
	typeof(a) __a = (a);          \
	typeof(b) __b = (b);          \
	(__a - 1) / __b + 1;        \
})


static int setup_mapping(const char *cipher, const char *name, 
			 const char *device, unsigned int payloadOffset,
			 const char *key, size_t keyLength, 
			 unsigned int sector, size_t srcLength, 
			 struct setup_backend *backend,
			 int mode)
{
	struct crypt_options k;
	struct crypt_options *options = &k;
	int device_sector_size = sector_size_for_device(device);
	int r;

	/*
	 * we need to round this to nearest multiple of the underlying
	 * device's sector size, otherwise the mapping will be refused.
	 */
	if(device_sector_size < 0) {
		fprintf(stderr,_("Unable to obtain sector size for %s"),device);
		return -EINVAL;
	}
	options->size = round_up_modulo(srcLength,device_sector_size)/SECTOR_SIZE;

	options->offset = sector;
	options->cipher = cipher;
	options->key_size = keyLength;
	options->skip = 0; 
	options->flags = 0;
	options->name = name;
	options->device = device;
	
	if (mode == O_RDONLY) {
		options->flags |= CRYPT_FLAG_READONLY;
	}

	r = backend->create(0, options, key);

	if (r <= 0)
		set_error(NULL);

	return r;
}

static int clear_mapping(const char *name, struct setup_backend *backend)
{
	struct crypt_options options;
	options.name=name;
	return backend->remove(&options);
}

static int LUKS_endec_template(char *src, size_t srcLength, 
			       struct luks_phdr *hdr, 
			       char *key, size_t keyLength, 
			       const char *device, 
			       unsigned int sector, struct setup_backend *backend,
			       ssize_t (*func)(int, void *, size_t),
			       int mode)
{
	int devfd;
	char *name = NULL;
	char *fullpath = NULL;
	char *dmCipherSpec = NULL;
	const char *dmDir = backend->dir(); 
	int r = -1;

	if(dmDir == NULL) {
		fputs(_("Failed to obtain device mapper directory."), stderr);
		return -1;
	}
	if(asprintf(&name,"temporary-cryptsetup-%d",getpid())               == -1 ||
	   asprintf(&fullpath,"%s/%s",dmDir,name)                           == -1 ||
	   asprintf(&dmCipherSpec,"%s-%s",hdr->cipherName, hdr->cipherMode) == -1) {
	        r = -ENOMEM;
		goto out1;
        }

	r = setup_mapping(dmCipherSpec,name,device,hdr->payloadOffset,key,keyLength,sector,srcLength,backend,mode);
	if(r < 0) {
		fprintf(stderr,"Failed to setup dm-crypt key mapping.\nCheck kernel for support for the %s cipher spec and verify that %s contains at least %d sectors.\n",
			dmCipherSpec, 
			device, 
			sector + div_round_up(srcLength,SECTOR_SIZE));
		r = -EIO;
		goto out1;
	}

	devfd = open(fullpath, mode | O_DIRECT | O_SYNC);
	if(devfd == -1) { r = -EIO; goto out2; }

	r = func(devfd,src,srcLength);
	if(r < 0) { r = -EIO; goto out3; }

	r = 0;
 out3:
	close(devfd);
 out2:
	clear_mapping(name,backend);
 out1:
	free(dmCipherSpec);
	free(fullpath); 
	free(name); 
	return r;
}

int LUKS_encrypt_to_storage(char *src, size_t srcLength, 
			    struct luks_phdr *hdr, 
			    char *key, size_t keyLength, 
			    const char *device, 
			    unsigned int sector, struct setup_backend *backend)
{
	
	return LUKS_endec_template(src,srcLength,hdr,key,keyLength, device, sector, backend,	
				   (ssize_t (*)(int, void *, size_t)) write_blockwise, O_RDWR);
}	

int LUKS_decrypt_from_storage(char *dst, size_t dstLength, 
			      struct luks_phdr *hdr, 
			      char *key, size_t keyLength, 
			      const char *device, 
			      unsigned int sector, struct setup_backend *backend)
{
	return LUKS_endec_template(dst,dstLength,hdr,key,keyLength, device, sector, backend, read_blockwise, O_RDONLY);
}
