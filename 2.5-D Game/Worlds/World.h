/*
* Class: World
* Author: Robert Ciborowski
* Date: 07/12/2017
* Description: A class used to store chunks.
*/

#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include "Chunk.h"
#include "../../Project Aela/3D/Maps/Map3D.h"

namespace Game {
	typedef std::unordered_map<glm::ivec2, Chunk, Vec2HashMapFunctions, Vec2HashMapFunctions> ChunkMap;

	class World {
		public:
			World() {}

			World(ChunkMap* chunks) {
				this->chunks = *chunks;
			}

			void addChunk(glm::ivec2 coordinate, Chunk* chunk);
			void setChunks(ChunkMap* chunks);
			Chunk* getChunk(glm::ivec2 position);
			ChunkMap* getChunks();
			void setMap3D(Map3D* map3D);
			Map3D* getMap3D();

		private:
			ChunkMap chunks;
			Map3D* map3D;
	};
}