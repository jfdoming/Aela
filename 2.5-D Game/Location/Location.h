/*
* Class: Location
* Author : Robert Ciborowski
* Date : 16/12/2017
* Description: A class used to represent a location in the game->
*/

#pragma once
#include <glm/glm.hpp>

namespace Game {
	enum class TileDirection {
		RIGHT, FORWARD, LEFT, BACKWARD, UP, DOWN
	};

	enum class ElevationDirection {
		UP, NONE, DOWN
	};

	class Location {
		public:
			Location();
			Location(size_t world, glm::ivec2 chunk, glm::ivec3 tile);
			Location(size_t world, int chunkX, int chunkY, int tileX, int tileY, int tileZ);

			void setWorld(size_t world);
			void setChunk(glm::ivec2 chunk);
			void setTile(glm::ivec3 tile);
			size_t getWorld();
			glm::ivec2 getChunk();
			glm::ivec3 getTileGroup();
			glm::vec3 getWorldSpaceLocation();

		private:
			size_t world;
			glm::ivec2 chunk;
			glm::ivec3 tile;
	};
}