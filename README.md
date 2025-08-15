# Simple DLL Injector

This is a simple DLL injector for Windows, created for educational and software development purposes.

## Features

*   Lists running processes.
*   Injects a DLL into a selected process using the `CreateRemoteThread` method.

## Building

## Building

To build this project, you will need a C++ compiler for Windows. The recommended tool is the **MinGW-w64** compiler, which provides the `g++` command.

### Compilation Steps (using MinGW-w64 g++)

1.  **Install MinGW-w64:** If you don't have it, you can get it from [MSYS2](https://www.msys2.org/) or the [MinGW-w64 project website](https://www.mingw-w64.org/downloads/).

2.  **Open a terminal or command prompt** that has `g++` in its PATH.

3.  **Compile the Injector Executable:**
    Run the following command to build `injector.exe`:
    ```bash
    g++ src/main.cpp src/injector.cpp -o injector.exe -mwindows -lcomctl32 -static
    ```
    *   `-mwindows`: Specifies that this is a GUI application, not a console one.
    *   `-lcomctl32`: Links the Common Controls library, needed for the list view.
    *   `-static`: Statically links standard libraries to avoid dependency issues.

4.  **Compile the Test DLL:**
    Run this command to build `test_dll.dll`:
    ```bash
    g++ src/test_dll.cpp -o test_dll.dll -shared
    ```
    *   `-shared`: Tells the compiler to create a shared library (.dll).

## Usage

1.  Run `injector.exe`.
2.  Select a process from the list.
3.  Enter the full path to the DLL you want to inject.
4.  Click the "Inject" button.

## Disclaimer

This tool is intended for legal and ethical use only. The user is responsible for their actions.
