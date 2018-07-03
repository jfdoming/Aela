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
#include "TileGroup.h"
#include "../../Project Aela/Utilities/glmut.h"

namespace Game {
	const unsigned int CHUNK_WIDTH = 16, CHUNK_LENGTH = 16;

	// The following represents a map of TileGroups.
	typedef std::unordered_map<glm::ivec3, TileGroup, IVec3HashMapFunctions, IVec3HashMapFunctions> TileGroupMap;

	class Chunk {
		public:
		Chunk() {}

		Chunk(TileGroupMap* tiles) {
			this->tiles = *tiles;
		}

		void addTile(glm::ivec3 coordinate, Tile* tile);
		void setTiles(TileGroupMap* tiles);
		TileGroup* getTileGroup(glm::ivec3 position);
		TileGroupMap* getTileGroups();
		void generateBlankTiles(int height);

		private:
		TileGroupMap tiles;
	};
}