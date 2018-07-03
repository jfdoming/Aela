/*
* Class: Character
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent an character.
*/

#include "Character.h"
#include <iostream>

using namespace Game;

Game::Character::Character() {
	// According to Platinum, it should be 0.00375.
	walkingSpeed = 0.00375f;
	// According to Platinum, it should be 0.0075, but that feels too fast.
	runningSpeed = 0.0075f;
	directionFacing = TileDirection::BACKWARD;
	health = 1;
	maxHealth = 1;
}

Game::Character::Character(std::string name) : Character() {
	this->name = name;
}

void Game::Character::setup(Location* location) {
	this->location = *location;
}

void Game::Character::setLocation(Location* location) {
	this->location = *location;
}

Game::Location* Game::Character::getLocation() {
	return &location;
}

Location* Game::Character::getLocationBeforeAnimation() {
	return &locationBeforeAnimation;
}

void Game::Character::setModel(Model* model) {
	baseModel = model;
}

Model* Game::Character::getModel() {
	return baseModel;
}

void Game::Character::setName(std::string name) {
	this->name = name;
}

std::string Game::Character::getName() {
	return name;
}

void Game::Character::setWalkingSpeed(float walkingSpeed) {
	this->walkingSpeed = walkingSpeed;
}

float Game::Character::getWalkingSpeed() {
	return walkingSpeed;
}

void Game::Character::setRunningSpeed(float runningSpeed) {
	this->runningSpeed = runningSpeed;
}

float Game::Character::getRunningSpeed() {
	return runningSpeed;
}

float Game::Character::getCurrentSpeed() {
	if (running) {
		return runningSpeed;
	} else {
		return walkingSpeed;
	}
}

void Game::Character::setRunning(bool running) {
	this->running = running;
}

bool Game::Character::getRunning() {
	return running;
}

void Game::Character::setTextureName(std::string textureName) {
	this->textureName = textureName;
}

std::string Game::Character::getTextureName() {
	return textureName;
}

Game::TileDirection Game::Character::getDirectionFacing() {
	return directionFacing;
}

Aela::ModelEntity* Game::Character::getEntity() {
	return entity;
}

void Game::Character::setEntity(ModelEntity* entity) {
	this->entity = entity;
}

Game::CharacterStep Game::Character::getCurrentStep() {
	return currentStep;
}

void Game::Character::switchStep() {
	if (currentStep == CharacterStep::LEFT) {
		currentStep = CharacterStep::RIGHT;
	} else {
		currentStep = CharacterStep::LEFT;
	}
}

bool Game::Character::isMoving() {
	return moving;
}

void Game::Character::animationHasEnded() {
	// Moving gets sets to false by the character manager! It does this since it performs some actions
	// when the character is done moving!
	moving = false;
	locationBeforeAnimation = location;
	animationHadJustEnded = true;
	timePassedAfterAnimationEnd = entity->getTimePassedAfterAnimationEnd();
}

bool Game::Character::animationHasJustEnded() {
	return animationHadJustEnded;
}

void Game::Character::turnSimple(TileDirection direction) {
	if (directionFacing != direction) {
		timePassedAfterAnimationEnd = 0;
		animationHadJustEnded = false;
	}

	directionFacing = direction;
}

void Game::Character::moveSimple(Movement* movement, std::string scriptOnCompletion) {
	moving = true;
	switchStep();

	addTranslation(movement, scriptOnCompletion);
}

void Game::Character::stopMoving() {
	moving = false;
}

void Game::Character::setHealth(int health) {
	this->health = health;
	if (health > maxHealth) {
		health = maxHealth;
	}
	if (health < 0) {
		health = 0;
	}
}

int Game::Character::getHealth() {
	return health;
}

void Game::Character::increaseHealth(int amount) {
	health += amount;
	if (health > maxHealth) {
		health = maxHealth;
	}
}

void Game::Character::decreaseHealth(int amount) {
	health -= amount;
	if (health < 0) {
		health = 0;
	}
}

void Game::Character::setMaxHealth(int maxHealth) {
	this->maxHealth = maxHealth;
	if (maxHealth < 1) {
		maxHealth = 1;
	}
	if (health > maxHealth) {
		health = maxHealth;
	}
}

int Game::Character::getMaxHealth() {
	return maxHealth;
}

void Game::Character::increaseMaxHealth(int amount) {
	maxHealth += amount;
}

void Game::Character::decreaseMaxHealth(int amount) {
	maxHealth -= amount;
	if (maxHealth < 1) {
		maxHealth = 1;
	}
	if (health > maxHealth) {
		health = maxHealth;
	}
}

void Game::Character::setVisibility(bool visible) {
	this->visible = visible;
	entity->setVisibility(visible);
}

bool Game::Character::isVisible() {
	return visible;
}

void Game::Character::allowNewMovements(bool newMovementsAreAllowed) {
	this->newMovementsAreAllowed = newMovementsAreAllowed;
}

bool Game::Character::isFrozen() {
	return newMovementsAreAllowed;
}

void Game::Character::update() {}

void Game::Character::addTranslation(Movement* movement, std::string scriptOnceComplete) {
	translations.push_back(std::pair<Movement, std::string>(*movement, scriptOnceComplete));
}

void Game::Character::removeNextTranslation() {
	if (translations.size() > 0) {
		translations.erase(translations.begin());
	}
}

std::pair<Movement, std::string>* Game::Character::getNextTranslation() {
	if (translations.size() == 0) {
		return nullptr;
	}
	return &translations[0];
}

std::pair<Movement, std::string>* Game::Character::getLastTranslation() {
	if (translations.size() == 0) {
		return nullptr;
	}
	return &translations[translations.size() - 1];
}