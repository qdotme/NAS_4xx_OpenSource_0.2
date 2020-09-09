/*
* Copyright (c) 2004 Massachusetts Institute of Technology
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy,
* modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/* $Id: api.c,v 1.1.1.1 2006/05/19 07:08:14 arsene Exp $ */

#include<kconfiginternal.h>
#include<assert.h>

kconf_conf_space * conf_root = NULL;
kconf_handle * conf_handles = NULL;
kconf_handle * conf_free_handles = NULL;

CRITICAL_SECTION cs_conf_global;
CRITICAL_SECTION cs_conf_handle;
LONG conf_init = 0;
LONG conf_status = 0;

void init_kconf(void) {
    if(InterlockedIncrement(&conf_init) == 1L) {
        /* we are the first */
        InitializeCriticalSection(&cs_conf_global);
        EnterCriticalSection(&cs_conf_global);
        conf_root = khc_create_empty_space();
        conf_root->name = wcsdup(L"Root");
        conf_root->regpath = wcsdup(CONFIG_REGPATHW);
        conf_root->refcount++;
        conf_status = 1;
        InitializeCriticalSection(&cs_conf_handle);
        LeaveCriticalSection(&cs_conf_global);
    }
    /* else assume we are already initialized */
}

void exit_kconf(void) {
    if(khc_is_config_running()) {
        kconf_handle * h;

        EnterCriticalSection(&cs_conf_global);

        conf_init = 0;
        conf_status = 0;

        khc_free_space(conf_root);

        EnterCriticalSection(&cs_conf_handle);
        while(conf_free_handles) {
            LPOP(&conf_free_handles, &h);
            if(h) {
                free(h);
            }
        }

        while(conf_handles) {
            LPOP(&conf_handles, &h);
            if(h) {
                free(h);
            }
        }
        LeaveCriticalSection(&cs_conf_handle);
        DeleteCriticalSection(&cs_conf_handle);

        LeaveCriticalSection(&cs_conf_global);
        DeleteCriticalSection(&cs_conf_global);
    }
}

kconf_handle * khc_handle_from_space(kconf_conf_space * s, khm_int32 flags)
{
    kconf_handle * h;

    EnterCriticalSection(&cs_conf_handle);
    LPOP(&conf_free_handles, &h);
    if(!h) {
        h = malloc(sizeof(kconf_handle));
		assert(h != NULL);
    }
    ZeroMemory((void *) h, sizeof(kconf_handle));

    h->magic = KCONF_HANDLE_MAGIC;
    khc_space_hold(s);
    h->space = s;
    h->flags = flags;

    LPUSH(&conf_handles, h);
    LeaveCriticalSection(&cs_conf_handle);

    return h;
}

/* must be called with cs_conf_global held */
void khc_handle_free(kconf_handle * h)
{
    kconf_handle * lower;

    EnterCriticalSection(&cs_conf_handle);
#ifdef DEBUG
    /* check if the handle is actually in use */
    {
        kconf_handle * a;
        a = conf_handles;
        while(a) {
            if(h == a)
                break;
            a = LNEXT(a);
        }

        if(a == NULL) {
            DebugBreak();
        }
    }
#endif
    while(h) {
        LDELETE(&conf_handles, h);
        if(h->space) {
            khc_space_release(h->space);
            h->space = NULL;
        }
        lower = h->lower;
        LPUSH(&conf_free_handles, h);
        h = lower;
    }
    LeaveCriticalSection(&cs_conf_handle);
}

kconf_handle * khc_handle_dup(kconf_handle * o)
{
    kconf_handle * h;
    kconf_handle * r;

    r = khc_handle_from_space(o->space, o->flags);
    h = r;

    while(o->lower) {
        h->lower = khc_handle_from_space(o->lower->space, o->lower->flags);

        o = o->lower;
        h = h->lower;
    }

    return r;
}

void khc_space_hold(kconf_conf_space * s) {
    InterlockedIncrement(&(s->refcount));
}

void khc_space_release(kconf_conf_space * s) {
    LONG l = InterlockedDecrement(&(s->refcount));
    if(!l) {
        EnterCriticalSection(&cs_conf_global);

        if(s->regkey_machine)
            RegCloseKey(s->regkey_machine);
        if(s->regkey_user)
            RegCloseKey(s->regkey_user);
        s->regkey_machine = NULL;
        s->regkey_user = NULL;

        LeaveCriticalSection(&cs_conf_global);
    }
}

/* case sensitive replacement for RegOpenKeyEx */
LONG 
khcint_RegOpenKeyEx(HKEY hkey, LPCWSTR sSubKey, DWORD ulOptions,
                    REGSAM samDesired, PHKEY phkResult) {
    int i;
    wchar_t sk_name[KCONF_MAXCCH_NAME];
    FILETIME ft;
    size_t cch;
    HKEY hkp;
    const wchar_t * t;
    LONG rv = ERROR_SUCCESS;

    hkp = hkey;

    /* descend down the components of the subkey */
    t = sSubKey;
    while(TRUE) {
        wchar_t * slash;
        HKEY hkt;

        slash = wcschr(t, L'\\');
        if (slash == NULL)
            break;

        if (FAILED(StringCchCopyN(sk_name, ARRAYLENGTH(sk_name),
                                  t, slash - t))) {
            rv = ERROR_CANTOPEN;
            goto _cleanup;
        }

        sk_name[slash - t] = L'\0';
        t = slash+1;

        if (khcint_RegOpenKeyEx(hkp, sk_name, ulOptions, samDesired, &hkt) ==
            ERROR_SUCCESS) {

            if (hkp != hkey)
                RegCloseKey(hkp);
            hkp = hkt;

        } else {

            rv = ERROR_CANTOPEN;
            goto _cleanup;

        }
    }

    /* by now hkp is a handle to the parent of the last component in
       the subkey.  t is a pointer to the last component. */

    if (FAILED(StringCchLength(t, KCONF_MAXCCH_NAME, &cch))) {
        rv = ERROR_CANTOPEN;
        goto _cleanup;
    }

    /* go through and find the case sensitive match for the key */

    for (i=0; ;i++) {
        LONG l;
        DWORD dw;

        dw = ARRAYLENGTH(sk_name);
        l = RegEnumKeyEx(hkp, i, sk_name, &dw,
                         NULL, NULL, NULL, &ft);

        if (l != ERROR_SUCCESS) {
            rv = ERROR_CANTOPEN;
            goto _cleanup;
        }

        if (!(wcsncmp(sk_name, t, cch))) {
            /* bingo! ?? */
            if (cch < KCONF_MAXCCH_NAME &&
                (sk_name[cch] == L'\0' ||
                 sk_name[cch] == L'~')) {
                rv = RegOpenKeyEx(hkp, sk_name, ulOptions,
                                  samDesired, phkResult);
                goto _cleanup;
            }
        }
    }

 _cleanup:
    if (hkp != hkey && hkp != NULL)
        RegCloseKey(hkp);

    return rv;
}

LONG
khcint_RegCreateKeyEx(HKEY hKey,
                      LPCTSTR lpSubKey,
                      DWORD Reserved,
                      LPTSTR lpClass,
                      DWORD dwOptions,
                      REGSAM samDesired,
                      LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                      PHKEY phkResult,
                      LPDWORD lpdwDisposition) {
    LONG l;
    int i;
    long index = 0;
    wchar_t sk_name[KCONF_MAXCCH_NAME]; /* hard limit in Windows */
    FILETIME ft;
    size_t cch;
    const wchar_t * t;
    LONG rv = ERROR_SUCCESS;
    HKEY hkp = NULL;

    hkp = hKey;
    t = lpSubKey;
    while(TRUE) {
        wchar_t * slash;
        HKEY hkt;

        slash = wcschr(t, L'\\');
        if (slash == NULL)
            break;

        if (FAILED(StringCchCopyN(sk_name, ARRAYLENGTH(sk_name),
                                  t, slash - t))) {
            rv = ERROR_CANTOPEN;
            goto _cleanup;
        }

        sk_name[slash - t] = L'\0';
        t = slash+1;

        if (khcint_RegOpenKeyEx(hkp, sk_name, 0, samDesired, &hkt) ==
            ERROR_SUCCESS) {

            if (hkp != hKey)
                RegCloseKey(hkp);
            hkp = hkt;

        } else {

            rv = RegCreateKeyEx(hKey,
                                lpSubKey,
                                Reserved,
                                lpClass,
                                dwOptions,
                                samDesired,
                                lpSecurityAttributes,
                                phkResult,
                                lpdwDisposition);
            goto _cleanup;

        }
    }

    if (FAILED(StringCchLength(t, KCONF_MAXCCH_NAME, &cch))) {
        rv = ERROR_CANTOPEN;
        goto _cleanup;
    }

    for (i=0; ;i++) {
        DWORD dw;

        dw = ARRAYLENGTH(sk_name);
        l = RegEnumKeyEx(hkp, i, sk_name, &dw,
                         NULL, NULL, NULL, &ft);

        if (l != ERROR_SUCCESS)
            break;

        if (!(wcsncmp(sk_name, t, cch))) {
            /* bingo! ?? */
            if (sk_name[cch] == L'\0' ||
                sk_name[cch] == L'~') {
                l = RegOpenKeyEx(hkp, sk_name, 0,
                                 samDesired, phkResult);
                if (l == ERROR_SUCCESS && lpdwDisposition)
                    *lpdwDisposition = REG_OPENED_EXISTING_KEY;
                rv = l;
                goto _cleanup;
            }
        }

        if (!wcsnicmp(sk_name, t, cch) &&
            (sk_name[cch] == L'\0' ||
             sk_name[cch] == L'~')) {
            long new_idx;

            if (sk_name[cch] == L'\0')
                new_idx = 1;
            else if (cch + 1 < KCONF_MAXCCH_NAME)
                new_idx = wcstol(sk_name + (cch + 1), NULL, 10);
            else
                return ERROR_BUFFER_OVERFLOW;

            assert(new_idx > 0);

            if (new_idx > index)
                index = new_idx;
        }
    }

    if (index != 0) {
        if (FAILED(StringCbPrintf(sk_name, sizeof(sk_name),
                                  L"%s~%d", t, index)))
            return ERROR_BUFFER_OVERFLOW;
    } else {
        StringCbCopy(sk_name, sizeof(sk_name), t);
    }

    rv = RegCreateKeyEx(hkp,
                        sk_name,
                        Reserved,
                        lpClass,
                        dwOptions,
                        samDesired,
                        lpSecurityAttributes,
                        phkResult,
                        lpdwDisposition);

 _cleanup:

    if (hkp != hKey && hkp != NULL)
        RegCloseKey(hkp);

    return rv;
}


HKEY khc_space_open_key(kconf_conf_space * s, khm_int32 flags) {
    HKEY hk = NULL;
    int nflags = 0;
    DWORD disp;
    if(flags & KCONF_FLAG_MACHINE) {
        if(s->regkey_machine)
            return s->regkey_machine;
        if((khcint_RegOpenKeyEx(HKEY_LOCAL_MACHINE, s->regpath, 0, 
                                KEY_READ | KEY_WRITE, &hk) != 
            ERROR_SUCCESS) && 
           !(flags & KHM_PERM_WRITE)) {

            if(khcint_RegOpenKeyEx(HKEY_LOCAL_MACHINE, s->regpath, 0, 
                                   KEY_READ, &hk) == ERROR_SUCCESS) {
                nflags = KHM_PERM_READ;
            }

        }
        if(!hk && (flags & KHM_FLAG_CREATE)) {

            khcint_RegCreateKeyEx(HKEY_LOCAL_MACHINE, 
                                  s->regpath, 
                                  0,
                                  NULL,
                                  REG_OPTION_NON_VOLATILE,
                                  KEY_READ | KEY_WRITE,
                                  NULL,
                                  &hk,
                                  &disp);
        }
        if(hk) {
            EnterCriticalSection(&cs_conf_global);
            s->regkey_machine = hk;
            s->regkey_machine_flags = nflags;
            LeaveCriticalSection(&cs_conf_global);
        }

        return hk;
    } else {
        if(s->regkey_user)
            return s->regkey_user;
        if((khcint_RegOpenKeyEx(HKEY_CURRENT_USER, s->regpath, 0, 
                                KEY_READ | KEY_WRITE, &hk) != 
            ERROR_SUCCESS) && 
           !(flags & KHM_PERM_WRITE)) {
            if(khcint_RegOpenKeyEx(HKEY_CURRENT_USER, s->regpath, 0, 
                                   KEY_READ, &hk) == ERROR_SUCCESS) {
                nflags = KHM_PERM_READ;
            }
        }
        if(!hk && (flags & KHM_FLAG_CREATE)) {
            khcint_RegCreateKeyEx(HKEY_CURRENT_USER, 
                                  s->regpath, 
                                  0,
                                  NULL,
                                  REG_OPTION_NON_VOLATILE,
                                  KEY_READ | KEY_WRITE,
                                  NULL,
                                  &hk,
                                  &disp);
        }
        if(hk) {
            EnterCriticalSection(&cs_conf_global);
            s->regkey_user = hk;
            s->regkey_user_flags = nflags;
            LeaveCriticalSection(&cs_conf_global);
        }

        return hk;
    }
}

KHMEXP khm_int32 KHMAPI khc_shadow_space(khm_handle upper, khm_handle lower)
{
    kconf_handle * h;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(!khc_is_handle(upper))
        return KHM_ERROR_INVALID_PARM;

    h = (kconf_handle *) upper;

    EnterCriticalSection(&cs_conf_handle);
    if(h->lower) {
        LeaveCriticalSection(&cs_conf_handle);
        EnterCriticalSection(&cs_conf_global);
        khc_handle_free(h->lower);
        LeaveCriticalSection(&cs_conf_global);
        EnterCriticalSection(&cs_conf_handle);
        h->lower = NULL;
    }

    if(khc_is_handle(lower)) {
        kconf_handle * l;
        kconf_handle * lc;

        l = (kconf_handle *) lower;
        LeaveCriticalSection(&cs_conf_handle);
        lc = khc_handle_dup(l);
        EnterCriticalSection(&cs_conf_handle);
        h->lower = lc;
    }
    LeaveCriticalSection(&cs_conf_handle);

    return KHM_ERROR_SUCCESS;
}

kconf_conf_space * khc_create_empty_space(void) {
    kconf_conf_space * r;

    r = malloc(sizeof(kconf_conf_space));
	assert(r != NULL);
    ZeroMemory(r,sizeof(kconf_conf_space));

    return r;
}

void khc_free_space(kconf_conf_space * r) {
    kconf_conf_space * c;

    if(!r)
        return;

    LPOP(&r->children, &c);
    while(c) {
        khc_free_space(c);
        LPOP(&r->children, &c);
    }

    if(r->name)
        free(r->name);

    if(r->regpath)
        free(r->regpath);

    if(r->regkey_machine)
        RegCloseKey(r->regkey_machine);

    if(r->regkey_user)
        RegCloseKey(r->regkey_user);

    free(r);
}

khm_int32 khcint_open_space_int(kconf_conf_space * parent, wchar_t * sname, size_t n_sname, khm_int32 flags, kconf_conf_space **result) {
    kconf_conf_space * p;
    kconf_conf_space * c;
    HKEY pkey = NULL;
    HKEY ckey = NULL;
    wchar_t buf[KCONF_MAXCCH_NAME];

    if(!parent)
        p = conf_root;
    else
        p = parent;

    if(n_sname >= KCONF_MAXCCH_NAME || n_sname <= 0)
        return KHM_ERROR_INVALID_PARM;

	/*SAFE: buf: buffer size == KCONF_MAXCCH_NAME * wchar_t >
          n_sname * wchar_t */
    wcsncpy(buf, sname, n_sname);
    buf[n_sname] = L'\0';

    /* see if there is already a config space by this name. if so,
    return it.  Note that if the configuration space is specified in a
    schema, we would find it here. */
    EnterCriticalSection(&cs_conf_global);
    c = TFIRSTCHILD(p);
    while(c) {
        if(c->name && !wcscmp(c->name, buf))
            break;

        c = LNEXT(c);
    }
    LeaveCriticalSection(&cs_conf_global);

    if(c) {
        khc_space_hold(c);
        *result = c;
        return KHM_ERROR_SUCCESS;
    }

    if(!(flags & KHM_FLAG_CREATE)) {

        /* we are not creating the space, so it must exist in the form of a
        registry key in HKLM or HKCU.  If it existed as a schema, we
        would have already retured it above. */
        if(flags & KCONF_FLAG_USER)
            pkey = khc_space_open_key(p, KHM_PERM_READ | KCONF_FLAG_USER);

        if((!pkey || 
            (khcint_RegOpenKeyEx(pkey, buf, 0, KEY_READ, &ckey) != 
             ERROR_SUCCESS)) 
           && (flags & KCONF_FLAG_MACHINE)) {

            pkey = khc_space_open_key(p, KHM_PERM_READ | KCONF_FLAG_MACHINE);
            if(!pkey || 
               (khcint_RegOpenKeyEx(pkey, buf, 0, KEY_READ, &ckey) != 
                ERROR_SUCCESS)) {
                *result = NULL;
                return KHM_ERROR_NOT_FOUND;
            }
        }

        if(ckey) {
            RegCloseKey(ckey);
            ckey = NULL;
        }
    }

    c = khc_create_empty_space();
    
    /*SAFE: buf: is of known length < KCONF_MAXCCH_NAME */
    c->name = wcsdup(buf);

    /*SAFE: p->regpath: is valid since it was set using this same
      function. */
    /*SAFE: buf: see above */
    c->regpath = malloc((wcslen(p->regpath) + wcslen(buf) + 2) * sizeof(wchar_t));

    assert(c->regpath != NULL);

#pragma warning( push )
#pragma warning( disable: 4995 )
    /*SAFE: c->regpath: allocated above to be big enough */
    /*SAFE: p->regpath: see above */
    wcscpy(c->regpath, p->regpath);
    wcscat(c->regpath, L"\\");
    /*SAFE: buf: see above */
    wcscat(c->regpath, buf);
#pragma warning( pop )

    khc_space_hold(c);

    EnterCriticalSection(&cs_conf_global);
    TADDCHILD(p,c);
    LeaveCriticalSection(&cs_conf_global);

    *result = c;
    return KHM_ERROR_SUCCESS;
}

KHMEXP khm_int32 KHMAPI khc_open_space(khm_handle parent, wchar_t * cspace, khm_int32 flags, khm_handle * result) {
    kconf_handle * h;
    kconf_conf_space * p;
    kconf_conf_space * c = NULL;
    size_t cbsize;
    wchar_t * str;
    khm_int32 rv = KHM_ERROR_SUCCESS;

    if(!khc_is_config_running()) {
        return KHM_ERROR_NOT_READY;
    }

    if(!result || (parent && !khc_is_handle(parent)))
        return KHM_ERROR_INVALID_PARM;

    if(!parent)
        p = conf_root;
    else {
        h = (kconf_handle *) parent;
        p = khc_space_from_handle(parent);
    }

    khc_space_hold(p);

    /* if none of these flags are specified, make it seem like all of
       them were */
    if(!(flags & KCONF_FLAG_USER) &&
        !(flags & KCONF_FLAG_MACHINE) &&
        !(flags & KCONF_FLAG_SCHEMA))
        flags |= KCONF_FLAG_USER | KCONF_FLAG_MACHINE | KCONF_FLAG_SCHEMA;

    if(cspace == NULL) {
        khc_space_release(p);
        *result = (khm_handle) khc_handle_from_space(p, flags);
        return KHM_ERROR_SUCCESS;
    }

    if(FAILED(StringCbLength(cspace, KCONF_MAXCB_PATH, &cbsize))) {
        khc_space_release(p);
        *result = NULL;
        return KHM_ERROR_INVALID_PARM;
    }

    str = cspace;
    while(TRUE) {
        wchar_t * end = NULL;

        if (!(flags & KCONF_FLAG_NOPARSENAME)) {

            end = wcschr(str, L'\\'); /* safe because cspace was
                                     validated above */
#if 0
            if(!end)
                end = wcschr(str, L'/');  /* safe because cspace was
                                         validated above */
#endif
        }

        if(!end) {
            if(flags & KCONF_FLAG_TRAILINGVALUE) {
                /* we are at the value component */
                c = p;
                khc_space_hold(c);
                break;
            } else
                end = str + wcslen(str);  /* safe because cspace was
                                             validated above */
        }

        rv = khcint_open_space_int(p, str, end - str, flags, &c);

        if(KHM_SUCCEEDED(rv) && (*end == L'\\'
#if 0
                                 || *end == L'/'
#endif
                                 )) {
            khc_space_release(p);
            p = c;
            c = NULL;
            str = end+1;
        }
        else
            break;
    }

    khc_space_release(p);
    if(KHM_SUCCEEDED(rv)) {
        *result = khc_handle_from_space(c, flags);
    } else
        *result = NULL;

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_close_space(khm_handle csp) {
    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(!khc_is_handle(csp))
        return KHM_ERROR_INVALID_PARM;

    khc_handle_free((kconf_handle *) csp);
    return KHM_ERROR_SUCCESS;
}

KHMEXP khm_int32 KHMAPI khc_read_string(khm_handle pconf, 
                                        wchar_t * pvalue, 
                                        wchar_t * buf, 
                                        khm_size * bufsize) 
{
    kconf_conf_space * c;
    khm_int32 rv = KHM_ERROR_SUCCESS;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    do {
        HKEY hku = NULL;
        HKEY hkm = NULL;
        wchar_t * value = NULL;
        int free_space = 0;
        khm_handle conf = NULL;
        DWORD size;
        DWORD type;
        LONG hr;

        int i;

        if(wcschr(pvalue, L'\\')
#if 0
           || wcschr(pvalue, L'/')
#endif
           ) {

            if(KHM_FAILED(khc_open_space(
                pconf, 
                pvalue, 
                KCONF_FLAG_TRAILINGVALUE | (pconf?khc_handle_flags(pconf):0), 
                &conf)))
                goto _shadow;

            free_space = 1;
#if 0
            wchar_t * back, * forward;

            back = wcsrchr(pvalue, L'\\');
            forward = wcsrchr(pvalue, L'/');
            value = (back > forward)?back:forward; /* works for nulls too */
#else
            value = wcsrchr(pvalue, L'\\');
#endif
        } else {
            value = pvalue;
            conf = pconf;
            free_space = 0;
        }

        if(!khc_is_handle(conf))
            goto _shadow;

        c = khc_space_from_handle(conf);

        if(khc_is_user_handle(conf))
            hku = khc_space_open_key(c, KHM_PERM_READ);

        if(khc_is_machine_handle(conf))
            hkm = khc_space_open_key(c, KHM_PERM_READ | KCONF_FLAG_MACHINE);

        size = (DWORD) *bufsize;
        if(hku) {
            hr = RegQueryValueEx(hku, value, NULL, &type, (LPBYTE) buf, &size);
            if(hr == ERROR_SUCCESS) {
                if(type != REG_SZ) {
                    rv = KHM_ERROR_TYPE_MISMATCH;
                    goto _exit;
                }
                else {
                    *bufsize = size;
                    /* if buf==NULL, RegQueryValueEx will return success and just return the
                       required buffer size in 'size' */
                    rv = (buf)? KHM_ERROR_SUCCESS: KHM_ERROR_TOO_LONG;
                    goto _exit;
                }
            } else {
                if(hr == ERROR_MORE_DATA) {
                    *bufsize = size;
                    rv = KHM_ERROR_TOO_LONG;
                    goto _exit;
                }
            }
        }

        size = (DWORD) *bufsize;
        if(hkm) {
            hr = RegQueryValueEx(hkm, value, NULL, &type, (LPBYTE) buf, &size);
            if(hr == ERROR_SUCCESS) {
                if(type != REG_SZ) {
                    rv = KHM_ERROR_TYPE_MISMATCH;
                    goto _exit;
                }
                else {
                    *bufsize = size;
                    rv = (buf)? KHM_ERROR_SUCCESS: KHM_ERROR_TOO_LONG;
                    goto _exit;
                }
            } else {
                if(hr == ERROR_MORE_DATA) {
                    *bufsize = size;
                    rv = KHM_ERROR_TOO_LONG;
                    goto _exit;
                }
            }
        }

        if(c->schema && khc_is_schema_handle(conf)) {
            for(i=0;i<c->nSchema;i++) {
                if(c->schema[i].type == KC_STRING && !wcscmp(value, c->schema[i].name)) {
                    /* found it */
                    size_t cbsize = 0;

                    if(!c->schema[i].value) {
                        rv = KHM_ERROR_NOT_FOUND;
                        goto _exit;
                    }

                    if(FAILED(StringCbLength((wchar_t *) c->schema[i].value, KCONF_MAXCB_STRING, &cbsize))) {
                        rv = KHM_ERROR_NOT_FOUND;
                        goto _exit;
                    }
                    cbsize += sizeof(wchar_t);

                    if(!buf || *bufsize < cbsize) {
                        *bufsize = cbsize;
                        rv = KHM_ERROR_TOO_LONG;
                        goto _exit;
                    }

                    StringCbCopy(buf, *bufsize, (wchar_t *) c->schema[i].value);
                    *bufsize = cbsize;
                    rv = KHM_ERROR_SUCCESS;
                    goto _exit;
                }
            }
        }

_shadow:
        if(free_space && conf)
            khc_close_space(conf);

        if(khc_is_shadowed(pconf)) {
            pconf = khc_shadow(pconf);
            continue;
        } else {
            rv = KHM_ERROR_NOT_FOUND;
            break;
        }

_exit:
        if(free_space && conf)
            khc_close_space(conf);
        break;

    } while(TRUE);

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_read_int32(khm_handle pconf, wchar_t * pvalue, khm_int32 * buf) {
    kconf_conf_space * c;
    khm_int32 rv = KHM_ERROR_SUCCESS;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(!buf || !pvalue)
        return KHM_ERROR_INVALID_PARM;

    do {
        DWORD size;
        DWORD type;
        LONG hr;
        HKEY hku = NULL;
        HKEY hkm = NULL;

        wchar_t * value = NULL;
        int free_space = 0;
        khm_handle conf = NULL;

        int i;

        if(wcschr(pvalue, L'\\')
#if 0
           || wcschr(pvalue, L'/')
#endif
           ) {
            if(KHM_FAILED(khc_open_space(
                pconf, 
                pvalue, 
                KCONF_FLAG_TRAILINGVALUE | (pconf?khc_handle_flags(pconf):0), 
                &conf)))
                goto _shadow;
            free_space = 1;
#if 0
            wchar_t * back, * forward;

            back = wcsrchr(pvalue, L'\\');
            forward = wcsrchr(pvalue, L'/');
            value = (back > forward)?back:forward;
#else
            value = wcsrchr(pvalue, L'\\');
#endif
        } else {
            value = pvalue;
            conf = pconf;
            free_space = 0;
        }

        if(!khc_is_handle(conf) || !buf)
            goto _shadow;

        c = khc_space_from_handle(conf);

        if(khc_is_user_handle(conf))
            hku = khc_space_open_key(c, KHM_PERM_READ);

        if(khc_is_machine_handle(conf))
            hkm = khc_space_open_key(c, KHM_PERM_READ | KCONF_FLAG_MACHINE);

        size = sizeof(DWORD);
        if(hku) {
            hr = RegQueryValueEx(hku, value, NULL, &type, (LPBYTE) buf, &size);
            if(hr == ERROR_SUCCESS) {
                if(type != REG_DWORD) {
                    rv = KHM_ERROR_TYPE_MISMATCH;
                    goto _exit;
                }
                else {
                    rv = KHM_ERROR_SUCCESS;
                    goto _exit;
                }
            }
        }

        size = sizeof(DWORD);
        if(hkm) {
            hr = RegQueryValueEx(hkm, value, NULL, &type, (LPBYTE) buf, &size);
            if(hr == ERROR_SUCCESS) {
                if(type != REG_DWORD) {
                    rv= KHM_ERROR_TYPE_MISMATCH;
                    goto _exit;
                }
                else {
                    rv=  KHM_ERROR_SUCCESS;
                    goto _exit;
                }
            }
        }

        if(c->schema && khc_is_schema_handle(conf)) {
            for(i=0;i<c->nSchema;i++) {
                if(c->schema[i].type == KC_INT32 && !wcscmp(value, c->schema[i].name)) {
                    *buf = (khm_int32) c->schema[i].value;
                    rv = KHM_ERROR_SUCCESS;
                    goto _exit;
                }
            }
        }
_shadow:
        if(free_space && conf)
            khc_close_space(conf);

        if(khc_is_shadowed(pconf)) {
            pconf = khc_shadow(pconf);
            continue;
        } else {
            rv = KHM_ERROR_NOT_FOUND;
            break;
        }
_exit:
        if(free_space && conf)
            khc_close_space(conf);
        break;
    }
    while(TRUE);

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_read_int64(khm_handle pconf, wchar_t * pvalue, khm_int64 * buf) {
    kconf_conf_space * c;
    khm_int32 rv = KHM_ERROR_SUCCESS;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    do {
        DWORD size;
        DWORD type;
        LONG hr;
        HKEY hku = NULL;
        HKEY hkm = NULL;

        wchar_t * value = NULL;
        int free_space = 0;
        khm_handle conf = NULL;

        int i;

        if(wcschr(pvalue, L'\\')
#if 0
           || wcschr(pvalue, L'/')
#endif
           ) {
            if(KHM_FAILED(khc_open_space(
                pconf, 
                pvalue, 
                KCONF_FLAG_TRAILINGVALUE | (pconf?khc_handle_flags(pconf):0), 
                &conf)))
                goto _shadow;
            free_space = 1;
#if 0
            wchar_t * back, *forward;

            back = wcsrchr(pvalue, L'\\');
            forward = wcsrchr(pvalue, L'/');
            value = (back > forward)?back:forward;
#else
            value = wcsrchr(pvalue, L'\\');
#endif
        } else {
            value = pvalue;
            conf = pconf;
            free_space = 0;
        }

        if(!khc_is_handle(conf) || !buf)
            goto _shadow;

        c = khc_space_from_handle(conf);

        if(khc_is_user_handle(conf))
            hku = khc_space_open_key(c, KHM_PERM_READ);

        if(khc_is_machine_handle(conf))
            hkm = khc_space_open_key(c, KHM_PERM_READ | KCONF_FLAG_MACHINE);

        size = sizeof(khm_int64);
        if(hku) {
            hr = RegQueryValueEx(hku, value, NULL, &type, (LPBYTE) buf, &size);
            if(hr == ERROR_SUCCESS) {
                if(type != REG_QWORD) {
                    rv= KHM_ERROR_TYPE_MISMATCH;
                    goto _exit;
                }
                else {
                    rv = KHM_ERROR_SUCCESS;
                    goto _exit;
                }
            }
        }

        size = sizeof(khm_int64);
        if(hkm) {
            hr = RegQueryValueEx(hkm, value, NULL, &type, (LPBYTE) buf, &size);
            if(hr == ERROR_SUCCESS) {
                if(type != REG_QWORD) {
                    rv = KHM_ERROR_TYPE_MISMATCH;
                    goto _exit;
                }
                else {
                    rv = KHM_ERROR_SUCCESS;
                    goto _exit;
                }
            }
        }

        if(c->schema && khc_is_schema_handle(conf)) {
            for(i=0;i<c->nSchema;i++) {
                if(c->schema[i].type == KC_INT64 && !wcscmp(value, c->schema[i].name)) {
                    *buf = (khm_int64) c->schema[i].value;
                    rv = KHM_ERROR_SUCCESS;
                    goto _exit;
                }
            }
        }

_shadow:
        if(free_space && conf)
            khc_close_space(conf);
        if(khc_is_shadowed(pconf)) {
            pconf = khc_shadow(pconf);
            continue;
        } else {
            rv = KHM_ERROR_NOT_FOUND;
            break;
        }

_exit:
        if(free_space && conf)
            khc_close_space(conf);
        break;

    } while(TRUE);
    return rv;
}

KHMEXP khm_int32 KHMAPI khc_read_binary(khm_handle pconf, wchar_t * pvalue, void * buf, khm_size * bufsize) {
    kconf_conf_space * c;
    khm_int32 rv = KHM_ERROR_SUCCESS;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    do {
        DWORD size;
        DWORD type;
        LONG hr;
        HKEY hku = NULL;
        HKEY hkm = NULL;

        wchar_t * value = NULL;
        int free_space = 0;
        khm_handle conf = NULL;

        if(wcschr(pvalue, L'\\')
#if 0
           || wcschr(pvalue, L'/')
#endif
           ) {
            if(KHM_FAILED(khc_open_space(
                pconf, 
                pvalue, 
                KCONF_FLAG_TRAILINGVALUE | (pconf?khc_handle_flags(pconf):0), 
                &conf)))
                goto _shadow;
            free_space = 1;
#if 0
            wchar_t * back, *forward;

            back = wcsrchr(pvalue, L'\\');
            forward = wcsrchr(pvalue, L'/');
            value = (back > forward)?back:forward;
#else
            value = wcsrchr(pvalue, L'\\');
#endif
        } else {
            value = pvalue;
            conf = pconf;
            free_space = 0;
        }

        if(!khc_is_handle(conf))
            goto _shadow;

        c = khc_space_from_handle(conf);

        if(khc_is_user_handle(conf))
            hku = khc_space_open_key(c, KHM_PERM_READ);

        if(khc_is_machine_handle(conf))
            hkm = khc_space_open_key(c, KHM_PERM_READ | KCONF_FLAG_MACHINE);

        size = (DWORD) *bufsize;
        if(hku) {
            hr = RegQueryValueEx(hku, value, NULL, &type, (LPBYTE) buf, &size);
            if(hr == ERROR_SUCCESS) {
                if(type != REG_BINARY) {
                    rv = KHM_ERROR_TYPE_MISMATCH;
                    goto _exit;
                }
                else {
                    *bufsize = size;
                    rv =  KHM_ERROR_SUCCESS;
                    goto _exit;
                }
            } else {
                if(hr == ERROR_MORE_DATA) {
                    *bufsize = size;
                    rv = KHM_ERROR_TOO_LONG;
                    goto _exit;
                }
            }
        }

        size = (DWORD) *bufsize;
        if(hkm) {
            hr = RegQueryValueEx(hkm, value, NULL, &type, (LPBYTE) buf, &size);
            if(hr == ERROR_SUCCESS) {
                if(type != REG_BINARY) {
                    rv = KHM_ERROR_TYPE_MISMATCH;
                    goto _exit;
                }
                else {
                    *bufsize = size;
                    rv = KHM_ERROR_SUCCESS;
                    goto _exit;
                }
            } else {
                if(hr == ERROR_MORE_DATA) {
                    *bufsize = size;
                    rv = KHM_ERROR_TOO_LONG;
                    goto _exit;
                }
            }
        }

        /* binary values aren't supported in schema */
_shadow:
        if(free_space && conf)
            khc_close_space(conf);
        if(khc_is_shadowed(pconf)) {
            pconf = khc_shadow(pconf);
            continue;
        } else {
            rv = KHM_ERROR_NOT_FOUND;
            break;
        }

_exit:
        if(free_space && conf)
            khc_close_space(conf);
        break;

    }while (TRUE);

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_write_string(
    khm_handle pconf, 
    wchar_t * pvalue, 
    wchar_t * buf) 
{
    HKEY pk = NULL;
    kconf_conf_space * c;
    khm_int32 rv = KHM_ERROR_SUCCESS;
    LONG hr;
    size_t cbsize;
    wchar_t * value = NULL;
    int free_space;
    khm_handle conf = NULL;


    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(pconf && !khc_is_machine_handle(pconf) && !khc_is_user_handle(pconf))
        return KHM_ERROR_INVALID_OPERATION;

    if(wcschr(pvalue, L'\\')
#if 0
       || wcschr(pvalue, L'/')
#endif
       ) {
        if(KHM_FAILED(khc_open_space(
            pconf, 
            pvalue, 
            KCONF_FLAG_TRAILINGVALUE | (pconf?khc_handle_flags(pconf):0), 
            &conf)))
            return KHM_ERROR_INVALID_PARM;
        free_space = 1;
#if 0
        wchar_t * back, *forward;

        back = wcsrchr(pvalue, L'\\');
        forward = wcsrchr(pvalue, L'/');
        value = (back > forward)?back:forward;
#else
        value = wcsrchr(pvalue, L'\\');
#endif
    } else {
        value = pvalue;
        conf = pconf;
        free_space = 0;
    }

    if(!khc_is_handle(conf) || !buf) {
        rv = KHM_ERROR_INVALID_PARM;
        goto _exit;
    }

    c = khc_space_from_handle(conf);

    if(FAILED(StringCbLength(buf, KCONF_MAXCB_STRING, &cbsize))) {
        rv = KHM_ERROR_INVALID_PARM;
        goto _exit;
    }

    cbsize += sizeof(wchar_t);

    if(khc_is_user_handle(conf)) {
        pk = khc_space_open_key(c, KHM_PERM_WRITE | KHM_FLAG_CREATE);
    } else {
        pk = khc_space_open_key(c, KHM_PERM_WRITE | KCONF_FLAG_MACHINE | KHM_FLAG_CREATE);
    }

    hr = RegSetValueEx(pk, value, 0, REG_SZ, (LPBYTE) buf, (DWORD) cbsize);

    if(hr != ERROR_SUCCESS)
        rv = KHM_ERROR_INVALID_OPERATION;

_exit:
    if(free_space)
        khc_close_space(conf);
    return rv;
}

KHMEXP khm_int32 KHMAPI khc_write_int32(
                                        khm_handle pconf, 
                                        wchar_t * pvalue, 
                                        khm_int32 buf) 
{
    HKEY pk = NULL;
    kconf_conf_space * c;
    khm_int32 rv = KHM_ERROR_SUCCESS;
    LONG hr;
    wchar_t * value = NULL;
    int free_space;
    khm_handle conf = NULL;


    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(pconf && !khc_is_machine_handle(pconf) && !khc_is_user_handle(pconf))
        return KHM_ERROR_INVALID_OPERATION;

    if(wcschr(pvalue, L'\\')
#if 0
       || wcschr(pvalue, L'/')
#endif
       ) {
        if(KHM_FAILED(khc_open_space(
            pconf, 
            pvalue, 
            KCONF_FLAG_TRAILINGVALUE | (pconf?khc_handle_flags(pconf):0), 
            &conf)))
            return KHM_ERROR_INVALID_PARM;
        free_space = 1;
#if 0
        wchar_t * back, *forward;

        back = wcsrchr(pvalue, L'\\');
        forward = wcsrchr(pvalue, L'/');
        value = (back > forward)?back:forward;
#else
        value = wcsrchr(pvalue, L'\\');
#endif
    } else {
        value = pvalue;
        conf = pconf;
        free_space = 0;
    }

    if(!khc_is_handle(conf))
        return KHM_ERROR_INVALID_PARM;

    c = khc_space_from_handle( conf);

    if(khc_is_user_handle(conf)) {
        pk = khc_space_open_key(c, KHM_PERM_WRITE | KHM_FLAG_CREATE);
    } else {
        pk = khc_space_open_key(c, KHM_PERM_WRITE | KCONF_FLAG_MACHINE | KHM_FLAG_CREATE);
    }

    hr = RegSetValueEx(pk, value, 0, REG_DWORD, (LPBYTE) &buf, sizeof(khm_int32));

    if(hr != ERROR_SUCCESS)
        rv = KHM_ERROR_INVALID_OPERATION;

    if(free_space)
        khc_close_space(conf);

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_write_int64(khm_handle pconf, wchar_t * pvalue, khm_int64 buf) {
    HKEY pk = NULL;
    kconf_conf_space * c;
    khm_int32 rv = KHM_ERROR_SUCCESS;
    LONG hr;
    wchar_t * value = NULL;
    int free_space;
    khm_handle conf = NULL;


    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(pconf && !khc_is_machine_handle(pconf) && !khc_is_user_handle(pconf))
        return KHM_ERROR_INVALID_OPERATION;

    if(wcschr(pvalue, L'\\')
#if 0
       || wcschr(pvalue, L'/')
#endif
       ) {
        if(KHM_FAILED(khc_open_space(
            pconf, 
            pvalue, 
            KCONF_FLAG_TRAILINGVALUE | (pconf?khc_handle_flags(pconf):0), 
            &conf)))
            return KHM_ERROR_INVALID_PARM;
        free_space = 1;
#if 0
        wchar_t * back, *forward;

        back = wcsrchr(pvalue, L'\\');
        forward = wcsrchr(pvalue, L'/');
        value = (back > forward)?back:forward;
#else
        value = wcsrchr(pvalue, L'\\');
#endif
    } else {
        value = pvalue;
        conf = pconf;
        free_space = 0;
    }

    if(!khc_is_handle(conf))
        return KHM_ERROR_INVALID_PARM;

    c = khc_space_from_handle( conf);

    if(khc_is_user_handle(conf)) {
        pk = khc_space_open_key(c, KHM_PERM_WRITE | KHM_FLAG_CREATE);
    } else {
        pk = khc_space_open_key(c, KHM_PERM_WRITE | KCONF_FLAG_MACHINE | KHM_FLAG_CREATE);
    }

    hr = RegSetValueEx(pk, value, 0, REG_QWORD, (LPBYTE) &buf, sizeof(khm_int64));

    if(hr != ERROR_SUCCESS)
        rv = KHM_ERROR_INVALID_OPERATION;

    if(free_space)
        khc_close_space(conf);

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_write_binary(khm_handle pconf, wchar_t * pvalue, void * buf, khm_size bufsize) {
    HKEY pk = NULL;
    kconf_conf_space * c;
    khm_int32 rv = KHM_ERROR_SUCCESS;
    LONG hr;
    wchar_t * value = NULL;
    int free_space;
    khm_handle conf = NULL;


    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(pconf && !khc_is_machine_handle(pconf) && !khc_is_user_handle(pconf))
        return KHM_ERROR_INVALID_OPERATION;

    if(wcschr(pvalue, L'\\')
#if 0
       || wcschr(pvalue, L'/')
#endif
       ) {
        if(KHM_FAILED(khc_open_space(
            pconf, 
            pvalue, 
            KCONF_FLAG_TRAILINGVALUE | (pconf?khc_handle_flags(pconf):0), 
            &conf)))
            return KHM_ERROR_INVALID_PARM;
        free_space = 1;
#if 0
        wchar_t * back, *forward;

        back = wcsrchr(pvalue, L'\\');
        forward = wcsrchr(pvalue, L'/');
        value = (back > forward)?back:forward;
#else
        value = wcsrchr(pvalue, L'\\');
#endif
    } else {
        value = pvalue;
        conf = pconf;
        free_space = 0;
    }

    if(!khc_is_handle(conf))
        return KHM_ERROR_INVALID_PARM;

    c = khc_space_from_handle(conf);

    if(khc_is_user_handle(conf)) {
        pk = khc_space_open_key(c, KHM_PERM_WRITE | KHM_FLAG_CREATE);
    } else {
        pk = khc_space_open_key(c, KHM_PERM_WRITE | KCONF_FLAG_MACHINE | KHM_FLAG_CREATE);
    }

    hr = RegSetValueEx(pk, value, 0, REG_BINARY, buf, (DWORD) bufsize);

    if(hr != ERROR_SUCCESS)
        rv = KHM_ERROR_INVALID_OPERATION;

    if(free_space)
        khc_close_space(conf);

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_get_config_space_name(khm_handle conf, wchar_t * buf, khm_size * bufsize) {
    kconf_conf_space * c;
    khm_int32 rv = KHM_ERROR_SUCCESS;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(!khc_is_handle(conf))
        return KHM_ERROR_INVALID_PARM;

    c = khc_space_from_handle(conf);

    if(!c->name) {
        if(buf && *bufsize > 0)
            buf[0] = L'\0';
        else {
            *bufsize = sizeof(wchar_t);
            rv = KHM_ERROR_TOO_LONG;
        }
    } else {
        size_t cbsize;

        if(FAILED(StringCbLength(c->name, KCONF_MAXCB_NAME, &cbsize)))
            return KHM_ERROR_UNKNOWN;

        cbsize += sizeof(wchar_t);

        if(!buf || cbsize > *bufsize) {
            *bufsize = cbsize;
            rv = KHM_ERROR_TOO_LONG;
        } else {
            StringCbCopy(buf, *bufsize, c->name);
            *bufsize = cbsize;
        }
    }

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_get_config_space_parent(khm_handle conf, khm_handle * parent) {
    kconf_conf_space * c;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(!khc_is_handle(conf))
        return KHM_ERROR_INVALID_PARM;

    c = khc_space_from_handle(conf);

    if(c == conf_root || c->parent == conf_root)
        *parent = NULL;
    else
        *parent = khc_handle_from_space(c->parent, khc_handle_flags(conf));

    return KHM_ERROR_SUCCESS;
}

KHMEXP khm_int32 KHMAPI khc_get_type(khm_handle conf, wchar_t * value) {
    HKEY hkm = NULL;
    HKEY hku = NULL;
    kconf_conf_space * c;
    khm_int32 rv;
    LONG hr;
    DWORD type = 0;

    if(!khc_is_config_running())
        return KC_NONE;

    if(!khc_is_handle(conf))
        return KC_NONE;

    c = (kconf_conf_space *) conf;

    if(!khc_is_machine_handle(conf))
        hku = khc_space_open_key(c, KHM_PERM_READ);
    hkm = khc_space_open_key(c, KHM_PERM_READ | KCONF_FLAG_MACHINE);

    if(hku)
        hr = RegQueryValueEx(hku, value, NULL, &type, NULL, NULL);
    if(!hku || hr != ERROR_SUCCESS)
        hr = RegQueryValueEx(hkm, value, NULL, &type, NULL, NULL);
    if(((!hku && !hkm) || hr != ERROR_SUCCESS) && c->schema) {
        int i;

        for(i=0; i<c->nSchema; i++) {
            if(!wcscmp(c->schema[i].name, value)) {
                return c->schema[i].type;
            }
        }

        return KC_NONE;
    }

    switch(type) {
        case REG_MULTI_SZ:
        case REG_SZ:
            rv = KC_STRING;
            break;
        case REG_DWORD:
            rv = KC_INT32;
            break;
        case REG_QWORD:
            rv = KC_INT64;
            break;
        case REG_BINARY:
            rv = KC_BINARY;
            break;
        default:
            rv = KC_NONE;
    }

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_value_exists(khm_handle conf, wchar_t * value) {
    HKEY hku = NULL;
    HKEY hkm = NULL;
    kconf_conf_space * c;
    khm_int32 rv = 0;
    DWORD t;
    int i;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(!khc_is_handle(conf))
        return KHM_ERROR_INVALID_PARM;

    c = khc_space_from_handle(conf);

    if(!khc_is_machine_handle(conf))
        hku = khc_space_open_key(c, KHM_PERM_READ);
    hkm = khc_space_open_key(c, KHM_PERM_READ | KCONF_FLAG_MACHINE);

    if(hku && (RegQueryValueEx(hku, value, NULL, &t, NULL, NULL) == ERROR_SUCCESS))
        rv |= KCONF_FLAG_USER;
    if(hkm && (RegQueryValueEx(hkm, value, NULL, &t, NULL, NULL) == ERROR_SUCCESS))
        rv |= KCONF_FLAG_MACHINE;

    if(c->schema) {
        for(i=0; i<c->nSchema; i++) {
            if(!wcscmp(c->schema[i].name, value)) {
                rv |= KCONF_FLAG_SCHEMA;
                break;
            }
        }
    }

    return rv;
}

khm_boolean khc_is_valid_name(wchar_t * name)
{
    size_t cbsize;
    if(FAILED(StringCbLength(name, KCONF_MAXCB_NAME, &cbsize)))
        return FALSE;
    return TRUE;
}

khm_int32 khc_validate_schema(kconf_schema * schema,
                              int begin,
                              int *end)
{
    int i;
    int state = 0;
    int end_found = 0;

    i=begin;
    while(!end_found) {
        switch(state) {
            case 0: /* initial.  this record should start a config space */
                if(!khc_is_valid_name(schema[i].name) ||
                    schema[i].type != KC_SPACE)
                    return KHM_ERROR_INVALID_PARM;
                state = 1;
                break;

            case 1: /* we are inside a config space, in the values area */
                if(!khc_is_valid_name(schema[i].name))
                    return KHM_ERROR_INVALID_PARM;
                if(schema[i].type == KC_SPACE) {
                    if(KHM_FAILED(khc_validate_schema(schema, i, &i)))
                        return KHM_ERROR_INVALID_PARM;
                    state = 2;
                } else if(schema[i].type == KC_ENDSPACE) {
                    end_found = 1;
                    if(end)
                        *end = i;
                } else {
                    if(schema[i].type != KC_STRING &&
                        schema[i].type != KC_INT32 &&
                        schema[i].type != KC_INT64 &&
                        schema[i].type != KC_BINARY)
                        return KHM_ERROR_INVALID_PARM;
                }
                break;

            case 2: /* we are inside a config space, in the subspace area */
                if(schema[i].type == KC_SPACE) {
                    if(KHM_FAILED(khc_validate_schema(schema, i, &i)))
                        return KHM_ERROR_INVALID_PARM;
                } else if(schema[i].type == KC_ENDSPACE) {
                    end_found = 1;
                    if(end)
                        *end = i;
                } else {
                    return KHM_ERROR_INVALID_PARM;
                }
                break;

            default:
                /* unreachable */
                return KHM_ERROR_INVALID_PARM;
        }
        i++;
    }

    return KHM_ERROR_SUCCESS;
}

khm_int32 khc_load_schema_i(khm_handle parent, kconf_schema * schema, int begin, int * end)
{
    int i;
    int state = 0;
    int end_found = 0;
    kconf_conf_space * thisconf = NULL;
    khm_handle h;

    i=begin;
    while(!end_found) {
        switch(state) {
            case 0: /* initial.  this record should start a config space */
                if(KHM_FAILED(khc_open_space(parent, schema[i].name, KHM_FLAG_CREATE, &h)))
                    return KHM_ERROR_INVALID_PARM;
                thisconf = khc_space_from_handle(h);
                thisconf->schema = schema + (begin + 1);
                state = 1;
                break;

            case 1: /* we are inside a config space, in the values area */
                if(schema[i].type == KC_SPACE) {
                    thisconf->nSchema = i - (begin + 1);
                    if(KHM_FAILED(khc_load_schema_i(h, schema, i, &i)))
                        return KHM_ERROR_INVALID_PARM;
                    state = 2;
                } else if(schema[i].type == KC_ENDSPACE) {
                    thisconf->nSchema = i - (begin + 1);
                    end_found = 1;
                    if(end)
                        *end = i;
                    khc_close_space(h);
                }
                break;

            case 2: /* we are inside a config space, in the subspace area */
                if(schema[i].type == KC_SPACE) {
                    if(KHM_FAILED(khc_load_schema_i(h, schema, i, &i)))
                        return KHM_ERROR_INVALID_PARM;
                } else if(schema[i].type == KC_ENDSPACE) {
                    end_found = 1;
                    if(end)
                        *end = i;
                    khc_close_space(h);
                } else {
                    return KHM_ERROR_INVALID_PARM;
                }
                break;

            default:
                /* unreachable */
                return KHM_ERROR_INVALID_PARM;
        }
        i++;
    }

    return KHM_ERROR_SUCCESS;
}

KHMEXP khm_int32 KHMAPI khc_load_schema(khm_handle conf, kconf_schema * schema)
{
    khm_int32 rv = KHM_ERROR_SUCCESS;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(conf && !khc_is_handle(conf))
        return KHM_ERROR_INVALID_PARM;

    if(KHM_FAILED(khc_validate_schema(schema, 0, NULL)))
        return KHM_ERROR_INVALID_PARM;

    EnterCriticalSection(&cs_conf_global);
    rv = khc_load_schema_i(conf, schema, 0, NULL);        
    LeaveCriticalSection(&cs_conf_global);

    return rv;
}

khm_int32 khc_unload_schema_i(khm_handle parent, kconf_schema * schema, int begin, int * end)
{
    int i;
    int state = 0;
    int end_found = 0;
    kconf_conf_space * thisconf = NULL;
    khm_handle h;

    i=begin;
    while(!end_found) {
        switch(state) {
            case 0: /* initial.  this record should start a config space */
                if(KHM_FAILED(khc_open_space(parent, schema[i].name, 0, &h)))
                    return KHM_ERROR_INVALID_PARM;
                thisconf = khc_space_from_handle(h);
                if(thisconf->schema == (schema + (begin + 1))) {
                    thisconf->schema = NULL;
                    thisconf->nSchema = 0;
                }
                state = 1;
                break;

            case 1: /* we are inside a config space, in the values area */
                if(schema[i].type == KC_SPACE) {
                    if(KHM_FAILED(khc_unload_schema_i(h, schema, i, &i)))
                        return KHM_ERROR_INVALID_PARM;
                    state = 2;
                } else if(schema[i].type == KC_ENDSPACE) {
                    end_found = 1;
                    if(end)
                        *end = i;
                    khc_close_space(h);
                }
                break;

            case 2: /* we are inside a config space, in the subspace area */
                if(schema[i].type == KC_SPACE) {
                    if(KHM_FAILED(khc_unload_schema_i(h, schema, i, &i)))
                        return KHM_ERROR_INVALID_PARM;
                } else if(schema[i].type == KC_ENDSPACE) {
                    end_found = 1;
                    if(end)
                        *end = i;
                    khc_close_space(h);
                } else {
                    return KHM_ERROR_INVALID_PARM;
                }
                break;

            default:
                /* unreachable */
                return KHM_ERROR_INVALID_PARM;
        }
        i++;
    }

    return KHM_ERROR_SUCCESS;
}

KHMEXP khm_int32 KHMAPI khc_unload_schema(khm_handle conf, kconf_schema * schema)
{
    khm_int32 rv = KHM_ERROR_SUCCESS;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(conf && !khc_is_handle(conf))
        return KHM_ERROR_INVALID_PARM;

    if(KHM_FAILED(khc_validate_schema(schema, 0, NULL)))
        return KHM_ERROR_INVALID_PARM;

    EnterCriticalSection(&cs_conf_global);
    rv = khc_unload_schema_i(conf, schema, 0, NULL);        
    LeaveCriticalSection(&cs_conf_global);

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_enum_subspaces(
    khm_handle conf,
    khm_handle prev,
    khm_handle * next)
{
    kconf_conf_space * s;
    kconf_conf_space * c;
    kconf_conf_space * p;
    khm_int32 rv = KHM_ERROR_SUCCESS;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(!khc_is_handle(conf) || next == NULL ||
        (prev != NULL && !khc_is_handle(prev)))
        return KHM_ERROR_INVALID_PARM;

    s = khc_space_from_handle(conf);

    if(prev == NULL) {
        /* first off, we enumerate all the registry spaces regardless of
        whether the handle is applicable for some registry space or not.
        See notes for khc_begin_enum_subspaces() for reasons as to why
        this is done (notes are in kconfig.h)*/

        /* go through the user hive first */
        {
            HKEY hk_conf;

            hk_conf = khc_space_open_key(s, 0);
            if(hk_conf) {
                wchar_t name[KCONF_MAXCCH_NAME];
                khm_handle h;
                int idx;

                idx = 0;
                while(RegEnumKey(hk_conf, idx, 
                                 name, ARRAYLENGTH(name)) == ERROR_SUCCESS) {
                    wchar_t * tilde;
                    tilde = wcschr(name, L'~');
                    if (tilde)
                        *tilde = 0;
                    if(KHM_SUCCEEDED(khc_open_space(conf, name, 0, &h)))
                        khc_close_space(h);
                    idx++;
                }
            }
        }

        /* go through the machine hive next */
        {
            HKEY hk_conf;

            hk_conf = khc_space_open_key(s, KCONF_FLAG_MACHINE);
            if(hk_conf) {
                wchar_t name[KCONF_MAXCCH_NAME];
                khm_handle h;
                int idx;

                idx = 0;
                while(RegEnumKey(hk_conf, idx, 
                                 name, ARRAYLENGTH(name)) == ERROR_SUCCESS) {
                    wchar_t * tilde;
                    tilde = wcschr(name, L'~');
                    if (tilde)
                        *tilde = 0;

                    if(KHM_SUCCEEDED(khc_open_space(conf, name, 
                                                    KCONF_FLAG_MACHINE, &h)))
                        khc_close_space(h);
                    idx++;
                }
            }
        }

        /* don't need to go through schema, because that was already
        done when the schema was loaded. */
    }

    /* at last we are now ready to return the results */
    EnterCriticalSection(&cs_conf_global);
    if(prev == NULL) {
        c = TFIRSTCHILD(s);
        rv = KHM_ERROR_SUCCESS;
    } else {
        p = khc_space_from_handle(prev);
        if(TPARENT(p) == s)
            c = LNEXT(p);
        else
            c = NULL;
    }
    LeaveCriticalSection(&cs_conf_global);

    if(prev != NULL)
        khc_close_space(prev);

    if(c) {
        *next = khc_handle_from_space(c, khc_handle_flags(conf));
        rv = KHM_ERROR_SUCCESS;
    } else {
        *next = NULL;
        rv = KHM_ERROR_NOT_FOUND;
    }

    return rv;
}

KHMEXP khm_int32 KHMAPI khc_write_multi_string(khm_handle conf, wchar_t * value, wchar_t * buf)
{
    size_t cb;
    wchar_t *tb;
    khm_int32 rv;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;
    if(!khc_is_handle(conf) || buf == NULL || value == NULL)
        return KHM_ERROR_INVALID_PARM;

    if(multi_string_to_csv(NULL, &cb, buf) != KHM_ERROR_TOO_LONG)
        return KHM_ERROR_INVALID_PARM;

    tb = malloc(cb);
	assert(tb != NULL);
    multi_string_to_csv(tb, &cb, buf);
    rv = khc_write_string(conf, value, tb);

    free(tb);
    return rv;
}

KHMEXP khm_int32 KHMAPI khc_read_multi_string(khm_handle conf, wchar_t * value, wchar_t * buf, khm_size * bufsize)
{
    wchar_t * tb;
    khm_size cbbuf;
    khm_int32 rv = KHM_ERROR_SUCCESS;

    if(!khc_is_config_running())
        return KHM_ERROR_NOT_READY;

    if(!bufsize)
        return KHM_ERROR_INVALID_PARM;

    rv = khc_read_string(conf, value, NULL, &cbbuf);
    if(rv != KHM_ERROR_TOO_LONG)
        return rv;

    tb = malloc(cbbuf);
	assert(tb != NULL);
    rv = khc_read_string(conf, value, tb, &cbbuf);

    if(KHM_FAILED(rv))
        goto _exit;

    rv = csv_to_multi_string(buf, bufsize, tb);

_exit:
    free(tb);

    return rv;
}
