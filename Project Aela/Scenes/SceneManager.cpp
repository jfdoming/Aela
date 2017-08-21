#include "stdafx.h"
#include "SceneManager.h"

using namespace Aela;

SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {
}

void SceneManager::registerScene(Scene* scene, int id) {
	scene->setId(id);
	scenes.emplace(id, scene);
}

void SceneManager::setCurrentScene(int id) {
	if (currentScene != nullptr) {
		previousScene = currentScene;
	}

	// determine the requested scene
	auto item = scenes.find(id);
	if (item == scenes.end()) {
		currentScene = nullptr;
	} else {
		currentScene = item->second;
	}
}

Scene* SceneManager::getCurrentScene() {
	return currentScene;
}

bool SceneManager::consumeSceneChangeEvent() {
	if (previousScene != nullptr) {
		// hide the previous scene
		previousScene->hide();

		// show the current scene
		currentScene->show();
		
		previousScene == nullptr;
		return true;
	}
	return false;
}