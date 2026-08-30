#pragma once
#include "registry.h"
#include <string>

class SceneLayer {

	using ScriptEnter = void(*)();
	using ScriptExit = void(*)();
	using ScriptUpdate = bool(*)(Registry&, float);

	Registry			registry;
	std::string			name;
	std::vector<size_t> activeList;

	ScriptEnter			onScriptEnter{ nullptr };
	ScriptUpdate		onScriptUpdate{ nullptr };
	ScriptExit			onScriptExit{ nullptr };

public:
	void onEnter();
	void onExit();
	bool update(float);		// Returns true if "passthrough"
	void render(float);


	void updateActiveList();

	void unlinkScriptingFunctions();	// Only for engine.
};

// Static class
class SceneManager {
public:
	enum SceneAction {
		PUSH,
		POP,
		SWAPTOP,
		SWAPFULL,
		ENCODE,
		NONE
	};

	static void queueSceneAction(SceneAction, const std::string& = "");

	static void update(float);
	static void render(float);

private:
	static void pushScene(const std::string&);
	static void popScene();
	static void swapTopScene(const std::string&);
	static void swapFullScene(const std::string&);

	static SceneLayer loadScene(const std::string&);
	static std::vector<SceneLayer> sceneStack;
	static SceneAction queuedAction;
	static std::string queuedName;
};
