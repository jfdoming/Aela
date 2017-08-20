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

void Aela::SceneManager::update() {
	currentScene->update();
}

void SceneManager::setCurrentScene(int id) {
	if (currentScene != nullptr) {
		currentScene->getMenu()->setUseOfChildren(false);
		currentScene->getMenu()->setInUse(false);
	}
	auto item = scenes.find(id);
	if (item == scenes.end()) {
		currentScene = nullptr;
	} else {
		currentScene = item->second;
		currentScene->getMenu()->setUseOfChildren(true);
		currentScene->getMenu()->setInUse(true);
	}
}

Scene* SceneManager::getCurrentScene() {
	return currentScene;
}

std::unordered_map<int, Aela::Scene*>* Aela::SceneManager::getScenes() {
	return &scenes;
}
