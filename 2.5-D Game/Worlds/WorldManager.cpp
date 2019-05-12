#include <utility>

/*
* Class: WorldManager
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A class used to manage worlds.
*/

#include "WorldManager.h"
#include "WorldExporter.h"
#include "../Resources/ResourceInfo.h"
#include "../Character/CharacterProvider.h"
#include "../Displays/Dialogue/DialogueDisplay.h"
#include "../Tiles/TileAtlas.h"
#include "../../Project Aela/Resource Management/ResourceManager.h"
#include "../Scripts/ScriptManager.h"
#include "../../Project Aela/3D/Maps/Map3DLoader.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include "../Player/Player.h"
#include "../Tiles/TileBehaviourExecutor.h"
#include "../Aela Game/AelaGame.h"
#include <glm/gtc/constants.hpp>
#include <iostream>

// I'm including this because it contains some useful defines.
#include "../Scripts/Scripts to Move to LUA/SceneScript.h"
#include "../Save States/SaveState.h"

Game::WorldManager::WorldManager() {
	resourceManager = GameObjectProvider::getResourceManager();
	animationLooper = GameObjectProvider::getAnimationLooper();
}

void Game::WorldManager::setup() {
	characterProvider = GameObjectProvider::getCharacterProvider();
	tileBehaviourExecutor = GameObjectProvider::getTileBehaviourExecuter();
	scriptManager = GameObjectProvider::getScriptManager();
	tileAtlas = GameObjectProvider::getTileAtlas();
	playerCharacter = GameObjectProvider::getPlayer()->getCharacter();
	game = GameObjectProvider::getGame();
	worldExporter = GameObjectProvider::getWorldExporter();
	mapRebuilder.setup();
}

void Game::WorldManager::scenesWereSetUp() {
	mapRebuilder.scenesWereSetup();
}

void Game::WorldManager::update() {
	if (mapNeedsToBeRebuilt) {
		rebuildMap();
		mapNeedsToBeRebuilt = false;
	}
}

void Game::WorldManager::rebuildMapNextUpdate() {
	mapNeedsToBeRebuilt = true;
}

void Game::WorldManager::addWorld(World* world, size_t worldID) {
	worlds[worldID] = *world;
}

Game::World* Game::WorldManager::getWorld(size_t id) {
	auto pair = worlds.find(id);
	if (pair == worlds.end()) {
		return nullptr;
	} else {
		return &pair->second;
	}
}

bool Game::WorldManager::removeWorld(size_t id) {
	auto pos = worlds.find(id);
	if (pos == worlds.end()) {
		return false;
	}
	worlds.erase(pos);
	return true;
}

bool Game::WorldManager::doesWorldExist(size_t id) {
	auto pos = worlds.find(id);
	return pos != worlds.end();
}

size_t Game::WorldManager::getNumberOfWorlds() {
	return worlds.size();
}

Map3D* Game::WorldManager::getMap3D() {
	return worlds[playerCharacter->getLocation()->getWorld()].getMap3D();
}

Game::Teleporter* Game::WorldManager::getTeleporter(const Location& location) {
	TileGroup* tileGroup = getTileGroup(location);
	if (tileGroup == nullptr) {
		return nullptr;
	}

	TileMap* tileMap = tileGroup->getTiles();

	for (auto& pair : *tileMap) {
		Teleporter* teleporter = pair.second.getTeleporter();
		if (teleporter != nullptr) {
			return teleporter;
		}
	}

	return nullptr;
}

Game::Teleporter* Game::WorldManager::getTeleporter(const Location& location, size_t tileType) {
	TileGroup* tileGroup = getTileGroup(location);
	if (tileGroup == nullptr) {
		return nullptr;
	}

	Tile* tile = tileGroup->getTile(tileType);
	if (tile == nullptr) {
		return nullptr;
	}

	return tile->getTeleporter();
}

Game::Chunk* Game::WorldManager::getChunk(const Location& location) {
	World* world = getWorld(location.getWorld());
	if (world == nullptr) {
		return nullptr;
	}

	return world->getChunk(location.getChunk());
}

Game::TileGroup* Game::WorldManager::getTileGroup(const Location& location) {
	Chunk* chunk = getChunk(location);
	if (chunk == nullptr) {
		return nullptr;
	}
	return chunk->getTileGroup(location.getTileGroup());
}

glm::ivec3* Game::WorldManager::getChunkRenderDistances() {
	return mapRebuilder.getChunkRenderDistances();;
}

float Game::WorldManager::getCharacterYOffsetInWorldspace() {
	return mapRebuilder.getCharacterYOffsetInWorldspace();
}

void Game::WorldManager::setChunkRenderDistances(glm::vec3 chunkRenderDistances) {
	mapRebuilder.setChunkRenderingDistances(chunkRenderDistances);
	rebuildMapNextUpdate();
}

void Game::WorldManager::getCoordinateOfNeighbouringTile(glm::vec3& tile, glm::ivec2& chunk, TileDirection direction) {
	switch (direction) {
		case TileDirection::RIGHT:
			tile += glm::ivec3(-1, 0, 0);
			if (tile.x < 0) {
				tile.x = CHUNK_WIDTH - 1;
				chunk.x--;
			}
			break;
		case TileDirection::FORWARD:
			tile += glm::ivec3(0, 0, 1);
			if (tile.z >= CHUNK_LENGTH) {
				tile.z = 0;
				chunk.y++;
			}
			break;
		case TileDirection::LEFT:
			tile += glm::ivec3(1, 0, 0);
			if (tile.x >= CHUNK_WIDTH) {
				tile.x = 0;
				chunk.x++;
			}
			break;
		case TileDirection::BACKWARD:
			tile += glm::ivec3(0, 0, -1);
			if (tile.z < 0) {
				tile.z = CHUNK_LENGTH - 1;
				chunk.y--;
			}
			break;
		case TileDirection::UP:
			tile += glm::ivec3(0, 1, 0);
			break;
		case TileDirection::DOWN:
			tile += glm::ivec3(0, -1, 0);
			break;
	}
}

void Game::WorldManager::getCoordinateOfNeighbouringTile(glm::ivec3& tile, glm::ivec2& chunk, TileDirection direction) {
	glm::vec3 tile2 = tile;
	getCoordinateOfNeighbouringTile(tile2, chunk, direction);
	tile = tile2;
}

void Game::WorldManager::addWalkedOnScript(std::string script, const Location& location) {
	auto chunk = worlds[location.getWorld()].getChunk(location.getChunk());
	if (chunk == nullptr) {
		return;
	}

	auto tileGroup = chunk->getTileGroup(location.getTileGroup());
	if (tileGroup == nullptr) {
		return;
	}

	tileGroup->setWalkedOnScript(std::move(script));
}

void Game::WorldManager::addPromptedScript(std::string script, const Location& location) {
	auto chunk = worlds[location.getWorld()].getChunk(location.getChunk());
	if (chunk == nullptr) {
		return;
	}

	auto tileGroup = chunk->getTileGroup(location.getTileGroup());
	if (tileGroup == nullptr) {
		return;
	}

	tileGroup->setPromptedScript(std::move(script));
}

void Game::WorldManager::addTileSwitchScript(std::string script, const Location& location) {
	auto chunk = worlds[location.getWorld()].getChunk(location.getChunk());
	if (chunk == nullptr) {
		return;
	}

	auto tileGroup = chunk->getTileGroup(location.getTileGroup());
	if (tileGroup == nullptr) {
		return;
	}

	tileGroup->setSwitchScript(std::move(script));
}

void Game::WorldManager::removeWalkedOnScript(const Location& location) {
	auto chunk = worlds[location.getWorld()].getChunk(location.getChunk());
	if (chunk == nullptr) {
		return;
	}

	auto tileGroup = chunk->getTileGroup(location.getTileGroup());
	if (tileGroup == nullptr) {
		return;
	}

	tileGroup->setWalkedOnScript("");
}

void Game::WorldManager::removePromptedScript(const Location& location) {
	auto chunk = worlds[location.getWorld()].getChunk(location.getChunk());
	if (chunk == nullptr) {
		return;
	}

	auto tileGroup = chunk->getTileGroup(location.getTileGroup());
	if (tileGroup == nullptr) {
		return;
	}

	tileGroup->setPromptedScript("");
}

void Game::WorldManager::removeTileSwitchScript(const Location& location) {
	auto chunk = worlds[location.getWorld()].getChunk(location.getChunk());
	if (chunk == nullptr) {
		return;
	}

	auto tileGroup = chunk->getTileGroup(location.getTileGroup());
	if (tileGroup == nullptr) {
		return;
	}

	tileGroup->setSwitchScript("");
}

bool Game::WorldManager::addTeleporterToFloorTile(const Location& teleporterLocation, const Location& teleporterDestination) {
	TileGroup* tileGroup = getTileGroup(teleporterLocation);
	if (tileGroup == nullptr) {
		return false;
	}

	Tile* tile = tileGroup->getTeleporterTile();
	if (tile == nullptr) {
		return false;
	}

	tile->createTeleporter(teleporterDestination);
	return true;
}

bool Game::WorldManager::addTeleporterToSwitchableFloorTile(const Location& teleporterLocation, const Location& teleporterDestination) {
	TileGroup* tileGroup = getTileGroup(teleporterLocation);
	if (tileGroup == nullptr) {
		return false;
	}

	Tile* tile = tileGroup->getSwitchableFloorTile();
	if (tile == nullptr) {
		return false;
	}

	tile->createTeleporter(teleporterDestination);
	return true;
}

bool Game::WorldManager::addTeleporterToTile(const Location& teleporterLocation, const Location& teleporterDestination, size_t tileType) {
	TileGroup* tileGroup = getTileGroup(teleporterLocation);
	if (tileGroup == nullptr) {
		return false;
	}

	Tile* tile = tileGroup->getTile(tileType);
	if (tile == nullptr) {
		return false;
	}

	tile->createTeleporter(teleporterDestination);
	return true;
}

void Game::WorldManager::runWalkedScriptOfTile(const Location& location) {
	size_t world = location.getWorld();
	if (world < worlds.size()) {
		Chunk* chunk = worlds[world].getChunk(location.getChunk());
		if (chunk != nullptr) {
			TileGroup* tileGroup = worlds[world].getChunk(location.getChunk())->getTileGroup(location.getTileGroup());
			if (tileGroup != nullptr) {
				scriptManager->runScript(*tileGroup->getWalkedOnScriptID());
			}
		}
	}
}

void Game::WorldManager::runPromptedScriptOfTile(const Location& location) {
	size_t world = location.getWorld();
	if (world < worlds.size()) {
		Chunk* chunk = worlds[world].getChunk(location.getChunk());
		if (chunk != nullptr) {
			TileGroup* tileGroup = worlds[world].getChunk(location.getChunk())->getTileGroup(location.getTileGroup());
			if (tileGroup != nullptr) {
				scriptManager->runScript(*tileGroup->getPromptedScriptID());
			}
		}
	}
}

void Game::WorldManager::runTileSwitchScriptOfTileGroup(const Location& location) {
	size_t world = location.getWorld();
	if (world < worlds.size()) {
		Chunk* chunk = worlds[world].getChunk(location.getChunk());
		if (chunk != nullptr) {
			TileGroup* tileGroup = worlds[world].getChunk(location.getChunk())->getTileGroup(location.getTileGroup());
			glm::ivec3 meme = location.getTileGroup();
			if (tileGroup != nullptr) {
				scriptManager->runScript(*tileGroup->getSwitchScript());
			}
		}
	}
}

void Game::WorldManager::rebuildMap() {
	mapRebuilder.rebuildMap(&worlds[playerCharacter->getLocation()->getWorld()]);
}

void Game::WorldManager::createChunkInCurrentWorld(glm::ivec2 coordinate) {
	auto chunk = Chunk();
	worlds[playerCharacter->getLocation()->getWorld()].addChunk(coordinate, &chunk);
}

void Game::WorldManager::getCoordinateOfNeighbouringTile(Location& location, TileDirection direction) {
	glm::vec3 tile = location.getTileGroup();
	glm::ivec2 chunk = location.getChunk();
	getCoordinateOfNeighbouringTile(tile, chunk, direction);
	location.setTileGroup(tile);
	location.setChunk(chunk);
}

void Game::WorldManager::createLayerInCurrentWorld(glm::ivec2 chunkCoordinate, unsigned int layer) {
	worlds[playerCharacter->getLocation()->getWorld()].getChunk(chunkCoordinate)->generateBlankTiles(layer);
}

bool Game::WorldManager::exportCurrentWorld() {
	return worldExporter->exportWorld(RESOURCE_ROOT + TILED_MAP_LOCATION + MAP_BEING_EDITED + ".txt", &worlds[playerCharacter->getLocation()->getWorld()]);
}

bool Game::WorldManager::exportCurrentWorld(std::string path) {
	return worldExporter->exportWorld(RESOURCE_ROOT + path, &worlds[playerCharacter->getLocation()->getWorld()]);
}

bool Game::WorldManager::autoExportCurrentWorld() {
	return worldExporter->exportWorld(RESOURCE_ROOT + WORLD_AUTO_EXPORT_PATH, &worlds[playerCharacter->getLocation()->getWorld()]);
}

void Game::WorldManager::tileWasPlaced(const Location& location, size_t tileType) {
	runTileSwitchScriptOfTileGroup(location);
	tileBehaviourExecutor->runBehaviour(location, tileType);
	mapNeedsToBeRebuilt = true;
}

void Game::WorldManager::saveDataToSaveState(SaveState* saveState) {
	World* world = (World*) saveState->getData("world");
	if (world != nullptr) {
		try {
			delete world;
		} catch (...) {
			// I don't even know.
		}
	}

	size_t id = GameObjectProvider::getPlayer()->getCharacter()->getLocation()->getWorld();
	auto pair = worlds.find(id);
	if (pair == worlds.end()) {
		return;
	}

	world = new World();
	(*world) = pair->second;
	saveState->addData("world", world);
}

void Game::WorldManager::loadDataFromSaveState(SaveState* saveState) {
	void* data = saveState->getData("world");
	if (data == nullptr) {
;		return;
	}
	World* world = static_cast<World*>(saveState->getData("world"));
	World world2 = *world;
	worlds[GameObjectProvider::getPlayer()->getCharacter()->getLocation()->getWorld()] = world2;
}
