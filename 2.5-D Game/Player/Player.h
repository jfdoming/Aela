/*
* Class: Player
* Author: Robert Ciborowski
* Date: 02/01/2018
* Description: A class used to represent the player and their properties.
*              I wanted to make Player extend Character, but I can't due
*              to the way that Characters must be stored in the CharacterProvider.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Tiles/TileInventory.h"
#include <unordered_map>

namespace Game {
	class Player {
		public:
			Player();

			// These are getters and setters.
			void setCharacterID(size_t id);
			size_t getCharacterID();
			TileInventory* getTileInventory();
			void setCharacter(Character* character);
			Character* getCharacter();
			void setupTileInventoryForMapEditor();

			void kill();
			void revive();
			bool isAlive();

		private:
			// This stores the Character ID of the player, which is assigned by the Character Tracker.
			size_t id;
			Character* character;

			TileInventory tileInventory;
	};
}