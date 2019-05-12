#include "DoorProvider.h"
#include "../Save States/SaveState.h"
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

void DoorProvider::removeAllDoors() {
	doors.clear();
}

std::unordered_map<std::string, Door>* Game::DoorProvider::getDoors() {
	return &doors;
}

void Game::DoorProvider::saveDataToSaveState(SaveState* saveState) {
	std::unordered_map<std::string, Door>* data = (std::unordered_map<std::string, Door>*) saveState->getData("doors");
	if (data != nullptr) {
		delete data;
	}

	for (auto& door : doors) {
		door.second.makeBackupOfLockData();
	}

	data = new std::unordered_map<std::string, Door>();
	(*data) = doors;
	saveState->addData("doors", data);
}

void Game::DoorProvider::loadDataFromSaveState(SaveState* saveState) {
	std::unordered_map<std::string, Door>* data = (std::unordered_map<std::string, Door>*) saveState->getData("doors");
	doors = *data;

	for (auto& door : doors) {
		door.second.useBackupOfLockData();
	}
}
