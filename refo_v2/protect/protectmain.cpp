#include "protectmain.h"
#include "antidump.h"
#include "anti_debugger.h"


std::uintptr_t process_find(const std::string& name)
{
	const auto snap = LI_FN(CreateToolhelp32Snapshot).safe()(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	PROCESSENTRY32 proc_entry{};
	proc_entry.dwSize = sizeof proc_entry;

	auto found_process = false;
	if (!!LI_FN(Process32First).safe()(snap, &proc_entry)) {
		do {
			if (name == proc_entry.szExeFile) {
				found_process = true;
				break;
			}
		} while (!!LI_FN(Process32Next).safe()(snap, &proc_entry));
	}

	LI_FN(CloseHandle).safe()(snap);
	return found_process
		? proc_entry.th32ProcessID
		: 0;
}


void blue_screen()
{
    BOOLEAN bluescr;
    ULONG cevap;
    RtlAdjustPrivilege(19, TRUE, FALSE, &bluescr);
    NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &cevap);
}

void kill_process()
{
	system(_xor_("taskkill /f /im HTTPDebuggerUI.exe >nul 2>&1").c_str());
	system(_xor_("taskkill /f /im HTTPDebuggerSvc.exe >nul 2>&1").c_str());
	system(_xor_("sc stop HTTPDebuggerPro >nul 2>&1").c_str());
	system(_xor_("taskkill /FI \"IMAGENAME eq cheatengine*\" /IM * /F /T >nul 2>&1").c_str());
	system(_xor_("taskkill /FI \"IMAGENAME eq httpdebugger*\" /IM * /F /T >nul 2>&1").c_str());
	system(_xor_("taskkill /FI \"IMAGENAME eq processhacker*\" /IM * /F /T >nul 2>&1").c_str());
}

void find_exe_title()
{
	while (true) {
		if (process_find(_xor_("KsDumperClient.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("HTTPDebuggerUI.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("HTTPDebuggerSvc.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("FolderChangesView.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("ProcessHacker.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("procmon.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("idaq.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("idaq64.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("Wireshark.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("Fiddler.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("Xenos64.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("Cheat Engine.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("HTTP Debugger Windows Service (32 bit).exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("KsDumper.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("x64dbg.exe")))
		{
			blue_screen();
		}
		else if (process_find(_xor_("ProcessHacker.exe")))
		{
			blue_screen();
		}
		else if (FindWindow(0, _xor_("IDA: Quick start").c_str()))
		{
			blue_screen();
		}

		else if (FindWindow(0, _xor_("Memory Viewer").c_str()))
		{
			blue_screen();
		}
		else if (FindWindow(0, _xor_("Process List").c_str()))
		{
			blue_screen();
		}
		else if (FindWindow(0, _xor_("KsDumper").c_str()))
		{
			blue_screen();
		}
		else if (FindWindow(0, _xor_("HTTP Debugger").c_str()))
		{
			blue_screen();
		}
		else if (FindWindow(0, _xor_("OllyDbg").c_str()))
		{
			blue_screen();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	}
}

void mainprotect() {
	std::thread(hidethread).detach();
	std::thread(remotepresent).detach();
	std::thread(contextthread).detach();
	std::thread(debugstring).detach();
	std::thread(kill_process).detach();
	std::thread(find_exe_title).detach();
}