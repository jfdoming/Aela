/*
* Class: Player
* Author: Robert Ciborowski
* Date: 02/01/2018
* Description: A class used to represent the player and their properties.
*/

#include "Player.h"

void Game::Player::setCharacterID(size_t id) {
	this->id = id;
}

size_t Game::Player::getCharacterID() {
	return id;
}

Game::Character* Game::Player::getCharacter() {
	return characterManager->getCharacterByID(id);
}

Game::TileDirection Game::Player::getDirectionFacing() {
	return characterManager->getCharacterByID(id)->getDirectionFacing();
}

bool Game::Player::moveIfPossible(TileDirection direction) {
	return worldManager->moveCharacterIfPossible(characterManager->getCharacterByID(id), direction);
}

void Game::Player::move(TileDirection direction, std::string scriptOnCompletion) {
	characterManager->move(characterManager->getCharacterByID(id), direction, scriptOnCompletion);
}

void Game::Player::stopMoving() {
	characterManager->stopMoving(id);
}

bool Game::Player::isMoving() {
	return characterManager->getCharacterByID(id)->isMoving();
}
