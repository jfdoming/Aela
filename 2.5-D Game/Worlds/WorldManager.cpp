/*
* Class: WorldManager
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A class used to manage worlds.
*/

#include "WorldManager.h"
#include "3D\Maps\Map3DLoader.h"
// I'm including this because it contains some useful defines.
#include "../Scripts/Scripts to Move to LUA/SceneScript.h"
#include <glm/gtc/constants.hpp>

bool Game::WorldManager::setup(ResourceManager* resourceManager, GLRenderer* renderer, Animator* animator,
	ScriptManager* scriptManager, DialogueHandler* dialogueHandler, Character* player) {
	this->resourceManager = resourceManager;
	this->renderer = renderer;
	this->scriptManager = scriptManager;
	this->dialogueHandler = dialogueHandler;
	this->player = player;

	bool success = resourceManager->obtain<Map3D>(mapFileLocation, map);
	if (!success) {
		// Is it even possible to get here if the map managed to load in the first place?
		AelaErrorHandling::windowError("World Manager", "The World Manager could not obtain this vital file: "
			+ mapFileLocation);
		return false;
	}

	characterManager.setup(resourceManager, animator, scriptManager);
	rebuildMap();
	return true;
}

void Game::WorldManager::update() {
	if (characterManager.doesMapNeedToBeRebuilt()) {
		rebuildMap();
		characterManager.mapWasRebuilt();
	}
	characterManager.update();
}

Game::TileAtlas* Game::WorldManager::getTileAtlas() {
	return &tileAtlas;
}

Game::CharacterManager* Game::WorldManager::getCharacterManager() {
	return &characterManager;
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

bool Game::WorldManager::setCurrentWorld(size_t id) {
	if (id < worlds.size()) {
		currentWorld = id;
		return true;
	}
	return false;
}

size_t Game::WorldManager::getCurrentWorld() {
	return currentWorld;
}

void Game::WorldManager::setChunkRenderDistances(glm::vec3 chunkRenderDistances) {
	this->chunkRenderDistances = chunkRenderDistances;
	rebuildMap();
}

bool Game::WorldManager::moveCharacterIfPossible(Character* character, TileDirection direction) {
	Location location = *character->getLocation();
	glm::vec2 chunkCoord = location.getChunk();
	glm::vec3 tileCoord = location.getTile();
	switch (direction) {
		case TileDirection::RIGHT:
			tileCoord += glm::ivec3(-1, 0, 0);
			break;
		case TileDirection::FORWARD:
			tileCoord += glm::ivec3(0, 0, 1);
			break;
		case TileDirection::LEFT:
			tileCoord += glm::ivec3(1, 0, 0);
			break;
		case TileDirection::BACKWARD:
			tileCoord += glm::ivec3(0, 0, -1);
			break;
	}
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
	Tile* tile = worlds[location.getWorld()].getChunk(location.getChunk())->getTile(location.getTile());
	if (tile == nullptr || tileAtlas.getTileType(tile->getType())->isCollidable()
		|| characterManager.getCharacterByLocation(&location) != nullptr) {
		characterManager.turn(character, direction);
		return false;
	}
	dialogueHandler->closeDialog();
	std::string script = *tile->getWalkedOnScriptID();
	characterManager.move(character, direction, script);
	unsigned int x = tile->getType();
	return true;
}

bool Game::WorldManager::moveCharacterIfPossible(size_t id, TileDirection direction) {
	return moveCharacterIfPossible(characterManager.getCharacterByID(id), direction);
}

bool Game::WorldManager::moveCharacterIfPossible(std::string name, TileDirection direction) {
	return moveCharacterIfPossible(characterManager.getCharacterByName(name), direction);
}

void Game::WorldManager::addWalkedOnScript(std::string script, Location* location) {
	worlds[location->getWorld()].getChunk(location->getChunk())->getTile(location->getTile())->setWalkedOnScript(script);
}

void Game::WorldManager::addPromptedScript(std::string script, Location* location) {
	worlds[location->getWorld()].getChunk(location->getChunk())->getTile(location->getTile())->setPromptedScript(script);
}

void Game::WorldManager::rebuildMap() {
	// Clean the map here (by removing entities and such).
	map->removeAllModels();

	// This sets the loaded Map3D back up. Note that the Map3D's file is nearly blank.
	for (auto chunkPair : *worlds[currentWorld].getChunks()) {
		Chunk* chunk = worlds[currentWorld].getChunk(glm::ivec2(chunkPair.first.x, chunkPair.first.y));

		if (chunk != nullptr) {
			glm::ivec2 chunkPositionOnMap;
			chunkPositionOnMap.x = ((-chunkRenderDistances.x) + chunkPair.first.x) * CHUNK_WIDTH;
			chunkPositionOnMap.y = ((-chunkRenderDistances.z) + chunkPair.first.y) * CHUNK_LENGTH;

			// In addition to adding tiles to the world, characters must also be added.
			auto charactersPointer = characterManager.getCharactersInChunk(currentWorld, chunkPair.first);
			if (charactersPointer != nullptr) {
				auto characters = *charactersPointer;
				for (auto iter : characters) {
					Character* character = characterManager.getCharacterByID(iter.second);

					// Note that characters are represented by models.
					ModelEntity modelEntity;
					size_t entityInMap;
					glm::ivec3 tileOfChunk = character->getLocation()->getTile();

					// The position of the character on the map. The distance the character is translated up from its
					// tileCoord is sin(PI/6) * 0.5 + 0.05;
					glm::vec3 characterPositionOnMap((float) (chunkPositionOnMap.x + tileOfChunk.x),
						(float) (tileOfChunk.y + 0.3f), (float) (chunkPositionOnMap.y + tileOfChunk.z));

					modelEntity.setPosition(characterPositionOnMap);
					modelEntity.setRotation((float) ELEVEN_SIXTHS_PI, 0, 0);
					modelEntity.setModel(character->getModel());
					entityInMap = map->getModels()->size();
					map->addModelWithTransparency(entityInMap, &modelEntity);
					character->setEntity(map->getModel(entityInMap));
				}
			}

			size_t counter = 0;
			for (auto tilePair : *chunk->getTiles()) {
				Tile* tile = chunk->getTile(tilePair.first);

				// Note: a tileCoord type of zero signifies a blank space.
				if (tile != nullptr && tile->getType() != 0) {
					// Note that tiles are represented by models.
					ModelEntity modelEntity;
					size_t entityInMap;
					glm::vec3 tilePositionOnMap;
					std::string textureName;

					tilePositionOnMap.x = (float) (chunkPositionOnMap.x + tilePair.first.x);
					tilePositionOnMap.y = (float) tilePair.first.y;
					tilePositionOnMap.z = (float) (chunkPositionOnMap.y + tilePair.first.z);

					switch (tileAtlas.getTileType(tile->getType())->getBehaviour()) {
						case TileBehaviour::FLOOR:
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_RIGHT:
							tilePositionOnMap.y += 0.5f;
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation((float) -QUARTER_PI, (float) THREE_HALVES_PI, 0);
							modelEntity.setScaling((float) ROOT_OF_TWO, (float) ROOT_OF_TWO, 1);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_UP:
							tilePositionOnMap.y += 0.5f;
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation((float) -QUARTER_PI, 0, 0);
							modelEntity.setScaling(1, (float) ROOT_OF_TWO, (float) ROOT_OF_TWO);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_LEFT:
							tilePositionOnMap.y += 0.5f;
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation((float) -QUARTER_PI, (float) HALF_PI, 0);
							modelEntity.setScaling((float) ROOT_OF_TWO, (float) ROOT_OF_TWO, 1);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_DOWN:
							tilePositionOnMap.y += 0.5f;
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation((float) -QUARTER_PI, (float) PI, 0);
							modelEntity.setScaling(1, (float) ROOT_OF_TWO, (float) ROOT_OF_TWO);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_UP_RIGHT_DEPRESSED:
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation(0, 0, 0);
							modelEntity.setScaling(1, 1, 1);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_UP_LEFT_DEPRESSED:
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation(0, (float) HALF_PI, 0);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_DOWN_LEFT_DEPRESSED:
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation(0, (float) PI, 0);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_DOWN_RIGHT_DEPRESSED:
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation(0, (float) THREE_HALVES_PI, 0);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_UP_RIGHT_ELEVATED:
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation(0, 0, 0);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_UP_LEFT_ELEVATED:
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation(0, (float) HALF_PI, 0);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_DOWN_LEFT_ELEVATED:
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation(0, (float) PI, 0);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						case TileBehaviour::RAMP_DOWN_RIGHT_ELEVATED:
							modelEntity.setPosition(tilePositionOnMap);
							modelEntity.setRotation(0, (float) THREE_HALVES_PI, 0);
							modelEntity.setModel(tileAtlas.getTileModel(tile->getType()));
							entityInMap = map->getModels()->size();
							map->addModel(entityInMap, &modelEntity);
							break;
						default:
							// This type is unknown.
							continue;
					}

					tile->setEntity(map->getModel(entityInMap));
				}
			}
		}
	}
}
