#include "TileType.h"

using namespace Game;

Game::TileType::TileType() {
	collidable = false;
	name = "tile";
}

Game::TileType::TileType(bool collidable, TileBehaviour behaviour, std::string name) {
	this->collidable = collidable;
	this->behaviour = behaviour;
	this->name = name;
}

bool Game::TileType::isCollidable() {
	return collidable;
}

TileBehaviour Game::TileType::getBehaviour() {
	return behaviour;
}

std::string Game::TileType::getName() {
	return name;
}