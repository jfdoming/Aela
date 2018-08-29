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
#include "Resource Management/ResourceManager.h"
#include "../Scripts/ScriptManager.h"
#include "3D/Maps/Map3DLoader.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include "../Player/Player.h"
#include "../Tiles/TileBehaviourExecuter.h"
#include "../Aela Game/AelaGame.h"
#include <glm/gtc/constants.hpp>
#include <iostream>

// I'm including this because it contains some useful defines.
#include "../Scripts/Scripts to Move to LUA/SceneScript.h"

Game::WorldManager::WorldManager() {
	resourceManager = GameObjectProvider::getResourceManager();
	animationLooper = GameObjectProvider::getAnimationLooper();
}

void Game::WorldManager::setup() {
	characterProvider = GameObjectProvider::getCharacterProvider();
	tileBehaviourExecuter = GameObjectProvider::getTileBehaviourExecuter();
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

size_t Game::WorldManager::addWorld(World* world) {
	worlds.push_back(*world);
	return worlds.size() - 1;
}

Game::World* Game::WorldManager::getWorld(size_t id) {
	if (id < worlds.size()) {
		return &worlds[id];
	}
	return nullptr;
}

size_t Game::WorldManager::getNumberOfWorlds() {
	return worlds.size();
}

Map3D* Game::WorldManager::getMap3D() {
	return worlds[playerCharacter->getLocation()->getWorld()].getMap3D();
}

Game::Teleporter* Game::WorldManager::getTeleporter(Location* location) {
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

Game::Teleporter* Game::WorldManager::getTeleporter(Location* location, size_t tileType) {
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

Game::Chunk* Game::WorldManager::getChunk(Location* location) {
	size_t worldID = location->getWorld();
	if (worldID >= worlds.size()) {
		return nullptr;
	}

	World* world = &worlds[worldID];
	return world->getChunk(location->getChunk());
}

Game::TileGroup* Game::WorldManager::getTileGroup(Location* location) {
	Chunk* chunk = getChunk(location);
	if (chunk == nullptr) {
		return nullptr;
	}
	return chunk->getTileGroup(location->getTileGroup());
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

void Game::WorldManager::getCoordinateOfNeighbouringTile(glm::ivec3& tile, glm::ivec2& chunk, TileDirection direction) {
	switch (direction) {
		case TileDirection::RIGHT:
			tile += glm::ivec3(-1, 0, 0);
			if (tile.x == -1) {
				tile.x = CHUNK_WIDTH - 1;
				chunk.x--;
			}
			break;
		case TileDirection::FORWARD:
			tile += glm::ivec3(0, 0, 1);
			if (tile.z == CHUNK_LENGTH) {
				tile.z = 0;
				chunk.y++;
			}
			break;
		case TileDirection::LEFT:
			tile += glm::ivec3(1, 0, 0);
			if (tile.x == CHUNK_WIDTH) {
				tile.x = 0;
				chunk.x++;
			}
			break;
		case TileDirection::BACKWARD:
			tile += glm::ivec3(0, 0, -1);
			if (tile.z == -1) {
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

void Game::WorldManager::addWalkedOnScript(std::string script, Location* location) {
	worlds[location->getWorld()].getChunk(location->getChunk())->getTileGroup(location->getTileGroup())->setWalkedOnScript(script);
}

void Game::WorldManager::addPromptedScript(std::string script, Location* location) {
	worlds[location->getWorld()].getChunk(location->getChunk())->getTileGroup(location->getTileGroup())->setPromptedScript(script);
}

void Game::WorldManager::addTileSwitchScript(std::string script, Location* location) {
	worlds[location->getWorld()].getChunk(location->getChunk())->getTileGroup(location->getTileGroup())->setSwitchScript(script);
}

void Game::WorldManager::removeWalkedOnScript(Location* location) {
	worlds[location->getWorld()].getChunk(location->getChunk())->getTileGroup(location->getTileGroup())->setWalkedOnScript("");
}

void Game::WorldManager::removePromptedScript(Location* location) {
	worlds[location->getWorld()].getChunk(location->getChunk())->getTileGroup(location->getTileGroup())->setPromptedScript("");
}

void Game::WorldManager::removeTileSwitchScript(Location* location) {
	worlds[location->getWorld()].getChunk(location->getChunk())->getTileGroup(location->getTileGroup())->setSwitchScript("");
}

bool Game::WorldManager::addTeleporterToFloorTile(Location* teleporterLocation, Location* teleporterDestination) {
	TileGroup* tileGroup = getTileGroup(teleporterLocation);
	if (tileGroup == nullptr) {
		return false;
	}

	Tile* tile = tileGroup->getFloorTile(tileAtlas);
	if (tile == nullptr) {
		return false;
	}

	tile->createTeleporter(teleporterDestination);
	return true;
}

bool Game::WorldManager::addTeleporterToSwitchableFloorTile(Location* teleporterLocation, Location* teleporterDestination) {
	TileGroup* tileGroup = getTileGroup(teleporterLocation);
	if (tileGroup == nullptr) {
		return false;
	}

	Tile* tile = tileGroup->getSwitchableFloorTile(tileAtlas);
	if (tile == nullptr) {
		return false;
	}

	tile->createTeleporter(teleporterDestination);
	return true;
}

bool Game::WorldManager::addTeleporterToTile(Location* teleporterLocation, Location* teleporterDestination, size_t tileType) {
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

void Game::WorldManager::runPromptedScriptOfTile(Location* location) {
	size_t world = location->getWorld();
	if (world < worlds.size()) {
		Chunk* chunk = worlds[world].getChunk(location->getChunk());
		if (chunk != nullptr) {
			TileGroup* tileGroup = worlds[world].getChunk(location->getChunk())->getTileGroup(location->getTileGroup());
			if (tileGroup != nullptr) {
				scriptManager->runScript(*tileGroup->getPromptedScriptID());
			}
		}
	}
}

void Game::WorldManager::runTileSwitchScriptOfTileGroup(Location* location) {
	size_t world = location->getWorld();
	if (world < worlds.size()) {
		Chunk* chunk = worlds[world].getChunk(location->getChunk());
		if (chunk != nullptr) {
			TileGroup* tileGroup = worlds[world].getChunk(location->getChunk())->getTileGroup(location->getTileGroup());
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
	worlds[playerCharacter->getLocation()->getWorld()].addChunk(coordinate, &Chunk());
}

void Game::WorldManager::createLayerInCurrentWorld(glm::ivec2 chunkCoordinate, unsigned int layer) {
	worlds[playerCharacter->getLocation()->getWorld()].getChunk(chunkCoordinate)->generateBlankTiles(layer);
}

bool Game::WorldManager::exportCurrentWorld() {
	return worldExporter->exportWorld((std::string) RESOURCE_ROOT + MAP_BEING_EDITED, &worlds[playerCharacter->getLocation()->getWorld()]);
}

bool Game::WorldManager::exportCurrentWorld(std::string path) {
	return worldExporter->exportWorld((std::string) RESOURCE_ROOT + path, &worlds[playerCharacter->getLocation()->getWorld()]);
}

bool Game::WorldManager::autoExportCurrentWorld() {
	return worldExporter->exportWorld((std::string) RESOURCE_ROOT + WORLD_AUTO_EXPORT_PATH, &worlds[playerCharacter->getLocation()->getWorld()]);
}

void Game::WorldManager::tileWasPlaced(Location* location, size_t tileType) {
	runTileSwitchScriptOfTileGroup(location);
	tileBehaviourExecuter->runBehaviour(location, tileType);
	mapNeedsToBeRebuilt = true;
}

void Game::WorldManager::saveDataToSaveState(SaveState* saveState) {
	World* world = new World();
	(*world) = worlds[GameObjectProvider::getPlayer()->getCharacter()->getLocation()->getWorld()];
	saveState->addData("world", world);
}

void Game::WorldManager::loadDataFromSaveState(SaveState* saveState) {
	worlds[GameObjectProvider::getPlayer()->getCharacter()->getLocation()->getWorld()] = *((World*) saveState->getData("world"));
}
