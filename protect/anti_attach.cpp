#pragma once
#include <Windows.h>
#include "anti_attach.h"
#include "../encryption/xor.h"

#define DbgBreakPoint_FUNC_SIZE 0x2
#define DbgUiRemoteBreakin_FUNC_SIZE 0x54
#define NtContinue_FUNC_SIZE 0x18

struct FUNC {
	const char* name;
	FARPROC addr;
	SIZE_T size;
};

FUNC funcList[] = {
	{ _xor_("DbgBreakPoint").c_str(), 0, DbgBreakPoint_FUNC_SIZE },
	{ _xor_("DbgUiRemoteBreakin").c_str(), 0, DbgUiRemoteBreakin_FUNC_SIZE },
	{ _xor_("NtContinue").c_str(), 0, NtContinue_FUNC_SIZE }
};


void AntiAttach() {
	while (true)
	{
		DWORD pid = GetCurrentProcessId();

		WCHAR modName[MAX_PATH] = { 0 };
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

		HMODULE hMod = LoadLibrary(_xor_("ntdll.dll").c_str());
		for (int i = 0; i < _countof(funcList); ++i) {
			funcList[i].addr = GetProcAddress(hMod, funcList[i].name);
		}

		bool result = false;
		auto base_address = GetModuleHandleA(0);
		if (wcsstr((WCHAR*)base_address, _xor_(L"ntdll").c_str()) || wcsstr((WCHAR*)base_address, _xor_(L"NTDLL").c_str())) {
			for (int i = 0; i < _countof(funcList); ++i) {
				DWORD dwOldProtect;

				VirtualProtectEx(hProcess, funcList[i].addr, funcList[i].size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
				result = WriteProcessMemory(hProcess, funcList[i].addr, funcList[i].addr, funcList[i].size, NULL);
				VirtualProtectEx(hProcess, funcList[i].addr, funcList[i].size, dwOldProtect, NULL);

				if (!result) break;
			}
		}

		CloseHandle(hProcess);
	}
}
