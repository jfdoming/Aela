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

void Game::Character::setModel(Model* entity) {
	this->model = entity;
}

Model* Game::Character::getModel() {
	return model;
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

void Game::Character::setTextureName(TileDirection direction, std::string name) {
	textureNames[direction] = name;
}

void Game::Character::setTextureNames(std::unordered_map<TileDirection, std::string>* textureNames) {
	this->textureNames = *textureNames;
}

void Game::Character::setTextureNames(std::string right, std::string forward, std::string left, std::string backward) {
	textureNames[TileDirection::RIGHT] = right;
	textureNames[TileDirection::FORWARD] = forward;
	textureNames[TileDirection::LEFT] = left;
	textureNames[TileDirection::BACKWARD] = backward;
}

std::string Game::Character::getTextureName(TileDirection direction) {
	return textureNames[direction];
}

std::unordered_map<Game::TileDirection, std::string>* Game::Character::getTextureNames() {
	return &textureNames;
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

bool Game::Character::isMoving() {
	return moving;
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