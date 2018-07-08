/*
* Class: Player
* Author: Robert Ciborowski
* Date: 02/01/2018
* Description: A class used to represent the player and their properties.
*/

#include "Player.h"
#include "../Character/Character.h"
#include "../Worlds/WorldManager.h"
#include "../Worlds/TileInventory.h"

void Game::Player::setCharacterID(size_t id) {
	this->id = id;
}

size_t Game::Player::getCharacterID() {
	return id;
}

Game::TileInventory* Game::Player::getTileInventory() {
	return &tileInventory;
}

void Game::Player::setCharacter(Character* character) {
	this->character = character;
}

Game::Character* Game::Player::getCharacter() {
	return character;
}
