#include "Elevator.h"

Game::Elevator::Elevator(int currentLevel, int minLevel, int maxLevel) {
	if (minLevel > maxLevel) {
		// What?!?!?!
		maxLevel = minLevel;
	}
	if (currentLevel < minLevel || currentLevel > maxLevel) {
		currentLevel = minLevel;
	}
	this->currentLevel = currentLevel;
	this->minLevel = minLevel;
	this->maxLevel = maxLevel;
}

Game::Elevator::Elevator(int minLevel, int maxLevel) : Elevator(minLevel, minLevel, maxLevel) {
}

void Game::Elevator::addTile(Tile* tile) {
	tiles.push_back(tile);
}

bool Game::Elevator::moveUpIfPossible() {
	currentLevel++;
	if (currentLevel > maxLevel) {
		currentLevel = maxLevel;
		return false;
	}
	return true;
}

bool Game::Elevator::moveDownIfPossible() {
	currentLevel--;
	if (currentLevel < minLevel) {
		currentLevel = minLevel;
		return false;
	}
	return true;
}
