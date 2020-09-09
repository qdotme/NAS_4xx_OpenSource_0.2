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

/* $Id: cfg_general_wnd.c,v 1.1.1.1 2006/05/19 07:08:14 arsene Exp $ */

#include<khmapp.h>
#include<assert.h>

typedef struct tag_cfg_data {
    BOOL auto_init;
    BOOL auto_start;
    BOOL auto_import;
    BOOL keep_running;
    BOOL auto_detect_net;
} cfg_data;

typedef struct tag_dlg_data {
    khui_config_node node;
    cfg_data saved;
    cfg_data work;
} dlg_data;

static void
read_params(dlg_data * dd) {
    cfg_data * d;
    khm_handle csp_cw;
    khm_int32 t;

    d = &dd->saved;

    if (KHM_FAILED(khc_open_space(NULL, L"CredWindow", KHM_PERM_READ,
                                  &csp_cw))) {
#ifdef DEBUG
        assert(FALSE);
#endif
        return;
    }

    khc_read_int32(csp_cw, L"AutoInit", &t);
    d->auto_init = !!t;

    khc_read_int32(csp_cw, L"AutoStart", &t);
    d->auto_start = !!t;

    khc_read_int32(csp_cw, L"AutoImport", &t);
    d->auto_import = !!t;

    khc_read_int32(csp_cw, L"KeepRunning", &t);
    d->keep_running = !!t;

    khc_read_int32(csp_cw, L"AutoDetectNet", &t);
    d->auto_detect_net = !!t;

    khc_close_space(csp_cw);

    dd->work = *d;
}

static void
write_params(dlg_data * dd) {
    cfg_data * d, * s;
    khm_handle csp_cw;

    d = &dd->work;
    s = &dd->saved;

    if (KHM_FAILED(khc_open_space(NULL, L"CredWindow", KHM_PERM_WRITE,
                                  &csp_cw))) {
#ifdef DEBUG
        assert(FALSE);
#endif
        return;
    }

    if (!!d->auto_init != !!s->auto_init)
        khc_write_int32(csp_cw, L"AutoInit", d->auto_init);

    if (!!d->auto_start != !!s->auto_start)
        khc_write_int32(csp_cw, L"AutoStart", d->auto_start);

    if (!!d->auto_import != !!s->auto_import)
        khc_write_int32(csp_cw, L"AutoImport", d->auto_import);

    if (!!d->keep_running != !!s->keep_running)
        khc_write_int32(csp_cw, L"KeepRunning", d->keep_running);

    if (!!d->auto_detect_net != !!s->auto_detect_net)
        khc_write_int32(csp_cw, L"AutoDetectNet", d->auto_detect_net);

    khc_close_space(csp_cw);

    khui_cfg_set_flags(dd->node,
                       KHUI_CNFLAG_APPLIED,
                       KHUI_CNFLAG_APPLIED | KHUI_CNFLAG_MODIFIED);

    *s = *d;
}

static void
check_for_modification(dlg_data * dd) {
    cfg_data * d, * s;
    d = &dd->work;
    s = &dd->saved;

    if (!!d->auto_init != !!s->auto_init ||
        !!d->auto_start != !!s->auto_start ||
        !!d->auto_import != !!s->auto_import ||
        !!d->keep_running != !!s->keep_running ||
        !!d->auto_detect_net != !!s->auto_detect_net) {

        khui_cfg_set_flags(dd->node,
                           KHUI_CNFLAG_MODIFIED,
                           KHUI_CNFLAG_MODIFIED);

    } else {

        khui_cfg_set_flags(dd->node,
                           0,
                           KHUI_CNFLAG_MODIFIED);

    }
}

static void
refresh_view(HWND hwnd, dlg_data * d) {
    CheckDlgButton(hwnd, IDC_CFG_AUTOINIT,
                   (d->work.auto_init?BST_CHECKED:BST_UNCHECKED));
    CheckDlgButton(hwnd, IDC_CFG_AUTOSTART,
                   (d->work.auto_start?BST_CHECKED:BST_UNCHECKED));
    CheckDlgButton(hwnd, IDC_CFG_AUTOIMPORT,
                   (d->work.auto_import?BST_CHECKED:BST_UNCHECKED));
    CheckDlgButton(hwnd, IDC_CFG_KEEPRUNNING,
                   (d->work.keep_running?BST_CHECKED:BST_UNCHECKED));
    CheckDlgButton(hwnd, IDC_CFG_NETDETECT,
                   (d->work.auto_detect_net?BST_CHECKED:BST_UNCHECKED));
}

static void
refresh_data(HWND hwnd, dlg_data * d) {
    d->work.auto_init = (IsDlgButtonChecked(hwnd, IDC_CFG_AUTOINIT)
                         == BST_CHECKED);
    d->work.auto_start = (IsDlgButtonChecked(hwnd, IDC_CFG_AUTOSTART)
                          == BST_CHECKED);
    d->work.auto_import = (IsDlgButtonChecked(hwnd, IDC_CFG_AUTOIMPORT)
                           == BST_CHECKED);
    d->work.keep_running = (IsDlgButtonChecked(hwnd, IDC_CFG_KEEPRUNNING)
                            == BST_CHECKED);
    d->work.auto_detect_net = (IsDlgButtonChecked(hwnd, IDC_CFG_NETDETECT)
                               == BST_CHECKED);
}

INT_PTR CALLBACK
khm_cfg_general_proc(HWND hwnd,
                     UINT uMsg,
                     WPARAM wParam,
                     LPARAM lParam) {
    dlg_data * d;

    switch(uMsg) {
    case WM_INITDIALOG:
        d = malloc(sizeof(*d));
#ifdef DEBUG
        assert(d != NULL);
#endif

#pragma warning(push)
#pragma warning(disable: 4244)
        SetWindowLongPtr(hwnd, DWLP_USER, (LONG_PTR) d);
#pragma warning(pop)

        ZeroMemory(d, sizeof(*d));

        d->node = (khui_config_node) lParam;

        read_params(d);

        refresh_view(hwnd, d);

        return FALSE;

    case WM_COMMAND:
        d = (dlg_data *) (DWORD_PTR) GetWindowLongPtr(hwnd, DWLP_USER);

        if (HIWORD(wParam) == BN_CLICKED) {
            refresh_data(hwnd, d);
            check_for_modification(d);
        }

        khm_set_dialog_result(hwnd, 0);

        return TRUE;

    case KHUI_WM_CFG_NOTIFY:
        d = (dlg_data *) (DWORD_PTR) GetWindowLongPtr(hwnd, DWLP_USER);

        if (HIWORD(wParam) == WMCFG_APPLY) {
            write_params(d);
        }

        khm_set_dialog_result(hwnd, 0);

        return TRUE;
    }

    return FALSE;
}
