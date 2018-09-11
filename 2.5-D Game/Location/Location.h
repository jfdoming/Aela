/*
* Class: Location
* Author : Robert Ciborowski
* Date : 16/12/2017
* Description: A class used to represent a location in the game->
*/

#pragma once
#include <glm/glm.hpp>
#include <ostream>

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
			size_t getWorld() const;
			glm::ivec2 getChunk() const;
			glm::ivec3 getTileGroup() const;
			glm::vec3 getWorldSpaceLocation() const;

			inline bool operator==(const Location& other) {
				return world == other.world && chunk == other.chunk && tile == other.tile;
			}

			inline bool operator!=(const Location& other) {
				return !(*this == other);
			}

		inline friend std::ostream& operator<<(std::ostream &os, Location const& me) {
				return (os << "World: " << me.world << ", Chunk: (" << me.chunk.x << ", " << me.chunk.y << "), Tile: (" << me.tile.x
					<< ", " << me.tile.y << ", " << me.tile.z << ")");
			}

		private:
			size_t world;
			glm::ivec2 chunk;
			glm::ivec3 tile;
	};
}