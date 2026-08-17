#include <iostream>
#include <engine_main.h>
#include <string>

int engine_main(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {

	AllocConsole();

	// Redirectiong cin and cout to the allocated console
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    std::ios::sync_with_stdio();

	std::cout << "hello world!!\n";

	std::string s;
	while (std::getline(std::cin,s)) { 
		break;
	}
	
	return 0;
}