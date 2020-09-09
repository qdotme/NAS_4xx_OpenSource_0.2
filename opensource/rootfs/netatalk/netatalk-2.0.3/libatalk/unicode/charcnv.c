/* 
   Unix SMB/CIFS implementation.
   Character set conversion Extensions
   Copyright (C) Igor Vergeichik <iverg@mail.ru> 2001
   Copyright (C) Andrew Tridgell 2001
   Copyright (C) Simo Sorce 2001
   Copyright (C) Martin Pool 2003
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/param.h>
#ifdef HAVE_USABLE_ICONV
#include <iconv.h>
#endif
#if HAVE_LOCALE_H
#include <locale.h>
#endif
#if HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#include <netatalk/endian.h>
#include <atalk/logger.h>
#include <atalk/unicode.h>
#include <atalk/util.h>
#include "byteorder.h"


/**
 * @file
 *
 * @brief Character-set conversion routines built on our iconv.
 * 
 * @note Samba's internal character set (at least in the 3.0 series)
 * is always the same as the one for the Unix filesystem.  It is
 * <b>not</b> necessarily UTF-8 and may be different on machines that
 * need i18n filenames to be compatible with Unix software.  It does
 * have to be a superset of ASCII.  All multibyte sequences must start
 * with a byte with the high bit set.
 *
 * @sa lib/iconv.c
 */


#define MAX_CHARSETS 10

#define CHECK_FLAGS(a,b) (((a)!=NULL) ? (*(a) & (b)) : 0 )

static atalk_iconv_t conv_handles[MAX_CHARSETS][MAX_CHARSETS];
static char* charset_names[MAX_CHARSETS];
static struct charset_functions* charsets[MAX_CHARSETS];
static char hexdig[] = "0123456789abcdef";
#define hextoint( c )   ( isdigit( c ) ? c - '0' : c + 10 - 'a' )

static char* read_charsets_from_env(charset_t ch) 
{
	char *name;

	switch (ch) {
	    case CH_MAC:
                if (( name = getenv( "ATALK_MAC_CHARSET" )) != NULL ) 
		    return name;
		else
		    return "MAC_ROMAN";
		break;
            case CH_UNIX:
                if (( name = getenv( "ATALK_UNIX_CHARSET" )) != NULL ) 
		    return name;
		else
		    return "LOCALE";
		break;
	    default:
		break;
        }
        return "ASCII";
} 
           

/**
 * Return the name of a charset to give to iconv().
 **/
static const char *charset_name(charset_t ch)
{
	const char *ret = NULL;
	static int first = 1;
	static char macname[128];
	static char unixname[128];

	if (first) {
		memset(macname, 0, sizeof(macname));
		memset(unixname, 0, sizeof(unixname));
		first = 0;
	}

	if (ch == CH_UCS2) ret = "UCS-2";
	else if (ch == CH_UTF8) ret = "UTF8";
	else if (ch == CH_UTF8_MAC) ret = "UTF8-MAC";
	else if (ch == CH_UNIX) {
		if (unixname[0] == '\0') {
			ret = read_charsets_from_env(CH_UNIX);
			strlcpy(unixname, ret, sizeof(unixname));
		}
		else
			ret = unixname;
	}
	else if (ch == CH_MAC) {
		if (macname[0] == '\0') {
			ret = read_charsets_from_env(CH_MAC);
			strlcpy(macname, ret, sizeof(macname));
		}
		else
			ret = macname;
	}

	if (!ret)
		ret = charset_names[ch];

#if defined(HAVE_NL_LANGINFO) && defined(CODESET)
	if (ret && strcasecmp(ret, "LOCALE") == 0) {
		const char *ln = NULL;

#ifdef HAVE_SETLOCALE
		setlocale(LC_ALL, "");
#endif
		ln = nl_langinfo(CODESET);
		if (ln) {
			/* Check whether the charset name is supported
			   by iconv */
			atalk_iconv_t handle = atalk_iconv_open(ln, "UCS-2");
			if (handle == (atalk_iconv_t) -1) {
				LOG(log_debug, logtype_default, "Locale charset '%s' unsupported, using ASCII instead", ln);
				ln = "ASCII";
			} else {
				atalk_iconv_close(handle);
			}
			if (ch==CH_UNIX)
				strlcpy(unixname, ln, sizeof(unixname));
		}
		ret = ln;
	}
#else /* system doesn't have LOCALE support */
if (ch == CH_UNIX) ret = NULL;
#endif

	if (!ret || !*ret) ret = "ASCII";
	return ret;
}

struct charset_functions* get_charset_functions (charset_t ch)
{
	if (charsets[ch] != NULL)
		return charsets[ch];

	charsets[ch] = find_charset_functions(charset_name(ch));

	return charsets[ch];
}
	

void lazy_initialize_conv(void)
{
	static int initialized = 0;

	if (!initialized) {
		initialized = 1;
		init_iconv();
	}
}

charset_t add_charset(char* name)
{
	static charset_t max_charset_t = NUM_CHARSETS-1;
	charset_t cur_charset_t = max_charset_t+1;
	unsigned int c1;

	lazy_initialize_conv();

	for (c1=0; c1<=max_charset_t;c1++) {
		if ( strcasecmp(name, charset_name(c1)) == 0)
			return (c1);
	}

	if ( cur_charset_t >= MAX_CHARSETS )  {
		LOG (log_debug, logtype_default, "Adding charset %s failed, too many charsets (max. %u allowed)", 
			name, MAX_CHARSETS);
		return (charset_t) -1;
	}

	/* First try to setup the required conversions */

	conv_handles[cur_charset_t][CH_UCS2] = atalk_iconv_open( charset_name(CH_UCS2), name);
        if (conv_handles[cur_charset_t][CH_UCS2] == (atalk_iconv_t)-1) {
		LOG(log_error, logtype_default, "Required conversion from %s to %s not supported",
			name,  charset_name(CH_UCS2));
		conv_handles[cur_charset_t][CH_UCS2] = NULL;
		return (charset_t) -1;
	}

	conv_handles[CH_UCS2][cur_charset_t] = atalk_iconv_open( name, charset_name(CH_UCS2));
        if (conv_handles[CH_UCS2][cur_charset_t] == (atalk_iconv_t)-1) {
		LOG(log_error, logtype_default, "Required conversion from %s to %s not supported",
			charset_name(CH_UCS2), name);
		conv_handles[CH_UCS2][cur_charset_t] = NULL;
		return (charset_t) -1;
	}

	/* register the new charset_t name */
	charset_names[cur_charset_t] = strdup(name);

	charsets[cur_charset_t] = get_charset_functions (cur_charset_t);
	max_charset_t++;

#ifdef DEBUG
	LOG(log_debug, logtype_default, "Added charset %s with handle %u", name, cur_charset_t);
#endif /* DEBUG */
	return (cur_charset_t);
}

/**
 * Initialize iconv conversion descriptors.
 *
 * This is called the first time it is needed, and also called again
 * every time the configuration is reloaded, because the charset or
 * codepage might have changed.
 **/
void init_iconv(void)
{
	int c1;

	for (c1=0;c1<NUM_CHARSETS;c1++) {
		const char *name = charset_name((charset_t)c1);

		conv_handles[c1][CH_UCS2] = atalk_iconv_open( charset_name(CH_UCS2), name);
	        if (conv_handles[c1][CH_UCS2] == (atalk_iconv_t)-1) {
			LOG(log_error, logtype_default, "Required conversion from %s to %s not supported",
				name,  charset_name(CH_UCS2));
			conv_handles[c1][CH_UCS2] = NULL;
		}

		if (c1 != CH_UCS2) { /* avoid lost memory, make valgrind happy */
			conv_handles[CH_UCS2][c1] = atalk_iconv_open( name, charset_name(CH_UCS2));
		      	if (conv_handles[CH_UCS2][c1] == (atalk_iconv_t)-1) {
				LOG(log_error, logtype_default, "Required conversion from %s to %s not supported",
					charset_name(CH_UCS2), name);
				conv_handles[CH_UCS2][c1] = NULL;
			}
		}
		
		charsets[c1] = get_charset_functions (c1);
	}
}

/**
 * Convert string from one encoding to another, making error checking etc
 *
 * @param src pointer to source string (multibyte or singlebyte)
 * @param srclen length of the source string in bytes
 * @param dest pointer to destination string (multibyte or singlebyte)
 * @param destlen maximal length allowed for string
 * @returns the number of bytes occupied in the destination
 **/
static size_t convert_string_internal(charset_t from, charset_t to,
		      void const *src, size_t srclen, 
		      void *dest, size_t destlen)
{
	size_t i_len, o_len;
	size_t retval;
	const char* inbuf = (const char*)src;
	char* outbuf = (char*)dest;
	char* o_save = outbuf;
	atalk_iconv_t descriptor;

	if (srclen == (size_t)-1)
		srclen = strlen(src)+1;

	lazy_initialize_conv();

	descriptor = conv_handles[from][to];

	if (descriptor == (atalk_iconv_t)-1 || descriptor == (atalk_iconv_t)0) {
		return (size_t) -1;
	}

	i_len=srclen;
	o_len=destlen;
	retval = atalk_iconv(descriptor,  &inbuf, &i_len, &outbuf, &o_len);
	if(retval==(size_t)-1) {
	    	const char *reason="unknown error";
		switch(errno) {
			case EINVAL:
				reason="Incomplete multibyte sequence";
				break;
			case E2BIG:
				reason="No more room"; 
		               break;
			case EILSEQ:
			       reason="Illegal multibyte sequence";
			       break;
		}
		LOG(log_debug, logtype_default,"Conversion error: %s",reason);
		return (size_t)-1;
	}

	/* Terminate the string */
	if (to == CH_UCS2 && destlen-o_len >= 2) {
		o_save[destlen-o_len]   = 0;
		o_save[destlen-o_len+1] = 0;
	}
	else if ( to != CH_UCS2 && destlen-o_len > 0 )
		o_save[destlen-o_len] = 0;
	else {
		/* FIXME: what should we do here, string *might* be unterminated. E2BIG? */
	}

	return destlen-o_len;
}


size_t convert_string(charset_t from, charset_t to,
		      void const *src, size_t srclen, 
		      void *dest, size_t destlen)
{
	size_t i_len, o_len;
	ucs2_t *u;
 	ucs2_t buffer[MAXPATHLEN];
 	ucs2_t buffer2[MAXPATHLEN];
	int composition = 0;

	lazy_initialize_conv();

	/* convert from_set to UCS2 */
 	if ((size_t)(-1) == ( o_len = convert_string_internal( from, CH_UCS2, src, srclen, 
                                                               (char*) buffer, sizeof(buffer))) ) {
		LOG(log_error, logtype_default, "Conversion failed ( %s to CH_UCS2 )", charset_name(from));
		return (size_t) -1;
	}

	/* Do pre/decomposition */
	if ( ((!(charsets[to])   || !(charsets[to]->flags & CHARSET_DECOMPOSED)) && 
		(!(charsets[from]) || (charsets[from]->flags & CHARSET_DECOMPOSED))))
 	    composition = 1;
 	if ((charsets[to] && charsets[to]->flags & CHARSET_DECOMPOSED) )
 	    composition = 2;
 
	i_len = sizeof(buffer2);
	u = buffer2;

	switch (composition) {
	case 0:
 	    u = buffer;
 	    i_len = o_len;
	    break;
	case 1:
            if ( (size_t)-1 == (i_len = precompose_w(buffer, o_len, u, &i_len)) )
 	        return (size_t)(-1);
	    break;
	case 2:
            if ( (size_t)-1 == (i_len = decompose_w(buffer, o_len, u, &i_len)) )
 	        return (size_t)(-1);
	    break;
	}
		
	/* Convert UCS2 to to_set */
	if ((size_t)(-1) == ( o_len = convert_string_internal( CH_UCS2, to, (char*) u, i_len, dest, destlen)) ) {
		LOG(log_error, logtype_default, "Conversion failed (CH_UCS2 to %s):%s", charset_name(to), strerror(errno));
		return (size_t) -1;
	}

	return o_len;
}	

	

/**
 * Convert between character sets, allocating a new buffer for the result.
 *
 * @param srclen length of source buffer.
 * @param dest always set at least to NULL
 * @note -1 is not accepted for srclen.
 *
 * @returns Size in bytes of the converted string; or -1 in case of error.
 **/

static size_t convert_string_allocate_internal(charset_t from, charset_t to,
			       void const *src, size_t srclen, char **dest)
{
	size_t i_len, o_len, destlen;
	size_t retval;
	const char *inbuf = (const char *)src;
	char *outbuf = NULL, *ob = NULL;
	atalk_iconv_t descriptor;

	*dest = NULL;

	if (src == NULL || srclen == (size_t)-1)
		return (size_t)-1;

	lazy_initialize_conv();

	descriptor = conv_handles[from][to];

	if (descriptor == (atalk_iconv_t)-1 || descriptor == (atalk_iconv_t)0) {
		/* conversion not supported, return -1*/
		LOG(log_debug, logtype_default, "convert_string_allocate: conversion not supported!");
		return -1;
	}

	destlen = MAX(srclen, 512);
convert:
	destlen = destlen * 2;
	outbuf = (char *)realloc(ob, destlen);
	if (!outbuf) {
		LOG(log_debug, logtype_default,"convert_string_allocate: realloc failed!");
		SAFE_FREE(ob);
		return (size_t)-1;
	} else {
		ob = outbuf;
	}
	inbuf = src;   /* this restarts the whole conversion if buffer needed to be increased */
	i_len = srclen;
	o_len = destlen;
	retval = atalk_iconv(descriptor,
			   &inbuf, &i_len,
			   &outbuf, &o_len);
	if(retval == (size_t)-1) 		{
	    	const char *reason="unknown error";
		switch(errno) {
			case EINVAL:
				reason="Incomplete multibyte sequence";
				break;
			case E2BIG:
				goto convert;		
			case EILSEQ:
				reason="Illegal multibyte sequence";
				break;
		}
		LOG(log_debug, logtype_default,"Conversion error: %s(%s)",reason,inbuf);
		SAFE_FREE(ob);
		return (size_t)-1;
	}

	
	destlen = destlen - o_len;

	/* Terminate the string */
	if (to == CH_UCS2 && o_len >= 2) {
		ob[destlen] = 0;
		ob[destlen+1] = 0;
		*dest = (char *)realloc(ob,destlen+2);
	}
	else if ( to != CH_UCS2 && o_len > 0 ) {
		ob[destlen] = 0;
		*dest = (char *)realloc(ob,destlen+1);
	}
	else {
		goto convert; /* realloc */
	}

	if (destlen && !*dest) {
		LOG(log_debug, logtype_default, "convert_string_allocate: out of memory!");
		SAFE_FREE(ob);
		return (size_t)-1;
	}

	return destlen;
}


size_t convert_string_allocate(charset_t from, charset_t to,
		      void const *src, size_t srclen, 
		      char ** dest)
{
	size_t i_len, o_len;
	ucs2_t *u;
 	ucs2_t buffer[MAXPATHLEN];
 	ucs2_t buffer2[MAXPATHLEN];
	int composition = 0;

	lazy_initialize_conv();

	*dest = NULL;

	/* convert from_set to UCS2 */
 	if ((size_t)(-1) == ( o_len = convert_string_internal( from, CH_UCS2, src, srclen, 
                                                               buffer, sizeof(buffer))) ) {
		LOG(log_error, logtype_default, "Conversion failed ( %s to CH_UCS2 )", charset_name(from));
		return (size_t) -1;
	}

	/* Do pre/decomposition */
	if ( ((!(charsets[to])   || !(charsets[to]->flags & CHARSET_DECOMPOSED)) && 
		(!(charsets[from]) || (charsets[from]->flags & CHARSET_DECOMPOSED))))
 	    composition = 1;
 	if ((charsets[to] && charsets[to]->flags & CHARSET_DECOMPOSED) )
 	    composition = 2;
 
	i_len = sizeof(buffer2);
	u = buffer2;

	switch (composition) {
	case 0:
 	    u = buffer;
 	    i_len = o_len;
	    break;
	case 1:
            if ( (size_t)-1 == (i_len = precompose_w(buffer, o_len, u, &i_len)) )
 	        return (size_t)(-1);
	    break;
	case 2:
            if ( (size_t)-1 == (i_len = decompose_w(buffer, o_len, u, &i_len)) )
 	        return (size_t)(-1);
	    break;
	}
		
	/* Convert UCS2 to to_set */
	if ((size_t)(-1) == ( o_len = convert_string_allocate_internal( CH_UCS2, to, (char*)u, i_len, dest)) ) 
		LOG(log_error, logtype_default, "Conversion failed (CH_UCS2 to %s):%s", charset_name(to), strerror(errno));
		
	return o_len;

}

size_t charset_strupper(charset_t ch, const char *src, size_t srclen, char *dest, size_t destlen)
{
	size_t size;
	char *buffer;
	
	size = convert_string_allocate_internal(ch, CH_UCS2, src, srclen,
				       (char**) &buffer);
	if (size == (size_t)-1) {
		SAFE_FREE(buffer);
		return size;
	}
	if (!strupper_w((ucs2_t *)buffer) && (dest == src)) {
		free(buffer);
		return srclen;
	}
	
	size = convert_string_internal(CH_UCS2, ch, buffer, size, dest, destlen);
	free(buffer);
	return size;
}

size_t charset_strlower(charset_t ch, const char *src, size_t srclen, char *dest, size_t destlen)
{
	size_t size;
	char *buffer;
	
	size = convert_string_allocate_internal(ch, CH_UCS2, src, srclen,
				       (char **) &buffer);
	if (size == (size_t)-1) {
		SAFE_FREE(buffer);
		return size;
	}
	if (!strlower_w((ucs2_t *)buffer) && (dest == src)) {
		free(buffer);
		return srclen;
	}
	
	size = convert_string_internal(CH_UCS2, ch, buffer, size, dest, destlen);
	free(buffer);
	return size;
}


size_t unix_strupper(const char *src, size_t srclen, char *dest, size_t destlen)
{
	return charset_strupper( CH_UNIX, src, srclen, dest, destlen);
}

size_t unix_strlower(const char *src, size_t srclen, char *dest, size_t destlen)
{
	return charset_strlower( CH_UNIX, src, srclen, dest, destlen);
}

size_t utf8_strupper(const char *src, size_t srclen, char *dest, size_t destlen)
{
	return charset_strupper( CH_UTF8, src, srclen, dest, destlen);
}

size_t utf8_strlower(const char *src, size_t srclen, char *dest, size_t destlen)
{
	return charset_strlower( CH_UTF8, src, srclen, dest, destlen);
}

/**
 * Copy a string from a charset_t char* src to a UCS2 destination, allocating a buffer
 *
 * @param dest always set at least to NULL 
 *
 * @returns The number of bytes occupied by the string in the destination
 *         or -1 in case of error.
 **/

size_t charset_to_ucs2_allocate(charset_t ch, ucs2_t **dest, const char *src)
{
	size_t src_len = strlen(src);

	*dest = NULL;
	return convert_string_allocate(ch, CH_UCS2, src, src_len, (char**) dest);	
}

/**
 * Copy a string from a charset_t char* src to a UTF-8 destination, allocating a buffer
 *
 * @param dest always set at least to NULL 
 *
 * @returns The number of bytes occupied by the string in the destination
 **/

size_t charset_to_utf8_allocate(charset_t ch, char **dest, const char *src)
{
	size_t src_len = strlen(src);

	*dest = NULL;
	return convert_string_allocate(ch, CH_UTF8, src, src_len, dest);	
}

/**
 * Copy a string from a UCS2 src to a unix char * destination, allocating a buffer
 *
 * @param dest always set at least to NULL 
 *
 * @returns The number of bytes occupied by the string in the destination
 **/

size_t ucs2_to_charset(charset_t ch, const ucs2_t *src, char *dest, size_t destlen)
{
	size_t src_len = (strlen_w(src)) * sizeof(ucs2_t);
	return convert_string(CH_UCS2, ch, src, src_len, dest, destlen);	
}


size_t ucs2_to_charset_allocate(charset_t ch, char **dest, const ucs2_t *src)
{
	size_t src_len = (strlen_w(src)) * sizeof(ucs2_t);
	*dest = NULL;
	return convert_string_allocate(CH_UCS2, ch, src, src_len, dest);	
}

/**
 * Copy a string from a UTF-8 src to a unix char * destination, allocating a buffer
 *
 * @param dest always set at least to NULL 
 *
 * @returns The number of bytes occupied by the string in the destination
 **/

size_t utf8_to_charset_allocate(charset_t ch, char **dest, const char *src)
{
	size_t src_len = strlen(src);
	*dest = NULL;
	return convert_string_allocate(CH_UTF8, ch, src, src_len, dest);	
}

size_t charset_precompose ( charset_t ch, char * src, size_t inlen, char * dst, size_t outlen)
{
	char *buffer;
	ucs2_t u[MAXPATHLEN];
	size_t len;
	size_t ilen;

        if ((size_t)(-1) == (len = convert_string_allocate_internal(ch, CH_UCS2, src, inlen, &buffer)) )
            return len;

	ilen=sizeof(u);

	if ( (size_t)-1 == (ilen = precompose_w((ucs2_t *)buffer, len, u, &ilen)) ) {
	    free (buffer);
	    return (size_t)(-1);
	}

        if ((size_t)(-1) == (len = convert_string_internal( CH_UCS2, ch, (char*)u, ilen, dst, outlen)) ) {
	    free (buffer);
	    return (size_t)(-1);
	}
	
	free(buffer);
	dst[len] = 0;
	return (len);
}

size_t charset_decompose ( charset_t ch, char * src, size_t inlen, char * dst, size_t outlen)
{
	char *buffer;
	ucs2_t u[MAXPATHLEN];
	size_t len;
	size_t ilen;

        if ((size_t)(-1) == (len = convert_string_allocate_internal(ch, CH_UCS2, src, inlen, &buffer)) )
            return len;

	ilen=sizeof(u);

	if ( (size_t)-1 == (ilen = decompose_w((ucs2_t *)buffer, len, u, &ilen)) ) {
	    free (buffer);
	    return (size_t)(-1);
	}

        if ((size_t)(-1) == (len = convert_string_internal( CH_UCS2, ch, (char*)u, ilen, dst, outlen)) ) {
	    free (buffer);
	    return (size_t)(-1);
	}

	free(buffer);
	dst[len] = 0;
	return (len);
}

size_t utf8_precompose ( char * src, size_t inlen, char * dst, size_t outlen)
{
	return charset_precompose ( CH_UTF8, src, inlen, dst, outlen);
}

size_t utf8_decompose ( char * src, size_t inlen, char * dst, size_t outlen)
{
	return charset_decompose ( CH_UTF8, src, inlen, dst, outlen);
}

#if 0
static char  debugbuf[ MAXPATHLEN +1 ];
char * debug_out ( char * seq, size_t len)
{
        size_t i = 0;
        unsigned char *p;
        char *q;

        p = (unsigned char*) seq;
        q = debugbuf;

        for ( i = 0; i<=(len-1); i++)
        {
                sprintf(q, "%2.2x.", *p);
                q += 3;
                p++;
        }
        *q=0;
        q = debugbuf;
        return q;
}
#endif

/* 
 * Convert from MB to UCS2 charset 
 * Flags:
 *		CONV_UNESCAPEHEX:	 ':XX' will be converted to an UCS2 character
 *		CONV_IGNORE:	 	 return the first convertable characters.
 * FIXME:
 *		This will *not* work if the destination charset is not multibyte, i.e. UCS2->UCS2 will fail
 *		The (un)escape scheme is not compatible to the old cap style escape. This is bad, we need it 
 *		for e.g. HFS cdroms.
 */

static size_t pull_charset_flags (charset_t from_set, charset_t cap_charset, char* src, size_t srclen, char* dest, size_t destlen, u_int16_t *flags)
{
	size_t i_len, o_len, hlen;
	size_t retval, j = 0;
	const char* inbuf = (const char*)src;
	char* outbuf = (char*)dest;
	atalk_iconv_t descriptor;
	atalk_iconv_t descriptor_cap;
	char *o_save, *s;
	char h[MAXPATHLEN];
	const char *h_buf;

	if (srclen == (size_t)-1)
		srclen = strlen(src)+1;

	lazy_initialize_conv();

	descriptor = conv_handles[from_set][CH_UCS2];
	descriptor_cap = conv_handles[cap_charset][CH_UCS2];

	if (descriptor == (atalk_iconv_t)-1 || descriptor == (atalk_iconv_t)0) {
		return (size_t) -1;
	}

	i_len=srclen;
	o_len=destlen;
	o_save=outbuf;
	
conversion_loop:
	if ( flags && (*flags & CONV_UNESCAPEHEX)) {
		if ( NULL != (s = strchr ( inbuf, ':'))) {
			j = i_len - (s - inbuf);
			if ( 0 == (i_len = (s - inbuf)))
				goto unhex_char;
	}
	}
	
	retval = atalk_iconv(descriptor,  &inbuf, &i_len, &outbuf, &o_len);
	if(retval==(size_t)-1) {
	    if (errno == EILSEQ && flags && (*flags & CONV_IGNORE)) {
				*flags |= CONV_REQMANGLE;
				return destlen-o_len;
	    }
	    else
	    	return (size_t) -1;
    }
    
unhex_char:
	if (j && flags && (*flags & CONV_UNESCAPEHEX )) {
		/* we're at the start on an hex encoded ucs2 char */
		if (o_len < 2) {
			errno = E2BIG;
			return (size_t) -1;
		}
		if ( j >= 3 && 
			isxdigit( *(inbuf+1)) && isxdigit( *(inbuf+2)) ) {
			hlen = 0;
			while ( *inbuf == ':' && j >=3 && 
				isxdigit( *(inbuf+1)) && isxdigit( *(inbuf+2)) ) {
				inbuf++;
				h[hlen]   = hextoint( *inbuf ) << 4;
				inbuf++;
				h[hlen++] |= hextoint( *inbuf );			
				inbuf++;
				j -= 3;
			}
			h_buf = (const char*) h;
			if ((size_t) -1 == (retval = atalk_iconv(descriptor_cap, &h_buf, &hlen, &outbuf, &o_len)) ) {
				if (errno == EILSEQ && CHECK_FLAGS(flags, CONV_IGNORE)) {
					*flags |= CONV_REQMANGLE;
					return destlen-o_len;
				}
				else {
					return retval;
				}
			}
		}
		else {
			/* We have an invalid :xx sequence */
			if (CHECK_FLAGS(flags, CONV_IGNORE)) {
				*flags |= CONV_REQMANGLE;
				return destlen-o_len;
			}
			else {
				errno=EILSEQ;
				return (size_t) -1;
			}
		}
		i_len = j;
		j = 0;
		if (i_len > 0)
			goto conversion_loop;
	}



	return destlen-o_len;
}

/* 
 * Convert from UCS2 to MB charset 
 * Flags:
 * 		CONV_ESCAPEDOTS: escape leading dots
 *		CONV_ESCAPEHEX:	 unconvertable characters and '/' will be escaped to :XX
 *		CONV_IGNORE:	 unconvertable characters will be replaced with '_'
 * FIXME:
 *		CONV_IGNORE and CONV_ESCAPEHEX can't work together. Should we check this ?
 *		This will *not* work if the destination charset is not multibyte, i.e. UCS2->UCS2 will fail
 *		The escape scheme is not compatible to the old cap style escape. This is bad, we need it 
 *		for e.g. HFS cdroms.
 */


static size_t push_charset_flags (charset_t to_set, charset_t cap_set, char* src, size_t srclen, char* dest, size_t destlen, u_int16_t *flags)
{
    size_t i_len, o_len, i;
    size_t retval, j = 0;
    const char* inbuf = (const char*)src;
    char* outbuf = (char*)dest;
    atalk_iconv_t descriptor;
    char *o_save;
    char *buf, *buf_save;
    size_t buflen;
    
    lazy_initialize_conv();
    
    descriptor = conv_handles[CH_UCS2][to_set];
    
    if (descriptor == (atalk_iconv_t)-1 || descriptor == (atalk_iconv_t)0) {
        return (size_t) -1;
    }
    
    i_len=srclen;
    o_len=destlen;
    o_save=outbuf;
    
    if ( SVAL(inbuf,0) == 0x002e && flags && (*flags & CONV_ESCAPEDOTS)) { /* 0x002e = . */
        if (o_len < 3) {
            errno = E2BIG;
            return (size_t) -1;
        }
        o_save[0] = ':';
        o_save[1] = '2';
        o_save[2] = 'e';
        o_len -= 3;
        inbuf += 2;
        i_len -= 2;
        outbuf = o_save + 3;
        if (flags) *flags |= CONV_REQESCAPE;
    }
	
conversion_loop:
    if ( flags && (*flags & CONV_ESCAPEHEX)) {
        for ( i = 0; i < i_len; i+=2) {
            if ( SVAL((inbuf+i),0) == 0x002f) { /* 0x002f = / */
                j = i_len - i;
                if ( 0 == ( i_len = i))
                    goto escape_slash;
                break;
            } else if ( SVAL(inbuf+i,0) == 0x003a) { /* 0x003a = : */
		errno = EILSEQ;
		return (size_t) -1;
	    }
        }
    }
    
    retval = atalk_iconv(descriptor,  &inbuf, &i_len, &outbuf, &o_len);
    if (retval==(size_t)-1) {
        if (errno == EILSEQ && CHECK_FLAGS(flags, CONV_IGNORE)) {
            *flags |= CONV_REQMANGLE;
    	    return destlen -o_len;
        }
        else if ( errno == EILSEQ && flags && (*flags & CONV_ESCAPEHEX)) {
            if (o_len < 3) {
                errno = E2BIG;
                return (size_t) -1;
            }
   	    if ((size_t) -1 == (buflen = convert_string_allocate_internal(CH_UCS2, cap_set, inbuf, 2, &buf)) ) 
		return buflen;
	    buf_save = buf;
    	    while (buflen > 0) {
		if ( o_len < 3) {
			errno = E2BIG;
			return (size_t) -1;
		}
		*outbuf++ = ':';
		*outbuf++ = hexdig[ ( *buf & 0xf0 ) >> 4 ];
       		*outbuf++ = hexdig[ *buf & 0x0f ];
       		buf++;
		buflen--;
		o_len -= 3;
	    }
   	    SAFE_FREE(buf_save);
	    buflen = 0;
	    i_len -= 2;
	    inbuf += 2;
            if (flags) *flags |= CONV_REQESCAPE;
	    if ( i_len > 0)
            	goto conversion_loop;
	}
        else
           return (size_t)(-1);	
    }
	
escape_slash:
    if (j && flags && (*flags & CONV_ESCAPEHEX)) {
        if (o_len < 3) {
            errno = E2BIG;
            return (size_t) -1;
        }
        o_save[destlen -o_len]   = ':';
        o_save[destlen -o_len+1] = '2';
        o_save[destlen -o_len+2] = 'f';
        inbuf  += 2;
        i_len   = j-2;
        o_len  -= 3;
        outbuf += 3;
        j = 0;
	if ( i_len > 0)
        	goto conversion_loop;
    }
    return destlen -o_len;
}

size_t convert_charset ( charset_t from_set, charset_t to_set, charset_t cap_charset, char* src, size_t src_len, char* dest, size_t dest_len, u_int16_t *flags)
{
	size_t i_len, o_len;
	ucs2_t *u;
	ucs2_t buffer[MAXPATHLEN];
	ucs2_t buffer2[MAXPATHLEN];
 	int composition = 0;
	
	lazy_initialize_conv();

	/* convert from_set to UCS2 */
 	if ((size_t)(-1) == ( o_len = pull_charset_flags( from_set, cap_charset, src, src_len, 
                                                          (char *) buffer, sizeof(buffer), flags)) ) {
		LOG(log_error, logtype_default, "Conversion failed ( %s to CH_UCS2 )", charset_name(from_set));
		return (size_t) -1;
	}

	if ( o_len == 0)
		return o_len;

	/* Do pre/decomposition */
	if (CHECK_FLAGS(flags, CONV_PRECOMPOSE) || 
		((!(charsets[to_set])   || !(charsets[to_set]->flags & CHARSET_DECOMPOSED)) && 
		(!(charsets[from_set]) || (charsets[from_set]->flags & CHARSET_DECOMPOSED))))
 	    composition = 1;
 	if (CHECK_FLAGS(flags, CONV_DECOMPOSE) || (charsets[to_set] && charsets[to_set]->flags & CHARSET_DECOMPOSED) )
 	    composition = 2;
 
	i_len = sizeof(buffer2);
	u = buffer2;

	switch (composition) {
	case 0:
 	    u = buffer;
 	    i_len = o_len;
	    break;
	case 1:
            if ( (size_t)-1 == (i_len = precompose_w(buffer, o_len, u, &i_len)) )
 	        return (size_t)(-1);
	    break;
	case 2:
            if ( (size_t)-1 == (i_len = decompose_w(buffer, o_len, u, &i_len)) )
 	        return (size_t)(-1);
	    break;
	}
		
  	/* Do case conversions */	
 	if (CHECK_FLAGS(flags, CONV_TOUPPER)) {
 	    strupper_w(u);
  	}
 	if (CHECK_FLAGS(flags, CONV_TOLOWER)) {
 	    strlower_w(u);
  	}

	/* Convert UCS2 to to_set */
	if ((size_t)(-1) == ( o_len = push_charset_flags( to_set, cap_charset, (char *)u, i_len, dest, dest_len, flags )) ) {
		LOG(log_error, logtype_default, 
		       "Conversion failed (CH_UCS2 to %s):%s", charset_name(to_set), strerror(errno));
		return (size_t) -1;
	}

	return o_len;
}
