/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 17/08/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#pragma once

#include "Aela_Engine.h"

using namespace Aela;

class AelaGame {
	public:
		AelaGame() {

		}

		void setup();
		void update();

		void setEngine(Engine* engine);
	private:
		Engine* engine;
		ResourceManager* resourceManager;

		void loadResources();
		void loadScenes();
};