#pragma once
#include "stdafx.h"

Signature fsRenderManager = {
    /*
    00DF10AC            |.  891D C4365901           mov     dword ptr [DarkSoulsII.15936C4], ebx
    00DF10B2            |.  57                      push    edi
    00DF10B3            |.  8D8E 60010000           lea     ecx, [esi+160]
    00DF10B9            |.  C786 60010000 044B2C01  mov     dword ptr [esi+160], DarkSoulsII.012C4B04
    00DF10C3            |.  E8 4854B3FF             call    DarkSoulsII.00926510     ; [DarkSoulsII.00926510
    */
    "\x89\x1D\x00\x00\x00\x00\x57\x8D\x8E\x00\x00\x00\x00\xC7\x86\x00"
    "\x00\x00\x00\x00\x00\x00\x00\xE8",
    "xx????xxx????xx????????x",
    strlen(fsRenderManager.mask)
};

Signature fsSetDurability = {
    /*
    >>> inject <<<
    0076FFD6            |.  51                      push    ecx                      ; /Arg2 = DarkSoulsII.<ModuleEntryPoint>
    0076FFD7            |.  53                      push    ebx                      ; |Arg1
    0076FFD8            |.  8BCE                    mov     ecx, esi                 ; |
    0076FFDA            |.  F30F1147 6C             movss   dword ptr [edi+6C], xmm0 ; |set durability
    <<< return >>>
    0076FFDF            |.  E8 6CD9FFFF             call    DarkSoulsII.0076D950     ; \DarkSoulsII.0076D950
    */
    "\x51\x53\x8B\xCE\xF3\x0F\x11\x47\x6C",
    "xxxxxxxxx",
    strlen(fsSetDurability.mask)
};