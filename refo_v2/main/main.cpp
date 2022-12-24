#include "../protect/protectmain.h"
#include <iostream>

int main() {
	mainprotect();
	std::cout << _xor_("hello world \n").c_str();
	std::cin.get();
}