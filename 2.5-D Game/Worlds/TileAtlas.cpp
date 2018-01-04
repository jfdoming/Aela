/*
* Class: TileAtlas
* Author: Robert Ciborowski
* Date: 06/12/2017
* Description: A class used to represent an in-game tile/space.
*/

#include "TileAtlas.h"

using namespace Aela;

Game::TileType* Game::TileAtlas::getTileType(unsigned long long id) {
	if (id < tileTypes.size()) {
		return &tileTypes[id];
	}
	return nullptr;
}

unsigned long long Game::TileAtlas::getNumberOfTiles() {
	return tileTypes.size();
}

void Game::TileAtlas::clearAllTiles() {
	tileTypes.clear();
	tileModels.clear();
}

void Game::TileAtlas::addTile(TileType& type, Model* model) {
	tileTypes.push_back(type);
	tileModels.push_back(model);
}

Model* Game::TileAtlas::getTileModel(unsigned long long id) {
	return tileModels.at(id);
}
