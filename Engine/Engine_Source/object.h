#pragma once
#include <string>
#include <vector>
#include "GL/glew.h"

struct Object {
	GLuint vao{0};
	std::vector<GLuint> buffers;        // Vertex buffer objects
};


class ObjectManager {
public:
	static void init();
	static void free();
	static const Object& getObject(unsigned id);
	static std::string getObjectName(unsigned id);
	static void addObject(const std::string& name);

private:

	static void releaseObject(const std::string& name);
	static void releaseObject(unsigned id);

	static std::vector<Object>		objectList;
	static std::vector<std::string> nameList;
	static std::string				meshFilePath;
};
