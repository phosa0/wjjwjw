#pragma once
#include <Windows.h>
#include <string>
#include <vector>

struct ProcessInfo {
    DWORD pid;
    std::wstring name;
};

std::vector<ProcessInfo> get_process_list();
bool inject_dll(DWORD pid, const std::wstring& dll_path);
