/*
* Class: Aela Game
* Author: Ekkon Games
* Date: 06/08/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#pragma once

#include "Aela_Engine.h"

using namespace Aela;

class AelaGame {
	public:
		AelaGame(Engine* engine) : engine(engine) {
			resourceManager = engine->getResourceManager();
		}

		void setup();
		void update();
		void cleanup();
	private:
		Engine* engine;
		ResourceManager* resourceManager;

		void loadResources();
		void loadScenes();
};