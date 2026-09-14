#include "application_manager.h"
#include <iostream>
#include <utility>

//	Hardcoded base values for now. Will be changed ot be part of a configuration file
//	later on.
size_t		ApplicationManager::width{ 1600 };
size_t		ApplicationManager::height{ 960 };
std::string	ApplicationManager::windowName{ "yippie!" };
GLFWwindow*	ApplicationManager::windowPtr{ nullptr };
bool		ApplicationManager::fullscreen{ false };

std::vector<ApplicationManager::errorCb>   ApplicationManager::errorCbVec;
std::vector<ApplicationManager::fbsizeCb>  ApplicationManager::fbsizeCbVec;
std::vector<ApplicationManager::keyCb>     ApplicationManager::keyCbVec;
std::vector<ApplicationManager::mButtonCb> ApplicationManager::mButtonCbVec;
std::vector<ApplicationManager::mScrollCb> ApplicationManager::mScrollCbVec;
std::vector<ApplicationManager::mPosCb>    ApplicationManager::mPosCbVec;
std::vector<ApplicationManager::focusCb>   ApplicationManager::focusCbVec;

void ApplicationManager::init() {

	// Initialize config parameters here...


	if (!glfwInit()) {
		std::cout << "uh oh!!!!!!!!!\n";
		return;
	}

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	// Configure OpenGL context properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);	// OpenGL version: 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);		// Window is resizeable
	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);		// Border + Close widgets
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);	// Double framebuffer

	// we'll use the entire window as viewport ...
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	// check whether to start in fullscreen mode
	if (fullscreen) {    
		windowPtr = glfwCreateWindow(mode->width, mode->height, windowName.c_str(), glfwGetPrimaryMonitor(), NULL);
	}
	else {    // start in windowed mode
		windowPtr = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
		size_t posX{ (mode->width - width) / 2 }, posY{ (mode->height - height) / 2 };   
		glfwSetWindowPos(windowPtr, posX, posY);
	}

	glfwMakeContextCurrent(windowPtr);
	glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "oh no glew ded!!\n";
		return;
	}

}

void ApplicationManager::run() {

}




void ApplicationManager::addErrorCb(errorCb cb)			{ errorCbVec.push_back(cb); }
void ApplicationManager::addFbsizeCb(fbsizeCb cb)		{ fbsizeCbVec.push_back(cb); }
void ApplicationManager::addKeyCb(keyCb cb)				{ keyCbVec.push_back(cb); }
void ApplicationManager::addMousebuttonCb(mButtonCb cb) { mButtonCbVec.push_back(cb); }
void ApplicationManager::addMousescrollCb(mScrollCb cb) { mScrollCbVec.push_back(cb); }
void ApplicationManager::addMouseposCb(mPosCb cb)		{ mPosCbVec.push_back(cb); }
void ApplicationManager::addFocusCb(focusCb cb)			{ focusCbVec.push_back(cb); }

// A template function to prevent too much repeated code as they are all doing the same thing...
template<typename Func, typename... Args>
static void iterateInternal(std::vector<Func>& v, Args&&... a) {
	for (Func f : v) {
		f(std::forward<Args>(a)...);
	}
}

void ApplicationManager::errorCbInternal(int error, char const* description)							{ iterateInternal(errorCbVec, error, description); }
void ApplicationManager::fbsizeCbInternal(GLFWwindow* pwin, int width, int height)						{ iterateInternal(fbsizeCbVec, pwin, width, height); }
void ApplicationManager::keyCbInternal(GLFWwindow* pwin, int key, int scancode, int action, int mod)	{ iterateInternal(keyCbVec, pwin, key, scancode, action, mod); }
void ApplicationManager::mButtonCbInternal(GLFWwindow* pwin, int button, int action, int mod)			{ iterateInternal(mButtonCbVec, pwin, button, action, mod); }
void ApplicationManager::mScrollCbInternal(GLFWwindow* pwin, double xoffset, double yoffset)			{ iterateInternal(mScrollCbVec, pwin, xoffset, yoffset); }
void ApplicationManager::mPosCbInternal(GLFWwindow* pwin, double xpos, double ypos)						{ iterateInternal(mPosCbVec, pwin, xpos, ypos); }
void ApplicationManager::focusCbInternal(GLFWwindow* pwin, int focused)									{ iterateInternal(focusCbVec, pwin, focused); }