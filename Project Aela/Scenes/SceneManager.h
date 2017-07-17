/*
 * Class: SceneManager
 * Author: Julian Dominguez-Schatz
 * Date: 26/02/2017
 * Description: Keeps track of the scene system.
 */

#pragma once
#include "stdafx.h"
#include <unordered_map>
#include "Scene.h"

namespace Aela {
	class SceneManager {
	public:
		SceneManager();
		~SceneManager();

		void registerScene(Aela::Scene* scene, int id);
		void setCurrentScene(int id);
		Scene* getCurrentScene();
	private:
		std::unordered_map<int, Aela::Scene*> scenes;
		Scene* currentScene = nullptr;
	};
}
