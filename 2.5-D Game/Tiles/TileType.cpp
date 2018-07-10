#include "TileType.h"

using namespace Game;

Game::TileType::TileType() {
	collidable = false;
	name = "tile";
}

Game::TileType::TileType(bool collidable, TileShape shape, std::string name) {
	this->collidable = collidable;
	this->shape = shape;
	this->name = name;
}

bool Game::TileType::isCollidable() {
	return collidable;
}

TileShape Game::TileType::getShape() {
	return shape;
}

std::string Game::TileType::getName() {
	return name;
}