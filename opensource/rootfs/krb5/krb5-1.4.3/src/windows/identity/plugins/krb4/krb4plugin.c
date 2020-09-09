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

/* $Id: krb4plugin.c,v 1.1.1.1 2006/05/19 07:08:14 arsene Exp $ */

#include<krbcred.h>
#include<kherror.h>
#include<khmsgtypes.h>
#include<khuidefs.h>
#include<commctrl.h>
#include<strsafe.h>
#include<krb5.h>

khm_int32 credtype_id_krb4 = KCDB_CREDTYPE_INVALID;
khm_boolean krb4_initialized = FALSE;
khm_handle krb4_credset = NULL;

/* Kerberos IV stuff */
khm_int32 KHMAPI 
krb4_msg_system(khm_int32 msg_type, khm_int32 msg_subtype, 
                khm_ui_4 uparam, void * vparam)
{
    khm_int32 rv = KHM_ERROR_SUCCESS;

    switch(msg_subtype) {
    case KMSG_SYSTEM_INIT:
        {
            kcdb_credtype ct;
            wchar_t buf[KCDB_MAXCCH_SHORT_DESC];
            size_t cbsize;
            khui_config_node_reg reg;
            wchar_t wshort_desc[KHUI_MAXCCH_SHORT_DESC];
            wchar_t wlong_desc[KHUI_MAXCCH_LONG_DESC];

            /* perform critical registrations and initialization
               stuff */
            ZeroMemory(&ct, sizeof(ct));
            ct.id = KCDB_CREDTYPE_AUTO;
            ct.name = KRB4_CREDTYPE_NAME;

            if(LoadString(hResModule, IDS_KRB4_SHORT_DESC, 
                          buf, ARRAYLENGTH(buf)))
                {
                    StringCbLength(buf, KCDB_MAXCB_SHORT_DESC, &cbsize);
                    cbsize += sizeof(wchar_t);
                    ct.short_desc = malloc(cbsize);
                    StringCbCopy(ct.short_desc, cbsize, buf);
                }

            /* even though ideally we should be setting limits
               based KCDB_MAXCB_LONG_DESC, our long description
               actually fits nicely in KCDB_MAXCB_SHORT_DESC */
            if(LoadString(hResModule, IDS_KRB4_LONG_DESC, 
                          buf, ARRAYLENGTH(buf)))
                {
                    StringCbLength(buf, KCDB_MAXCB_SHORT_DESC, &cbsize);
                    cbsize += sizeof(wchar_t);
                    ct.long_desc = malloc(cbsize);
                    StringCbCopy(ct.long_desc, cbsize, buf);
                }

            ct.icon = NULL; /* TODO: set a proper icon */
            kmq_create_subscription(krb4_cb, &ct.sub);

            rv = kcdb_credtype_register(&ct, &credtype_id_krb4);

            if(KHM_SUCCEEDED(rv))
                rv = kcdb_credset_create(&krb4_credset);

            if(ct.short_desc)
                free(ct.short_desc);

            if(ct.long_desc)
                free(ct.long_desc);

            ZeroMemory(&reg, sizeof(reg));

            reg.name = KRB4_CONFIG_NODE_NAME;
            reg.short_desc = wshort_desc;
            reg.long_desc = wlong_desc;
            reg.h_module = hResModule;
            reg.dlg_template = MAKEINTRESOURCE(IDD_CFG_KRB4);
            reg.dlg_proc = krb4_confg_proc;
            reg.flags = 0;

            LoadString(hResModule, IDS_CFG_KRB4_LONG,
                       wlong_desc, ARRAYLENGTH(wlong_desc));
            LoadString(hResModule, IDS_CFG_KRB4_SHORT,
                       wshort_desc, ARRAYLENGTH(wshort_desc));

            khui_cfg_register(NULL, &reg);

            if(KHM_SUCCEEDED(rv)) {
                krb4_initialized = TRUE;

                khm_krb4_list_tickets();
            }
        }
        break;

    case KMSG_SYSTEM_EXIT:
        if(credtype_id_krb4 >= 0)
            {
                /* basically just unregister the credential type */
                kcdb_credtype_unregister(credtype_id_krb4);

                kcdb_credset_delete(krb4_credset);
            }
        break;
    }

    return rv;
}

khm_int32 KHMAPI 
krb4_msg_cred(khm_int32 msg_type, khm_int32 msg_subtype, 
              khm_ui_4 uparam, void * vparam)
{
    khm_int32 rv = KHM_ERROR_SUCCESS;

    switch(msg_subtype) {
        case KMSG_CRED_REFRESH:
            {
                khm_krb4_list_tickets();
            }
            break;
    }

    return rv;
}

khm_int32 KHMAPI 
krb4_cb(khm_int32 msg_type, khm_int32 msg_subtype, 
        khm_ui_4 uparam, void * vparam)
{
    switch(msg_type) {
        case KMSG_SYSTEM:
            return krb4_msg_system(msg_type, msg_subtype, uparam, vparam);
        case KMSG_CRED:
            return krb4_msg_cred(msg_type, msg_subtype, uparam, vparam);
    }
    return KHM_ERROR_SUCCESS;
}
