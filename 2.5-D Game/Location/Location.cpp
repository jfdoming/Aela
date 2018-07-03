/*
* Class: Location
* Author : Robert Ciborowski
* Date : 16/12/2017
* Description: A class used to represent a location in the game.
*/

#include "Location.h"

void Game::Location::setWorld(int world) {
	this->world = world;
}

void Game::Location::setChunk(glm::ivec2 chunk) {
	this->chunk = chunk;
}

void Game::Location::setTile(glm::ivec3 tile) {
	this->tile = tile;
}

unsigned int Game::Location::getWorld() {
	return world;
}

glm::ivec2 Game::Location::getChunk() {
	return chunk;
}

glm::ivec3 Game::Location::getTileGroup() {
	return tile;
}

glm::vec3 Game::Location::getWorldSpaceLocation() {
	return glm::vec3(chunk.x * CHUNK_WIDTH + tile.x, tile.y, chunk.y * CHUNK_LENGTH + tile.z);
}
