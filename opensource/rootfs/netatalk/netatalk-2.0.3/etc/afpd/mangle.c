/* 
 * $Id: mangle.c,v 1.1.1.1 2008/06/18 10:55:41 jason Exp $ 
 *
 * Copyright (c) 2002. Joe Marcus Clarke (marcus@marcuscom.com)
 * All Rights Reserved.  See COPYRIGHT.
 *
 * mangle, demangle (filename):
 * mangle or demangle filenames if they are greater than the max allowed
 * characters for a given version of AFP.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <ctype.h>
#include "mangle.h"
#include "desktop.h"
#include <atalk/util.h>  

#define hextoint( c )   ( isdigit( c ) ? c - '0' : c + 10 - 'A' )
#define isuxdigit(x)    (isdigit(x) || (isupper(x) && isxdigit(x)))

static char *demangle_checks ( const struct vol *vol, char* uname, char * mfilename, size_t prefix, char * ext)
{
    u_int16_t flags;
    static char buffer[MAXPATHLEN +1];
    size_t len;
    size_t mfilenamelen;
    char *u;

    /* We need to check, whether we really need to demangle the filename 	*/
    /* i.e. it's not just a file with a valid #HEX in the name ...		*/
    /* but we don't want to miss valid demangle as well.			*/

    /* check whether file extensions match */
    if ( NULL != (u = strchr(uname, '.')) ) {
	if (strcmp(ext,u))
		return mfilename;
    }
    else if ( *ext == '.' )
	return mfilename;

    /* First we convert the unix name to our volume maccharset     	*/
    /* This assumes, OSX will not send us a mangled name for *any* 	*/
    /* other reason than a hint/filename mismatch on the OSX side!! */
    /* If the filename needed mangling, we'll get the mac filename	*/
    /* till the first unconvertable char, so these have to	match	*/
    /* the mangled name we got ..					*/

    flags = CONV_IGNORE | CONV_UNESCAPEHEX;
    if ( (size_t) -1 == (len = convert_charset(vol->v_volcharset, vol->v_maccharset, 0, 
				      uname, strlen(uname), buffer, MAXPATHLEN, &flags)) ) {
	return mfilename;
    }
    /* If the filename is too long we also needed to mangle */
    mfilenamelen = strlen(mfilename);
    if ( len >= vol->max_filename || mfilenamelen == MACFILELEN ) {
        flags |= CONV_REQMANGLE;
        len = prefix;
    }
    
    /* Ok, mangling was needed, now we do some further checks    */
    /* this is still necessary, as we might have a file abcde:xx */
    /* with id 12, mangled to abcde#12, and a passed filename    */
    /* abcd#12 						     */ 
    /* if we only checked if "prefix" number of characters match */
    /* we get a false postive in above case			     */

    if ( flags & CONV_REQMANGLE ) {
        if (len) { 
            /* convert the buffer to UTF8_MAC ... */
            if ((size_t) -1 == (len = convert_charset(vol->v_maccharset, CH_UTF8_MAC, 0, 
            			buffer, len, buffer, MAXPATHLEN, &flags)) ) {
                return mfilename;
            }
            /* Now compare the two names, they have to match the number of characters in buffer */
            /* prefix can be longer than len, OSX might send us the first character(s) of a     */
            /* decomposed char as the *last* character(s) before the #, so our match below will */
            /* still work, but leaves room for a race ... FIXME				    */
            if ( (prefix >= len || mfilenamelen == MACFILELEN) 
                 && !strncmp (mfilename, buffer, len)) {
                 return uname;
            }
        }
        else {
            /* We couldn't convert the name to maccharset at all, so we'd expect a name */
            /* in the "???#ID" form ... */
            if ( !strncmp("???", mfilename, prefix)) {
                return uname;
            }
            /* ..but OSX might send us only the first characters of a decomposed character. */
            /*  So convert to UTF8_MAC again, now at least the prefix number of 	  */
            /* characters have to match ... again a possible race FIXME			  */
            
            if ( (size_t) -1 == (len = convert_charset(vol->v_volcharset, CH_UTF8_MAC, 0, 
	                          uname, strlen(uname), buffer, MAXPATHLEN, &flags)) ) {
	        return mfilename;
	    }

            if ( !strncmp (mfilename, buffer, prefix) ) {
                return uname;
            }
        }
    }
    return mfilename;
}

/* -------------------------------------------------------
*/
static char *
private_demangle(const struct vol *vol, char *mfilename, cnid_t did, cnid_t *osx) 
{
    char *t;
    char *u_name;
    u_int32_t id, file_id;
    static char buffer[12 + MAXPATHLEN + 1];
    int len = 12 + MAXPATHLEN + 1;
    struct dir	*dir;
    size_t prefix;

    id = file_id = 0;

    t = strchr(mfilename, MANGLE_CHAR);
    if (t == NULL) {
        return mfilename;
    }
    prefix = t - mfilename;
    /* FIXME 
     * is prefix == 0 a valid mangled filename ?
    */
    /* may be a mangled filename */
    t++;
    if (*t == '0') { /* can't start with a 0 */
        return mfilename;
    }
    while(isuxdigit(*t)) {
        id = (id *16) + hextoint(*t);
        t++;
    }
    if ((*t != 0 && *t != '.') || strlen(t) > MAX_EXT_LENGTH || id < 17) {
        return mfilename;
    }

    file_id = id = htonl(id);
    if (osx) {
        *osx = id;
    }

    /* is it a dir?, there's a conflict with pre OSX 'trash #2'  */
    if ((dir = dirsearch(vol, id))) {
        if (dir->d_parent && dir->d_parent->d_did != did) {
            /* not in the same folder, there's a race with outdate cache
             * but we have to live with it, hopefully client will recover
            */
            return mfilename;
        }
        if (!osx) {
            /* it's not from cname so mfilename and dir must be the same */
            if (!strcmp(dir->d_m_name, mfilename)) {
                return dir->d_u_name;
            }
        } 
        else {
	    return demangle_checks (vol, dir->d_u_name, mfilename, prefix, t);
	}
    }
    else if (NULL != (u_name = cnid_resolve(vol->v_cdb, &id, buffer, len)) ) {
        if (id != did) {
            return mfilename;
        }
        if (!osx) {
            /* convert back to mac name and check it's the same */
            t = utompath(vol, u_name, file_id, utf8_encoding());
            if (!strcmp(t, mfilename)) {
                return u_name;
            }
        }
        else {
            return demangle_checks (vol, u_name, mfilename, prefix, t);
        }
    }

    return mfilename;
}

/* -------------------------------------------------------
*/
char *
demangle(const struct vol *vol, char *mfilename, cnid_t did)
{
    return private_demangle(vol, mfilename, did, NULL);
}

/* -------------------------------------------------------
 * OS X  
*/
char *
demangle_osx(const struct vol *vol, char *mfilename, cnid_t did, cnid_t *fileid) 
{
    return private_demangle(vol, mfilename, did, fileid);
}


/* -------------------------------------------------------
 * find the start of a utf8 character
 */
static unsigned char *
utf8_mangle_validate(unsigned char *path, size_t len)
{
    unsigned char *p = path + len;
    int           dec = 0;

    /* char matches with 10xxxxxx ? */
    while ( len && *p && ((*p & 0xc0) == 0x80)) {
	dec = 1;
        len--;
        p--;
    }
    if (dec)
        *p = 0;

    return path;
}

/* -----------------------
   with utf8 filename not always round trip
   filename   mac filename too long or first chars if unmatchable chars.
   uname      unix filename 
   id         file/folder ID or 0
   
*/
unsigned char *
mangle(const struct vol *vol, unsigned char *filename, size_t filenamelen, unsigned char *uname, cnid_t id, int flags) {
    unsigned char *ext = NULL;
    unsigned char *m = NULL;
    static unsigned char mfilename[MAXPATHLEN + 1];
    unsigned char mangle_suffix[MANGLE_LENGTH + 1];
    size_t ext_len = 0;
    size_t maxlen;
    int k;
    
    maxlen = (flags & 2)?255:MACFILELEN; /* was vol->max_filename */
    /* Do we really need to mangle this filename? */
    if (!(flags & 1) && filenamelen <= maxlen) {
	return filename;
    }

    if (!id) {
        /* we don't have the file id! only catsearch call mangle with id == 0 */
        return NULL;
    }
    /* First, attempt to locate a file extension. */
    if (NULL != (ext = strrchr(uname, '.')) ) {
	ext_len = strlen(ext);
	if (ext_len > MAX_EXT_LENGTH) {
	    /* Do some bounds checking to prevent an extension overflow. */
	    ext_len = MAX_EXT_LENGTH;
	}
    }
    m = mfilename;
    k = sprintf(mangle_suffix, "%c%X", MANGLE_CHAR, ntohl(id));

    strlcpy(m, filename, maxlen  - k - ext_len +1);
    if (flags & 2)
        m = utf8_mangle_validate(m, maxlen - k - ext_len +1);
    if (*m == 0) {
        strcat(m, "???");
    }
    strcat(m, mangle_suffix);
    if (ext) {
	strncat(m, ext, ext_len);
    }

    return m;
}
