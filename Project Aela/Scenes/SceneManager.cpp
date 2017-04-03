#include "stdafx.h"
#include "SceneManager.h"


Aela::SceneManager::SceneManager() {
}


Aela::SceneManager::~SceneManager() {
}

void Aela::SceneManager::registerScene(Aela::Scene* scene) {
	scenes.emplace(scene->id, scene);
}

void Aela::SceneManager::setCurrentScene(int id) {
	currentScene = scenes.at(id);
}

Aela::Scene* Aela::SceneManager::getCurrentScene() {
	return currentScene;
}
