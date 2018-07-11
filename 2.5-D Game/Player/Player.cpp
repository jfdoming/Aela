/*
* Class: Player
* Author: Robert Ciborowski
* Date: 02/01/2018
* Description: A class used to represent the player and their properties.
*/

#include "Player.h"
#include "../Character/Character.h"
#include "../Worlds/WorldManager.h"
#include "../Tiles/TileInventory.h"

Game::Player::Player() {}

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

void Game::Player::setupTileInventoryForMapEditor() {
	TileAtlas* atlas = GameObjectProvider::getTileAtlas();

	for (size_t i = 0; i < atlas->getNumberOfTiles(); i++) {
		Tile tile(i);
		if (i != 0) {
			ModelEntity* modelEntity = new ModelEntity();
			modelEntity->setModel(atlas->getTileModel(i));
			tile.setEntity(modelEntity);
		}
		tileInventory.addTile(&tile);
	}
}

void Game::Player::kill() {
	character->kill();
}

void Game::Player::revive() {
	character->revive();
}

bool Game::Player::isAlive() {
	return character->isAlive();
}
