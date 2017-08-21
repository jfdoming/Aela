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
		previousScene = currentScene;
	}

	currentScene = getScene(id);
	sceneChangeRequested = true;
}

Scene* SceneManager::getCurrentScene() {
	return currentScene;
}

Scene* SceneManager::getScene(int id) {
	// determine the requested scene
	auto item = scenes.find(id);
	if (item == scenes.end()) {
		return nullptr;
	}
	return item->second;
}

bool SceneManager::consumeSceneChangeEvent() {
	if (sceneChangeRequested) {
		if (previousScene != nullptr) {
			// hide the previous scene
			previousScene->hide();
			previousScene = nullptr;
		}

		if (currentScene != nullptr) {
			// show the current scene
			currentScene->show();
		}

		sceneChangeRequested = false;
		return true;
	}

	return false;
}