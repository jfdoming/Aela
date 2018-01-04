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
#include "Resource Management\ResourceManager.h"
#include "3D\Materials\Material.h"

using namespace Aela;

namespace Game {
	class TileAtlas {
		public:
			TileAtlas() {}
			~TileAtlas() {}

			unsigned long long getNumberOfTiles();
			void clearAllTiles();
			TileType* getTileType(unsigned long long id);
			void addTile(TileType& type, Model* model);
			Model* getTileModel(unsigned long long id);

		private:
			std::vector<TileType> tileTypes;
			std::vector<Model*> tileModels;
	};
}