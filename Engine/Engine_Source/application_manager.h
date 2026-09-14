#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ApplicationManager {
public:

	using errorCb =		void(*)(int, char const*);
	using fbsizeCb =	void(*)(GLFWwindow*, int, int);
	using keyCb =		void(*)(GLFWwindow*, int, int, int, int);
	using mButtonCb =	void(*)(GLFWwindow*, int, int, int);
	using mScrollCb =	void(*)(GLFWwindow*, double, double);
	using mPosCb =		void(*)(GLFWwindow*, double, double);
	using focusCb =		void(*)(GLFWwindow*, int);

public:

	static void init();
	static void run();
	static void free();

	// The callbacks are done with a double indirection layer, but should be fine...
	static void addErrorCb(errorCb cb);
	static void addFbsizeCb(fbsizeCb cb);
	static void addKeyCb(keyCb cb);
	static void addMousebuttonCb(mButtonCb cb);
	static void addMousescrollCb(mScrollCb cb);
	static void addMouseposCb(mPosCb cb);
	static void addFocusCb(focusCb cb);

private:

	static void errorCbInternal(int error, char const* description);
	static void fbsizeCbInternal(GLFWwindow* ptr_win, int width, int height);
	static void keyCbInternal(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void mButtonCbInternal(GLFWwindow* pwin, int button, int action, int mod);
	static void mScrollCbInternal(GLFWwindow* pwin, double xoffset, double yoffset);
	static void mPosCbInternal(GLFWwindow* pwin, double xpos, double ypos);
	static void focusCbInternal(GLFWwindow* window, int focused);

	static size_t		width;
	static size_t		height;
	static std::string	windowName;
	static GLFWwindow*	windowPtr;
	static bool			fullscreen;
	
	static std::vector<errorCb>   errorCbVec;
	static std::vector<fbsizeCb>  fbsizeCbVec;
	static std::vector<keyCb>     keyCbVec;
	static std::vector<mButtonCb> mButtonCbVec;
	static std::vector<mScrollCb> mScrollCbVec;
	static std::vector<mPosCb>    mPosCbVec;
	static std::vector<focusCb>   focusCbVec;
};