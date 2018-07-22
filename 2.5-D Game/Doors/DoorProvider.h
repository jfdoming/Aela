/*
* Class: DoorProvider
* Author: Robert Ciborowski
* Date: 13/07/2018
* Description: A class used to store and provide doors.
*/

#pragma once
#include "Door.h"
#include <unordered_map>

namespace Game {
	class DoorProvider {
		public:
			DoorProvider();

			void addDoor(std::string tag, Door* door);
			Door* getDoor(std::string tag);
			bool removeDoor(std::string tag);

		private:
			std::unordered_map<std::string, Door> doors;
	};
}