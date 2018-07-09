#include "TileGroup.h"
#include "../../Project Aela/Utilities/enumut.h"

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
	for (auto& pair : tiles) {
		TileBehaviour behaviour = tileAtlas->getTileType(pair.second.getType())->getBehaviour();
		if (behaviour == TileBehaviour::FLOOR || behaviour == TileBehaviour::LIQUID_FLOOR || behaviour == TileBehaviour::BOXED_FLOOR) {
			return &pair.second;
		}
	}
	return nullptr;
}

Tile* Game::TileGroup::getSwitchableFloorTile(TileAtlas* tileAtlas) {
	for (auto& pair : tiles) {
		std::cout << tileAtlas << " is tileatlas.\n";
		TileBehaviour behaviour = tileAtlas->getTileType(pair.second.getType())->getBehaviour();
		std::cout << enumToInteger(behaviour) << " is behaviour\n";
		if (behaviour == TileBehaviour::LIQUID_FLOOR || behaviour == TileBehaviour::BOXED_FLOOR) {
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
