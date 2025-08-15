#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include <vector>
#include "../include/injector.h"
#include "../include/resource.h"

#pragma comment(lib, "comctl32.lib")

HWND g_h_wnd;
HWND g_h_process_list;
HWND g_h_dll_path_edit;

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
void create_controls(HWND hwnd);
void populate_process_list();
void handle_inject();
void handle_browse();

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, LPSTR lp_cmd_line, int n_cmd_show) {
    const wchar_t CLASS_NAME[] = L"DllInjectorWindowClass";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = h_instance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);

    g_h_wnd = CreateWindowExW(
        0, CLASS_NAME, L"Simple DLL Injector",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
        NULL, NULL, h_instance, NULL
    );

    if (!g_h_wnd) return 0;

    ShowWindow(g_h_wnd, n_cmd_show);
    UpdateWindow(g_h_wnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
        case WM_CREATE:
            create_controls(hwnd);
            populate_process_list();
            break;
        case WM_COMMAND:
            switch (LOWORD(w_param)) {
                case IDC_REFRESH_BUTTON: populate_process_list(); break;
                case IDC_INJECT_BUTTON: handle_inject(); break;
                case IDC_BROWSE_BUTTON: handle_browse(); break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, w_param, l_param);
    }
    return 0;
}

void create_controls(HWND hwnd) {
    g_h_process_list = CreateWindowExW(WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL,
        10, 10, 350, 300, hwnd, (HMENU)IDC_PROCESS_LIST, NULL, NULL);

    LVCOLUMNW lvc = {};
    lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    lvc.cx = 250; lvc.pszText = (LPWSTR)L"Process Name";
    ListView_InsertColumn(g_h_process_list, 0, &lvc);
    lvc.cx = 100; lvc.pszText = (LPWSTR)L"PID";
    ListView_InsertColumn(g_h_process_list, 1, &lvc);

    CreateWindowW(L"BUTTON", L"Refresh", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        370, 10, 100, 30, hwnd, (HMENU)IDC_REFRESH_BUTTON, NULL, NULL);

    g_h_dll_path_edit = CreateWindowW(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE,
        10, 320, 260, 25, hwnd, (HMENU)IDC_DLL_PATH_EDIT, NULL, NULL);

    CreateWindowW(L"BUTTON", L"Browse...", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        280, 320, 80, 25, hwnd, (HMENU)IDC_BROWSE_BUTTON, NULL, NULL);

    CreateWindowW(L"BUTTON", L"Inject", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        370, 50, 100, 30, hwnd, (HMENU)IDC_INJECT_BUTTON, NULL, NULL);
}

void populate_process_list() {
    ListView_DeleteAllItems(g_h_process_list);
    std::vector<ProcessInfo> processes = get_process_list();

    for (int i = 0; i < processes.size(); ++i) {
        LVITEMW lvi = {};
        lvi.mask = LVIF_TEXT | LVIF_PARAM;
        lvi.iItem = i;
        lvi.pszText = (LPWSTR)processes[i].name.c_str();
        lvi.lParam = (LPARAM)processes[i].pid;
        ListView_InsertItem(g_h_process_list, &lvi);

        wchar_t pid_str[20];
        swprintf(pid_str, 20, L"%lu", processes[i].pid);
        ListView_SetItemTextW(g_h_process_list, i, 1, pid_str); // ✅ Unicode version
    }
}

void handle_inject() {
    int selected_item = ListView_GetNextItem(g_h_process_list, -1, LVNI_SELECTED);
    if (selected_item == -1) {
        MessageBoxW(g_h_wnd, L"Please select a process.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    LVITEMW lvi = {};
    lvi.mask = LVIF_PARAM;
    lvi.iItem = selected_item;
    ListView_GetItem(g_h_process_list, &lvi);
    DWORD pid = (DWORD)lvi.lParam;

    wchar_t dll_path[MAX_PATH];
    GetWindowTextW(g_h_dll_path_edit, dll_path, MAX_PATH);

    if (wcslen(dll_path) == 0) {
        MessageBoxW(g_h_wnd, L"Please enter a DLL path.", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    if (inject_dll(pid, dll_path)) {
        MessageBoxW(g_h_wnd, L"DLL injected successfully!", L"Success", MB_OK);
    } else {
        MessageBoxW(g_h_wnd, L"Failed to inject DLL.", L"Error", MB_OK | MB_ICONERROR);
    }
}

void handle_browse() {
    wchar_t file_name[MAX_PATH] = { 0 };
    OPENFILENAMEW ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_h_wnd;
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"DLL Files (*.dll)\0*.dll\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        SetWindowTextW(g_h_dll_path_edit, file_name);
    }
}
