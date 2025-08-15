#include <windows.h>
#include <commctrl.h>
#include <cstdio>
#include <string>

// Function declarations
DWORD WINAPI InjectDLL(LPVOID lpParam);

// Global variables
HWND hListView;
HINSTANCE g_hInstance;

// Custom data structure for ListView
typedef struct {
    LPWSTR fileName;
    DWORD fileSize;
} FileInfo;

// Function to handle messages for the main window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Initialize ListView common controls
            INITCOMMONCONTROLSEX icex;
            icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icex.dwICC = ICC_LISTVIEW_CLASSES;
            InitCommonControlsEx(&icex);

            // Create the ListView control
            hListView = CreateWindowExW(
                0,
                WC_LISTVIEWW,
                L"",
                WS_VISIBLE | WS_CHILD | LVS_REPORT,
                10, 10, 480, 200,
                hwnd,
                (HMENU)1001,
                g_hInstance,
                NULL);

            // Add columns to the ListView
            LVCOLUMNW lvc;
            lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
            lvc.fmt = LVCFMT_LEFT;

            lvc.iSubItem = 0;
            lvc.cx = 250;
            lvc.pszText = (LPWSTR)L"File Name";
            ListView_InsertColumn(hListView, 0, &lvc);

            lvc.iSubItem = 1;
            lvc.cx = 100;
            lvc.pszText = (LPWSTR)L"File Size";
            ListView_InsertColumn(hListView, 1, &lvc);

            // Example data
            FileInfo info1;
            info1.fileName = (LPWSTR)L"example.dll";
            info1.fileSize = 12345;

            FileInfo info2;
            info2.fileName = (LPWSTR)L"another.dll";
            info2.fileSize = 67890;

            // Add items to the ListView
            LVITEMW lvi;
            lvi.mask = LVIF_TEXT;
            lvi.iSubItem = 0;
            lvi.iItem = 0;
            lvi.pszText = info1.fileName;
            ListView_InsertItem(hListView, &lvi);
            ListView_SetItemText(hListView, 0, 1, (LPWSTR)std::to_wstring(info1.fileSize).c_str());

            lvi.iItem = 1;
            lvi.pszText = info2.fileName;
            ListView_InsertItem(hListView, &lvi);
            ListView_SetItemText(hListView, 1, 1, (LPWSTR)std::to_wstring(info2.fileSize).c_str());

            break;
        }
        case WM_COMMAND:
            if (LOWORD(wParam) == 1001) {
                // Handle ListView events
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Entry point for the application (wWinMain for Unicode compatibility)
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    g_hInstance = hInstance;
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Injector GUI",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 520, 300,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (hwnd == NULL) {
        MessageBoxW(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Create a new thread for DLL injection (example usage)
    HANDLE hThread = CreateThread(NULL, 0, InjectDLL, (LPVOID)L"C:\\path\\to\\your.dll", 0, NULL);
    if (hThread) {
        CloseHandle(hThread);
    } else {
        MessageBoxW(NULL, L"Failed to create injection thread!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
    }

    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}
