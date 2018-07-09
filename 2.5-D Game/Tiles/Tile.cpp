/*
* Class: Tile
* Author: Robert Ciborowski
* Date: 03/12/2017
* Description: A class used to represent an in-game tile/space.
*/

#include "Tile.h"

size_t Game::Tile::getType() {
	return type;
}

void Game::Tile::setType(size_t type) {
	this->type = type;
}

Aela::ModelEntity* Game::Tile::getEntity() {
	return entity;
}

void Game::Tile::setEntity(ModelEntity* entity) {
	this->entity = entity;
}
