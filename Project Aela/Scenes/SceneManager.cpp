#include "stdafx.h"
#include "SceneManager.h"

using namespace Aela;

// public members

SceneManager::SceneManager() {
}

SceneManager::~SceneManager() {
}

void SceneManager::registerScene(Scene* scene, unsigned int id) {
	scene->setId(id);
	scenes.emplace(id, scene);
}

void Aela::SceneManager::update() {
	// determine the current scene
	consumeSceneChangeEvent();

	// update the current scene
	if (currentScene != nullptr) {
		currentScene->update();
	}
}

void Aela::SceneManager::render(Renderer* renderer) {
	// render the current scene
	if (currentScene != nullptr) {
		currentScene->render(renderer);
	}
}

void SceneManager::setCurrentScene(unsigned int id) {
	if (currentScene != nullptr) {
		previousScene = currentScene;
	}

	currentScene = getScene(id);
	sceneChangeRequested = true;
}

unsigned int SceneManager::getCurrentSceneId() {
	return currentScene->getId();
}

// private members

Scene* SceneManager::getScene(unsigned int id) {
	// determine the requested scene
	auto item = scenes.find(id);
	if (item == scenes.end()) {
		return nullptr;
	}
	return item->second;
}

void SceneManager::consumeSceneChangeEvent() {
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
	}
}