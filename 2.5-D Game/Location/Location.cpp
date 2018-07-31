/*
* Class: Location
* Author : Robert Ciborowski
* Date : 16/12/2017
* Description: A class used to represent a location in the game->
*/

#include "Location.h"
#include "../Chunks/Chunk.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Worlds/WorldManager.h"

Game::Location::Location() {
	world = 0;
	chunk = glm::ivec2();
	tile = glm::ivec3();
}

Game::Location::Location(size_t world, glm::ivec2 chunk, glm::ivec3 tile) {
	this->world = world;
	this->chunk = chunk;
	this->tile = tile;
}

Game::Location::Location(size_t world, int chunkX, int chunkY, int tileX, int tileY, int tileZ) {
	this->world = world;
	this->chunk = glm::ivec2(chunkX, chunkY);
	this->tile = glm::ivec3(tileX, tileY, tileZ);
}

void Game::Location::setWorld(size_t world) {
	this->world = world;
}

void Game::Location::setChunk(glm::ivec2 chunk) {
	this->chunk = chunk;
}

void Game::Location::setTile(glm::ivec3 tile) {
	this->tile = tile;
}

size_t Game::Location::getWorld() {
	return world;
}

glm::ivec2 Game::Location::getChunk() {
	return chunk;
}

glm::ivec3 Game::Location::getTileGroup() {
	return tile;
}

glm::vec3 Game::Location::getWorldSpaceLocation() {
	glm::ivec3 chunkRenderDistances = *GameObjectProvider::getWorldManager()->getChunkRenderDistances();
	glm::vec2 chunkPositionOnMap;
	chunkPositionOnMap.x = (float) ((-chunkRenderDistances.x) + chunk.x + 1) * CHUNK_WIDTH;
	chunkPositionOnMap.y = (float) ((-chunkRenderDistances.z) + chunk.y + 1) * CHUNK_LENGTH;
	return glm::vec3(chunkPositionOnMap.x + tile.x, tile.y, chunkPositionOnMap.y + tile.z);
}
