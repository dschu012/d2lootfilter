#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include "ItemFilter.h"

ItemFilter* filter;

void Error(const wchar_t* wszMessage) {
    MessageBox(nullptr, wszMessage, L"Error", MB_OK | MB_ICONSTOP);
    exit(0);
}

BOOL DllAttach() {

#ifdef _DEBUG
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
#endif
    if (GetGameVersion() == D2Version::ERR) {
        Error(L"Could not determine the game version.");
    }

    filter = new ItemFilter();
    return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        return DllAttach();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

