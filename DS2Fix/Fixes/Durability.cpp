#include "stdafx.h"
#include "Durability.h"
#include "Core\Offsets.h"

float fCurrentDurability;
float fNewDurability;
float fOldDurability;

float fGetCurrentFPS()
{
    DWORD ptr;
    ptr = *(DWORD*)(DarkSoulsII + RenderManager);
    return *(float*)(ptr + CurrentFPS);
}

float fFixDurability(float fCurrentDurability)
{
    float fCurrentFPS = fGetCurrentFPS();
    fOldDurability = fNewDurability;
    fNewDurability = fCurrentDurability;

    if (fOldDurability <= 0 || fNewDurability <= 0 || fCurrentFPS <= 0)
    {
        return fCurrentDurability;
    }

    float fGameDamage = fOldDurability - fNewDurability;

    if (fGameDamage <= 0)
    {
        return fCurrentDurability;
    }

    float fFixedDamage = fGameDamage / (fCurrentFPS / 30.0f);

    if (fFixedDamage <= 0)
    {
        return fCurrentDurability;
    }

    return fOldDurability - fFixedDamage;
}

DWORD dwRetDurability = retSetDurability;
__declspec(naked) void __stdcall tSetDurability()
{
    /*CPU Disasm
    Address             Hex dump               Command                               Comments
    0076FFC8            |> \52                 push    edx                           ; |Arg1 = DarkSoulsII.<ModuleEntryPoint>, cases 10, 11, 12, 13 of switch DarkSoulsII.76FFA3
    0076FFC9            |.  E8 124DF2FF        call    DarkSoulsII.00694CE0          ; \DarkSoulsII.00694CE0
    0076FFCE            |>  8B4D 0C            mov     ecx, dword ptr [ebp+0C]       ; Default case of switch DarkSoulsII.76FFA3
    0076FFD1            |.  F30F1045 10        movss   xmm0, dword ptr [ebp+10]      ; new durability
    >>> inject <<<
    0076FFD6            |.  51                 push    ecx                           ; /Arg2
    0076FFD7            |.  53                 push    ebx                           ; |Arg1
    0076FFD8            |.  8BCE               mov     ecx, esi                      ; |
    0076FFDA            |.  F30F1147 6C        movss   dword ptr [edi+6C], xmm0      ; |set durability
    <<< return >>>
    0076FFDF            |.  E8 6CD9FFFF        call    DarkSoulsII.0076D950          ; \DarkSoulsII.0076D950
    0076FFE4            |.  5F                 pop     edi
    0076FFE5            |.  5E                 pop     esi
    0076FFE6            |.  5B                 pop     ebx
    0076FFE7            |.  5D                 pop     ebp
    0076FFE8            \.  C2 0C00            retn    0C
    */
    _asm
    {
        cmp     eax, -1         // the routine gets called to move floats around
        je      nevermind       // for many reasons - eax usually is -1, 0, 1 or 2
        cmp     eax, 2          // in those cases, while it becomes a valid address
        jbe     nevermind       // when actual durability is being modified

        movss   [fCurrentDurability], xmm0
    }

    fNewDurability = fFixDurability(fCurrentDurability);

    _asm
    {
        movss   xmm0, [fNewDurability]

     nevermind:                 // we got called for the wrong reasons, proceed as nothing happened
        push    ecx
        push    ebx
        mov     ecx, esi
        movss   dword ptr[edi + 0x6C], xmm0

        jmp     [dwRetDurability]
    }
}