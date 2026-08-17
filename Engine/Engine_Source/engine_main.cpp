#include <iostream>
#include <engine_main.h>

int square(int i, int y) { return i * i + y * y; }

class wallahi {
public:
	int square(int i, int y) { return i * i * y * y; }
};

static wallahi wallah;

EngineAPI getEngineAPI() {
	return { std::bind(&wallahi::square,wallah,std::placeholders::_1,std::placeholders::_2) };
}