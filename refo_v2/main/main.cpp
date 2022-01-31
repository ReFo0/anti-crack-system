#include "../protect/protectmain.h"
#include <iostream>

int main() {
	mainprotect();
	std::cout << _xor_("Hello :)\n").c_str();
	std::cin.get();
}