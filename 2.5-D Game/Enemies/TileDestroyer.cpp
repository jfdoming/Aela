#include "TileDestroyer.h"
#include "../Worlds/WorldManager.h"

Game::TileDestroyer::TileDestroyer() {
}

Game::TileDestroyer::TileDestroyer(std::string name, const Location& location, unsigned range) : Enemy(name, location) {
	this->range = range;
}

Game::TileDestroyer::~TileDestroyer() {
}

void Game::TileDestroyer::destroyMatter() {
	glm::ivec2 chunk = location.getChunk();
	glm::ivec3 tileGroup = location.getTileGroup();
	WorldManager* worldManager = GameObjectProvider::getWorldManager();

	for (auto& pair : destructionAngles) {
		if (pair.second) {
			for (unsigned int i = 0; i < range; i++) {
				worldManager->getCoordinateOfNeighbouringTile(tileGroup, chunk, pair.first);
				TileGroup* group = worldManager->getTileGroup(Location(location.getWorld(), chunk, tileGroup));
				Tile* tile = group->getFloorTile();
				if (tile != nullptr) {
					group->removeTile(tile->getType());
					group->addTile(9);
				}
			}
		}
	}
}

void Game::TileDestroyer::setDestructionAngle(TileDirection direction, bool detect) {
	destructionAngles[direction] = detect;
}

void Game::TileDestroyer::setDestructionAngles(bool right, bool forward, bool left, bool backward) {
	destructionAngles[TileDirection::RIGHT] = right;
	destructionAngles[TileDirection::FORWARD] = forward;
	destructionAngles[TileDirection::LEFT] = left;
	destructionAngles[TileDirection::BACKWARD] = backward;
}

void Game::TileDestroyer::attack(Character* character) {
	if (hostile) {
		character->decreaseHealth(strength);
		destroyMatter();
	}
}

void Game::TileDestroyer::update() {
	Enemy::update();

	for (size_t i = 0; i < targets.size(); i++) {
		Character* character = GameObjectProvider::getCharacterProvider()->getCharacterByID(targets[i]);

		if (character == nullptr) {
			targets.erase(targets.begin() + i);
			continue;
		}

		const bool isInSight = (isInLineOfSight(character->getLocationBeforeAnimation(), TileDirection::RIGHT, range)
			&& destructionAngles[TileDirection::RIGHT])
			|| (isInLineOfSight(character->getLocationBeforeAnimation(), TileDirection::FORWARD, range)
				&& destructionAngles[TileDirection::FORWARD])
			|| (isInLineOfSight(character->getLocationBeforeAnimation(), TileDirection::LEFT, range)
				&& destructionAngles[TileDirection::LEFT])
			|| (isInLineOfSight(character->getLocationBeforeAnimation(), TileDirection::BACKWARD, range)
				&& destructionAngles[TileDirection::BACKWARD]);

		if (isInSight) {
			attack(GameObjectProvider::getPlayer()->getCharacter());
		}
	}
}
