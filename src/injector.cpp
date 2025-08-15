#include "../include/injector.h"
#include <tlhelp32.h>
#include <stdexcept>

std::vector<ProcessInfo> get_process_list() {
    std::vector<ProcessInfo> process_list;
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (h_snap == INVALID_HANDLE_VALUE) {
        // In a real GUI app, you'd probably want to show an error message box
        // For now, we can throw or return an empty list.
        // Let's return an empty list to avoid crashing the app.
        return process_list;
    }

    PROCESSENTRY32W pe32; // Use the wide-character version
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(h_snap, &pe32)) { // Use the wide-character version
        do {
            ProcessInfo pi;
            pi.pid = pe32.th32ProcessID;
            pi.name = pe32.szExeFile;
            process_list.push_back(pi);
        } while (Process32NextW(h_snap, &pe32)); // Use the wide-character version
    }

    CloseHandle(h_snap);
    return process_list;
}

bool inject_dll(DWORD pid, const std::wstring& dll_path) {
    HANDLE h_proc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pid);
    if (h_proc == NULL) {
        return false;
    }

    size_t dll_path_size = (dll_path.length() + 1) * sizeof(wchar_t);
    LPVOID remote_mem = VirtualAllocEx(h_proc, NULL, dll_path_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (remote_mem == NULL) {
        CloseHandle(h_proc);
        return false;
    }

    if (!WriteProcessMemory(h_proc, remote_mem, dll_path.c_str(), dll_path_size, NULL)) {
        VirtualFreeEx(h_proc, remote_mem, 0, MEM_RELEASE);
        CloseHandle(h_proc);
        return false;
    }

    HMODULE h_kernel32 = GetModuleHandle(L"kernel32.dll");
    if (h_kernel32 == NULL) {
        VirtualFreeEx(h_proc, remote_mem, 0, MEM_RELEASE);
        CloseHandle(h_proc);
        return false;
    }

    LPTHREAD_START_ROUTINE load_library_addr = (LPTHREAD_START_ROUTINE)GetProcAddress(h_kernel32, "LoadLibraryW");
    if (load_library_addr == NULL) {
        VirtualFreeEx(h_proc, remote_mem, 0, MEM_RELEASE);
        CloseHandle(h_proc);
        return false;
    }

    HANDLE h_remote_thread = CreateRemoteThread(h_proc, NULL, 0, load_library_addr, remote_mem, 0, NULL);
    if (h_remote_thread == NULL) {
        VirtualFreeEx(h_proc, remote_mem, 0, MEM_RELEASE);
        CloseHandle(h_proc);
        return false;
    }

    WaitForSingleObject(h_remote_thread, INFINITE);

    CloseHandle(h_remote_thread);
    VirtualFreeEx(h_proc, remote_mem, 0, MEM_RELEASE);
    CloseHandle(h_proc);

    return true;
}
