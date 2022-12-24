#include "remap.h"

#include <stdio.h>

#include "pe_header.h"
#include "ntapi.h"
#include "../../encryption/xor.h"

#define NtCurrentProcess		((HANDLE)(LONG_PTR)-1)

void remap::RemapSelfImage(const PVOID RegionBase)
{
    PE_HEADER pe;
    if (!FillPEHeader(SIZE_T(RegionBase), pe))
        return;

    HANDLE hRemapSection = NULL;
    LARGE_INTEGER sectionMaxSize = {};
    sectionMaxSize.QuadPart = pe.optionalHeader->SizeOfImage;
    ntapi::NTSTATUS status = ntapi::NtCreateSection(&hRemapSection,SECTION_ALL_ACCESS,NULL,&sectionMaxSize,PAGE_EXECUTE_READWRITE,SEC_COMMIT | ntapi::SEC_NO_CHANGE, NULL);
  

    PVOID copyViewBase = NULL;
    LARGE_INTEGER copySectionOffset = {};
    SIZE_T copyViewSize = 0;
    status = ntapi::NtMapViewOfSection(hRemapSection, NtCurrentProcess,&copyViewBase,0,pe.optionalHeader->SizeOfImage,&copySectionOffset,&copyViewSize,ntapi::ViewUnmap,0,PAGE_READWRITE);


    memcpy(copyViewBase, PVOID(pe.optionalHeader->ImageBase), pe.optionalHeader->SizeOfImage);

    const PIMAGE_SECTION_HEADER text = GetPeSectionByName(pe, _xor_(".text").c_str());
    const PIMAGE_SECTION_HEADER rdata = GetPeSectionByName(pe, _xor_(".rdata").c_str());
    const PIMAGE_SECTION_HEADER data = GetPeSectionByName(pe, _xor_(".data").c_str());
    if (!(text && text < rdata && rdata < data))
        return;

    HMODULE hmNtdll = GetModuleHandleA(_xor_("ntdll.dll").c_str());
    ntapi::NtUnmapViewOfSection_t unmapFn = ntapi::NtUnmapViewOfSection_t(GetProcAddress(hmNtdll, _xor_("NtUnmapViewOfSection").c_str()));
    ntapi::NtMapViewOfSection_t mapFn = ntapi::NtMapViewOfSection_t(GetProcAddress(hmNtdll, _xor_("NtMapViewOfSection").c_str()));
    void *closeHandleFn = &CloseHandle;

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    status = unmapFn(NtCurrentProcess, PVOID(pe.optionalHeader->ImageBase));

    PVOID viewBase = PVOID(pe.optionalHeader->ImageBase);
    LARGE_INTEGER sectionOffset = {};
    sectionOffset.QuadPart = 0;
    SIZE_T viewSize = rdata->VirtualAddress;
    while (viewSize % si.dwAllocationGranularity != 0) {
        viewSize++;
    }
    status = mapFn(hRemapSection, NtCurrentProcess, &viewBase, 0, 0, &sectionOffset, &viewSize, ntapi::ViewUnmap, ntapi::SEC_NO_CHANGE, PAGE_EXECUTE_READ);
    PVOID initialBase = viewBase;

    viewBase = PVOID((UINT_PTR)viewBase + viewSize);
    sectionOffset.QuadPart += viewSize;
    viewSize = data->VirtualAddress - viewSize;
    while (viewSize % si.dwAllocationGranularity != 0) {
        viewSize--;
    }
    status = mapFn(hRemapSection, NtCurrentProcess, &viewBase, 0, 0, &sectionOffset, &viewSize, ntapi::ViewUnmap, ntapi::SEC_NO_CHANGE, PAGE_READONLY);

    viewBase = PVOID((UINT_PTR)viewBase + viewSize);
    sectionOffset.QuadPart += viewSize;
    viewSize = 0;
    status = mapFn(hRemapSection, NtCurrentProcess, &viewBase, 0, 0, &sectionOffset, &viewSize, ntapi::ViewUnmap, ntapi::SEC_NO_CHANGE, PAGE_READWRITE);

    status = unmapFn(NtCurrentProcess, copyViewBase);
 
    if (!reinterpret_cast<BOOL(WINAPI*)(HANDLE)>(closeHandleFn)(hRemapSection));
}