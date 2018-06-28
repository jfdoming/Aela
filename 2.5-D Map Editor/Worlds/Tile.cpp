/*
* Class: Tile
* Author: Robert Ciborowski
* Date: 03/12/2017
* Description: A class used to represent an in-game tile/space.
*/

#include "Tile.h"

std::string* Game::Tile::getWalkedOnScriptID() {
	return &walkedOnScriptID;
}

std::string* Game::Tile::getPromptedScriptID() {
	return &promptedScriptID;
}

std::string* Game::Tile::getSwitchScript() {
	return &switchScriptID;
}

size_t Game::Tile::getType() {
	return type;
}

void Game::Tile::setType(size_t type) {
	this->type = type;
}

Aela::ModelEntity* Game::Tile::getEntity() {
	return entity;
}

void Game::Tile::setEntity(ModelEntity* entity) {
	this->entity = entity;
}

void Game::Tile::setWalkedOnScript(std::string name) {
	walkedOnScriptID = name;
}

void Game::Tile::setPromptedScript(std::string name) {
	promptedScriptID = name;
}

void Game::Tile::setSwitchScript(std::string name) {
	switchScriptID = name;
}
