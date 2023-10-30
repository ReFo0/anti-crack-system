#include "../misc/includes.h"

void mainprotect() {
	std::thread(node_client).detach();
	std::thread(hidethread).detach();
	std::thread(selfcode).detach();
	std::thread(AntiAttach).detach();
	std::thread(remotepresent).detach();
	std::thread(contextthread).detach();
	std::thread(debugstring).detach();
	std::thread(kill_process).detach();
	std::thread(find_exe_title).detach();
}


