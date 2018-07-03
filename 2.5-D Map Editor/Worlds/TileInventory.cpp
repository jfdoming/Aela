/*
* Class: TileInventory
* Author: Robert Ciborowski
* Date: 24/02/2018
* Description: A class used to manage tiles owned by the player.
*/

#include "TileInventory.h"

using namespace Game;

Tile* Game::TileInventory::getTile(Tile* tile, size_t whichTile) {
	return &tiles[whichTile];
}

Tile* Game::TileInventory::getCurrentTile(Tile* tile) {
	return &tiles[currentTile];
}

void Game::TileInventory::placeTile(TileGroup* tileGroup) {
	tileGroup->addTile(&tiles[currentTile]);
}

int Game::TileInventory::addTile(Tile* tile) {
	tiles.push_back(*tile);
	return tiles.size() - 1;
}

size_t Game::TileInventory::getNumberOfTiles() {
	return tiles.size();
}

size_t Game::TileInventory::getCurrentTileIndex() {
	return currentTile;
}

Tile* Game::TileInventory::getCurrentTile() {
	return &tiles[currentTile];
}

Tile* Game::TileInventory::getTile(size_t whichTile) {
	return &tiles[whichTile];
}

void Game::TileInventory::setCurrentTile(size_t currentTile) {
	if (currentTile >= 0 && currentTile < tiles.size()) {
		this->currentTile = currentTile;
	} else if (currentTile == (size_t) 0 - 1) {
		this->currentTile = tiles.size() - 1;
	} else {
		this->currentTile = 0;
	}
}

void Game::TileInventory::increaseCurrentTileIfPossible() {
	setCurrentTile(currentTile + 1);
}

void Game::TileInventory::decreaseCurrentTileIfPossible() {
	setCurrentTile(currentTile - 1);
}
