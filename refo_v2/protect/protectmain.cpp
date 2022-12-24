#include "protectmain.h"
#include "anti_dump.h"
#include "anti_debugger.h"
#include "anti_attach.h"
#include "killprocess.h"


void mainprotect() {
	std::thread(hidethread).detach();
	std::thread(AntiAttach).detach();
	std::thread(remotepresent).detach();
	std::thread(contextthread).detach();
	std::thread(debugstring).detach();
	std::thread(kill_process).detach();
	std::thread(find_exe_title).detach();
}


