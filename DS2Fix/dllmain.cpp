#include "stdafx.h"
#include "DS2Fix.h"
#include "Utils\Log.h"

VOID InitInstance(HMODULE);
VOID ExitInstance();

const char* logFilePath = nullptr;
HANDLE hFile;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        InitInstance(hModule);
        break;

    case DLL_PROCESS_DETACH:
        ExitInstance();
        break;
    }
    return TRUE;
}

VOID InitInstance(HMODULE hModule)
{
    char fileName[MAX_PATH];
    GetModuleFileName(hModule, fileName, MAX_PATH);
    std::string dll = fileName;
    dll += ".log";
    logFilePath = dll.c_str();
    hFile = CreateFile(logFilePath, FILE_GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    Log("Dark Souls II Fix injected");
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&Attach, hModule, 0, nullptr);
}

VOID ExitInstance()
{
    Log("Dark Souls II Fix exiting");
    Detach();
}