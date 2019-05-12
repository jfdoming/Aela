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
#include "Personality.h"
#include "../Save States/Saveable.h"

namespace Game {
	class Player : Saveable {
		public:
			Player();

			// These are getters and setters.
			void setCharacterID(size_t id);
			size_t getCharacterID();
			TileInventory* getTileInventory();
			void setCharacter(Character* character);
			Character* getCharacter();
			void setupTileInventoryForMapEditor();
			void setFunValue(int funValue);
			int getFunValue();
			void increaseFunValue(int amount);
			Personality* getPersonality();
			void setPersonality(Personality* personality);

			void kill();
			void revive();
			bool isAlive();

			void lookAtWatch();
			void stopLookingAtWatch();
			void showAndEnableTileGun();
			void hideAndDisableTileGun();

			virtual void saveDataToSaveState(SaveState* saveState);
			virtual void loadDataFromSaveState(SaveState* saveState);

			void setCurrentStage(int currentStage);
			int getCurrentStage();
			void setCurrentLevel(int currentLevel);
			int getCurrentLevel();

		private:
			// This stores the Character ID of the player, which is assigned by the Character Tracker.
			size_t id;
			Character* character;
			TileInventory tileInventory;

			bool lookingAtWatch = false, showingTileGun = false;
			int funValue = 0;
			Personality personality;
			int currentStage = 1;
			int currentLevel = 1;

			// Wtf, gcc won't let me make these const because doing so deletes operator=.
			std::string TEXTURE_DEFAULT = "player_1", TEXTURE_WITH_WATCH = "player_2",
				TEXTURE_WITH_GUN = "player_3", TEXTURE_WITH_GUN_AND_WATCH = "player_4";
	};
}
