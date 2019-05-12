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
#include "../Save States/SaveState.h"
#include "../Aela Game/AelaGame.h"
#include "../Displays/Tiles/TileInventoryDisplay.h"

Game::Player::Player() {
}

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

void Game::Player::setFunValue(int funValue) {
	this->funValue = funValue;
}

int Game::Player::getFunValue() {
	return funValue;
}

void Game::Player::increaseFunValue(int amount) {
	funValue += amount;
}

Game::Personality* Game::Player::getPersonality() {
	return &personality;
}

void Game::Player::setPersonality(Personality* personality) {
	this->personality = *personality;
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

void Game::Player::lookAtWatch() {
	lookingAtWatch = true;

	if (showingTileGun) {
		character->setTexture(TEXTURE_WITH_GUN_AND_WATCH);
	} else {
		character->setTexture(TEXTURE_WITH_WATCH);
	}
}

void Game::Player::stopLookingAtWatch() {
	lookingAtWatch = false;

	if (showingTileGun) {
		character->setTexture(TEXTURE_WITH_GUN);
	} else {
		character->setTexture(TEXTURE_DEFAULT);
	}

}

void Game::Player::showAndEnableTileGun() {
	showingTileGun = true;
	GameObjectProvider::getGame()->getTileSwitchGun()->setActive(true);
	GameObjectProvider::getTileInventoryDisplay()->show();

	if (lookingAtWatch) {
		character->setTexture(TEXTURE_WITH_GUN_AND_WATCH);
	} else {
		character->setTexture(TEXTURE_WITH_GUN);
	}
}

void Game::Player::hideAndDisableTileGun() {
	showingTileGun = false;
	GameObjectProvider::getGame()->getTileSwitchGun()->setActive(false);
	GameObjectProvider::getTileInventoryDisplay()->hide();

	if (lookingAtWatch) {
		character->setTexture(TEXTURE_WITH_WATCH);
	} else {
		character->setTexture(TEXTURE_DEFAULT);
	}
}

void Game::Player::saveDataToSaveState(SaveState* saveState) {
	Player* player = (Player*) saveState->getData(PLAYER_NAME);
	if (player != nullptr) {
		delete player;
	}
	Player* player2 = new Player(*this);
	saveState->addData(PLAYER_NAME, player2);
}

void Game::Player::loadDataFromSaveState(SaveState* saveState) {
	Player* player = (Player*) saveState->getData(PLAYER_NAME);
	id = player->id;
	character = player->character;
	tileInventory = player->tileInventory;
	lookingAtWatch = player->lookingAtWatch;
	showingTileGun = player->showingTileGun;
	funValue = player->funValue;
	personality = player->personality;
	currentStage = player->currentStage;
	currentLevel = player->currentLevel;
}

void Game::Player::setCurrentStage(int currentStage) {
	this->currentStage = currentStage;
}

int Game::Player::getCurrentStage() {
	return currentStage;
}

void Game::Player::setCurrentLevel(int currentLevel) {
	this->currentLevel = currentLevel;
}

int Game::Player::getCurrentLevel() {
	return currentLevel;
}
