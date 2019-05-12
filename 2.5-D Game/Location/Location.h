/*
* Class: Location
* Author : Robert Ciborowski
* Date : 16/12/2017
* Description: A class used to represent a location in the game.
*/

#pragma once
#include <glm/glm.hpp>
#include <ostream>

namespace Game {
	enum class TileDirection {
		RIGHT, FORWARD, LEFT, BACKWARD, UP, DOWN, NONE
	};

	enum class ElevationDirection {
		UP, NONE, DOWN
	};

	class Location {
		public:
			Location();
			Location(size_t world, glm::ivec2 chunk, glm::vec3 tile);
			Location(size_t world, int chunkX, int chunkY, float tileX, float tileY, float tileZ);
			Location(std::string string, char delimitter);

			void setWorld(size_t world);
			void setChunk(glm::ivec2 chunk);
			void setTileGroup(glm::vec3 tile);
			size_t getWorld() const;
			glm::ivec2 getChunk() const;
			glm::vec3 getTileGroup() const;
			glm::vec3 getWorldSpaceLocation() const;
			std::string getValuesAsString();

			// This translates by a certain amount and returns whether floor(tile coordinate) has changed.
			void translateByAmount(glm::vec3 amount);

			inline bool operator==(const Location& other) {
				return world == other.world && chunk == other.chunk && tile == other.tile;
			}

			inline bool operator!=(const Location& other) {
				return !(*this == other);
			}

			inline void operator+=(const glm::vec3 amount) {
				translateByAmount(amount);
			}

			inline void operator-=(const glm::vec3 amount) {
				translateByAmount(amount * glm::vec3(-1));
			}

			inline friend std::ostream& operator<<(std::ostream &os, Location const& me) {
					return (os << "World: " << me.world << ", Chunk: (" << me.chunk.x << ", " << me.chunk.y << "), Tile: (" << me.tile.x
						<< ", " << me.tile.y << ", " << me.tile.z << ")");
				}

		private:
			size_t world;
			glm::ivec2 chunk;
			glm::vec3 tile;
	};
}