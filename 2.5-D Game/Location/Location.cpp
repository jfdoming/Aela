/*
* Class: Location
* Author : Robert Ciborowski
* Date : 16/12/2017
* Description: A class used to represent a location in the game->
*/

#include "Location.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Worlds/WorldManager.h"

Game::Location::Location() {
	world = 0;
	chunk = glm::ivec2();
	tile = glm::vec3();
}

Game::Location::Location(size_t world, glm::ivec2 chunk, glm::vec3 tile) {
	this->world = world;
	this->chunk = chunk;
	this->tile = tile;
}

Game::Location::Location(size_t world, int chunkX, int chunkY, float tileX, float tileY, float tileZ) {
	this->world = world;
	this->chunk = glm::ivec2(chunkX, chunkY);
	this->tile = glm::vec3(tileX, tileY, tileZ);
}

Game::Location::Location(std::string string, char delimitter) {
	std::vector<std::string> values = split(string, delimitter);
	if (values.size() != 6) {
		// The inputted string is garbage.
		world = 0;
		chunk = glm::ivec2();
		tile = glm::ivec3();
	} else {
		world = (size_t) std::stoi(values[0]);
		chunk = glm::ivec2(std::stof(values[1]), std::stof(values[2]));
		tile = glm::vec3(std::stof(values[3]), std::stof(values[4]), std::stof(values[5]));
	}
}

void Game::Location::setWorld(size_t world) {
	this->world = world;
}

void Game::Location::setChunk(glm::ivec2 chunk) {
	this->chunk = chunk;
}

void Game::Location::setTileGroup(glm::vec3 tile) {
	this->tile = tile;
}

size_t Game::Location::getWorld() const {
	return world;
}

glm::ivec2 Game::Location::getChunk() const {
	return chunk;
}

glm::vec3 Game::Location::getTileGroup() const {
	return tile;
}

glm::vec3 Game::Location::getWorldSpaceLocation() const {
	glm::ivec3 chunkRenderDistances = *GameObjectProvider::getWorldManager()->getChunkRenderDistances();
	glm::vec2 chunkPositionOnMap;
	chunkPositionOnMap.x = (float) ((-chunkRenderDistances.x) + chunk.x + 1) * CHUNK_WIDTH;
	chunkPositionOnMap.y = (float) ((-chunkRenderDistances.z) + chunk.y + 1) * CHUNK_LENGTH;
	return glm::vec3(chunkPositionOnMap.x + tile.x, tile.y, chunkPositionOnMap.y + tile.z);
}

std::string Game::Location::getValuesAsString() {
	return std::to_string(world) + "," + std::to_string(chunk.x) + "," + std::to_string(chunk.y)
		+ "," + toStringWithATrailingZero(tile.x) + "," + toStringWithATrailingZero(tile.y) + "," + toStringWithATrailingZero(tile.z);
}

void Game::Location::translateByAmount(glm::vec3 amount) {
	tile += amount;

	int sign;
	if (tile.x < 0) {
		sign = -1;
	} else {
		sign = 1;
	}
	if (tile.x < 0) {
		float amountToIncreaseBy = floor(tile.x / CHUNK_WIDTH);
		chunk.x += (int) amountToIncreaseBy;
		tile.x -= amountToIncreaseBy * CHUNK_WIDTH;
	} else if (tile.x > CHUNK_WIDTH) {
		float amountToIncreaseBy = floor(tile.x / CHUNK_WIDTH);
		chunk.x += (int) amountToIncreaseBy;
		tile.x -= amountToIncreaseBy * CHUNK_WIDTH;
	}

	if (tile.z < 0) {
		sign = -1;
	} else {
		sign = 1;
	}
	if (tile.z < 0) {
		float amountToIncreaseBy = floor(tile.z / CHUNK_LENGTH);
		chunk.y += (int) amountToIncreaseBy;
		tile.z -= amountToIncreaseBy * CHUNK_LENGTH;
	} else if (tile.z > CHUNK_WIDTH) {
		float amountToIncreaseBy = floor(tile.z / CHUNK_LENGTH);
		chunk.y += (int) amountToIncreaseBy;
		tile.z -= amountToIncreaseBy * CHUNK_LENGTH;
	}
}
