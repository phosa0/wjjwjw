#include <windows.h>
#include <cstdio>
#include <string>

// Function to perform DLL injection
DWORD WINAPI InjectDLL(LPVOID lpParam) {
    LPWSTR dllPath = (LPWSTR)lpParam;
    DWORD processId = 0; // Replace with the actual process ID

    // Example of getting a process handle and a module handle
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        MessageBoxW(NULL, L"Failed to open process!", L"Error", MB_ICONERROR);
        return 1;
    }

    // Get a handle to kernel32.dll
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    if (hKernel32 == NULL) {
        CloseHandle(hProcess);
        MessageBoxW(NULL, L"Failed to get handle to kernel32.dll!", L"Error", MB_ICONERROR);
        return 1;
    }

    // Get the address of the LoadLibraryW function
    LPVOID lpLoadLibrary = (LPVOID)GetProcAddress(hKernel32, "LoadLibraryW");
    if (lpLoadLibrary == NULL) {
        CloseHandle(hProcess);
        MessageBoxW(NULL, L"Failed to get address of LoadLibraryW!", L"Error", MB_ICONERROR);
        return 1;
    }

    // Allocate memory in the target process
    LPVOID lpRemoteString = VirtualAllocEx(hProcess, NULL, (wcslen(dllPath) + 1) * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (lpRemoteString == NULL) {
        CloseHandle(hProcess);
        MessageBoxW(NULL, L"Failed to allocate memory in remote process!", L"Error", MB_ICONERROR);
        return 1;
    }

    // Write the DLL path to the allocated memory
    if (!WriteProcessMemory(hProcess, lpRemoteString, dllPath, (wcslen(dllPath) + 1) * sizeof(wchar_t), NULL)) {
        VirtualFreeEx(hProcess, lpRemoteString, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        MessageBoxW(NULL, L"Failed to write to process memory!", L"Error", MB_ICONERROR);
        return 1;
    }

    // Create a remote thread to call LoadLibraryW
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpLoadLibrary, lpRemoteString, 0, NULL);
    if (hThread == NULL) {
        VirtualFreeEx(hProcess, lpRemoteString, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        MessageBoxW(NULL, L"Failed to create remote thread!", L"Error", MB_ICONERROR);
        return 1;
    }

    // Wait for the thread to finish and clean up
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    VirtualFreeEx(hProcess, lpRemoteString, 0, MEM_RELEASE);
    CloseHandle(hProcess);

    return 0;
}
