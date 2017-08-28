/*
* Class: Gameplay Manager
* Author: Robert Ciborowski
* Date: 28/08/2017
* Description: A class used to keep track of gameplay-related properties.
*/

#pragma once
#include "../Player/Player.h"
#include "../NPCs/NPCManager.h"
#include "Aela_Engine.h"

using namespace Aela;

class GameplayManager {
	public:
		GameplayManager(Engine* engine) : engine(engine) {

		}

		void setup();
		void update();

		// These are getters and setters.
		Player* getPlayer();
		NPCManager* getNPCManager();

	private:
		// These are Aela Engine objects.
		Engine* engine = nullptr;

		// These are game-related objects.
		Player player;
		NPCManager npcManager;
};