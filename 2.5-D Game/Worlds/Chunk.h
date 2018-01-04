/*
* Class: Chunk
* Author: Robert Ciborowski
* Date: 07/12/2017
* Description: A class used to store tiles.
*/

#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include "Tile.h"
#include "Utilities\glmut.h"

namespace Game {
	const unsigned int CHUNK_WIDTH = 16, CHUNK_LENGTH = 16;
	typedef std::unordered_map<glm::ivec3, Tile, Vec3HashMapFunctions, Vec3HashMapFunctions> TileMap;

	class Chunk {
		public:
			Chunk() {}

			Chunk(TileMap* tiles) {
				this->tiles = *tiles;
			}

			void addTile(glm::ivec3 coordinate, Tile* tile);
			void setTiles(TileMap* tiles);
			Tile* getTile(glm::ivec3 position);
			TileMap* getTiles();
			void generateBlankTiles(int height);

		private:
			TileMap tiles;
	};
}