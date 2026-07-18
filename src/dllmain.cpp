// dllmain.cpp
// Console
// Sadece
#include "Hooks.h"
#include "ConfigSystem.h"
#include "CrashLogger.h"
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

static DWORD WINAPI MainThread(LPVOID hMod) {
    // Crash
    CL::Install();
    CRUMB("client.dll wait");

    // Guard
    while (!GetModuleHandleA("client.dll")) Sleep(300);

    // Guard
    CRUMB("startup sleep");
    Sleep(8000);

    CRUMB("G.Init");
    G.Init();

    CfgSys::Load("default");

    CRUMB("SetupHooks");
    if (!SetupHooks()) {
        CL::Remove();
        FreeLibraryAndExitThread((HMODULE)hMod, 1);
    }

    CRUMB("MainLoop");
    while (true) {
        Sleep(100);
        if (GetAsyncKeyState(VK_END) & 1) break;
    }

    CRUMB("RemoveHooks");
    RemoveHooks();
    CL::Remove();
    Sleep(200);
    FreeLibraryAndExitThread((HMODULE)hMod, 0);
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hMod);
        CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
    }
    return TRUE;
}
