/*
* Class: Player
* Author: Robert Ciborowski
* Date: 02/01/2018
* Description: A class used to represent the player and their properties.
*/

#pragma once
#include <unordered_map>	
#include "../Character/Character.h"
#include "../Worlds/WorldManager.h"

namespace Game {
	class Player {
		public:
			Player(WorldManager* worldManager) {
				this->worldManager = worldManager;
				this->characterManager = worldManager->getCharacterManager();
			}

			// These are getters and setters.
			void setCharacterID(unsigned long long id);
			unsigned long long getCharacterID();
			Character* getCharacter();
			TileDirection getDirectionFacing();

			// During regular gameplay, the player can be moved if its possible to do so.
			bool moveIfPossible(TileDirection direction);

			// If the player is in a non-collision mode, they can be forced to move. Note that this won't triggered tile events.
			void move(TileDirection direction, std::string scriptOnCompletion);

			void stopMoving();
			bool isMoving();

		private:
			// This stores the Character ID of the player, which is assigned by the Character Manager.
			unsigned long long id;

			WorldManager* worldManager;
			CharacterManager* characterManager;
	};
}