/*
* Class: Tile
* Author: Robert Ciborowski
* Date: 03/12/2017
* Description: A class used to represent an in-game tile/space.
*/

#include "Tile.h"

Game::Tile::Tile() {
	type = 0;
	entity = nullptr;
}

Game::Tile::Tile(size_t type) {
	this->type = type;
	entity = nullptr;
}

Game::Tile::~Tile() {
	/*if (teleporter != nullptr) {
		delete teleporter;
	}*/
}

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

Game::Teleporter* Game::Tile::getTeleporter() {
	if (teleporterPtr == nullptr) {
		return nullptr;
	}
	return &teleporter;
}

void Game::Tile::createTeleporter(Location* location) {
	teleporter = Teleporter(location);
	teleporterPtr = &teleporter;
}

void Game::Tile::removeTeleporter() {
	teleporterPtr = nullptr;
}
