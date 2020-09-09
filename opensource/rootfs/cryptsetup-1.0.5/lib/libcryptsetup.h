#ifndef _LIBCRYPTSETUP_H
#define _LIBCRYPTSETUP_H

#include <stdint.h>

#define	CRYPT_FLAG_VERIFY	        (1 << 0)
#define CRYPT_FLAG_READONLY	        (1 << 1)
#define	CRYPT_FLAG_VERIFY_IF_POSSIBLE	(1 << 2)
#define	CRYPT_FLAG_VERIFY_ON_DELKEY	(1 << 3)

struct crypt_options {
	const char	*name;
	const char	*device;

	const char	*cipher;
	const char	*hash;

	const char	*passphrase;
	int		passphrase_fd;
	const char	*key_file;
	const char	*new_key_file;	
	int		key_size;
	
	unsigned int	flags;
	int 	        key_slot;

	uint64_t	size;
	uint64_t	offset;
	uint64_t	skip;
	uint64_t        iteration_time;
 	uint64_t	timeout;

 	uint64_t	align_payload;
	int             tries;
};

int crypt_create_device(struct crypt_options *options);
int crypt_update_device(struct crypt_options *options);
int crypt_resize_device(struct crypt_options *options);
int crypt_query_device(struct crypt_options *options);
int crypt_remove_device(struct crypt_options *options);
int crypt_luksInit(struct crypt_options *options);
int crypt_luksOpen(struct crypt_options *options);
int crypt_luksDelKey(struct crypt_options *options);
int crypt_luksAddKey(struct crypt_options *options);
int crypt_luksUUID(struct crypt_options *options);
int crypt_isLuks(struct crypt_options *options);

void crypt_get_error(char *buf, size_t size);
void crypt_put_options(struct crypt_options *options);
const char *crypt_get_dir(void);

#endif /* _LIBCRYPTSETUP_H */
