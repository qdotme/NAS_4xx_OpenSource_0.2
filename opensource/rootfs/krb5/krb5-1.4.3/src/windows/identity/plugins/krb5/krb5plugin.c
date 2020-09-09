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

/* $Id: krb5plugin.c,v 1.1.1.1 2006/05/19 07:08:14 arsene Exp $ */

#include<krbcred.h>
#include<kherror.h>
#include<khmsgtypes.h>
#include<commctrl.h>
#include<strsafe.h>
#include<krb5.h>

khm_int32 credtype_id_krb5 = KCDB_CREDTYPE_INVALID;
khm_boolean krb5_initialized = FALSE;
khm_handle krb5_credset = NULL;

khm_handle k5_sub = NULL;

LPVOID k5_main_fiber = NULL;
LPVOID k5_kinit_fiber = NULL;

VOID CALLBACK k5_kinit_fiber_proc(PVOID lpParameter);

krb5_context k5_identpro_ctx = NULL;

/*  The system message handler.

    Runs in the context of the plugin thread */
khm_int32 KHMAPI k5_msg_system(khm_int32 msg_type, khm_int32 msg_subtype, khm_ui_4 uparam, void * vparam)
{
    khm_int32 rv = KHM_ERROR_SUCCESS;

    switch(msg_subtype) {
        case KMSG_SYSTEM_INIT:
            {
                kcdb_credtype ct;
                wchar_t buf[KCDB_MAXCCH_SHORT_DESC];
                size_t cbsize;

                /* perform critical registrations and initialization
                   stuff */
                ZeroMemory(&ct, sizeof(ct));
                ct.id = KCDB_CREDTYPE_AUTO;
                ct.name = KRB5_CREDTYPE_NAME;

                if(LoadString(hResModule, IDS_KRB5_SHORT_DESC, buf, ARRAYLENGTH(buf)))
                {
                    StringCbLength(buf, KCDB_MAXCB_SHORT_DESC, &cbsize);
                    cbsize += sizeof(wchar_t);
                    ct.short_desc = malloc(cbsize);
                    StringCbCopy(ct.short_desc, cbsize, buf);
                }

                /* even though ideally we should be setting limits
                    based KCDB_MAXCB_LONG_DESC, our long description
                    actually fits nicely in KCDB_MAXCB_SHORT_DESC */
                if(LoadString(hResModule, IDS_KRB5_LONG_DESC, buf, ARRAYLENGTH(buf)))
                {
                    StringCbLength(buf, KCDB_MAXCB_SHORT_DESC, &cbsize);
                    cbsize += sizeof(wchar_t);
                    ct.long_desc = malloc(cbsize);
                    StringCbCopy(ct.long_desc, cbsize, buf);
                }

                ct.icon = NULL; /* TODO: set a proper icon */

                kmq_create_subscription(k5_msg_callback, &ct.sub);

                rv = kcdb_credtype_register(&ct, &credtype_id_krb5);

                if(KHM_SUCCEEDED(rv))
                    rv = kcdb_credset_create(&krb5_credset);

                if(ct.short_desc)
                    free(ct.short_desc);

                if(ct.long_desc)
                    free(ct.long_desc);

                if (is_k5_identpro)
                    kcdb_identity_set_type(credtype_id_krb5);

                if(KHM_SUCCEEDED(rv)) {
                    krb5_context ctx = NULL;

                    krb5_initialized = TRUE;

                    khm_krb5_list_tickets(&ctx);

                    if(ctx != NULL)
                        pkrb5_free_context(ctx);

                    /* now convert this thread to a fiber and create a
                       separate fiber to do kinit stuff */
                    k5_main_fiber = ConvertThreadToFiber(NULL);
                    k5_kinit_fiber = CreateFiber(0,k5_kinit_fiber_proc,NULL);

                    ZeroMemory(&g_fjob, sizeof(g_fjob));

                    kmq_create_subscription(k5_msg_callback, &k5_sub);

                    pkrb5_init_context(&k5_identpro_ctx);

                    k5_register_config_panels();
                }
            }
            break;

        case KMSG_SYSTEM_EXIT:

            k5_unregister_config_panels();

            if(credtype_id_krb5 >= 0)
            {
                /* basically just unregister the credential type */
                kcdb_credtype_unregister(credtype_id_krb5);

                /* kcdb knows how to deal with bad handles */
                kcdb_credset_delete(krb5_credset);
                krb5_credset = NULL;
            }

            if(k5_main_fiber != NULL) {
                ConvertFiberToThread();
                k5_main_fiber = NULL;
            }

            if(k5_sub != NULL) {
                kmq_delete_subscription(k5_sub);
                k5_sub = NULL;
            }

            if (k5_identpro_ctx) {
                pkrb5_free_context(k5_identpro_ctx);
                k5_identpro_ctx = NULL;
            }

            break;
    }

    return rv;
}


/* Handler for CRED type messages

    Runs in the context of the Krb5 plugin
*/
khm_int32 KHMAPI k5_msg_cred(khm_int32 msg_type, khm_int32 msg_subtype, khm_ui_4 uparam, void * vparam)
{
    khm_int32 rv = KHM_ERROR_SUCCESS;

    switch(msg_subtype) {
        case KMSG_CRED_REFRESH:
            {
                krb5_context ctx = NULL;

                khm_krb5_list_tickets(&ctx);

                if(ctx != NULL)
                    pkrb5_free_context(ctx);
            }
            break;

        case KMSG_CRED_DESTROY_CREDS:
            {
                khui_action_context * ctx;

                ctx = (khui_action_context *) vparam;
                
                if (ctx->credset)
                    khm_krb5_destroy_by_credset(ctx->credset);
            }
            break;

        case KMSG_CRED_PP_BEGIN:
            k5_pp_begin((khui_property_sheet *) vparam);
            break;

        case KMSG_CRED_PP_END:
            k5_pp_end((khui_property_sheet *) vparam);
            break;

        default:
            if(IS_CRED_ACQ_MSG(msg_subtype))
                return k5_msg_cred_dialog(msg_type, msg_subtype, 
                                            uparam, vparam);
    }

    return rv;
}

/*  The main message handler.  We don't do much here, except delegate
    to other message handlers

    Runs in the context of the Krb5 plugin
*/
khm_int32 KHMAPI k5_msg_callback(khm_int32 msg_type, khm_int32 msg_subtype, khm_ui_4 uparam, void * vparam)
{
    switch(msg_type) {
        case KMSG_SYSTEM:
            return k5_msg_system(msg_type, msg_subtype, uparam, vparam);
        case KMSG_CRED:
            return k5_msg_cred(msg_type, msg_subtype, uparam, vparam);
        case KMSG_IDENT:
            return k5_msg_ident(msg_type, msg_subtype, uparam, vparam);
    }
    return KHM_ERROR_SUCCESS;
}
