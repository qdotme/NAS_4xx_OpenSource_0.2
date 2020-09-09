#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>

#include "libcryptsetup.h"
#include "internal.h"
#include "blockdev.h"
#include "luks.h"

struct device_infos {
	uint64_t	size;
	int		readonly;
};

static int memory_unsafe = 0;
static char *default_backend = NULL;

#define at_least_one(a) ({ __typeof__(a) __at_least_one=(a); (__at_least_one)?__at_least_one:1; })

static int setup_enter(struct setup_backend *backend)
{
	int r;

	/*
	 * from here we could have sensible data in memory
	 * so protect it from being swapped out
	 */
	r = mlockall(MCL_CURRENT | MCL_FUTURE);
	if (r < 0) {
		perror("mlockall failed");
		fprintf(stderr, "WARNING!!! Possibly insecure memory. Are you root?\n");
		memory_unsafe = 1;
	}

	set_error(NULL);

	if (backend) {
		r = backend->init();
		if (r < 0)
			return r;
		if (r > 0)
			memory_unsafe = 1;
	}

	return 0;
}

static int setup_leave(struct setup_backend *backend)
{
	const char *error;

	if (backend)
		backend->exit();

	/* dangerous, we can't wipe all the memory */
	if (!memory_unsafe)
		munlockall();

	return 0;
}

static int untimed_read(int fd, char *pass, size_t maxlen)
{
	ssize_t i;

	i = read(fd, pass, maxlen);
	if (i > 0) {
		pass[i-1] = '\0';
		i = 0;
	} else if (i == 0) { /* EOF */
		*pass = 0;
		i = -1;
	}
	return i;
}

static int timed_read(int fd, char *pass, size_t maxlen, long timeout)
{
	struct timeval t;
	fd_set fds;
	int failed = -1;

	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	t.tv_sec = timeout;
	t.tv_usec = 0;

	if (select(fd+1, &fds, NULL, NULL, &t) > 0)
		failed = untimed_read(fd, pass, maxlen);
	else
		fprintf(stderr, "Operation timed out.\n");
	return failed;
}

static int interactive_pass(const char *prompt, char *pass, size_t maxlen,
		long timeout)
{
	struct termios orig, tmp;
	int failed = -1;
	int infd, outfd;

	if (maxlen < 1)
		goto out_err;

	/* Read and write to /dev/tty if available */
	if ((infd = outfd = open("/dev/tty", O_RDWR)) == -1) {
		infd = STDIN_FILENO;
		outfd = STDERR_FILENO;
	}

	if (tcgetattr(infd, &orig)) {
		set_error("Unable to get terminal");
		goto out_err;
	}
	memcpy(&tmp, &orig, sizeof(tmp));
	tmp.c_lflag &= ~ECHO;

	write(outfd, prompt, strlen(prompt));
	tcsetattr(infd, TCSAFLUSH, &tmp);
	if (timeout)
		failed = timed_read(infd, pass, maxlen, timeout);
	else
		failed = untimed_read(infd, pass, maxlen);
	tcsetattr(infd, TCSAFLUSH, &orig);

out_err:
	if (!failed)
		write(outfd, "\n", 1);
	if (infd != STDIN_FILENO)
		close(infd);
	return failed;
}

/*
 * Password reading behaviour matrix of get_key
 * 
 *                    p   v   n   h
 * -----------------+---+---+---+---
 * interactive      | Y | Y | Y | Inf
 * from fd          | N | N | Y | Inf
 * from binary file | N | N | N | Inf or options->key_size
 *
 * Legend: p..prompt, v..can verify, n..newline-stop, h..read horizon
 *
 * Note: --key-file=- is interpreted as a read from a binary file (stdin)
 *
 * Returns true when more keys are available (that is when password
 * reading can be retried as for interactive terminals).
 */

static int get_key(struct crypt_options *options, char *prompt, char **key, int *passLen)
{
	int fd;
	const int verify = options->flags & CRYPT_FLAG_VERIFY;
	const int verify_if_possible = options->flags & CRYPT_FLAG_VERIFY_IF_POSSIBLE;
	char *pass = NULL;
	int newline_stop;
	int read_horizon;
	
	if(options->key_file && !strcmp(options->key_file, "-")) {
		/* Allow binary reading from stdin */
		fd = options->passphrase_fd;
		newline_stop = 0;
		read_horizon = 0;
	} else if (options->key_file) {
		fd = open(options->key_file, O_RDONLY);
		if (fd < 0) {
			char buf[128];
			set_error("Error opening key file: %s",
				  strerror_r(errno, buf, 128));
			goto out_err;
		}
		newline_stop = 0;

		/* This can either be 0 (LUKS) or the actually number
		 * of key bytes (default or passed by -s) */
		read_horizon = options->key_size;
	} else {
		fd = options->passphrase_fd;
		newline_stop = 1;
		read_horizon = 0;   /* Infinite, if read from terminal or fd */
	}	

	/* Interactive case */
	if(isatty(fd)) {
		int i;

		pass = safe_alloc(512);
		if (!pass || (i = interactive_pass(prompt, pass, 512, options->timeout))) {
			set_error("Error reading passphrase");
			goto out_err;
		}
		if (verify || verify_if_possible) {
			char pass_verify[512];
			i = interactive_pass("Verify passphrase: ", pass_verify, sizeof(pass_verify), options->timeout);
			if (i || strcmp(pass, pass_verify) != 0) {
				set_error("Passphrases do not match");
				goto out_err;
			}
			memset(pass_verify, 0, sizeof(pass_verify));
		}
		*passLen = strlen(pass);
		*key = pass;
	} else {
		/* 
		 * This is either a fd-input or a file, in neither case we can verify the input,
		 * however we don't stop on new lines if it's a binary file.
		 */
		int buflen, i;

		if(verify) {
			set_error("Can't do passphrase verification on non-tty inputs");
			goto out_err;
		}
		/* The following for control loop does an exhausting
		 * read on the key material file, if requested with
		 * key_size == 0, as it's done by LUKS. However, we
		 * should warn the user, if it's a non-regular file,
		 * such as /dev/random, because in this case, the loop
		 * will read forever.
		 */ 
		if(options->key_file && strcmp(options->key_file, "-") && read_horizon == 0) {
			struct stat st;
			if(stat(options->key_file, &st) < 0) {
		 		set_error("Can't stat key file");
				goto out_err;
			}
			if(!S_ISREG(st.st_mode)) {
				//		 		set_error("Can't do exhausting read on non regular files");
				// goto out_err;
				fprintf(stderr,"Warning: exhausting read requested, but key file is not a regular file, function might never return.\n");
			}
		}
		buflen = 0;
		for(i = 0; read_horizon == 0 || i < read_horizon; i++) {
			if(i >= buflen - 1) {
				buflen += 128;
				pass = safe_realloc(pass, buflen);
				if (!pass) {
					set_error("Not enough memory while "
					          "reading passphrase");
					goto out_err;
				}
			}
			if(read(fd, pass + i, 1) != 1 || (newline_stop && pass[i] == '\n'))
				break;
		}
		if(options->key_file)
			close(fd);
		pass[i] = 0;
		*key = pass;
		*passLen = i;
	}

	return isatty(fd); /* Return true, when password reading can be tried on interactive fds */

out_err:
	if(pass)
		safe_free(pass);
	*key = NULL;
	*passLen = 0;
	return 0;
}

/*
 * Password processing behaviour matrix of process_key
 * 
 * from binary file: check if there is sufficently large key material
 * interactive & from fd: hash if requested, otherwise crop or pad with '0'
 */

static char *process_key(struct crypt_options *options, char *pass, int passLen) {
	char *key = safe_alloc(options->key_size);
	memset(key, 0, options->key_size);

	/* key is coming from binary file */
	if (options->key_file && strcmp(options->key_file, "-")) {
		if(passLen < options->key_size) {
			set_error("Could not read %d bytes from key file",
			          options->key_size);
			safe_free(key);
			return NULL;
		} 
		memcpy(key,pass,options->key_size);
		return key;
	}
	
	/* key is coming from tty, fd or binary stdin */
	if (options->hash) {
		if (hash(NULL, options->hash,
			 key, options->key_size,
			 pass, passLen) < 0)
		{
			safe_free(key);
			return NULL;
		}
	} else if (passLen > options->key_size) {
			memcpy(key, pass, options->key_size);
	} else {
			memcpy(key, pass, passLen);
	}

	return key;
}

static int get_device_infos(const char *device, struct device_infos *infos)
{
	char buf[128];
	uint64_t size;
	unsigned long size_small;
	int readonly;
	int ret = -1;
	int fd;

	fd = open(device, O_RDONLY);
	if (fd < 0) {
		set_error("Error opening device: %s",
		          strerror_r(errno, buf, 128));
		return -1;
	}

#ifdef BLKROGET
	if (ioctl(fd, BLKROGET, &readonly) < 0) {
		set_error("BLKROGET failed on device: %s",
		          strerror_r(errno, buf, 128));
		return -1;
	}
#else
#	error BLKROGET not available
#endif

#ifdef BLKGETSIZE64
	if (ioctl(fd, BLKGETSIZE64, &size) >= 0) {
		size >>= SECTOR_SHIFT;
		ret = 0;
		goto out;
	}
#endif

#ifdef BLKGETSIZE
	if (ioctl(fd, BLKGETSIZE, &size_small) >= 0) {
		size = (uint64_t)size_small;
		ret = 0;
		goto out;
	}
#else
#	error Need at least the BLKGETSIZE ioctl!
#endif

	set_error("BLKGETSIZE ioctl failed on device: %s",
	          strerror_r(errno, buf, 128));

out:
	if (ret == 0) {
		infos->size = size;
		infos->readonly = readonly;
	}
	close(fd);
	return ret;
}

static int parse_into_name_and_mode(const char *nameAndMode, char *name,
				    char *mode)
{
	// Token content stringification, see info cpp/stringification
#define str(s) #s
#define xstr(s) str(s)
#define scanpattern1 "%" xstr(LUKS_CIPHERNAME_L) "[^-]-%" xstr(LUKS_CIPHERMODE_L)  "s"
#define scanpattern2 "%" xstr(LUKS_CIPHERNAME_L) "[^-]"

	int r;

	if(sscanf(nameAndMode,scanpattern1, name, mode) != 2) {
		if((r = sscanf(nameAndMode,scanpattern2,name)) == 1) {
			strncpy(mode,"cbc-plain",10);
		} 
		else {
			fprintf(stderr, "no known cipher-spec pattern detected\n");
			return -EINVAL;
		}
	}

	return 0;

#undef sp1
#undef sp2
#undef str
#undef xstr
}
static int __crypt_create_device(int reload, struct setup_backend *backend,
                                 struct crypt_options *options)
{
	struct crypt_options tmp = {
		.name = options->name,
	};
	struct device_infos infos;
	char *key = NULL;
	int keyLen;
	char *processed_key = NULL;
	int r;

	r = backend->status(0, &tmp, NULL);
	if (reload) {
		if (r < 0)
			return r;
	} else {
		if (r >= 0) {
			set_error("Device already exists");
			return -EEXIST;
		}
		if (r != -ENODEV)
			return r;
	}

	if (options->key_size < 0 || options->key_size > 1024) {
		set_error("Invalid key size");
		return -EINVAL;
	}

	if (get_device_infos(options->device, &infos) < 0)
		return -ENOTBLK;

	if (!options->size) {
		options->size = infos.size;
		if (!options->size) {
			set_error("Not a block device");
			return -ENOTBLK;
		}
		if (options->size <= options->offset) {
			set_error("Invalid offset");
			return -EINVAL;
		}
		options->size -= options->offset;
	}

	if (infos.readonly)
		options->flags |= CRYPT_FLAG_READONLY;

	get_key(options, "Enter passphrase: ", &key, &keyLen);
	if (!key) {
		set_error("Key reading error");
		return -ENOENT;
	}
	
	processed_key = process_key(options,key,keyLen);
	safe_free(key);
	
	if (!processed_key) {
		const char *error=get_error();
		if(error) {
			char *c_error_handling_sucks;
			asprintf(&c_error_handling_sucks,"Key processing error: %s",error);
			set_error(c_error_handling_sucks);
			free(c_error_handling_sucks);
		} else
			set_error("Key processing error");
		return -ENOENT;
	}
	
	r = backend->create(reload, options, processed_key);
	
	safe_free(processed_key);

	return r;
}

static int __crypt_query_device(int details, struct setup_backend *backend,
                                struct crypt_options *options)
{
	int r = backend->status(details, options, NULL);
	if (r == -ENODEV)
		return 0;
	else if (r >= 0)
		return 1;
	else
		return r;
}

static int __crypt_resize_device(int details, struct setup_backend *backend,
                                struct crypt_options *options)
{
	struct crypt_options tmp = {
		.name = options->name,
	};
	struct device_infos infos;
	char *key = NULL;
	int r;

	r = backend->status(1, &tmp, &key);
	if (r < 0)
		return r;

	if (get_device_infos(tmp.device, &infos) < 0)
		return -EINVAL;

	if (!options->size) {
		options->size = infos.size;
		if (!options->size) {
			set_error("Not a block device");
			return -ENOTBLK;
		}
		if (options->size <= tmp.offset) {
			set_error("Invalid offset");
			return -EINVAL;
		}
		options->size -= tmp.offset;
	}
	tmp.size = options->size;

	if (infos.readonly)
		options->flags |= CRYPT_FLAG_READONLY;

	r = backend->create(1, &tmp, key);

	safe_free(key);

	return r;
}

static int __crypt_remove_device(int arg, struct setup_backend *backend,
                                 struct crypt_options *options)
{
	int r;

	r = backend->status(0, options, NULL);
	if (r < 0)
		return r;
	if (r > 0) {
		set_error("Device busy");
		return -EBUSY;
	}

	return backend->remove(options);
}

static int __crypt_luks_format(int arg, struct setup_backend *backend, struct crypt_options *options)
{
	int r;
	
	struct luks_phdr header;
	struct luks_masterkey *mk=NULL;
	char *password; 
	char cipherName[LUKS_CIPHERNAME_L];
	char cipherMode[LUKS_CIPHERMODE_L];
	int passwordLen;
	int PBKDF2perSecond;
	
	mk = LUKS_generate_masterkey(options->key_size);
	if(NULL == mk) return -ENOMEM; 

#ifdef LUKS_DEBUG
#define printoffset(entry) printf("offset of " #entry " = %d\n", (char *)(&header.entry)-(char *)(&header))

	printf("sizeof phdr %d, key slot %d\n",sizeof(struct luks_phdr),sizeof(header.keyblock[0]));

	printoffset(magic);
	printoffset(version);
	printoffset(cipherName);
	printoffset(cipherMode);
	printoffset(hashSpec);
	printoffset(payloadOffset);
	printoffset(keyBytes);
	printoffset(mkDigest);
	printoffset(mkDigestSalt);
	printoffset(mkDigestIterations);
	printoffset(uuid);
#endif
	r = parse_into_name_and_mode(options->cipher, cipherName, cipherMode);
	if(r < 0) return r;

	r = LUKS_generate_phdr(&header,mk,cipherName, cipherMode,LUKS_STRIPES, options->align_payload);
	if(r < 0) { 
		set_error("Can't write phdr");
		return r; 
	}

	PBKDF2perSecond = LUKS_benchmarkt_iterations();
	header.keyblock[0].passwordIterations = at_least_one(PBKDF2perSecond * ((float)options->iteration_time / 1000.0));
#ifdef LUKS_DEBUG
	fprintf(stderr, "pitr %d\n", header.keyblock[0].passwordIterations);
#endif
	options->key_size = 0; // FIXME, define a clean interface some day.
	options->key_file = options->new_key_file;
	options->new_key_file = NULL;
	get_key(options,"Enter LUKS passphrase: ",&password,&passwordLen);
	if(!password) {
		r = -EINVAL; goto out;
	}
	r = LUKS_set_key(options->device, 0, password, passwordLen, &header, mk, backend);
	if(r < 0) goto out; 

	r = 0;
out:
	LUKS_dealloc_masterkey(mk);
	safe_free(password);
	return r;
}

static int __crypt_luks_open(int arg, struct setup_backend *backend, struct crypt_options *options)
{
	struct luks_masterkey *mk;
	struct luks_phdr hdr;
	char *password; int passwordLen;
	struct device_infos infos;
	struct crypt_options tmp = {
		.name = options->name,
	};
	char *dmCipherSpec;
	int r, tries = options->tries;
	
	r = backend->status(0, &tmp, NULL);
	if (r >= 0) {
		set_error("Device already exists");
		return -EEXIST;
	}

	if (get_device_infos(options->device, &infos) < 0) {
		set_error("Can't get device information.\n");
		r = -ENOTBLK; goto out;
	}
	if (infos.readonly)
		options->flags |= CRYPT_FLAG_READONLY;

start:
	mk=NULL;
	options->key_size = 0; // FIXME, define a clean interface some day.

	if(get_key(options,"Enter LUKS passphrase: ",&password,&passwordLen))
		tries--;
	else
		tries = 0;

	if(!password) {
		r = -EINVAL; goto out;
	}
	if((r = LUKS_open_any_key(options->device, password, passwordLen, &hdr, &mk, backend)) < 0) {
		set_error("No key available with this passphrase.\n");
		goto out1;
	}
	
	options->offset = hdr.payloadOffset;
 	asprintf(&dmCipherSpec, "%s-%s", hdr.cipherName, hdr.cipherMode);
	if(!dmCipherSpec) {
		r = -ENOMEM;
		goto out2;
	}
	options->cipher = dmCipherSpec;
	options->key_size = mk->keyLength;
	options->skip = 0;

	options->size = infos.size;
	if (!options->size) {
		set_error("Not a block device.\n");
		r = -ENOTBLK; goto out2;
	}
	if (options->size <= options->offset) {
		set_error("Invalid offset");
		r = -EINVAL; goto out2;
	}
	options->size -= options->offset;
	r = backend->create(0, options, mk->key);

 out2:
	free(dmCipherSpec);
 out1:
	safe_free(password);
 out:
	LUKS_dealloc_masterkey(mk);
	if (r == -EPERM && tries > 0)
		goto start;

	return r;
}

static int __crypt_luks_add_key(int arg, struct setup_backend *backend, struct crypt_options *options)
{
	struct luks_masterkey *mk=NULL;
	struct luks_phdr hdr;
	char *password; unsigned int passwordLen;
	unsigned int i; unsigned int keyIndex;
	const char *device = options->device;
	struct crypt_options optionsCheck = { 
		.key_file = options->key_file,
		.flags = options->flags & ~(CRYPT_FLAG_VERIFY | CRYPT_FLAG_VERIFY_IF_POSSIBLE),
	};
	struct crypt_options optionsSet = { 
		.key_file = options->new_key_file,
		.flags = options->flags,
	};
	int r;
	
	r = LUKS_read_phdr(device, &hdr);
	if(r < 0) return r;

	/* Find empty key slot */
	for(i=0; i<LUKS_NUMKEYS; i++) {
		if(hdr.keyblock[i].active == LUKS_KEY_DISABLED) break;
	}
	if(i==LUKS_NUMKEYS) {
		set_error("All slots full");
		return -EINVAL;
	}
	keyIndex = i;
	
	optionsCheck.key_size = 0; // FIXME, define a clean interface some day.
	get_key(&optionsCheck,"Enter any LUKS passphrase: ",&password,&passwordLen);
	if(!password) {
		r = -EINVAL; goto out;
	}
	if(LUKS_open_any_key(device, password, passwordLen, &hdr, &mk, backend) < 0) {
		printf("No key available with this passphrase.\n");
		r = -EPERM; goto out;
	}
	safe_free(password);
	
	optionsSet.key_size = 0; // FIXME, define a clean interface some day.
	get_key(&optionsSet,"Enter new passphrase for key slot: ",&password,&passwordLen);
	if(!password) {
		r = -EINVAL; goto out;
	}

	hdr.keyblock[keyIndex].passwordIterations = at_least_one(LUKS_benchmarkt_iterations() * ((float)options->iteration_time / 1000));

    	r = LUKS_set_key(device, keyIndex, password, passwordLen, &hdr, mk, backend);
	if(r < 0) goto out;

	r = 0;
out:
	safe_free(password);
	LUKS_dealloc_masterkey(mk);
	return r;
}

static int __crypt_luks_del_key(int arg, struct setup_backend *backend, struct crypt_options *options)
{
	struct luks_masterkey *mk;
	struct luks_phdr hdr;
	char *password=NULL; 
	unsigned int passwordLen;
	const char *device = options->device;
	int keyIndex = options->key_slot;
	int openedIndex;
	int r;
	
	if(options->flags & CRYPT_FLAG_VERIFY_ON_DELKEY) {
		options->flags &= ~CRYPT_FLAG_VERIFY_ON_DELKEY;
		options->key_size = 0; // FIXME, define a clean interface some day.
		get_key(options,"Enter any remaining LUKS passphrase: ",&password,&passwordLen);
		if(!password) {
			r = -EINVAL; goto out;
		}
		openedIndex = LUKS_open_any_key(device, password, passwordLen, &hdr, &mk, backend);
		if(openedIndex < 0 || keyIndex == openedIndex) {
			printf("No remaining key available with this passphrase.\n");
			r = -EPERM; goto out;
		}
	}
	r = LUKS_del_key(device, keyIndex);
	if(r < 0) goto out;

	r = 0;
out:
	safe_free(password);
	return r;
}


static int crypt_job(int (*job)(int arg, struct setup_backend *backend,
                                struct crypt_options *options),
                     int arg, struct crypt_options *options)
{
	struct setup_backend *backend;
	int r;

	backend = get_setup_backend(default_backend);

	setup_enter(backend);

	if (!backend) {
		set_error("No setup backend available");
		r = -ENOSYS;
		goto out;
	}

	r = job(arg, backend, options);
out:
	setup_leave(backend);
	if (backend)
		put_setup_backend(backend);

	if (r >= 0)
		set_error(NULL);

	return r;
}

int crypt_create_device(struct crypt_options *options)
{
	return crypt_job(__crypt_create_device, 0, options);
}

int crypt_update_device(struct crypt_options *options)
{
	return crypt_job(__crypt_create_device, 1, options);
}

int crypt_resize_device(struct crypt_options *options)
{
	return crypt_job(__crypt_resize_device, 0, options);
}

int crypt_query_device(struct crypt_options *options)
{
	return crypt_job(__crypt_query_device, 1, options);
}

int crypt_remove_device(struct crypt_options *options)
{
	return crypt_job(__crypt_remove_device, 0, options);

}

int crypt_luksFormat(struct crypt_options *options)
{
	return crypt_job(__crypt_luks_format, 0, options);
}

int crypt_luksOpen(struct crypt_options *options)
{
	return crypt_job(__crypt_luks_open, 0, options);
}

int crypt_luksDelKey(struct crypt_options *options)
{
	return crypt_job(__crypt_luks_del_key, 0, options);
}

int crypt_luksAddKey(struct crypt_options *options)
{
	return crypt_job(__crypt_luks_add_key, 0, options);
}

int crypt_luksUUID(struct crypt_options *options)
{
	struct luks_phdr hdr;
	int r;

	r = LUKS_read_phdr(options->device,&hdr);
	if(r < 0) return r;

	printf("%s\n",hdr.uuid);
	return 0;
}

int crypt_isLuks(struct crypt_options *options)
{
	struct luks_phdr hdr;
	return LUKS_read_phdr(options->device,&hdr);
}

int crypt_luksDump(struct crypt_options *options)
{
	struct luks_phdr hdr;
	int r,i;

	r = LUKS_read_phdr(options->device,&hdr);
	if(r < 0) return r;

	printf("LUKS header information for %s\n\n",options->device);
    	printf("Version:       \t%d\n",hdr.version);
	printf("Cipher name:   \t%s\n",hdr.cipherName);
	printf("Cipher mode:   \t%s\n",hdr.cipherMode);
	printf("Hash spec:     \t%s\n",hdr.hashSpec);
	printf("Payload offset:\t%d\n",hdr.payloadOffset);
	printf("MK bits:       \t%d\n",hdr.keyBytes*8);
	printf("MK digest:     \t");
	hexprint(hdr.mkDigest,LUKS_DIGESTSIZE);
	printf("\n");
	printf("MK salt:       \t");
	hexprint(hdr.mkDigestSalt,LUKS_SALTSIZE/2);
	printf("\n               \t");
	hexprint(hdr.mkDigestSalt+LUKS_SALTSIZE/2,LUKS_SALTSIZE/2);
	printf("\n");
	printf("MK iterations: \t%d\n",hdr.mkDigestIterations);
	printf("UUID:          \t%s\n\n",hdr.uuid);
	for(i=0;i<LUKS_NUMKEYS;i++) {
		if(hdr.keyblock[i].active == LUKS_KEY_ENABLED) {
			printf("Key Slot %d: ENABLED\n",i);
			printf("\tIterations:         \t%d\n",hdr.keyblock[i].passwordIterations);
			printf("\tSalt:               \t");
			hexprint(hdr.keyblock[i].passwordSalt,LUKS_SALTSIZE/2);
			printf("\n\t                      \t");
			hexprint(hdr.keyblock[i].passwordSalt+LUKS_SALTSIZE/2,LUKS_SALTSIZE/2);
			printf("\n");

			printf("\tKey material offset:\t%d\n",hdr.keyblock[i].keyMaterialOffset);
			printf("\tAF stripes:            \t%d\n",hdr.keyblock[i].stripes);
		}		
		else 
			printf("Key Slot %d: DISABLED\n",i);
	}
	return 0;
}


void crypt_get_error(char *buf, size_t size)
{
	const char *error = get_error();

	if (!buf || size < 1)
		set_error(NULL);
	else if (error) {
		strncpy(buf, error, size - 1);
		buf[size - 1] = '\0';
		set_error(NULL);
	} else
		buf[0] = '\0';
}

void crypt_put_options(struct crypt_options *options)
{
	if (options->flags & CRYPT_FLAG_FREE_DEVICE) {
		free((char *)options->device);
		options->device = NULL;
		options->flags &= ~CRYPT_FLAG_FREE_DEVICE;
	}
	if (options->flags & CRYPT_FLAG_FREE_CIPHER) {
		free((char *)options->cipher);
		options->cipher = NULL;
		options->flags &= ~CRYPT_FLAG_FREE_CIPHER;
	}
}

void crypt_set_default_backend(const char *backend)
{
	if (default_backend)
		free(default_backend);
	if (backend) 
		default_backend = strdup(backend);
	else
		default_backend = NULL;
}

const char *crypt_get_dir(void)
{
	struct setup_backend *backend;
	const char *dir;

	backend = get_setup_backend(default_backend);
	if (!backend)
		return NULL;

	dir = backend->dir();

	put_setup_backend(backend);

	return dir;
}
