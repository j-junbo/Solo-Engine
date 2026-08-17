#pragma once
#include "dll_export.h"
#include <Windows.h>

extern "C" int engine_main(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow);
extern "C" EngineAPI getEngineAPI();