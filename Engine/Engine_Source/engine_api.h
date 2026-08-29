#pragma once
#include <functional>
#include "registry.h"

struct EngineAPI {

	std::function<int(int, int)> square;

};