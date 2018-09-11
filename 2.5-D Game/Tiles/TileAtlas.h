/*
* Class: TileAtlas
* Author: Robert Ciborowski
* Date: 06/12/2017
* Description: A class used to store the properties of types of tiles.
*/

#pragma once
#include <string>
#include <vector>
#include "TileType.h"
#include "../../Project Aela/Resource Management\ResourceManager.h"

#define BLANK_TILE_TYPE 0

using namespace Aela;

namespace Game {
	class TileAtlas {
		public:
			TileAtlas() {}
			~TileAtlas() {}

			size_t getNumberOfTiles();
			void clearAllTiles();
			TileType* getTileType(size_t id);
			void addTile(TileType& type, Model* model);
			Model* getTileModel(size_t id);

		private:
			std::vector<TileType> tileTypes;
			std::vector<Model*> tileModels;
	};
}