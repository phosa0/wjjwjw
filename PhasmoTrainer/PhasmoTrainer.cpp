#include <windows.h>
#include <iostream>

// ====================================================================================================
// == IMPORTANT: CHEAT CONFIGURATION
// ====================================================================================================
// These are the core variables for the cheat.
// You MUST find the correct, up-to-date offsets for the current version of Phasmophobia
// using a memory scanner tool like Cheat Engine. The values here are placeholders and WILL NOT WORK.
//
// HOW TO FIND OFFSETS (Simplified):
// 1.  Open Phasmophobia and Cheat Engine.
// 2.  Attach Cheat Engine to the Phasmophobia.exe process.
// 3.  For Stamina: Search for a float value of 100. Sprint in-game, search for the new value.
//     Repeat until you have a single address. This is your stamina address.
// 4.  Find what writes to this address. This will likely be an instruction in GameAssembly.dll.
// 5.  The address of the instruction will be something like "GameAssembly.dll + 0xOFFSET". That is your offset.
//     You may need to follow pointers from a base address. This example uses a simple pointer path.

// Placeholder for the base of a player object pointer chain.
// You need to find a static pointer that leads to the player object.
// For example: "GameAssembly.dll" + PLAYER_BASE_POINTER_OFFSET
#define PLAYER_BASE_POINTER_OFFSET 0xDEADBEEF

// Placeholder for the offset from the player object to the speed value.
#define PLAYER_SPEED_OFFSET 0xBEEF
// Placeholder for the offset from the player object to the stamina value.
#define PLAYER_STAMINA_OFFSET 0xF00D

// The desired values for the cheats.
const float DESIRED_SPEED = 3.0f; // Default walk speed is ~1.6, sprint is ~3.0
const float DESIRED_STAMINA = 100.0f;

// Global flags to toggle cheats. In a more advanced cheat, the injector would set these.
bool bSpeedHack = true;
bool bStaminaHack = true;
// ====================================================================================================

// Main function for the cheat thread
DWORD WINAPI CheatThread(HMODULE hModule) {
    // Get the base address of the game's main assembly.
    // All other addresses are calculated relative to this.
    uintptr_t gameAssemblyBase = (uintptr_t)GetModuleHandle("GameAssembly.dll");

    // If the module isn't found, the game might not be fully loaded yet.
    // In a real cheat, you might loop here until it's found.
    if (gameAssemblyBase == 0) {
        MessageBoxA(NULL, "Could not find GameAssembly.dll", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Main cheat loop
    while (true) {
        // This is a simplified pointer chain. Real games are often more complex.
        // It assumes: GameAssembly.dll -> PlayerBase -> PlayerObject
        uintptr_t* playerBasePtr = (uintptr_t*)(gameAssemblyBase + PLAYER_BASE_POINTER_OFFSET);

        if (playerBasePtr && *playerBasePtr) {
            uintptr_t playerObject = *playerBasePtr;

            if (bSpeedHack) {
                float* speedAddress = (float*)(playerObject + PLAYER_SPEED_OFFSET);
                *speedAddress = DESIRED_SPEED;
            }

            if (bStaminaHack) {
                float* staminaAddress = (float*)(playerObject + PLAYER_STAMINA_OFFSET);
                *staminaAddress = DESIRED_STAMINA;
            }
        }

        // Sleep for a short time to avoid using 100% CPU
        Sleep(100);
    }

    return 0;
}


// DllMain is the entry point for the DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // When the DLL is injected, disable thread library calls for performance.
        DisableThreadLibraryCalls(hModule);
        // Create a new thread to run our cheat loop.
        // This prevents DllMain from blocking, which can crash the game.
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheatThread, hModule, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
