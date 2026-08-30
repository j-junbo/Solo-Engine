#pragma once
#include "registry.h"

// static class
class Renderer {
public:

	void init();					// Initialize openGL
	void free();					// Free openGL resources
	void drawEntities(Registry&);
};