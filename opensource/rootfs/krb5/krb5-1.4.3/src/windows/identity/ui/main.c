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

/* $Id: main.c,v 1.1.1.1 2006/05/19 07:08:14 arsene Exp $ */

#include<khmapp.h>

#if DEBUG
#include<assert.h>
#endif

HINSTANCE khm_hInstance;
const wchar_t * khm_facility = L"NetIDMgr";
int khm_nCmdShow;

khm_startup_options khm_startup;

void khm_init_gui(void) {
    khui_init_actions();
    khui_init_rescache();
    khui_init_menu();
    khui_init_toolbar();
    khui_init_notifier();
    khm_init_config();
}

void khm_exit_gui(void) {
    khm_exit_config();
    khui_exit_notifier();
    khui_exit_toolbar();
    khui_exit_menu();
    khui_exit_rescache();
    khui_exit_actions();
}

void khm_parse_commandline(void) {
    LPWSTR wcmdline;
    LPWSTR * wargs;
    int      wargc;
    int i;

    ZeroMemory(&khm_startup, sizeof(khm_startup));

    wcmdline = GetCommandLine();
    wargs = CommandLineToArgvW(wcmdline, &wargc);

    for (i=1; i<wargc; i++) {
        if (!wcscmp(wargs[i], L"-i") ||
            !wcscmp(wargs[i], L"--kinit")) {
            khm_startup.init = TRUE;
            khm_startup.exit = TRUE;
            khm_startup.no_main_window = TRUE;
        }
        else if (!wcscmp(wargs[i], L"-m") ||
                 !wcscmp(wargs[i], L"--import")) {
            khm_startup.import = TRUE;
            khm_startup.exit = TRUE;
            khm_startup.no_main_window = TRUE;
        }
        else if (!wcscmp(wargs[i], L"-r") ||
                 !wcscmp(wargs[i], L"--renew")) {
            khm_startup.renew = TRUE;
            khm_startup.exit = TRUE;
            khm_startup.no_main_window = TRUE;
        }
        else if (!wcscmp(wargs[i], L"-d") ||
                 !wcscmp(wargs[i], L"--destroy")) {
            khm_startup.destroy = TRUE;
            khm_startup.exit = TRUE;
            khm_startup.no_main_window = TRUE;
        }
        else if (!wcscmp(wargs[i], L"-a") ||
                 !wcscmp(wargs[i], L"--autoinit")) {
            khm_startup.autoinit = TRUE;
        }
        else {
            wchar_t help[2048];

            LoadString(khm_hInstance, IDS_CMDLINE_HELP,
                       help, ARRAYLENGTH(help));

            MessageBox(NULL, help, L"NetIDMgr", MB_OK);

            khm_startup.error_exit = TRUE;
            break;
        }
    }
}

void khm_register_window_classes(void) {
    INITCOMMONCONTROLSEX ics;

    ZeroMemory(&ics, sizeof(ics));
    ics.dwSize = sizeof(ics);
    ics.dwICC = 
        ICC_COOL_CLASSES |
        ICC_BAR_CLASSES |
        ICC_DATE_CLASSES |
        ICC_HOTKEY_CLASS |
        ICC_LINK_CLASS |
        ICC_LISTVIEW_CLASSES |
        ICC_STANDARD_CLASSES |
        ICC_TAB_CLASSES;
    InitCommonControlsEx(&ics);

    khm_register_main_wnd_class();
    khm_register_credwnd_class();
    khm_register_htwnd_class();
    khm_register_passwnd_class();
    khm_register_newcredwnd_class();
    khm_register_propertywnd_class();
}

void khm_unregister_window_classes(void) {
    khm_unregister_main_wnd_class();
    khm_unregister_credwnd_class();
    khm_unregister_htwnd_class();
    khm_unregister_passwnd_class();
    khm_unregister_newcredwnd_class();
    khm_unregister_propertywnd_class();
}


/* we support up to 16 simutaneous dialogs.  In reality, more than two
   is pretty unlikely.  Property sheets are special and are handled
   separately. */
#define MAX_UI_DIALOGS 16

typedef struct tag_khui_dialog {
    HWND hwnd;
    BOOL active;
} khui_dialog;

static khui_dialog khui_dialogs[MAX_UI_DIALOGS];
static int n_khui_dialogs = 0;
static HWND khui_modal_dialog = NULL;
static BOOL khui_main_window_active;

/* should only be called from the UI thread */
void khm_add_dialog(HWND dlg) {
    if(n_khui_dialogs < MAX_UI_DIALOGS - 1) {
        khui_dialogs[n_khui_dialogs].hwnd = dlg;
        /* we set .active=FALSE for now.  We don't need this to have a
           meaningful value until we enter a modal loop */
        khui_dialogs[n_khui_dialogs].active = FALSE;
        n_khui_dialogs++;
    }
#if DEBUG
    else {
        assert(FALSE);
    }
#endif
}

/* should only be called from the UI thread */
void khm_enter_modal(HWND hwnd) {
    int i;

    for(i=0; i < n_khui_dialogs; i++) {
        if(khui_dialogs[i].hwnd != hwnd) {
            khui_dialogs[i].active = IsWindowEnabled(khui_dialogs[i].hwnd);
            EnableWindow(khui_dialogs[i].hwnd, FALSE);
        }
    }

    khui_main_window_active = IsWindowEnabled(khm_hwnd_main);
    EnableWindow(khm_hwnd_main, FALSE);

    khui_modal_dialog = hwnd;
}

/* should only be called from the UI thread */
void khm_leave_modal(void) {
    int i;

    for(i=0; i < n_khui_dialogs; i++) {
        if(khui_dialogs[i].hwnd != khui_modal_dialog) {
            EnableWindow(khui_dialogs[i].hwnd, khui_dialogs[i].active);
        }
    }

    EnableWindow(khm_hwnd_main, khui_main_window_active);

    khui_modal_dialog = NULL;
}

/* should only be called from the UI thread */
void khm_del_dialog(HWND dlg) {
    int i;
    for(i=0;i < n_khui_dialogs; i++) {
        if(khui_dialogs[i].hwnd == dlg)
            break;
    }
    
    if(i < n_khui_dialogs)
        n_khui_dialogs--;
    else
        return;

    for(;i < n_khui_dialogs; i++) {
        khui_dialogs[i] = khui_dialogs[i+1];
    }
}

BOOL khm_check_dlg_message(LPMSG pmsg) {
    int i;
    for(i=0;i<n_khui_dialogs;i++) {
        if(IsDialogMessage(khui_dialogs[i].hwnd, pmsg))
            break;
    }

    if(i<n_khui_dialogs)
        return TRUE;
    else
        return FALSE;
}

BOOL khm_is_dialog_active(void) {
    HWND hwnd;
    int i;

    hwnd = GetForegroundWindow();

    for (i=0; i<n_khui_dialogs; i++) {
        if (khui_dialogs[i].hwnd == hwnd)
            return TRUE;
    }

    return FALSE;
}

/* We support at most 256 property sheets simultaneously.  256
   property sheets should be enough for everybody. */
#define MAX_UI_PROPSHEETS 256

khui_property_sheet *_ui_propsheets[MAX_UI_PROPSHEETS];
int _n_ui_propsheets = 0;

void khm_add_property_sheet(khui_property_sheet * s) {
    if(_n_ui_propsheets < MAX_UI_PROPSHEETS)
        _ui_propsheets[_n_ui_propsheets++] = s;
#ifdef DEBUG
    else
        assert(FALSE);
#endif
}

void khm_del_property_sheet(khui_property_sheet * s) {
    int i;

    for(i=0;i < _n_ui_propsheets; i++) {
        if(_ui_propsheets[i] == s)
            break;
    }

    if(i < _n_ui_propsheets)
        _n_ui_propsheets--;
    else
        return;

    for(;i < _n_ui_propsheets; i++) {
        _ui_propsheets[i] = _ui_propsheets[i+1];
    }
}

BOOL khm_check_ps_message(LPMSG pmsg) {
    int i;
    khui_property_sheet * ps;
    for(i=0;i<_n_ui_propsheets;i++) {
        if(khui_ps_check_message(_ui_propsheets[i], pmsg)) {
            if(_ui_propsheets[i]->status == KHUI_PS_STATUS_DONE) {
                ps = _ui_propsheets[i];

                ps->status = KHUI_PS_STATUS_DESTROY;
                kmq_post_message(KMSG_CRED, KMSG_CRED_PP_END, 0, (void *) ps);

                return TRUE;
            }
            return TRUE;
        }
    }

    return FALSE;
}

WPARAM khm_message_loop(void) {
    int r;
    MSG msg;
    HACCEL ha_menu;

    ha_menu = khui_create_global_accel_table();
    while(r = GetMessage(&msg, NULL, 0,0)) {
        if(r == -1)
            break;
        if(!khm_check_dlg_message(&msg) &&
            !khm_check_ps_message(&msg) &&
            !TranslateAccelerator(khm_hwnd_main, ha_menu, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    DestroyAcceleratorTable(ha_menu);
    return msg.wParam;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) 
{
    int rv = 0;
    HANDLE h_appmutex;
    BOOL slave = FALSE;

    khm_hInstance = hInstance;
    khm_nCmdShow = nCmdShow;

    khm_parse_commandline();

    if (khm_startup.error_exit)
        return 0;

    h_appmutex = CreateMutex(NULL, FALSE, L"Local\\NetIDMgr_GlobalAppMutex");
    if (h_appmutex == NULL)
        return 5;
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        slave = TRUE;

    khc_load_schema(NULL, schema_uiconfig);

    if(!slave) {
        /* we only open a main window if this is the only instance 
           of the application that is running. */
        kmq_init();
        kmm_init();
        khm_init_gui();

        kmq_set_completion_handler(KMSG_CRED, kmsg_cred_completion);

        /* load the standard plugins */
        kmm_load_default_modules();

        khm_register_window_classes();

        khm_init_request_daemon();

        khm_create_main_window();

        if (!khm_startup.no_main_window)
            khm_show_main_window();

        rv = (int) khm_message_loop();

        kmq_set_completion_handler(KMSG_CRED, NULL);

        khm_exit_request_daemon();

        khm_exit_gui();
        khm_unregister_window_classes();
        kmm_exit();
        kmq_exit();

        CloseHandle(h_appmutex);
    } else {
        HWND hwnd = NULL;
        int retries = 5;
        HANDLE hmap;
        wchar_t mapname[256];
        DWORD tid;
        void * xfer;

        CloseHandle(h_appmutex);

        while (hwnd == NULL && retries) {
            hwnd = FindWindowEx(NULL, NULL, KHUI_MAIN_WINDOW_CLASS, NULL);

            if (hwnd)
                break;

            retries--;
            Sleep(1000);
        }

        if (!hwnd)
            return 2;

        StringCbPrintf(mapname, sizeof(mapname),
                       COMMANDLINE_MAP_FMT,
                       (tid = GetCurrentThreadId()));

        hmap = CreateFileMapping(INVALID_HANDLE_VALUE,
                                 NULL,
                                 PAGE_READWRITE,
                                 0,
                                 4096,
                                 mapname);

        if (hmap == NULL)
            return 3;

        xfer = MapViewOfFile(hmap,
                             FILE_MAP_WRITE,
                             0, 0,
                             sizeof(khm_startup));

        if (xfer) {
            memcpy(xfer, &khm_startup, sizeof(khm_startup));

            SendMessage(hwnd, WM_KHUI_ASSIGN_COMMANDLINE,
                        0, (LPARAM) tid);
        }

        if (xfer)
            UnmapViewOfFile(xfer);

        if (hmap)
            CloseHandle(hmap);
    }

    return rv;
}
