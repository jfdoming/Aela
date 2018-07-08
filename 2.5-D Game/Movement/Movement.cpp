#include "Movement.h"

using namespace Game;

Game::Movement::Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation) {
	this->destination = *destination;
	this->worldSpaceTranslation = *worldSpaceTranslation;
	this->direction = direction;
	this->teleportation = teleportation;
}

Location* Game::Movement::getDestination() {
	return &destination;
}

glm::vec3* Game::Movement::getWorldspaceTranslation() {
	return &worldSpaceTranslation;
}

TileDirection Game::Movement::getDirection() {
	return direction;
}

bool Game::Movement::isATeleportation() {
	return teleportation;
}

void Game::Movement::setAnimated(bool animated) {
	this->animated = animated;
}

bool Game::Movement::isAnimated() {
	return animated;
}
