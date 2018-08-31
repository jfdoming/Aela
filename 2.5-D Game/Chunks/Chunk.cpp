/*
* Class: Chunk
* Author: Robert Ciborowski
* Date: 07/12/2017
* Description: A class used to store tiles.
*/

#include "Chunk.h"

void Game::Chunk::addTile(glm::ivec3 coordinate, Tile* tile) {
	if (coordinate.x >= 0 && coordinate.x < CHUNK_WIDTH && coordinate.y >= 0 && coordinate.y < CHUNK_LENGTH) {
		tiles[coordinate].addTile(tile);
	}
}

void Game::Chunk::setTiles(TileGroupMap* tiles) {
	// One could check to see if every single tile in the map given is at a legal position. However, this would make things
	// a little slow. Since a WorldLoader is really the only class that will be using this function, and since it's loading
	// method makes sure that tiles cannot have invalid positions, this function does not need a check.
	this->tiles = *tiles;
}

Game::TileGroup* Game::Chunk::getTileGroup(glm::ivec3 position) {
	auto iter = tiles.find(position);
	if (iter != tiles.end()) {
		return &iter->second;
	}
	return nullptr;
}

Game::TileGroupMap* Game::Chunk::getTileGroups() {
	return &tiles;
}

void Game::Chunk::generateBlankTiles(int height) {
	for (unsigned int y = 0; y < CHUNK_LENGTH; y++) {
		for (unsigned int x = 0; x < CHUNK_WIDTH; x++) {
			tiles[glm::ivec3(x, height, y)] = TileGroup();

			// Note that 0 represents a blank tile.
			auto tile = Tile();
			tiles[glm::ivec3(x, height, y)].addTile(&tile);
		}
	}
}
