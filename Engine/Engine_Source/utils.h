#pragma once
#include <fstream>
#include <string>

#ifdef DEBUG
#define DEBUG_ONLY_(x) x
#else
#define DEBUG_ONLY_(x)
#endif