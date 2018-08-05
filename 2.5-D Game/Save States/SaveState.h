/*
* Class: Save State
* Author: Robert Ciborowski
* Date: 04/08/2018
* Description: A class used to store the state of the player and the world.
*/

#pragma once
#include "../Character/CharacterInformationBlock.h"
#include "../Worlds/World.h"
#include "../Player/Player.h"

namespace Game {
	class SaveState {
		public:
			SaveState();

			void save();
			void load();

		private:
			World world;
			Player player;
			std::unordered_map<size_t, CharacterInformationBlock> characterInformation;
	};
}