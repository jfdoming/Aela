/*
* Class: Character
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent an Character.
*/

#include "Character.h"
#include <iostream>

void Game::Character::setup(Location* location) {
	this->location = *location;
}

void Game::Character::setLocation(Location* location) {
	this->location = *location;
}

Game::Location* Game::Character::getLocation() {
	return &location;
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
	moving = false;
	animationHadJustEnded = true;
	timePassedAfterAnimationEnd = entity->getTimePassedAfterAnimationEnd();
}

bool Game::Character::animationHasJustEnded() {
	return animationHadJustEnded;
}

void Game::Character::onTrackEnd(std::string trackID) {
	
}

void Game::Character::addTranslation(glm::vec3 translation, std::string scriptOnceComplete) {
	translations.push_back(std::pair<glm::vec3, std::string>(translation, scriptOnceComplete));
}

void Game::Character::removeNextTranslation() {
	if (translations.size() > 0) {
		translations.erase(translations.begin());
	}
}

std::pair<glm::vec3, std::string>* Game::Character::getNextTranslation() {
	if (translations.size() == 0) {
		return nullptr;
	}
	return &translations[0];
}

std::pair<glm::vec3, std::string>* Game::Character::getLastTranslation() {
	if (translations.size() == 0) {
		return nullptr;
	}
	return &translations[translations.size() - 1];
}