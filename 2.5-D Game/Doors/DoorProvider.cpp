#include "DoorProvider.h"
#include <iostream>

using namespace Game;

Game::DoorProvider::DoorProvider() {}

Game::DoorProvider::~DoorProvider() {
	for (auto& pair : doors) {
		std::cout << "A";
		pair.second.cleanup();
		std::cout << "B\n";
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
