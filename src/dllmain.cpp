// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <cocos2d.h>
#include "Main.h"

DWORD WINAPI my_thread(void* hModule) {

    MH_Initialize();
    Platformer::Run_Mod();
    MH_EnableHook(MH_ALL_HOOKS);

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0x1000, my_thread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

