/*
* Class: Location
* Author : Robert Ciborowski
* Date : 16/12/2017
* Description: A class used to represent a location in the game.
*/

#pragma once
#include <glm/glm.hpp>

namespace Game {
	enum class TileDirection {
		RIGHT, FORWARD, LEFT, BACKWARD
	};

	class Location {
		public:
			Location() {
				world = 0;
				chunk = glm::ivec2();
				tile = glm::ivec3();
			}

			Location(unsigned int world, glm::ivec2 chunk, glm::ivec3 tile) {
				this->world = world;
				this->chunk = chunk;
				this->tile = tile;
			}

			void setWorld(int world);
			void setChunk(glm::ivec2 chunk);
			void setTile(glm::ivec3 tile);
			unsigned int getWorld();
			glm::ivec2 getChunk();
			glm::ivec3 getTile();

		private:
			unsigned int world;
			glm::ivec2 chunk;
			glm::ivec3 tile;
	};
}