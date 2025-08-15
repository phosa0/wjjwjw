#define UNICODE
#define _UNICODE

#include <windows.h>
#include <string>

bool inject_dll(DWORD processID, const std::wstring& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (!hProcess) return false;

    size_t pathLen = (dllPath.length() + 1) * sizeof(wchar_t);
    LPVOID pRemote = VirtualAllocEx(hProcess, NULL, pathLen, MEM_COMMIT, PAGE_READWRITE);
    if (!pRemote) {
        CloseHandle(hProcess);
        return false;
    }

    WriteProcessMemory(hProcess, pRemote, dllPath.c_str(), pathLen, NULL);

    // ✅ Unicode version
    HMODULE h_kernel32 = GetModuleHandleW(L"kernel32.dll");
    FARPROC loadLibAddr = GetProcAddress(h_kernel32, "LoadLibraryW");
    if (!loadLibAddr) {
        VirtualFreeEx(hProcess, pRemote, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)loadLibAddr,
        pRemote, 0, NULL);

    if (!hThread) {
        VirtualFreeEx(hProcess, pRemote, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, pRemote, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return true;
}
