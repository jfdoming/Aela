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
