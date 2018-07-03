#include "TileGroup.h"

using namespace Game;

Game::TileGroup::TileGroup() {}

void Game::TileGroup::addTile(Tile* tile) {
	if (tiles.size() == 1 && tiles.find(0) != tiles.end()) {
		tiles.clear();
	}
	tiles[tile->getType()] = *tile;
}

bool Game::TileGroup::removeTile(size_t type) {
	auto pos = tiles.find(type);
	if (pos != tiles.end()) {
		tiles.erase(pos);

		if (tiles.size() == 0) {
			addTile(&Tile());
		}
	}
	return false;
}

void Game::TileGroup::clear() {
	tiles.clear();
	addTile(&Tile());
}

TileMap* Game::TileGroup::getTiles() {
	return &tiles;
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
	std::cout << tiles.size() << " is the size.\n";
	for (auto& pair : tiles) {
		std::cout << pair.second.getType() << " is the type.\n";
		TileBehaviour behaviour = tileAtlas->getTileType(pair.second.getType())->getBehaviour();
		if (behaviour == TileBehaviour::FLOOR || behaviour == TileBehaviour::LIQUID_FLOOR) {
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
