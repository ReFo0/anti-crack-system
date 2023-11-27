#include "../misc/includes.h"

void mainprotect() {
	std::thread(check_integrity).detach();
	std::thread(hidethread).detach();
	std::thread(selfcode).detach();
	std::thread(AntiAttach).detach();
	std::thread(remotepresent).detach();
	std::thread(contextthread).detach();
	std::thread(debugstring).detach();
	std::thread(kill_process).detach();
	std::thread(process_window).detach();
	std::thread(node_client).detach();
}


