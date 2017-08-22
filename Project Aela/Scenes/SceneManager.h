/*
 * Class: SceneManager
 * Author: Julian Dominguez-Schatz
 * Date: 26/02/2017
 * Description: Keeps track of the scene system.
 *
 * NOTE: This class has a non-virtual destructor! Do not modify this class to inherit from it.
 */

#pragma once
#include "stdafx.h"
#include <unordered_map>
#include "Scene.h"

namespace Aela {
	class SceneManager final {
	public:
		SceneManager();
		~SceneManager();

		void registerScene(Aela::Scene* scene, unsigned int id);

		void update();
		void render(Renderer* renderer);

		void setCurrentScene(unsigned int id);
		unsigned int getCurrentSceneId();
	private:
		std::unordered_map<unsigned int, Aela::Scene*> scenes;

		Scene* currentScene = nullptr;
		Scene* previousScene = nullptr;
		bool sceneChangeRequested = false;

		Scene* getScene(unsigned int id);
		void consumeSceneChangeEvent();
	};
}
