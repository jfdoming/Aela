/*
* Class: World
* Author: Robert Ciborowski
* Date: 07/12/2017
* Description: A class used to store chunks that are close to one another.
*/

#include "World.h"

void Game::World::addChunk(glm::ivec2 coordinate, Chunk* chunk) {
	chunks[coordinate] = *chunk;
}

void Game::World::setChunks(ChunkWorld* chunks) {
	this->chunks = *chunks;
}

Game::Chunk* Game::World::getChunk(glm::ivec2 position) {
	auto iter = chunks.find(position);
	if (iter != chunks.end()) {
		return &iter->second;
	}
	return nullptr;
}

Game::ChunkWorld* Game::World::getChunks() {
	return &chunks;
}
