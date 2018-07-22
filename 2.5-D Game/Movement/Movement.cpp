#include "Movement.h"

using namespace Game;

Game::Movement::Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation) {
	this->destination = *destination;
	this->worldSpaceTranslation = *worldSpaceTranslation;
	this->direction = direction;
	this->teleportation = teleportation;
	teleportationAnimation = TeleportationAnimation::FADE;
}

Game::Movement::Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation,
	bool animated) {
	this->destination = *destination;
	this->worldSpaceTranslation = *worldSpaceTranslation;
	this->direction = direction;
	this->teleportation = teleportation;
	this->animated = animated;
	teleportationAnimation = TeleportationAnimation::FADE;
}

Game::Movement::Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation,
	TeleportationAnimation teleportationAnimation) {
	this->destination = *destination;
	this->worldSpaceTranslation = *worldSpaceTranslation;
	this->direction = direction;
	this->teleportation = teleportation;
	this->teleportationAnimation = teleportationAnimation;
	teleportationAnimation = TeleportationAnimation::FADE;
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

void Game::Movement::setTeleportationAnimation(TeleportationAnimation teleportationAnimation) {
	this->teleportationAnimation = teleportationAnimation;
}

TeleportationAnimation Game::Movement::getTeleportationAnimation() {
	return teleportationAnimation;
}
