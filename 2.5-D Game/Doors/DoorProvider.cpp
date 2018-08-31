#include "DoorProvider.h"

using namespace Game;

Game::DoorProvider::DoorProvider() = default;

Game::DoorProvider::~DoorProvider() {
	for (auto& pair : doors) {
		pair.second.cleanup();
	}
}

void Game::DoorProvider::addDoor(std::string tag, Door* door) {
	doors[tag] = *door;
}

Door* Game::DoorProvider::getDoor(std::string tag) {
	auto pos = doors.find(tag);
	if (pos == doors.end()) {
		return nullptr;
	}
	return &pos->second;
}

bool Game::DoorProvider::removeDoor(std::string tag) {
	return doors.erase(tag) != 0;
}

std::unordered_map<std::string, Door>* Game::DoorProvider::getDoors() {
	return &doors;
}

void Game::DoorProvider::saveDataToSaveState(SaveState* saveState) {
	auto* doorsCopy = new std::unordered_map<std::string, Door>();
	(*doorsCopy) = doors;
	saveState->addData("doors", doorsCopy);
}

void Game::DoorProvider::loadDataFromSaveState(SaveState* saveState) {
	doors = *((std::unordered_map<std::string, Door>*) saveState->getData("doors"));
}
