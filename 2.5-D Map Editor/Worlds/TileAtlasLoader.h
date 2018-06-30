/*
* Class: TileAtlasLoader
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A class used to load Tile Atlases.
*/

#pragma once
#include <string>
#include "TileAtlas.h"

namespace Game {
	class TileAtlasLoader {
		public:
			TileAtlasLoader() {}

			bool loadAtlas(std::string resourceRoot, std::string path, TileAtlas& atlas);
			void setResourceManager(Aela::ResourceManager* resourceManager);

		private:
			Aela::ResourceManager* resourceManager = nullptr;
	};
}