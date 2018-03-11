/*
* Class: WorldManager
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A class used to manage worlds.
*/

#include "WorldManager.h"
#include "3D/Maps/Map3DLoader.h"
// I'm including this because it contains some useful defines.
#include "../Scripts/Scripts to Move to LUA/SceneScript.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include <glm/gtc/constants.hpp>

bool Game::WorldManager::setup(Engine* engine, ScriptManager* scriptManager, DialogueHandler* dialogueHandler, Character* player) {
	this->resourceManager = engine->getResourceManager();
	this->renderer = engine->getRenderer();
	this->animator = engine->getAnimator();
	this->animationLooper = engine->getAnimationLooper();
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

	characterManager.setup(engine, scriptManager);
	rebuildMap();
	setupAnimationLoopingForTiles();
	
	return true;
}

void Game::WorldManager::setupAnimationLoopingForTiles() {
	// The following code is an example for a simple water animation!
	AnimationTrackMaterial track;
	KeyFrameMaterial frame0, frame1;
	Texture* texture0 = nullptr, *texture1 = nullptr;
	Material* material0 = nullptr;
	
	if (!resourceManager->obtain<Texture>(DEFAULT_MATERIAL_PATH + (std::string) "water_0.dds", texture0)
		|| !resourceManager->obtain<Texture>(DEFAULT_MATERIAL_PATH + (std::string) "water_1.dds", texture1)) {
		return;
	}

	if (!resourceManager->obtain<Material>("water_0", material0)) {
		std::cout << DEFAULT_MATERIAL_PATH + (std::string) "water_0.mtl failed.\n";
		return;
	}

	frame0.setTexture(texture0);
	frame1.setTexture(texture1);
	frame0.setMaterial(material0);
	frame1.setMaterial(material0);

	track.addKeyFrameUsingSeconds(1, &frame0);
	track.addKeyFrameUsingSeconds(1, &frame1);
	
	animationLooper->loopAnimation(&track);
}

void Game::WorldManager::update() {
	if (mapNeedsToBeRebuilt || characterManager.doesMapNeedToBeRebuilt()) {
		rebuildMap();
		mapNeedsToBeRebuilt = false;
		characterManager.mapWasRebuilt();
	}

	for (size_t i = 0; i < characterMovementQueueByID.size(); i++) {
		auto& pair = characterMovementQueueByID.at(i);
		Character* character = characterManager.getCharacterByID(pair.first);
		if (!character->isMoving()) {
			processCharacterMovements(character, pair.second);
			characterMovementQueueByID.erase(characterMovementQueueByID.begin() + i);
			i--;
		}
	}


	for (size_t i = 0; i < characterMovementQueueByName.size(); i++) {
		auto& pair = characterMovementQueueByName.at(i);
		Character* character = characterManager.getCharacterByName(pair.first);
		if (!character->isMoving()) {
			processCharacterMovements(character, pair.second);
			characterMovementQueueByName.erase(characterMovementQueueByName.begin() + i);
			i--;
		}
	}

	characterManager.update();
}

void Game::WorldManager::rebuildMapWhenPossible() {
	mapNeedsToBeRebuilt = true;
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
	worlds[location->getWorld()].getChunk(location->getChunk())->getTile(location->getTile())->setWalkedOnScript(script);
}

void Game::WorldManager::addPromptedScript(std::string script, Location* location) {
	worlds[location->getWorld()].getChunk(location->getChunk())->getTile(location->getTile())->setPromptedScript(script);
}

void Game::WorldManager::runPromptedScriptOfTile(Location* location) {
	unsigned int world = location->getWorld();
	if (world < worlds.size()) {
		Chunk* chunk = worlds[world].getChunk(location->getChunk());
		if (chunk != nullptr) {
			Tile* tile = worlds[world].getChunk(location->getChunk())->getTile(location->getTile());
			if (tile != nullptr) {
				scriptManager->runScript(*tile->getPromptedScriptID());
			}
		}
	}
}

void Game::WorldManager::processCharacterMovements(Character* character, TileDirection& direction) {
	if (direction != character->getDirectionFacing()) {
		characterManager.turn(character, direction);
		return;
	}
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
	if (location.getWorld() < worlds.size()) {
		Chunk* chunk = worlds[location.getWorld()].getChunk(location.getChunk());
		if (chunk != nullptr) {
			Tile* tile = worlds[location.getWorld()].getChunk(location.getChunk())->getTile(location.getTile());
			if (tile == nullptr || tileAtlas.getTileType(tile->getType())->isCollidable()
				|| characterManager.getCharacterByLocation(&location) != nullptr) {
				// If the movement is impossible, don't do it!
				characterManager.turn(character, direction);
				return;
			}
			std::string script = *tile->getWalkedOnScriptID();
			characterManager.move(character, direction, script);
		}
	}
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

					// The position of the character on the map. The distance the character is character->getModeltranslated up from its
					// tileCoord is sin(PI/6) * 0.5 + 0.05;
					glm::vec3 characterPositionOnMap((float) (chunkPositionOnMap.x + tileOfChunk.x),
						(float) (tileOfChunk.y + 0.3f), (float) (chunkPositionOnMap.y + tileOfChunk.z));

					modelEntity.setPosition(characterPositionOnMap);
					modelEntity.setRotation((float) ELEVEN_SIXTHS_PI, 0, 0);
					modelEntity.setModel(character->getModel());
					entityInMap = map->getModels()->size();
					map->addModelWithTransparency(entityInMap, &modelEntity);
					ModelEntity* newEntity = map->getModel(entityInMap);
					character->setEntity(newEntity);

					// Make sure to update the character's animation's ModelEntity pointer.
					AnimationTrack3D* track3D = animator->get3DTrack(character->getName() + "_mv");
					if (track3D != nullptr) {
						for (auto& pair : *track3D->getKeyFrames()) {
							pair.second.setObject(newEntity);
						}
					}

					AnimationTrackModel* trackModel = animator->getModelTrack(character->getName() + "_st");
					if (trackModel != nullptr) {
						for (auto& pair : *trackModel->getKeyFrames()) {
							pair.second.setModelEntity(newEntity);
						}
					}
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

	// The animator must be updated in order to make sure that entity transformations are correct.
	animator->update();
}
