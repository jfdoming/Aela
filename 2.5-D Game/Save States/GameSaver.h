/*
* Class: Save State
* Author: Robert Ciborowski
* Date: 02/08/2018
* Description: A class used to restore the state of the player and the world.
*/

#pragma once
#include "SaveState.h"
#include "../Worlds/World.h"
#include "../Player/Player.h"

namespace Game {
	class GameSaver {
		public:
			GameSaver();

			bool save(std::string saveStateName);
			bool load(std::string saveStateName);
			bool loadFromFile(std::string saveStateName);
			void disableLoading();
			void enableLoading();

		private:
			// For now, we'll have one save state. Later, we can have a whole bunch of them.
			std::unordered_map<std::string, SaveState> saveStates;
			std::string userPath;
			bool allowLoading = true;
	};
}