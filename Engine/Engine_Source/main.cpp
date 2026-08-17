#include <iostream>
#include <functional>
#include <Windows.h>
#include <libloaderapi.h>
#include "dll_export.h"


int main(void) {

	std::cout << "Hello world!\n";

    HINSTANCE hinstDLL;
    FARPROC Engine_Main;
    BOOL fFreeDLL;

    hinstDLL = LoadLibrary("GameEngine.dll");
    //if (hinstDLL != NULL)
    //{
    //    Engine_Main = GetProcAddress(hinstDLL, "engine_main");

    //    if (Engine_Main != NULL) {
    //        Engine_Main();
    //    }
    //    else {
    //        std::cout << "Failed loading engine dll!\n";
    //    }

    //    fFreeDLL = FreeLibrary(hinstDLL);
    //}

    if (hinstDLL != NULL)
    {
        
        EngineAPI api = ((EngineAPI(*)(void))GetProcAddress(hinstDLL, "getEngineAPI"))();

        std::cout << "square is: " << api.square(3, 4);
        

        api = EngineAPI{};
        fFreeDLL = FreeLibrary(hinstDLL);
    }

	return 0;
}