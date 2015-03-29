// DS2Fix.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DS2Fix.h"
#include "Core\Offsets.h"
#include "Fixes\Durability.h"

#include "detours.h"
#pragma comment(lib, "detours.lib")


VOID Attach(HMODULE hModule)
{
    oSetDurability = (pSetDurability)injSetDurability;

#ifndef NDEBUG
    Log("hBase @ 0x%p", DarkSoulsII);
    Log("CurrentFPS @ DarkSoulsII.exe+%p + 0x%p", RenderManager, CurrentFPS);
    Log("oDurability @ 0x%p", oSetDurability);
#endif

    HRESULT hr = ApplyDetours();
}

VOID Detach()
{

}

HRESULT ApplyDetours()
{
    HRESULT hr;

    hr = DetourTransactionBegin();
    hr &= DetourAttach((PVOID*)&oSetDurability, (PVOID)tSetDurability);
    hr &= DetourTransactionCommit();

    return hr;
}