/*
* Class: WorldLoader
* Author: Robert Ciborowski
* Date: 06/12/2017
* Description: A class used to load Worlds.
*/

#pragma once
#include <string>
#include "World.h"

namespace Game {
	class WorldLoader {
		public:
			WorldLoader() {}

			bool loadWorld(std::string path, World& world);
		private:
			bool generateTileTypes(TileMap& tiles, std::vector<std::string> data, int height);
	};
}