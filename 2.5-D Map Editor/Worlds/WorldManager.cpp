/*
* Class: WorldManager
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A class used to manage worlds.
*/

#include "WorldManager.h"
#include "../../Project Aela/3D/Maps/Map3DLoader.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include "../Resources/ResourceInfo.h"
#include <glm/gtc/constants.hpp>

// I'm including this because it contains some useful defines.
#include "../Scripts/Scripts to Move to LUA/SceneScript.h"

bool Game::WorldManager::setup(Engine* engine, ScriptManager* scriptManager, DialogueHandler* dialogueHandler, Character* player) {
	this->resourceManager = engine->getResourceManager();
	this->renderer = engine->getRenderer();
	this->animator = engine->getAnimator();
	this->animationLooper = engine->getAnimationLooper();
	this->scriptManager = scriptManager;
	this->dialogueHandler = dialogueHandler;
	this->player = player;

	characterTracker.setup(engine, scriptManager);

	mapRebuilder.setAnimator(animator);
	mapRebuilder.setTileAtlas(&tileAtlas);
	mapRebuilder.bindMap(map);
	mapRebuilder.setPlayerCharacter(player);

	glm::ivec2 chunkCoord = player->getLocation()->getChunk();
	glm::ivec3 tileCoord = player->getLocation()->getTileGroup();
	coordinateLabel->setText("Chunk: (" + std::to_string(chunkCoord.x) + ", " + std::to_string(chunkCoord.y)
		+ ") Tile: (" + std::to_string(tileCoord.x) + ", " + std::to_string(tileCoord.y) + ", " + std::to_string(tileCoord.z) + ")");

	setupAnimationLoopingForTiles();
	
	return true;
}

void Game::WorldManager::setupAnimationLoopingForTiles() {
	// The following code is an example for a simple water animation!
	AnimationTrackMaterial track0, track1;
	KeyFrameMaterial frame0, frame1, frame2, frame3;
	Texture* texture0 = nullptr, *texture1 = nullptr;
	Material* material0 = nullptr, *material1 = nullptr;

	if (!resourceManager->obtain<Texture>((std::string) DEFAULT_MATERIAL_PATH + (std::string) "water_0.dds", texture0)
		|| !resourceManager->obtain<Texture>((std::string) DEFAULT_MATERIAL_PATH + (std::string) "water_1.dds", texture1)) {
		AelaErrorHandling::consoleWindowError("World Manager", "Could not load some textures for some tile animations.");
		return;
	}

	if (!resourceManager->obtain<Material>((std::string) DEFAULT_MATERIAL_PATH + "water_0.mtl/water_0", material0)
		|| !resourceManager->obtain<Material>((std::string) DEFAULT_MATERIAL_PATH + "water_1.mtl/water_1", material1)) {
		AelaErrorHandling::consoleWindowError("World Manager", "Could not load some materials for some tile animations.");
		std::cout << DEFAULT_MATERIAL_PATH + (std::string) "water_0.mtl/water_0 failed.\n";
		return;
	}

	frame0.setTexture(texture0);
	frame1.setTexture(texture1);
	frame0.setMaterial(material0);
	frame1.setMaterial(material0);

	track0.addKeyFrameUsingSeconds(1, &frame0);
	track0.addKeyFrameUsingSeconds(1, &frame1);

	animationLooper->loopAnimation(&track0);

	frame2.setTexture(texture1);
	frame3.setTexture(texture0);
	frame2.setMaterial(material1);
	frame3.setMaterial(material1);

	track1.addKeyFrameUsingSeconds(1, &frame1);
	track1.addKeyFrameUsingSeconds(1, &frame0);

	animationLooper->loopAnimation(&track1);
}

void Game::WorldManager::update() {
	if (mapNeedsToBeRebuilt || characterTracker.doesMapNeedToBeRebuilt()) {
		rebuildMap();
		mapNeedsToBeRebuilt = false;
		characterTracker.mapWasRebuilt();
	}

	for (size_t i = 0; i < characterMovementQueueByID.size(); i++) {
		auto& pair = characterMovementQueueByID.at(i);
		Character* character = characterTracker.getCharacterByID(pair.first);
		if (character->isFrozen()) {
			characterMovementQueueByID.erase(characterMovementQueueByID.begin() + i);
			i--;
		} else if (!character->isMoving()) {
			processCharacterMovement(character, pair.second);
			characterMovementQueueByID.erase(characterMovementQueueByID.begin() + i);
			i--;
		}
	}


	for (size_t i = 0; i < characterMovementQueueByName.size(); i++) {
		auto& pair = characterMovementQueueByName.at(i);
		Character* character = characterTracker.getCharacterByName(pair.first);
		if (character->isFrozen()) {
			characterMovementQueueByID.erase(characterMovementQueueByID.begin() + i);
			i--;
		} if (!character->isMoving()) {
			processCharacterMovement(character, pair.second);
			characterMovementQueueByName.erase(characterMovementQueueByName.begin() + i);
			i--;
		}
	}

	characterTracker.update();
}

void Game::WorldManager::rebuildMapWhenPossible() {
	mapNeedsToBeRebuilt = true;
}

Game::TileAtlas* Game::WorldManager::getTileAtlas() {
	return &tileAtlas;
}

Game::CharacterTracker* Game::WorldManager::getCharacterTracker() {
	return &characterTracker;
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

Map3D* Game::WorldManager::getMap3D() {
	return map;
}

bool Game::WorldManager::setCurrentWorld(size_t id) {
	if (id < worlds.size()) {
		currentWorld = id;
		map = worlds[currentWorld].getMap3D();
		mapRebuilder.bindMap(map);
		rebuildMap();
		return true;
	}
	return false;
}

size_t Game::WorldManager::getCurrentWorld() {
	return currentWorld;
}

Game::Teleporter* Game::WorldManager::getTeleporter(Location* location) {
	auto iter1 = teleporters.find(location->getWorld());
	if (iter1 == teleporters.end()) {
		return nullptr;
	}
	auto iter2 = iter1->second.find(location->getChunk());
	if (iter2 == iter1->second.end()) {
		return nullptr;
	}
	auto iter3 = iter2->second.find(location->getTileGroup());
	if (iter3 == iter2->second.end()) {
		return nullptr;
	}
	return &iter3->second;
}

void Game::WorldManager::setChunkRenderDistances(glm::vec3 chunkRenderDistances) {
	mapRebuilder.setChunkRenderingDistances(chunkRenderDistances);
	rebuildMapWhenPossible();
}

void Game::WorldManager::setGameplayScene(Scene* gameplayScene) {
	characterTracker.setGameplayScene(gameplayScene);
}

void Game::WorldManager::getCoordinateOfNeighbouringTile(glm::vec3& tile, glm::vec2& chunk, TileDirection direction) {
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
				tile.z = CHUNK_LENGTH;
				chunk.y--;
			}
			break;
	}
}

void Game::WorldManager::moveCharacterIfPossible(size_t id, TileDirection direction) {
	characterMovementQueueByID.push_back(std::pair<size_t, TileDirection>(id, direction));
}

void Game::WorldManager::moveCharacterIfPossible(std::string name, TileDirection direction) {
	characterMovementQueueByName.push_back(std::pair<std::string, TileDirection>(name, direction));
}

void Game::WorldManager::moveCharacterIfPossible(size_t id, std::vector<TileDirection> directions) {
	for (auto direction : directions) {
		characterMovementQueueByID.push_back(std::pair<size_t, TileDirection>(id, direction));
	}
}

void Game::WorldManager::moveCharacterIfPossible(std::string name, std::vector<TileDirection> directions) {
	for (auto direction : directions) {
		characterMovementQueueByName.push_back(std::pair<std::string, TileDirection>(name, direction));
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

void Game::WorldManager::addTeleporter(Teleporter* teleporter, Location* location) {
	teleporters[location->getWorld()][location->getChunk()][location->getTileGroup()] = *teleporter;
}

void Game::WorldManager::runPromptedScriptOfTile(Location* location) {
	unsigned int world = location->getWorld();
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

void Game::WorldManager::runTileSwitchScriptOfTile(Location* location) {
	unsigned int world = location->getWorld();
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

void Game::WorldManager::setCoordinateLabel(std::shared_ptr<Label> coordinateLabel) {
	this->coordinateLabel = coordinateLabel;
}

void Game::WorldManager::createChunkInCurrentWorld(glm::ivec2 coordinate) {
	worlds[currentWorld].addChunk(coordinate, &Chunk());
}

void Game::WorldManager::createLayerInCurrentWorld(glm::ivec2 chunkCoordinate, unsigned int layer) {
	worlds[currentWorld].getChunk(chunkCoordinate)->generateBlankTiles(layer);
}

bool Game::WorldManager::exportCurrentWorld() {
	return worldExporter.exportWorld((std::string) RESOURCE_ROOT + MAP_BEING_EDITED, &worlds[currentWorld]);
}

void Game::WorldManager::processCharacterMovement(Character* character, TileDirection& direction) {
	if (direction != character->getDirectionFacing()) {
		characterTracker.turnTrackedCharacter(character, direction);
		return;
	}
	Location location = *character->getLocation();
	glm::ivec2 chunkCoord = location.getChunk();
	glm::ivec3 tileCoord = location.getTileGroup();
	glm::vec3 translation;

	switch (direction) {
		case TileDirection::RIGHT:
			translation = glm::vec3(-1, 0, 0);
			break;
		case TileDirection::FORWARD:
			translation = glm::vec3(0, 0, 1);
			break;
		case TileDirection::LEFT:
			translation = glm::vec3(1, 0, 0);
			break;
		case TileDirection::BACKWARD:
			translation = glm::vec3(0, 0, -1);
			break;
		case TileDirection::UP:
			translation = glm::vec3(0, 1, 0);
			break;
		case TileDirection::DOWN:
			translation = glm::vec3(0, -1, 0);
			break;
	}

	tileCoord += translation;

	if (tileCoord.x < 0) {
		tileCoord.x = CHUNK_WIDTH - 1;
		chunkCoord.x--;
	}
	if (tileCoord.x == CHUNK_WIDTH) {
		tileCoord.x = 0;
		chunkCoord.x++;
	}
	if (tileCoord.z < 0) {
		tileCoord.z = CHUNK_LENGTH - 1;
		chunkCoord.y--;
	}
	if (tileCoord.z == CHUNK_LENGTH) {
		tileCoord.z = 0;
		chunkCoord.y++;
	}

	location.setChunk(chunkCoord);
	location.setTile(tileCoord);

	if (location.getWorld() < worlds.size()) {
		Chunk* chunk = worlds[location.getWorld()].getChunk(location.getChunk());
		if (chunk != nullptr) {
			TileGroup* tileGroup = worlds[location.getWorld()].getChunk(location.getChunk())->getTileGroup(location.getTileGroup());
			if (tileGroup == nullptr /*|| type->isCollidable()*/ || characterTracker.getCharacterByLocation(&location) != nullptr) {
				// If the movement is impossible, don't do it!
				// characterTracker.turnTrackedCharacter(character, direction);

				if (character->getName() == PLAYER_NAME) {
					coordinateLabel->setText("(Does not exist yet) Chunk: (" + std::to_string(chunkCoord.x) + ", " + std::to_string(chunkCoord.y)
						+ ") Tile: (" + std::to_string(tileCoord.x) + ", " + std::to_string(tileCoord.y) + ", " + std::to_string(tileCoord.z) + ")");
				}

				// return;
			} else if (character->getName() == PLAYER_NAME) {
				coordinateLabel->setText("Chunk: (" + std::to_string(chunkCoord.x) + ", " + std::to_string(chunkCoord.y)
					+ ") Tile: (" + std::to_string(tileCoord.x) + ", " + std::to_string(tileCoord.y) + ", " + std::to_string(tileCoord.z) + ")");
			}

			// std::string script = *tile->getWalkedOnScriptID();
			Movement movement(&location, &translation, direction, false);
			characterTracker.moveTrackedCharacter(character, &movement, "");

			/*Teleporter* teleporter = getTeleporter(&location);
			if (teleporter != nullptr) {
				character->allowNewMovements(true);
				Movement teleportation(teleporter->getDestination(), &glm::vec3(), player->getDirectionFacing(), true);
				characterTracker.moveTrackedCharacter(character, &teleportation, "");
			}*/
		} else {
			if (character->getName() == PLAYER_NAME) {
				coordinateLabel->setText("(Does not exist yet) Chunk: (" + std::to_string(chunkCoord.x) + ", " + std::to_string(chunkCoord.y)
					+ ") Tile: (" + std::to_string(tileCoord.x) + ", " + std::to_string(tileCoord.y) + ", " + std::to_string(tileCoord.z) + ")");
			}

			std::cout << "NOPE\n";
			Movement movement(&location, &translation, direction, false);
			characterTracker.moveTrackedCharacter(character, &movement, "");
		}
	}
}

void Game::WorldManager::rebuildMap() {
	mapRebuilder.rebuildMap(&worlds[currentWorld], currentWorld, &characterTracker);
}
