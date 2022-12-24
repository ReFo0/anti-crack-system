#include <Windows.h>
#include <stdio.h>
#include <array>
#include "filler.h"
#include "ntapi.h"
#include "pe_header.h"
#include "remap.h"
#include "../../encryption/xor.h"
#include "selfcode.h"

static bool ValidateSectionAlignment(const PVOID ImageBase)
{
    PE_HEADER pe;
    if (!FillPEHeader(SIZE_T(ImageBase), pe))
        return false;

    const PIMAGE_SECTION_HEADER text = GetPeSectionByName(pe, _xor_(".text").c_str());
    const PIMAGE_SECTION_HEADER rdata = GetPeSectionByName(pe, _xor_(".rdata").c_str());
    const PIMAGE_SECTION_HEADER data = GetPeSectionByName(pe, _xor_(".data").c_str());
    if (!(text && text < rdata && rdata < data))
        return false;

    auto printSectionSummary = [&ImageBase](const PIMAGE_SECTION_HEADER Section){};

    printSectionSummary(text);
    printSectionSummary(rdata);
    printSectionSummary(data);

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    auto isAligned = [&si](const PIMAGE_SECTION_HEADER Section)
    {
        return Section->VirtualAddress % si.dwAllocationGranularity == 0;
    };

    return isAligned(rdata) && isAligned(data);
}

static void TestVirtualProtect(const PVOID ImageBase)
{
    PE_HEADER pe;
    if (!FillPEHeader(SIZE_T(ImageBase), pe))
        return;

    const PIMAGE_SECTION_HEADER text = GetPeSectionByName(pe, _xor_(".text").c_str());
    const PIMAGE_SECTION_HEADER rdata = GetPeSectionByName(pe, _xor_(".rdata").c_str());
    if (!(text && text < rdata))
        return;

    auto testVirtualProtect = [](SIZE_T BaseAddress, SIZE_T RegionSize, DWORD NewProtection)
    {
        PVOID regionBase = PVOID(BaseAddress);
        SIZE_T regionSize = RegionSize;
        DWORD oldProtection = 0;
        ntapi::NTSTATUS status = ntapi::NtProtectVirtualMemory(GetCurrentProcess(),&regionBase,&regionSize,NewProtection,&oldProtection);

        if (status != ntapi::STATUS_INVALID_PAGE_PROTECTION && NewProtection != oldProtection);
    };

    const std::array<DWORD, 12> protectionValues =
    {
        PAGE_NOACCESS,
        PAGE_READONLY,
        PAGE_READWRITE,
        PAGE_WRITECOPY,
        PAGE_EXECUTE,
        PAGE_EXECUTE_READ,
        PAGE_EXECUTE_READWRITE,
        PAGE_EXECUTE_WRITECOPY,
        PAGE_GUARD,
        PAGE_NOCACHE,
        PAGE_WRITECOMBINE,
        PAGE_REVERT_TO_FILE_MAP
    };

    for (auto protection : protectionValues)
    {
        testVirtualProtect(pe.optionalHeader->ImageBase,PE_HEADER_SIZE + text->Misc.VirtualSize,protection);

        testVirtualProtect(pe.optionalHeader->ImageBase + rdata->VirtualAddress,rdata->Misc.VirtualSize,protection);
    }
}

int selfcode()
{
    const PVOID imagebase = GetModuleHandleA(0);
    if (ValidateSectionAlignment(imagebase))
    {
        const DWORD imageSize = GetSizeOfImage(imagebase);

        if (PVOID remapperRegion = VirtualAlloc(0, imageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE))
        {
            memcpy(remapperRegion, imagebase, imageSize);

            typedef void(*Remap_t)(PVOID);
            Remap_t Remap = Remap_t(SIZE_T(remapperRegion) + SIZE_T(remap::RemapSelfImage) - SIZE_T(imagebase));

            Remap(remapperRegion);

            VirtualFree(remapperRegion, 0, MEM_RELEASE);
            TestVirtualProtect(imagebase);
            for (;;)
            {
                Sleep(3000);
            }
        }

        if (SIZE_T(imagebase) == 1) {
            filler::text(); const double zxcv = filler::rdata[3];
        }
        return 0;
    }
}