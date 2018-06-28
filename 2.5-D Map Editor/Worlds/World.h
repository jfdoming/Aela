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

namespace Game {
	typedef std::unordered_map<glm::ivec2, Chunk, Vec2HashMapFunctions, Vec2HashMapFunctions> ChunkWorld;

	class World {
		public:
			World() {}

			World(ChunkWorld* chunks) {
				this->chunks = *chunks;
			}

			void addChunk(glm::ivec2 coordinate, Chunk* chunk);
			void setChunks(ChunkWorld* chunks);
			Chunk* getChunk(glm::ivec2 position);
			ChunkWorld* getChunks();

		private:
			ChunkWorld chunks;
	};
}