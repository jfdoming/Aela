
/*
* Class: Character Manager
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage characters.
*/

#include "stdafx.h"
#include "CharacterManager.h"
#include "CharacterModelGenerator.h"
#include "../Resources/ResourceInfo.h"
#include "Resource Management/ResourcePaths.h"
#include "../../Project Aela/3D/Animation/AnimationTrackMaterial.h"
#include "../../Project Aela/Utilities/enumut.h"

using namespace Game;

void Game::CharacterManager::setup(Engine* engine, ScriptManager* scriptManager) {
	this->resourceManager = engine->getResourceManager();
	this->animator = engine->getAnimator();
	this->animationLooper = engine->getAnimationLooper();
	this->camera = engine->getRenderer()->getCamera();
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
		} else if (!animator->trackWithTagExists(character.getName() + "_mv")) {
			std::pair<glm::vec3, std::string>* translation = character.getNextTranslation();
			if (translation != nullptr) {
				animateCharacterMovement(&character, translation->first, translation->second);
				character.removeNextTranslation();
			} else {
				character.moving = false;
				character.animationHadJustEnded = false;
			}
		}
	}
}

void Game::CharacterManager::generateCharacterModels(ResourceManager* resourceManager) {
	CharacterModelGenerator generator;
	generator.setCharacters(&characters);
	std::cout << characters[0].getName() << " is a name.\n";
	generator.setTemplateModelSource((std::string) RESOURCE_ROOT + DEFAULT_MODEL_PATH + "floor.obj");
	resourceManager->bindLoader(&generator);
	resourceManager->bindGroup("characters");

	// We want to generate one set of models, so we'll tell the resource manager to run the generator once.
	resourceManager->addToGroup("", false);

	if (resourceManager->loadGroup("characters") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"characters\": " << resourceManager->getNewInvalidResourceKeys()[0] << "\n";
	}
}

bool Game::CharacterManager::addCharacter(Character* character, size_t* id) {
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

bool Game::CharacterManager::removeCharacterByID(size_t id) {
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

Game::Character* Game::CharacterManager::getCharacterByID(size_t id) {
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

std::unordered_map<glm::ivec3, size_t, IVec3HashMapFunctions, IVec3HashMapFunctions>*
	Game::CharacterManager::getCharactersInChunk(size_t world, glm::ivec2 chunk) {
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
	character->turnSimple(direction);

	Model* model;
	if (resourceManager->obtain<Model>("ch_" + character->getTextureName() + "_" + std::to_string(enumToInteger(direction)) + "_0_mo", model)) {
		character->setModel(model);
		character->getEntity()->setModel(model);
		mapNeedsToBeRebuilt = true;
	}
}

void Game::CharacterManager::turn(size_t id, TileDirection direction) {
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
	character->moveSimple(direction, scriptOnCompletion);
	turn(character, direction);
}

void Game::CharacterManager::move(size_t id, TileDirection direction, std::string scriptOnCompletion) {
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

void Game::CharacterManager::teleport(Character* character, Location* location) {
	character->setLocation(location);
	mapNeedsToBeRebuilt = true;
}

void Game::CharacterManager::teleport(size_t id, Location* location) {
	if (id < characters.size()) {
		Character* character = &characters[id];
		teleport(character, location);
	}
}

void Game::CharacterManager::teleport(std::string name, Location* location) {
	auto iter = charactersByName.find(name);
	if (iter == charactersByName.end()) {
		return;
	}

	Character* character = &characters[iter->second];
	teleport(character, location);
}

void Game::CharacterManager::stopMoving(size_t id) {
	if (id < characters.size()) {
		Character* character = &characters[id];
		character->stopMoving();
	}
}

void Game::CharacterManager::stopMoving(std::string name) {
	auto iter = charactersByName.find(name);
	if (iter == charactersByName.end()) {
		return;
	}

	Character& character = characters[iter->second];
	character.stopMoving();
}

bool Game::CharacterManager::doesMapNeedToBeRebuilt() {
	return mapNeedsToBeRebuilt;
}

void Game::CharacterManager::mapWasRebuilt() {
	mapNeedsToBeRebuilt = false;
}

void Game::CharacterManager::animateCharacterMovement(Character* character, glm::ivec3 translation,
	std::string scriptOnCompletion) {
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
	glm::vec3 translationForAnimation = translation;
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

	location->setChunk(chunkCoord);
	location->setTile(tileCoord);
	charactersByLocation[location->getWorld()][location->getChunk()][location->getTile()] = charactersByName[character->getName()];

	long long timeToAdvanceTrackBy;

	if (character->animationHadJustEnded) {
		timeToAdvanceTrackBy = character->timePassedAfterAnimationEnd;
	} else {
		timeToAdvanceTrackBy = 0;
	}

	if (character->getName() == PLAYER_NAME) {
		// std::cout << "Starting a character animation.\n";
		animateCamera(translation, character->getCurrentSpeed(), timeToAdvanceTrackBy);
	}

	AnimationTrack3D track3D;

	// "mv" means "movement". The name is shortened to an abbreviation to save memory.
	track3D.setTag(character->getName() + "_mv");

	KeyFrame3D frame;
	frame.setObject(character->getEntity());
	glm::vec3 characterTranslation = *character->getEntity()->getPosition() + translationForAnimation;
	frame.setTranslation(&characterTranslation);
	scriptManager->bindScriptToFrame(scriptOnCompletion, &frame);
	auto action = [character]() {
		character->animationHasEnded();
	};

	frame.setEndingAction(std::bind(action));

	track3D.addKeyFrame((size_t) (1000000.0f / character->getCurrentSpeed()), &frame);
	frame.start();
	// track.updatePositionInTrack(timeToAdvanceTrackBy);
	// std::cout << timeToAdvanceTrackBy << " - " << track.getPositionInTrack() << " is the time.\n";
	animator->addAnimationTrack3D(&track3D);

	AnimationTrackModel modelTrack;

	// "st" means "stepping". The name is shortened to an abbreviation to save memory.
	modelTrack.setTag(character->getName() + "_st");

	std::string step = std::to_string(enumToInteger(character->getCurrentStep()) + 1);

	std::string direction = std::to_string(enumToInteger(character->getDirectionFacing()));
	Model* model1 = nullptr, *model2 = nullptr;
	if (!resourceManager->obtain<Model>("ch_" + character->getTextureName() + "_" + direction + "_" + step + "_mo", model1)
		|| !resourceManager->obtain<Model>("ch_" + character->getTextureName() + "_" + direction + "_0_mo", model2)) {
		return;
	}

	auto action1 = [character, model1]() {
		character->setModel(model1);
	};

	auto action2 = [character, model2]() {
		character->setModel(model2);
	};

	KeyFrameModel step1Frame, step2Frame;
	ModelEntity* entity = character->getEntity();

	step1Frame.setModel(model1);
	step1Frame.setModelEntity(entity);
	step1Frame.setEndingAction(action1);
	modelTrack.addKeyFrame(1, &step1Frame);

	step2Frame.setModel(model2);
	step2Frame.setModelEntity(entity);
	step2Frame.setEndingAction(action2);
	modelTrack.addKeyFrame((long long) (1000000.0f / character->getCurrentSpeed() / 2), &step2Frame);

	frame.setEndingAction(std::bind(action));

	animator->addAnimationTrackModel(&modelTrack);
}

void Game::CharacterManager::animateCamera(glm::vec3 translation, float speed, long long timeToAdvanceTrackBy) {
	glm::vec3 cameraTrans = *camera->getPosition() + translation;
	AnimationTrack3D track;
	track.setTag("camera_mv");
	KeyFrame3D frame;
	frame.setObject(camera);
	frame.setTranslation(&cameraTrans);
	track.addKeyFrame((long long) (1000000.0f / speed), &frame);
	frame.start();
	// track.updatePositionInTrack(timeToAdvanceTrackBy);
	animator->addAnimationTrack3D(&track);
}