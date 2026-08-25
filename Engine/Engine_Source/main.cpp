#include <iostream>
#include <Windows.h>
#include <string>
#include <libloaderapi.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    AllocConsole();

    // Redirectiong cin and cout to the allocated console
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    std::ios::sync_with_stdio();

    std::cout << "Hello world!\n";

    HINSTANCE hinstDLL;
    int(*Engine_Main)(HINSTANCE, HINSTANCE, PSTR, int);
    BOOL fFreeDLL;

    hinstDLL = LoadLibrary("GameEngine.dll");
    if (hinstDLL != NULL)
    {
        Engine_Main = (int(*)(HINSTANCE, HINSTANCE, PSTR, int))GetProcAddress(hinstDLL, "engine_main");

        if (Engine_Main != NULL) {
            Engine_Main(hInst, hInstPrev, cmdline, cmdshow);
        }
        else {
            std::cout << "Failed loading engine dll!\n";
        }

        std::cout << "freeing the dll...\n";
        fFreeDLL = FreeLibrary(hinstDLL);
    }
    else {
        std::cout << "dll not found...\n";
        std::cout << "error code: " << GetLastError() << '\n';
    }

    std::string s;
    while (std::getline(std::cin, s)) {
        break;
    }


    return 0;
}