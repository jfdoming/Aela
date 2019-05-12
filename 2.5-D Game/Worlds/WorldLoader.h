/*
* Class: WorldLoader
* Author: Robert Ciborowski
* Date: 06/12/2017
* Description: A class used to load Worlds.
*/

#pragma once
#include <string>
#include "World.h"
#include "../Game Object Provider/GameObjectProvider.h"

namespace Game {
	class WorldLoader {
		public:
			WorldLoader() {}

			bool loadWorld(std::string path, World& world);
			bool loadWorldAsTextFile(std::string path, World& world);

		private:
			bool generateTileTypes(TileGroupMap& tiles, std::stringstream& data, int height);
			bool interpretText(std::istream& in, World& world);
	};
}