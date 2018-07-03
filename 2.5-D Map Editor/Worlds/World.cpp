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

void Game::World::setChunks(ChunkMap* chunks) {
	this->chunks = *chunks;
}

Game::Chunk* Game::World::getChunk(glm::ivec2 position) {
	auto iter = chunks.find(position);
	if (iter != chunks.end()) {
		return &iter->second;
	}
	return nullptr;
}

Game::ChunkMap* Game::World::getChunks() {
	return &chunks;
}

void Game::World::setMap3D(Map3D* map3D) {
	this->map3D = map3D;
}

Map3D* Game::World::getMap3D() {
	return map3D;
}
