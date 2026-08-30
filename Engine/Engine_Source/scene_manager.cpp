#include "scene_manager.h"

// Static variables:
std::vector<SceneLayer>		SceneManager::sceneStack;
SceneManager::SceneAction	SceneManager::queuedAction;
std::string					SceneManager::queuedName;

void SceneManager::queueSceneAction(SceneAction action, const std::string& name) {
	if (queuedAction == NONE) {
		queuedAction = action;
		queuedName = name;
	}
}

void SceneManager::pushScene(const std::string& sceneName) {

	sceneStack.push_back(loadScene(sceneName));
	sceneStack.back().onEnter();

}
void SceneManager::popScene() {

	sceneStack.back().onExit();
	sceneStack.pop_back();

}
void SceneManager::swapTopScene(const std::string& sceneName) {

	sceneStack.back().onExit();
	sceneStack.pop_back();
	sceneStack.push_back(loadScene(sceneName));
	sceneStack.back().onEnter();

}

void SceneManager::swapFullScene(const std::string& sceneName) {

	sceneStack.push_back(loadScene(sceneName));
	sceneStack.back().onEnter();

}


void SceneManager::update(float dt) {

	// put global updates here, like fmod...

	auto stackIter = sceneStack.rbegin();

	// Update the top, then if "passthrough" is true, update the scene below
	while (stackIter->update(dt) && stackIter != sceneStack.rend()) {
		stackIter++;
	}

	// do whichever scene action that has been queued up
	switch (queuedAction) {
	case PUSH:
		pushScene(queuedName);
		break;
	case POP:
		popScene();
		break;
	case SWAPTOP:
		swapTopScene(queuedName);
		break;
	case SWAPFULL:
		swapFullScene(queuedName);
		break;
	}

	queuedAction = NONE;
	queuedName = "";

}
void SceneManager::render(float dt) {
	// Render from bottom up
	for (SceneLayer& sl : sceneStack) {
		sl.render(dt);
	}

}

void SceneLayer::onEnter() {
	// finds for scripting functions. none for now ig.
}

void SceneLayer::onExit() {

}

bool SceneLayer::update(float dt) {
	return false;
}

void SceneLayer::render(float dt) {

}

void SceneLayer::updateActiveList() {
	activeList = registry.getActiveEntities();
}

void SceneLayer::unlinkScriptingFunctions() {
	onScriptEnter = nullptr;
	onScriptExit = nullptr;
	onScriptUpdate = nullptr;
}


SceneLayer SceneManager::loadScene(const std::string& sceneName) {
	return {};
}