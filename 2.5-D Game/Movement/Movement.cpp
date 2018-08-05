#include "Movement.h"

using namespace Game;

Game::Movement::Movement(TileDirection direction) {
	this->direction = direction;
}

Game::Movement::Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation) {
	if (destination != nullptr) {
		this->destination = *destination;
	}
	if (worldSpaceTranslation != nullptr) {
		this->worldSpaceTranslation = *worldSpaceTranslation;
	}
	this->direction = direction;
	this->teleportation = teleportation;
	teleportationAnimation = TeleportationAnimation::FADE;
}

Game::Movement::Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation,
	bool animated) {
	if (destination != nullptr) {
		this->destination = *destination;
	}
	if (worldSpaceTranslation != nullptr) {
		this->worldSpaceTranslation = *worldSpaceTranslation;
	}
	this->direction = direction;
	this->teleportation = teleportation;
	this->animated = animated;
	teleportationAnimation = TeleportationAnimation::FADE;
}

Game::Movement::Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation,
	TeleportationAnimation teleportationAnimation) {
	if (destination != nullptr) {
		this->destination = *destination;
	}
	if (worldSpaceTranslation != nullptr) {
		this->worldSpaceTranslation = *worldSpaceTranslation;
	}
	this->direction = direction;
	this->teleportation = teleportation;
	this->teleportationAnimation = teleportationAnimation;
	teleportationAnimation = TeleportationAnimation::FADE;
}

void Game::Movement::setDestination(Location* destination) {
	this->destination = *destination;
}

Location* Game::Movement::getDestination() {
	return &destination;
}

void Game::Movement::setWorldspaceTranslation(glm::vec3* worldSpaceTranslation) {
	this->worldSpaceTranslation = *worldSpaceTranslation;
}

glm::vec3* Game::Movement::getWorldspaceTranslation() {
	return &worldSpaceTranslation;
}

void Game::Movement::setDirection(TileDirection direction) {
	this->direction = direction;
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

void Game::Movement::setTurnOnly(bool turnOnly) {
	this->turnOnly = turnOnly;
}

bool Game::Movement::isATurnOnly() {
	return turnOnly;
}

void Game::Movement::setPreprocessed(bool preprocessed) {
	this->preprocessed = preprocessed;
}

bool Game::Movement::isPreprocessed() {
	return preprocessed;
}
