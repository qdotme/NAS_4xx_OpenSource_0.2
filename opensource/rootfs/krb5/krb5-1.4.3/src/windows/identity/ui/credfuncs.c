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

/* $Id: credfuncs.c,v 1.1.1.1 2006/05/19 07:08:14 arsene Exp $ */

#include<khmapp.h>
#include<assert.h>

static BOOL in_dialog = FALSE;
static CRITICAL_SECTION cs_dialog;
static HANDLE in_dialog_evt = NULL;
static LONG init_dialog = 0;
static khm_int32 dialog_result = 0;

static void
dialog_sync_init(void) {
    if (InterlockedIncrement(&init_dialog) == 1) {
#ifdef DEBUG
        assert(in_dialog_evt == NULL);
        assert(in_dialog == FALSE);
#endif

        InitializeCriticalSection(&cs_dialog);

        in_dialog_evt = CreateEvent(NULL,
                                    TRUE,
                                    TRUE,
                                    L"DialogCompletionEvent");
    } else {
        InterlockedDecrement(&init_dialog);
        if (in_dialog_evt == NULL) {
            Sleep(100);
        }
    }
}

BOOL 
khm_cred_begin_dialog(void) {
    BOOL rv;

    dialog_sync_init();

    EnterCriticalSection(&cs_dialog);

    if (in_dialog)
        rv = FALSE;
    else {
        rv = TRUE;
        in_dialog = TRUE;
        ResetEvent(in_dialog_evt);
    }

    LeaveCriticalSection(&cs_dialog);
    return rv;
}

void 
khm_cred_end_dialog(khm_int32 result) {
    dialog_sync_init();

    EnterCriticalSection(&cs_dialog);
    if (in_dialog) {
        in_dialog = FALSE;
        SetEvent(in_dialog_evt);
    }
    dialog_result = result;
    LeaveCriticalSection(&cs_dialog);
}

BOOL
khm_cred_is_in_dialog(void) {
    BOOL rv;

    dialog_sync_init();

    EnterCriticalSection(&cs_dialog);
    rv = in_dialog;
    LeaveCriticalSection(&cs_dialog);

    return rv;
}

khm_int32
khm_cred_wait_for_dialog(DWORD timeout, khm_int32 * result) {
    khm_int32 rv;

    dialog_sync_init();

    EnterCriticalSection(&cs_dialog);
    if (!in_dialog)
        rv = KHM_ERROR_NOT_FOUND;
    else {
        DWORD dw;

        do {
            LeaveCriticalSection(&cs_dialog);

            dw = WaitForSingleObject(in_dialog_evt, timeout);

            EnterCriticalSection(&cs_dialog);

            if (!in_dialog) {
                rv = KHM_ERROR_SUCCESS;
                if (result)
                    *result = dialog_result;
                break;
            } else if(dw == WAIT_TIMEOUT) {
                rv = KHM_ERROR_TIMEOUT;
                break;
            }
        } while(TRUE);
    }
    LeaveCriticalSection(&cs_dialog);

    return rv;
}

/* completion handler for KMSG_CRED messages */
void KHMAPI 
kmsg_cred_completion(kmq_message *m)
{
    khui_new_creds * nc;

#ifdef DEBUG
    assert(m->type == KMSG_CRED);
#else
    if(m->type != KMSG_CRED)
        return; /* huh? */
#endif

    switch(m->subtype) {
    case KMSG_CRED_PASSWORD:
        /* fallthrough */
    case KMSG_CRED_NEW_CREDS:
        /* Cred types have attached themselves.  Trigger the next
           phase. */
        kmq_post_message(KMSG_CRED, KMSG_CRED_DIALOG_SETUP, 0, 
                         m->vparam);
        break;

    case KMSG_CRED_RENEW_CREDS:
        nc = (khui_new_creds *) m->vparam;

        /* khm_cred_dispatch_process_message() deals with the case
           where there are not credential types that wants to
           participate in this operation. */
        khm_cred_dispatch_process_message(nc);
        break;

    case KMSG_CRED_DIALOG_SETUP:
        nc = (khui_new_creds *) m->vparam;

        khm_prep_newcredwnd(nc->hwnd);
            
        /* all the controls have been created.  Now initialize them */
        if (nc->n_types > 0) {
            kmq_post_subs_msg(nc->type_subs, 
                              nc->n_types, 
                              KMSG_CRED, 
                              KMSG_CRED_DIALOG_PRESTART, 
                              0, 
                              m->vparam);
        } else {
            PostMessage(nc->hwnd, KHUI_WM_NC_NOTIFY, 
                        MAKEWPARAM(0, WMNC_DIALOG_PROCESS_COMPLETE), 0);
        }
        break;

    case KMSG_CRED_DIALOG_PRESTART:
        /* all prestart stuff is done.  Now to activate the dialog */
        nc = (khui_new_creds *) m->vparam;
        khm_show_newcredwnd(nc->hwnd);
        
        kmq_post_subs_msg(nc->type_subs,
                          nc->n_types,
                          KMSG_CRED, 
                          KMSG_CRED_DIALOG_START, 
                          0, 
                          m->vparam);
        /* at this point, the dialog window takes over.  We let it run
           the show until KMSG_CRED_DIALOG_END is posted by the dialog
           procedure. */
        break;

    case KMSG_CRED_PROCESS:
        /* a wave of these messages have completed.  We should check
           if there's more */
        nc = (khui_new_creds *) m->vparam;

        if(!khm_cred_dispatch_process_level(nc)) {

            if(kherr_is_error()) {
                khui_alert * alert;
                kherr_event * evt;
                kherr_context * ctx;
                wchar_t ws_title[1024];

                ctx = kherr_peek_context();
                evt = kherr_get_err_event(ctx);
                kherr_evaluate_event(evt);

                khui_alert_create_empty(&alert);

                if (nc->subtype == KMSG_CRED_PASSWORD)
                    LoadString(khm_hInstance, IDS_NC_PWD_FAILED_TITLE,
                               ws_title, ARRAYLENGTH(ws_title));
                else
                    LoadString(khm_hInstance, IDS_NC_FAILED_TITLE,
                               ws_title, ARRAYLENGTH(ws_title));

                khui_alert_set_title(alert, ws_title);
                khui_alert_set_severity(alert, evt->severity);
                if(!evt->long_desc)
                    khui_alert_set_message(alert, evt->short_desc);
                else
                    khui_alert_set_message(alert, evt->long_desc);
                if(evt->suggestion)
                    khui_alert_set_suggestion(alert, evt->suggestion);

                khui_alert_show(alert);
                khui_alert_release(alert);

                kherr_release_context(ctx);

                kherr_clear_error();
            }

            if (nc->subtype == KMSG_CRED_RENEW_CREDS) {
                kmq_post_message(KMSG_CRED, KMSG_CRED_END, 0, 
                                 m->vparam);
            } else {
                PostMessage(nc->hwnd, KHUI_WM_NC_NOTIFY, 
                            MAKEWPARAM(0, WMNC_DIALOG_PROCESS_COMPLETE),
                            0);
            }
        }
        break;

    case KMSG_CRED_END:
        /* all is done. */
        {
            khui_new_creds * nc;

            nc = (khui_new_creds *) m->vparam;

            if (nc->subtype == KMSG_CRED_NEW_CREDS ||
                nc->subtype == KMSG_CRED_PASSWORD) {

                if (nc->subtype == KMSG_CRED_NEW_CREDS)
                    khui_context_reset();

                khm_cred_end_dialog(nc->result);
            }

            khui_cw_destroy_cred_blob(nc);

            kmq_post_message(KMSG_CRED, KMSG_CRED_REFRESH, 0, 0);

            khm_cred_process_commandline();
        }
        break;

        /* property sheet stuff */

    case KMSG_CRED_PP_BEGIN:
        /* all the pages should have been added by now.  Just send out
           the precreate message */
        kmq_post_message(KMSG_CRED, KMSG_CRED_PP_PRECREATE, 0, 
                         m->vparam);
        break;

    case KMSG_CRED_PP_END:
        kmq_post_message(KMSG_CRED, KMSG_CRED_PP_DESTROY, 0, 
                         m->vparam);
        break;

    case KMSG_CRED_DESTROY_CREDS:
#ifdef DEBUG
        assert(m->vparam != NULL);
#endif
        khui_context_release((khui_action_context *) m->vparam);
        free(m->vparam);

        kmq_post_message(KMSG_CRED, KMSG_CRED_REFRESH, 0, 0);

        khm_cred_process_commandline();
        break;

    case KMSG_CRED_IMPORT:
        khm_cred_process_commandline();
        break;
    }
}

void khm_cred_import(void)
{
    _begin_task(KHERR_CF_TRANSITIVE);
    _report_sr0(KHERR_NONE, IDS_CTX_IMPORT);
    _describe();

    kmq_post_message(KMSG_CRED, KMSG_CRED_IMPORT, 0, 0);

    _end_task();
}

void khm_cred_set_default(void)
{
    khui_action_context ctx;
    khm_int32 rv;

    khui_context_get(&ctx);

    if (ctx.identity) {
        rv = kcdb_identity_set_default(ctx.identity);
    }

    khui_context_release(&ctx);
}

void khm_cred_destroy_creds(void)
{
    khui_action_context * pctx;

    pctx = malloc(sizeof(*pctx));
#ifdef DEBUG
    assert(pctx);
#endif

    khui_context_get(pctx);

    if(pctx->scope == KHUI_SCOPE_NONE) {
        /* this really shouldn't be necessary once we start enabling
           and disbling actions based on context */
        wchar_t title[256];
        wchar_t message[256];

        LoadString(khm_hInstance, 
                   IDS_ALERT_NOSEL_TITLE, 
                   title, 
                   ARRAYLENGTH(title));

        LoadString(khm_hInstance, 
                   IDS_ALERT_NOSEL, 
                   message, 
                   ARRAYLENGTH(message));

        khui_alert_show_simple(title, 
                               message, 
                               KHERR_WARNING);

        khui_context_release(pctx);
        free(pctx);
    } else {
        _begin_task(KHERR_CF_TRANSITIVE);
        _report_sr0(KHERR_NONE, IDS_CTX_RENEW_CREDS);
        _describe();

        kmq_post_message(KMSG_CRED,
                         KMSG_CRED_DESTROY_CREDS,
                         0,
                         (void *) pctx);

        _end_task();
    }
}

void khm_cred_renew_identity(khm_handle identity)
{
    khui_new_creds * c;

    khui_cw_create_cred_blob(&c);

    c->subtype = KMSG_CRED_RENEW_CREDS;
    c->result = KHUI_NC_RESULT_GET_CREDS;
    khui_context_create(&c->ctx,
                        KHUI_SCOPE_IDENT,
                        identity,
                        KCDB_CREDTYPE_INVALID,
                        NULL);

    _begin_task(KHERR_CF_TRANSITIVE);
    _report_sr0(KHERR_NONE, IDS_CTX_RENEW_CREDS);
    _describe();

    kmq_post_message(KMSG_CRED, KMSG_CRED_RENEW_CREDS, 0, (void *) c);

    _end_task();
}

void khm_cred_renew_cred(khm_handle cred)
{
    khui_new_creds * c;

    khui_cw_create_cred_blob(&c);

    c->subtype = KMSG_CRED_RENEW_CREDS;
    c->result = KHUI_NC_RESULT_GET_CREDS;
    khui_context_create(&c->ctx,
                        KHUI_SCOPE_CRED,
                        NULL,
                        KCDB_CREDTYPE_INVALID,
                        cred);

    _begin_task(KHERR_CF_TRANSITIVE);
    _report_sr0(KHERR_NONE, IDS_CTX_RENEW_CREDS);
    _describe();

    kmq_post_message(KMSG_CRED, KMSG_CRED_RENEW_CREDS, 0, (void *) c);

    _end_task();
}

void khm_cred_renew_creds(void)
{
    khui_new_creds * c;

    khui_cw_create_cred_blob(&c);
    c->subtype = KMSG_CRED_RENEW_CREDS;
    c->result = KHUI_NC_RESULT_GET_CREDS;
    khui_context_get(&c->ctx);

    _begin_task(KHERR_CF_TRANSITIVE);
    _report_sr0(KHERR_NONE, IDS_CTX_RENEW_CREDS);
    _describe();

    kmq_post_message(KMSG_CRED, KMSG_CRED_RENEW_CREDS, 0, (void *) c);

    _end_task();
}

void khm_cred_change_password(wchar_t * title)
{
    khui_new_creds * nc;
    LPNETID_DLGINFO pdlginfo;
    khm_size cb;

    if (!khm_cred_begin_dialog())
        return;

    khui_cw_create_cred_blob(&nc);
    nc->subtype = KMSG_CRED_PASSWORD;

    khui_context_get(&nc->ctx);

    kcdb_identpro_get_ui_cb((void *) &nc->ident_cb);

    assert(nc->ident_cb);

    if (title) {

        if (SUCCEEDED(StringCbLength(title, KHUI_MAXCB_TITLE, &cb))) {
            cb += sizeof(wchar_t);

            nc->window_title = malloc(cb);
#ifdef DEBUG
            assert(nc->window_title);
#endif
            StringCbCopy(nc->window_title, cb, title);
        }
    } else if (nc->ctx.cb_vparam == sizeof(NETID_DLGINFO) &&
               (pdlginfo = nc->ctx.vparam) &&
               pdlginfo->size == NETID_DLGINFO_V1_SZ &&
               pdlginfo->in.title[0] &&
               SUCCEEDED(StringCchLength(pdlginfo->in.title,
                                         NETID_TITLE_SZ,
                                         &cb))) {

        cb = (cb + 1) * sizeof(wchar_t);
        nc->window_title = malloc(cb);
#ifdef DEBUG
        assert(nc->window_title);
#endif
        StringCbCopy(nc->window_title, cb, pdlginfo->in.title);
    }

    khm_create_newcredwnd(khm_hwnd_main, nc);

    if (nc->hwnd != NULL) {
        _begin_task(KHERR_CF_TRANSITIVE);
        _report_sr0(KHERR_NONE, IDS_CTX_PASSWORD);
        _describe();

        kmq_post_message(KMSG_CRED, KMSG_CRED_PASSWORD, 0,
                         (void *) nc);

        _end_task();
    } else {
        khui_cw_destroy_cred_blob(nc);
    }
}

void khm_cred_obtain_new_creds(wchar_t * title)
{
    khui_new_creds * nc;
    LPNETID_DLGINFO pdlginfo;
    khm_size cb;

    if (!khm_cred_begin_dialog())
        return;

    khui_cw_create_cred_blob(&nc);
    nc->subtype = KMSG_CRED_NEW_CREDS;

    khui_context_get(&nc->ctx);

    kcdb_identpro_get_ui_cb((void *) &nc->ident_cb);

    assert(nc->ident_cb);

    if (title) {
        if (SUCCEEDED(StringCbLength(title, KHUI_MAXCB_TITLE, &cb))) {
            cb += sizeof(wchar_t);

            nc->window_title = malloc(cb);
#ifdef DEBUG
            assert(nc->window_title);
#endif
            StringCbCopy(nc->window_title, cb, title);
        }
    } else if (nc->ctx.cb_vparam == sizeof(NETID_DLGINFO) &&
               (pdlginfo = nc->ctx.vparam) &&
               pdlginfo->size == NETID_DLGINFO_V1_SZ &&
               pdlginfo->in.title[0] &&
               SUCCEEDED(StringCchLength(pdlginfo->in.title,
                                         NETID_TITLE_SZ,
                                         &cb))) {

        cb = (cb + 1) * sizeof(wchar_t);
        nc->window_title = malloc(cb);
#ifdef DEBUG
        assert(nc->window_title);
#endif
        StringCbCopy(nc->window_title, cb, pdlginfo->in.title);
    }

    khm_create_newcredwnd(khm_hwnd_main, nc);

    if (nc->hwnd != NULL) {
        _begin_task(KHERR_CF_TRANSITIVE);
        _report_sr0(KHERR_NONE, IDS_CTX_NEW_CREDS);
        _describe();

        kmq_post_message(KMSG_CRED, KMSG_CRED_NEW_CREDS, 0, 
                         (void *) nc);

        _end_task();
    } else {
        khui_cw_destroy_cred_blob(nc);
    }
}

/* this is called by khm_cred_dispatch_process_message and the
   kmsg_cred_completion to initiate and continue checked broadcasts of
   KMSG_CRED_DIALOG_PROCESS messages.
   
   Returns TRUE if more KMSG_CRED_DIALOG_PROCESS messages were
   posted. */
BOOL khm_cred_dispatch_process_level(khui_new_creds *nc)
{
    khm_size i,j;
    khm_handle subs[KHUI_MAX_NCTYPES];
    int n_subs = 0;
    BOOL cont = FALSE;
    khui_new_creds_by_type *t, *d;

    /* at each level, we dispatch a wave of notifications to plug-ins
       who's dependencies are all satisfied */
    EnterCriticalSection(&nc->cs);

    /* if any types have already completed, we mark them are processed
       and skip them */
    for (i=0; i < nc->n_types; i++) {
        t = nc->types[i];
        if(t->flags & KHUI_NC_RESPONSE_COMPLETED)
            t->flags |= KHUI_NCT_FLAG_PROCESSED;
    }

    for(i=0; i<nc->n_types; i++) {
        t = nc->types[i];

        if((t->flags & KHUI_NCT_FLAG_PROCESSED) ||
           (t->flags & KHUI_NC_RESPONSE_COMPLETED))
            continue;

        for(j=0; j<t->n_type_deps; j++) {
            if(KHM_FAILED(khui_cw_find_type(nc, t->type_deps[j], &d)))
                break;

            if(!(d->flags & KHUI_NC_RESPONSE_COMPLETED))
                break;
        }

        if(j<t->n_type_deps) /* there are unmet dependencies */
            continue;

        /* all dependencies for this type have been met. */
        subs[n_subs++] = kcdb_credtype_get_sub(t->type);
        t->flags |= KHUI_NCT_FLAG_PROCESSED;
        cont = TRUE;
    }

    LeaveCriticalSection(&nc->cs);

    /* the reason why we are posting messages in batches is because
       when the message has completed we know that all the types that
       have the KHUI_NCT_FLAG_PROCESSED set have completed processing.
       Otherwise we have to individually track each message and update
       the type */
    if(n_subs > 0)
        kmq_post_subs_msg(subs, n_subs, KMSG_CRED, KMSG_CRED_PROCESS, 0,
                          (void *) nc);

    return cont;
}

void 
khm_cred_dispatch_process_message(khui_new_creds *nc)
{
    khm_size i;
    BOOL pending;
    wchar_t wsinsert[512];
    khm_size cbsize;

    /* see if there's anything to do.  We can check this without
       obtaining a lock */
    if(nc->n_types == 0 ||
       (nc->subtype == KMSG_CRED_NEW_CREDS &&
        nc->n_identities == 0) ||
       (nc->subtype == KMSG_CRED_PASSWORD &&
        nc->n_identities == 0))
        goto _terminate_job;

    /* check dependencies and stuff first */
    EnterCriticalSection(&nc->cs);
    for(i=0; i<nc->n_types; i++) {
        nc->types[i]->flags &= ~ KHUI_NCT_FLAG_PROCESSED;
    }
    LeaveCriticalSection(&nc->cs);

    /* Consindering all that can go wrong here and the desire to
       handle errors here separately from others, we create a new task
       for the purpose of tracking the credentials acquisition
       process. */
    _begin_task(KHERR_CF_TRANSITIVE);

    /* Describe the context */
    if(nc->subtype == KMSG_CRED_NEW_CREDS) {
        cbsize = sizeof(wsinsert);
        kcdb_identity_get_name(nc->identities[0], wsinsert, &cbsize);

        _report_sr1(KHERR_NONE,  IDS_CTX_PROC_NEW_CREDS,
                    _cstr(wsinsert));
        _resolve();
    } else if (nc->subtype == KMSG_CRED_RENEW_CREDS) {
        cbsize = sizeof(wsinsert);

        if (nc->ctx.scope == KHUI_SCOPE_IDENT)
            kcdb_identity_get_name(nc->ctx.identity, wsinsert, &cbsize);
        else if (nc->ctx.scope == KHUI_SCOPE_CREDTYPE) {
            if (nc->ctx.identity != NULL)
                kcdb_identity_get_name(nc->ctx.identity, wsinsert, 
                                       &cbsize);
            else
                kcdb_credtype_get_name(nc->ctx.cred_type, wsinsert,
                                       &cbsize);
        } else if (nc->ctx.scope == KHUI_SCOPE_CRED) {
            kcdb_cred_get_name(nc->ctx.cred, wsinsert, &cbsize);
        } else {
            StringCbCopy(wsinsert, sizeof(wsinsert), L"(?)");
        }

        _report_sr1(KHERR_NONE, IDS_CTX_PROC_RENEW_CREDS, 
                    _cstr(wsinsert));
        _resolve();
    } else if (nc->subtype == KMSG_CRED_PASSWORD) {
        cbsize = sizeof(wsinsert);
        kcdb_identity_get_name(nc->identities[0], wsinsert, &cbsize);

        _report_sr1(KHERR_NONE, IDS_CTX_PROC_PASSWORD,
                    _cstr(wsinsert));
        _resolve();
    } else {
        assert(FALSE);
    }

    _describe();

    pending = khm_cred_dispatch_process_level(nc);

    _end_task();

    if(!pending)
        goto _terminate_job;

    return;

 _terminate_job:
    if (nc->subtype == KMSG_CRED_RENEW_CREDS)
        kmq_post_message(KMSG_CRED, KMSG_CRED_END, 0, (void *) nc);
    else
        PostMessage(nc->hwnd, KHUI_WM_NC_NOTIFY, 
                    MAKEWPARAM(0, WMNC_DIALOG_PROCESS_COMPLETE), 0);
}

void
khm_cred_process_commandline(void) {
    khm_handle defident = NULL;

    if (!khm_startup.processing)
        return;

    if (khm_startup.init ||
        khm_startup.renew ||
        khm_startup.destroy) {
        kcdb_identity_get_default(&defident);
    }

    do {
        if (khm_startup.init) {
            if (defident)
                khui_context_set(KHUI_SCOPE_IDENT,
                                 defident,
                                 KCDB_CREDTYPE_INVALID,
                                 NULL, NULL, 0,
                                 NULL);
            else
                khui_context_reset();

            khm_cred_obtain_new_creds(NULL);
            khm_startup.init = FALSE;
            break;
        }

        if (khm_startup.import) {
            khm_cred_import();
            khm_startup.import = FALSE;
            break;
        }

        if (khm_startup.renew) {
            if (defident)
                khui_context_set(KHUI_SCOPE_IDENT,
                                 defident,
                                 KCDB_CREDTYPE_INVALID,
                                 NULL, NULL, 0,
                                 NULL);
            else
                khui_context_reset();

            khm_cred_renew_creds();
            khm_startup.renew = FALSE;
            break;
        }

        if (khm_startup.destroy) {
            if (defident) {
                khui_context_set(KHUI_SCOPE_IDENT,
                                 defident,
                                 KCDB_CREDTYPE_INVALID,
                                 NULL, NULL, 0,
                                 NULL);

                khm_cred_destroy_creds();
            }

            khm_startup.destroy = FALSE;
            break;
        }

        if (khm_startup.autoinit) {
            khm_size count;

            kcdb_credset_get_size(NULL, &count);

            if (count == 0) {
                khm_cred_obtain_new_creds(NULL);
            }
            khm_startup.autoinit = FALSE;
            break;
        }

        if (khm_startup.exit) {
            PostMessage(khm_hwnd_main,
                        WM_COMMAND,
                        MAKEWPARAM(KHUI_ACTION_EXIT, 0), 0);
            khm_startup.exit = FALSE;
            break;
        }

        khm_startup.processing = FALSE;
    } while(FALSE);

    if (defident)
        kcdb_identity_release(defident);
}

void
khm_cred_begin_commandline(void) {
    if (khm_startup.seen)
        return;

    khm_startup.seen = TRUE;
    khm_startup.processing = TRUE;

    khm_cred_process_commandline();
}
