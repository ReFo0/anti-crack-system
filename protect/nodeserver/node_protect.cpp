#include <iostream>
#include <string.h>
#include <urlmon.h>
#include "node_protect.h"
#include "../../encryption/xor.h"
#pragma comment(lib, "urlmon.lib")




bool node_client()
{
	std::string url = _xor_("http://127.0.0.1:3000").c_str();
	IStream* stream;
	auto result = URLOpenBlockingStreamA(0, url.c_str(), &stream, 0, 0);
	return true;
}