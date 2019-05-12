#include <utility>

#include "TileAmountLock.h"
#include "Door.h"
#include "../Location/Location.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Worlds/WorldManager.h"
#include "../../Project Aela/Utilities/intut.h"
#include "DoorProvider.h"
Game::TileAmountLock::TileAmountLock() = default;

Game::TileAmountLock::~TileAmountLock() = default;

void Game::TileAmountLock::specifyRegionAsCuboid(const Location& location, size_t width, size_t height, size_t depth) {
	WorldManager* worldManager = GameObjectProvider::getWorldManager();

	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			for (size_t z = 0; z < depth; z++) {
				glm::ivec2 chunk = location.getChunk();
				glm::ivec3 tile = location.getTileGroup();
				int x2 = tile.x + (int) x, z2 = tile.z + (int) z;
				chunk.x += forceWithinRange<int>(&x2, (signed int) CHUNK_WIDTH);
				chunk.y += forceWithinRange<int>(&z2, (signed int) CHUNK_LENGTH);
				Location tileLocation(location.getWorld(), chunk, glm::ivec3(x2, tile.y + y, z2));
				TileGroup* tileGroup = worldManager->getTileGroup(tileLocation);

				if (tileGroup != nullptr) {
					region.push_back(tileLocation);
				}
			}
		}
	}

	checkIfUnlocked();
}

void Game::TileAmountLock::specifyRegion(std::list<Location>& region) {
	std::list<Location> newRegion;

	for (auto location : region) {
		if (GameObjectProvider::getWorldManager()->getTileGroup(location) != nullptr) {
			newRegion.push_back(location);
		}
	}

	this->region = newRegion;
	checkIfUnlocked();
}

void Game::TileAmountLock::addToRegion(Location* addition) {
	if (GameObjectProvider::getWorldManager()->getTileGroup(*addition) != nullptr) {
		this->region.push_back(*addition);
	}
	checkIfUnlocked();
}

void Game::TileAmountLock::addToRegion(std::list<Location>* addition) {
	for (auto location : region) {
		if (GameObjectProvider::getWorldManager()->getTileGroup(location) != nullptr) {
			this->region.push_back(location);
		}
	}

	checkIfUnlocked();
}

std::list<Game::Location>* Game::TileAmountLock::getRegion() {
	return &region;
}

void Game::TileAmountLock::addTileTypeToCheckFor(size_t type, size_t amountOfTiles) {
	amounts[type] = amountOfTiles;
	checkIfUnlocked();
}

void Game::TileAmountLock::checkIfUnlocked() {
	Door* door = GameObjectProvider::getDoorProvider()->getDoor(doorID);

	if (door == nullptr) {
		return;
	}

	std::unordered_map<size_t, size_t> tileAmounts;
	for (auto pair : amounts) {
		tileAmounts[pair.first] = 0;
	}

	for (auto location : region) {
		for (auto& pair : tileAmounts) {
			auto tileGroup = GameObjectProvider::getWorldManager()->getTileGroup(location);
			if (tileGroup != nullptr && tileGroup->getTile(pair.first) != nullptr) {
				pair.second++;
			}
		}
	}

	for (auto pair : amounts) {
		if (tileAmounts[pair.first] < amounts[pair.first]) {
			door->lock(id);
			return;
		}
	}

	door->unlock(id);
}

void Game::TileAmountLock::setDoorByID(std::string doorID) {
	this->doorID = doorID;
}

void Game::TileAmountLock::setIDInDoor(size_t id) {
	this->id = id;
}
