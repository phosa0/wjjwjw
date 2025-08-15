#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>

// Function to get the Process ID by process name
DWORD GetProcId(const char* procName) {
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry)) {
            do {
                if (!_stricmp(procEntry.szExeFile, procName)) {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return procId;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <process_name> <dll_path>" << std::endl;
        return 1;
    }

    const char* procName = argv[1];
    const char* dllPath = argv[2];

    // Get the process ID
    DWORD procId = GetProcId(procName);
    if (procId == 0) {
        std::cerr << "Process not found: " << procName << std::endl;
        return 1;
    }

    std::cout << "Process found. PID: " << procId << std::endl;

    // Get a handle to the process
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);
    if (hProc && hProc != INVALID_HANDLE_VALUE) {
        // Allocate memory for the DLL path in the target process
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (loc) {
            // Write the DLL path to the allocated memory
            WriteProcessMemory(hProc, loc, dllPath, strlen(dllPath) + 1, 0);
        } else {
            std::cerr << "Failed to allocate memory in target process." << std::endl;
            CloseHandle(hProc);
            return 1;
        }

        // Create a remote thread to load the DLL
        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
        if (hThread) {
            std::cout << "DLL injected successfully!" << std::endl;
            CloseHandle(hThread);
        } else {
            std::cerr << "Failed to create remote thread." << std::endl;
        }

        CloseHandle(hProc);
    } else {
        std::cerr << "Failed to open target process." << std::endl;
        return 1;
    }

    return 0;
}
