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

void Game::Player::moveIfPossible(TileDirection direction) {
	std::cout << "Movement start.\n";
	worldManager->moveCharacterIfPossible(id, direction);
}

void Game::Player::moveIfPossible(std::vector<TileDirection> directions) {
	worldManager->moveCharacterIfPossible(id, directions);
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
