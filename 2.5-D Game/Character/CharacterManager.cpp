/*
* Class: Character Manager
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage characters.
*/

#include "stdafx.h"
#include "CharacterManager.h"
#include "CharacterLoader.h"
#include "Resource Management/ResourcePaths.h"

using namespace Game;

void Game::CharacterManager::setup(ResourceManager* resourceManager, Animator* animator, ScriptManager* scriptManager) {
	this->resourceManager = resourceManager;
	this->animator = animator;
	this->scriptManager = scriptManager;
}

void Game::CharacterManager::update() {
	for (Character& character : characters) {
		if (!character.isMoving()) {
			std::pair<glm::vec3, std::string>* translation = character.getNextTranslation();
			if (translation != nullptr) {
				animateCharacterMovement(&character, translation->first, translation->second);
				character.removeNextTranslation();
			}
		} else if (!animator->trackWithTagExists(character.getName() + "_movement")) {
			std::pair<glm::vec3, std::string>* translation = character.getNextTranslation();
			if (translation != nullptr) {
				animateCharacterMovement(&character, translation->first, translation->second);
				character.removeNextTranslation();
			} else {
				character.moving = false;
			}
		}
	}
}

void Game::CharacterManager::generateCharacterModels(ResourceManager* resourceManager) {
	CharacterLoader loader;
	loader.setCharactersToLoad(&characters);
	loader.setResourceManager(resourceManager);
	resourceManager->bindLoader(&loader);
	resourceManager->bindGroup("characters");
	resourceManager->addToGroup(DEFAULT_MODEL_PATH + (std::string) "floor.obj", false);

	if (resourceManager->loadGroup("characters") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"characters\": " << resourceManager->getNewInvalidResourceKeys()[0] << "\n";
	}
}

bool Game::CharacterManager::addCharacter(Character* character, unsigned long long* id) {
	if (getCharacterByName(character->getName()) != nullptr || getCharacterByLocation(character->getLocation()) != nullptr) {
		std::cout << getCharacterByName(character->getName()) << " " << getCharacterByLocation(character->getLocation()) << "\n";
		return false;
	}
	Location* location = character->getLocation();
	characters.push_back(*character);
	charactersByName[character->getName()] = characters.size() - 1;
	charactersByLocation[location->getWorld()][location->getChunk()][location->getTile()] = characters.size() - 1;
	*id = characters.size() - 1;
	return true;
}

bool Game::CharacterManager::removeCharacterByID(unsigned long long id) {
	if (getCharacterByID(id) == nullptr) {
		return false;
	}
	Character* character = &character[id];
	Location* location = character->getLocation();
	charactersByName.erase(character->getName());
	charactersByLocation[location->getWorld()][location->getChunk()].erase(location->getTile());
	characters.erase(characters.begin() + id);

	for (auto& pair : charactersByName) {
		if (pair.second > id) {
			pair.second--;
		}
	}

	for (auto& pair1 : charactersByLocation) {
		for (auto& pair2 : pair1.second) {
			for (auto& pair3 : pair2.second) {
				if (pair3.second > id) {
					pair3.second--;
				}
			}
		}
	}

	return true;
}

Game::Character* Game::CharacterManager::getCharacterByID(unsigned long long id) {
	if (id < characters.size()) {
		return &characters.at(id);
	} else {
		return nullptr;
	}
}

Game::Character* Game::CharacterManager::getCharacterByName(std::string name) {
	auto iter = charactersByName.find(name);
	if (iter != charactersByName.end()) {
		return &characters[iter->second];
	}
	return nullptr;
}

Game::Character* Game::CharacterManager::getCharacterByLocation(Location* location) {
	auto iter1 = charactersByLocation.find(location->getWorld());
	if (iter1 == charactersByLocation.end()) {
		return nullptr;
	}
	auto iter2 = iter1->second.find(location->getChunk());
	if (iter2 == iter1->second.end()) {
		return nullptr;
	}
	auto iter3 = iter2->second.find(location->getTile());
	if (iter3 == iter2->second.end()) {
		return nullptr;
	}
	return &characters[iter3->second];
}

std::unordered_map<glm::ivec3, unsigned long long, IVec3HashMapFunctions, IVec3HashMapFunctions>*
	Game::CharacterManager::getCharactersInChunk(unsigned long long world, glm::ivec2 chunk) {
	auto iter1 = charactersByLocation.find(world);
	if (iter1 == charactersByLocation.end()) {
		return nullptr;
	}
	auto iter2 = iter1->second.find(chunk);
	if (iter2 == iter1->second.end()) {
		return nullptr;
	}
	return &charactersByLocation[world][chunk];
}

void Game::CharacterManager::turn(Character* character, TileDirection direction) {
	character->directionFacing = direction;

	Model* model;
	if (resourceManager->obtain<Model>("char_" + character->getTextureName(character->getDirectionFacing()) + "_model", model)) {
		character->setModel(model);
		character->getEntity()->setModel(model);
		mapNeedsToBeRebuilt = true;
	}
}

void Game::CharacterManager::turn(unsigned long long id, TileDirection direction) {
	if (id < characters.size()) {
		Character* character = &characters[id];
		turn(character, direction);
	}
}

void Game::CharacterManager::turn(std::string name, TileDirection direction) {
	auto iter = charactersByName.find(name);
	if (iter == charactersByName.end()) {
		return;
	}

	Character* character = &characters[iter->second];
	turn(character, direction);
}

void Game::CharacterManager::move(Character* character, TileDirection direction, std::string scriptOnCompletion) {
	character->moving = true;
	Location* location = character->getLocation();
	charactersByLocation[location->getWorld()][location->getChunk()].erase(location->getTile());
	if (charactersByLocation[location->getWorld()][location->getChunk()].size() == 0) {
		if (charactersByLocation[location->getWorld()].size() == 0) {
			charactersByLocation.erase(location->getWorld());
		} else {
			charactersByLocation[location->getWorld()].erase(location->getChunk());
		}
	}
	glm::ivec2 chunkCoord = location->getChunk();
	glm::ivec3 tileCoord = location->getTile();
	glm::vec3 translationForAnimation;
	switch (direction) {
		case TileDirection::RIGHT:
			tileCoord += glm::ivec3(-1, 0, 0);
			translationForAnimation = glm::vec3(-1, 0, 0);
			break;
		case TileDirection::FORWARD:
			tileCoord += glm::ivec3(0, 0, 1);
			translationForAnimation = glm::vec3(0, 0, 1);
			break;
		case TileDirection::LEFT:
			tileCoord += glm::ivec3(1, 0, 0);
			translationForAnimation = glm::vec3(1, 0, 0);
			break;
		case TileDirection::BACKWARD:
			tileCoord += glm::ivec3(0, 0, -1);
			translationForAnimation = glm::vec3(0, 0, -1);
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
	location->setChunk(chunkCoord);
	location->setTile(tileCoord);
	charactersByLocation[location->getWorld()][location->getChunk()][location->getTile()] = charactersByName[character->getName()];
	character->addTranslation(translationForAnimation, scriptOnCompletion);
	turn(character, direction);
}

void Game::CharacterManager::move(unsigned long long id, TileDirection direction, std::string scriptOnCompletion) {
	if (id < characters.size()) {
		Character* character = &characters[id];
		move(character, direction, scriptOnCompletion);
	}
}

void Game::CharacterManager::move(std::string name, TileDirection direction, std::string scriptOnCompletion) {
	auto iter = charactersByName.find(name);
	if (iter == charactersByName.end()) {
		return;
	}

	Character* character = &characters[iter->second];
	move(character, direction, scriptOnCompletion);
}

void Game::CharacterManager::stopMoving(unsigned long long id) {
	if (id < characters.size()) {
		Character* character = &characters[id];
		character->moving = false;
	}
}

void Game::CharacterManager::stopMoving(std::string name) {
	auto iter = charactersByName.find(name);
	if (iter == charactersByName.end()) {
		return;
	}

	Character& character = characters[iter->second];
	character.moving = false;
}

bool Game::CharacterManager::doesMapNeedToBeRebuilt() {
	return mapNeedsToBeRebuilt;
}

void Game::CharacterManager::mapWasRebuilt() {
	mapNeedsToBeRebuilt = false;
}

void Game::CharacterManager::animateCharacterMovement(Character* character, glm::vec3 translation,
	std::string scriptOnCompletion) {
	character->getEntity()->setPosition(*character->getEntity()->getPosition() - translation);
	AnimationTrack3D track;
	track.setTag(character->getName() + "_movement");
	KeyFrame3D frame;
	frame.setObject(character->getEntity());
	glm::vec3 characterTranslation = *character->getEntity()->getPosition() + translation;
	frame.setTranslation(&characterTranslation);
	scriptManager->bindScriptToFrame(scriptOnCompletion, &frame);
	track.addKeyFrame((unsigned long long) (1000000.0f / character->getWalkingSpeed()), &frame);
	animator->addAnimationTrack3D(&track);
}

