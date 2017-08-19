/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 17/08/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#pragma once

#include "Aela_Engine.h"

using namespace Aela;

class AelaGame : public Listener {
	public:
		AelaGame() {

		}

		void setup();
		void update();

		// This is triggered on an event.
		void onEvent(Event* event);

		// These are getters and setters.
		void setMapBeingEdited(Map3D* mapBeingEdited);
		Map3D* getMapBeingEdited();
		void setEngine(Engine* engine);
	private:
		Engine* engine;
		ResourceManager* resourceManager;
		Map3D* mapBeingEdited;

		void loadResources();
		void loadScenes();
};