#include "object.h"
#include <fstream>
#include <sstream>
#include "glm/glm.hpp"

std::vector<Object>			ObjectManager::objectList;
std::vector<std::string>	ObjectManager::nameList;
std::string					ObjectManager::meshFilePath;

void ObjectManager::Init() {
	if (std::string(GAME_NAME).empty()) {
		// Load with default engine list
		meshFilePath = { "Assets/Engine_Assets/Meshes/" };

	}
	else {
		meshFilePath = { "Assets/" GAME_NAME "_Assets/Meshes/" };
	}

	std::ifstream ifs(meshFilePath + "meshList.txt");

	// If no file, generate own new one and stop
	if (!ifs) {
		std::ofstream ofs(meshFilePath + "meshList.txt");
	}
	else {
		// Read the file and add it in order, the order will equal the id of object meshes
		std::string name;
		while (std::getline(ifs, name)) {
			AddObject(name);
		}

	}
}
void ObjectManager::Free() {

}
void ObjectManager::AddObject(const std::string& name) {
	// deserialize the object data
	std::ifstream meshFile(meshFilePath + name);
	std::string line;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texture;
	std::vector<glm::vec3> normals;
	std::vector<unsigned> vertex_idx;
	// Not used, but obj files support it. 
	// Not sure when ti would be useful...
	std::vector<unsigned> texture_idx;
	std::vector<unsigned> normal_idx;

	while (std::getline(meshFile, line)) {
		// Empty line / comment
		if (line.empty() || line[0] == '#') continue;

		// Removing comment in the end, if exists 
		line = line.substr(0, line.find_first_of('#'));

		std::string start = line.substr(0, 2);	// get the first 2 chars
		std::stringstream lineStream{ line };

		// Geometric vertex
		if (start == "v ") {
			float x, y, z;
			char c;

			lineStream >> c >> x >> y >> z;
			vertices.push_back({ x,y,z });
		}
		// Texture coordinates
		else if (start == "vt") {
			float u, v;
			char c;

			lineStream >> c >> c >> u >> v;
			texture.push_back({ u,v });
		}
		// Vertex normals
		else if (start == "vn") {
			float x, y, z;
			char c;

			lineStream >> c >> c >> x >> y >> z;
			normals.push_back({ x,y,z });
		}
		// Polygon Face
		else if (start == "f ") {

			char c;
			lineStream >> c;

			int backspace{ 0 };

			while (lineStream.good()) {

			this_is_not_good:

				if (lineStream.peek() == '/') {
					backspace++;
					lineStream.get();
					goto this_is_not_good;
				}
				else if (lineStream.peek() == ' ') backspace = 0;

				unsigned val;
				lineStream >> val;

				switch (backspace) {
				case 0:
					vertex_idx.push_back(val);
					break;
				case 1:
					texture_idx.push_back(val);
					break;
				case 2:
					normal_idx.push_back(val);
					break;
				}
			}
		}
		else {
			// Unsupported.
			// Empty on purpose
		}
	}


	// credit vadim for this
	//

	// Declare OpenGL buffer object IDs for indices, positions, normals, texCoords
	GLuint indexBuf = 0, posBuf = 0, normBuf = 0, tcBuf = 0;
	Object object;

	// Create and bind a buffer for the index data (element array buffer)
	glGenBuffers(1, &indexBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertex_idx.size() * sizeof(unsigned)), vertex_idx.data(), GL_STATIC_DRAW);

	// Create and bind a buffer for the vertex positions (array buffer)
	glGenBuffers(1, &posBuf);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3)), vertices.data(), GL_STATIC_DRAW);

	// Create and bind a buffer for the normal data (array buffer)
	glGenBuffers(1, &normBuf);
	glBindBuffer(GL_ARRAY_BUFFER, normBuf);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(normals.size() * sizeof(glm::vec3)), normals.data(), GL_STATIC_DRAW);

	object.buffers.push_back(indexBuf);
	object.buffers.push_back(posBuf);
	object.buffers.push_back(normBuf);

	// If the mesh contains texture coordinates, create and bind a buffer for them
	if (!texture.empty())
	{
		glGenBuffers(1, &tcBuf);
		glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(texture.size() * sizeof(GLfloat)), texture.data(), GL_STATIC_DRAW);

		object.buffers.push_back(tcBuf);
	}

	// Generate and bind a Vertex Array Object (VAO) to store the vertex attribute bindings
	glGenVertexArrays(1, &object.vao);
	glBindVertexArray(object.vao);

	// Bind the index buffer (element array buffer) for use in drawing
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuf);

	// Set up the vertex position attribute (3D coordinates: x, y, z)
	glBindBuffer(GL_ARRAY_BUFFER, posBuf);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Set up the vertex normal attribute (3D normal vector: nx, ny, nz)
	glBindBuffer(GL_ARRAY_BUFFER, normBuf);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// If texture coordinates are available, set up the texture coordinate attribute (2D: u, v)
	if (!texture.empty())
	{
		glBindBuffer(GL_ARRAY_BUFFER, tcBuf);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}

	// Unbind the VAO to finish setting up the vertex buffers and attributes
	glBindVertexArray(0);

	objectList.push_back(object);
	nameList.push_back(name);
}
void ObjectManager::ReleaseObject(const std::string& name) {
	// uh
}
void ObjectManager::ReleaseObject(unsigned id) {
	Object& obj = objectList[id];
	
	if (obj.vao != 0) {
		glDeleteVertexArrays(1, &obj.vao);
	}

	if (!obj.buffers.empty()) {
		glDeleteBuffers(static_cast<GLsizei>(obj.buffers.size()), obj.buffers.data());
		obj.buffers.clear();
	}
}