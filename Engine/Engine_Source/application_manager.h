#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ApplicationManager {
public:

	using errorCb =		void(*)(int, char const*);					// void errorCb(int error, char const* description);
	using fbsizeCb =	void(*)(GLFWwindow*, int, int);				// void fbsizeCb(GLFWwindow* pwin, int width, int height);
	using keyCb =		void(*)(GLFWwindow*, int, int, int, int);	// void keyCb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	using mButtonCb =	void(*)(GLFWwindow*, int, int, int);		// void mButtonCb(GLFWwindow* pwin, int button, int action, int mod);
	using mScrollCb =	void(*)(GLFWwindow*, double, double);		// void mScrollCb(GLFWwindow* pwin, double xoffset, double yoffset);
	using mPosCb =		void(*)(GLFWwindow*, double, double);		// void mPosCb(GLFWwindow* pwin, double xpos, double ypos);
	using focusCb =		void(*)(GLFWwindow*, int);					// void focusCb(GLFWwindow* pwin, int focused);

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

	template <typename... Args> static void errorCbInternal(Args... args);
	template <typename... Args> static void fbsizeCbInternal(Args... args);
	template <typename... Args> static void keyCbInternal(Args... args);
	template <typename... Args> static void mButtonCbInternal(Args... args);
	template <typename... Args> static void mScrollCbInternal(Args... args);
	template <typename... Args> static void mPosCbInternal(Args... args);
	template <typename... Args> static void focusCbInternal(Args... args);

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