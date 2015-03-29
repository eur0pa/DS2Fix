#include "stdafx.h"
#include "SigScan.h"

VOID* pBaseAddress = nullptr;
size_t szImageLength = 0;

BOOL GetImageInfo()
{
    BOOL bSuccess = false;

    HMODULE hMod = GetModuleHandle(NULL);
    if (hMod)
    {
        MEMORY_BASIC_INFORMATION memInfo;

        if (VirtualQuery((void*)hMod, &memInfo, sizeof(memInfo)) != 0)
        {
            IMAGE_DOS_HEADER* hDos = (IMAGE_DOS_HEADER*)hMod;
            IMAGE_NT_HEADERS* hPe = (IMAGE_NT_HEADERS*)((unsigned long)memInfo.AllocationBase + (unsigned long)hDos->e_lfanew);

            if ((hDos->e_magic == IMAGE_DOS_SIGNATURE) && (hPe->Signature == IMAGE_NT_SIGNATURE))
            {
                bSuccess = true;
                pBaseAddress = (void*)memInfo.AllocationBase;
                szImageLength = (size_t)hPe->OptionalHeader.SizeOfImage;
            }
        }
    }
    return bSuccess;
}

VOID* FindSignature(Signature* fnSig)
{
    char* pScan = (char*)pBaseAddress;
    int iMaxLength = 0;

    while (pScan < ((char*)pBaseAddress + szImageLength - fnSig->length))
    {
        size_t szLength = 0;
        
        for (int i = 0; i < fnSig->length; i++)
        {
            if (!((pScan[i] == fnSig->signature[i]) || (fnSig->mask[i] == '?')))
            {
                break;
            }
            szLength++;
        }

        if (szLength > iMaxLength)
        {
            iMaxLength = szLength;
        }

        if (szLength == fnSig->length)
        {
            return pScan;
        }

        pScan++;
    }

    return nullptr;
}