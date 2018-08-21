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
#include "../Doors/Door.h"

namespace Game {
	class SaveState {
		public:
			SaveState();

			void save();
			void load();

			void addData(std::string tag, void* dataPtr);
			void* getData(std::string tag);

		private:
			Player player;

			std::unordered_map<std::string, void*> data;
	};
}