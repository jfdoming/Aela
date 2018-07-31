#include "TileAmountLock.h"
#include "Door.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Worlds/WorldManager.h"
#include "../../Project Aela/Utilities/intut.h"

Game::TileAmountLock::TileAmountLock() {}

Game::TileAmountLock::~TileAmountLock() {
	std::cout << "DELETING... TileAmountLock\n";
}

void Game::TileAmountLock::specifyRegionAsCuboid(Location* location, size_t width, size_t height, size_t depth) {
	WorldManager* worldManager = GameObjectProvider::getWorldManager();

	for (size_t x = 0; x < width; x++) {
		for (size_t y = 0; y < height; y++) {
			for (size_t z = 0; z < depth; z++) {
				glm::ivec2 chunk = location->getChunk();
				glm::ivec3 tile = location->getTileGroup();
				size_t x2 = tile.x + x, z2 = tile.z + z;
				chunk.x += (int) forceWithinRange<size_t>(&x2, (size_t) 0, (size_t) CHUNK_WIDTH - 1);
				chunk.y += (int) forceWithinRange<size_t>(&z2, (size_t) 0, (size_t) CHUNK_LENGTH - 1);
				TileGroup* tileGroup = worldManager->getTileGroup(&Location(location->getWorld(), chunk, glm::ivec3(x2, tile.y + y, z2)));
				
				glm::ivec3 tile2 = glm::ivec3(x2, tile.y + y, z2);

				if (tileGroup != nullptr) {
					region.push_back(tileGroup);
					glm::ivec3 tile = location->getTileGroup() + glm::ivec3(x2, y, z2);
				}
			}
		}
	}

	checkIfUnlocked();
}

void Game::TileAmountLock::specifyRegion(std::list<TileGroup*> region) {
	this->region = region;
	checkIfUnlocked();
}

void Game::TileAmountLock::addToRegion(TileGroup* addition) {
	region.push_back(addition);
	checkIfUnlocked();
}

void Game::TileAmountLock::addToRegion(std::list<TileGroup*>* addition) {
	region.splice(region.end(), *addition);
	checkIfUnlocked();
}

std::list<Game::TileGroup*>* Game::TileAmountLock::getRegion() {
	return &region;
}

void Game::TileAmountLock::addTileTypeToCheckFor(size_t type, size_t amountOfTiles) {
	amounts[type] = amountOfTiles;
	checkIfUnlocked();
}

void Game::TileAmountLock::checkIfUnlocked() {
	if (door != nullptr) {
		std::unordered_map<size_t, size_t> tileAmounts;
		for (auto pair : amounts) {
			tileAmounts[pair.first] = 0;
		}

		for (auto tileGroup : region) {
			for (auto& pair : tileAmounts) {
				if (tileGroup->getTile(pair.first) != nullptr) {
					pair.second++;
				}
			}
		}

		for (auto pair : amounts) {
			if (tileAmounts[pair.first] < amounts[pair.first]) {
				door->lock(id);
				std::cout << "locking...\n";
				return;
			}
		}

		std::cout << "unlocking...\n";
		door->unlock(id);
	}
}

void Game::TileAmountLock::setDoor(Door* door) {
	this->door = door;
}

void Game::TileAmountLock::setIDInDoor(size_t id) {
	this->id = id;
}
