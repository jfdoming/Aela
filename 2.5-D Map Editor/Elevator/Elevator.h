/*
* Class: Elevator
* Author: Robert Ciborowski
* Date: 15/04/2018
* Description: An elevator, which is made up of floor tiles.
*              You know what, I don't care about desing anymore.
*/

#pragma once
#include "../Worlds/Tile.h"

namespace Game {
	class Elevator {
		friend class WorldManager;
		public:
			Elevator(int currentLevel, int minLevel, int maxLevel);
			Elevator(int minLevel, int maxLevel);

			void addTile(Tile* tile);

			bool moveUpIfPossible();
			bool moveDownIfPossible();

		private:
			std::vector<Tile*> tiles;

			int currentLevel, maxLevel, minLevel;
	};
}