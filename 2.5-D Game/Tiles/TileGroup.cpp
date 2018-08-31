#include "TileGroup.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Worlds/WorldManager.h"

using namespace Game;

Game::TileGroup::TileGroup() {}

void Game::TileGroup::addTile(Tile* tile) {
	if (tiles.size() == 1 && tiles.find(0) != tiles.end()) {
		tiles.clear();
	}
	tiles[tile->getType()] = *tile;

	// Maybe this should be done?
	// GameObjectProvider::getTileBehaviourExecutor()->runBehaviour()

	GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
}

void Game::TileGroup::addTile(size_t type) {
	if (tiles.size() == 1 && tiles.find(0) != tiles.end()) {
		tiles.clear();
	}
	tiles[type] = Tile(type);

	// Maybe this should be done?
	// GameObjectProvider::getTileBehaviourExecutor()->runBehaviour()

	GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
}

bool Game::TileGroup::removeTile(size_t type) {
	auto pos = tiles.find(type);
	if (pos != tiles.end()) {
		tiles.erase(pos);

		if (tiles.size() == 0) {
			auto tile = Tile();
			addTile(&tile);
		}

		GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
		return true;
	}
	return false;
}

void Game::TileGroup::clear() {
	tiles.clear();
	auto tile = Tile();
	addTile(&tile);
}

TileMap* Game::TileGroup::getTiles() {
	return &tiles;
}

Tile* Game::TileGroup::getTile(size_t type) {
	auto pos = tiles.find(type);
	if (pos != tiles.end()) {
		return &pos->second;
	}
	return nullptr;
}

std::string* Game::TileGroup::getWalkedOnScriptID() {
	return &walkedOnScriptID;
}

std::string* Game::TileGroup::getPromptedScriptID() {
	return &promptedScriptID;
}

std::string* Game::TileGroup::getSwitchScript() {
	return &switchScriptID;
}

void Game::TileGroup::setWalkedOnScript(std::string name) {
	walkedOnScriptID = name;
}

void Game::TileGroup::setPromptedScript(std::string name) {
	promptedScriptID = name;
}

void Game::TileGroup::setSwitchScript(std::string name) {
	switchScriptID = name;
}

Tile* Game::TileGroup::getFloorTile(TileAtlas* tileAtlas) {
	for (auto& pair : tiles) {
		TileShape shape = tileAtlas->getTileType(pair.second.getType())->getShape();
		if (shape == TileShape::FLOOR || shape == TileShape::LIQUID_FLOOR || shape == TileShape::BOXED_FLOOR) {
			return &pair.second;
		}
	}
	return nullptr;
}

Tile* Game::TileGroup::getLiquidFloorTile(TileAtlas* tileAtlas) {
	for (auto& pair : tiles) {
		TileShape shape = tileAtlas->getTileType(pair.second.getType())->getShape();
		if (shape == TileShape::LIQUID_FLOOR) {
			return &pair.second;
		}
	}
	return nullptr;
}

Tile* Game::TileGroup::getSwitchableFloorTile(TileAtlas* tileAtlas) {
	for (auto& pair : tiles) {
		TileShape shape = tileAtlas->getTileType(pair.second.getType())->getShape();
		if (shape == TileShape::LIQUID_FLOOR || shape == TileShape::BOXED_FLOOR) {
			return &pair.second;
		}
	}
	return nullptr;
}

bool Game::TileGroup::containsCollidableTile(TileAtlas* tileAtlas) {
	if (tiles.size() == 0) {
		return false;
	}

	for (auto pair : tiles) {
		if (tileAtlas->getTileType(pair.second.getType())->isCollidable()) {
			return true;
		}
	}

	return false;
}

bool Game::TileGroup::isCollidable(TileAtlas* tileAtlas) {
	if (tiles.size() == 0) {
		return true;
	}

	if (getFloorTile(tileAtlas) == nullptr) {
		return true;
	}

	for (auto pair : tiles) {
		if (tileAtlas->getTileType(pair.second.getType())->isCollidable()) {
			return true;
		}
	}

	return false;
}

bool Game::TileGroup::containsTile(size_t type) {
	return tiles.find(type) != tiles.end();
}

bool Game::TileGroup::containsCollidableNonSwitchableTile(TileAtlas* tileAtlas) {
	for (auto pair : tiles) {
		TileType* type = tileAtlas->getTileType(pair.second.getType());
		TileShape shape = type->getShape();
		if (type->isCollidable() && shape != TileShape::BOXED_FLOOR && shape != TileShape::LIQUID_FLOOR) {
			return true;
		}
	}

	return false;
}
