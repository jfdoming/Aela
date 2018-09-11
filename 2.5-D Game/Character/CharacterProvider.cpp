
/*
* Class: Character Tracker
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage characters.
*/

#include "CharacterProvider.h"
#include "../../Project Aela/Aela_Engine.h"
#include "../Scripts/ScriptManager.h"
#include "../../Project Aela/Resource Management/ResourceManager.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include "../Particles/TileSwitchParticleEmitter.h"
#include "../Particles/CharacterTeleportParticleEmitter.h"
#include "../Worlds/WorldManager.h"

using namespace Game;

Game::CharacterProvider::CharacterProvider() {}

Game::CharacterProvider::~CharacterProvider() {
	for (auto pair : characters) {
		delete pair.second;
	}
}

void Game::CharacterProvider::update() {
	for (auto pair : characters) {
		pair.second->update();
	}
}

void Game::CharacterProvider::generateCharacterModelsForAllCharacters() {
	ResourceManager* resourceManager = GameObjectProvider::getResourceManager();
	CharacterModelGenerator generator;
	generator.setCharacters(&characters);

	std::string modelA = (std::string) DEFAULT_MODEL_PATH + "character_A.obj";
	std::string modelB = (std::string) DEFAULT_MODEL_PATH + "character_B.obj";
	std::string modelC = (std::string) DEFAULT_MODEL_PATH + "character_C.obj";
	std::string modelD = (std::string) DEFAULT_MODEL_PATH + "character_D.obj";

	resourceManager->bindLoader(&generator);
	resourceManager->bindGroup("characters");

	for (unsigned int y = 0; y < 4; y++) {
		for (unsigned int x = 0; x < 3; x++) {
			if (x == 0 && (y == 1 || y == 3)) {
				generator.setTemplateModelSource(modelA);
			} else if (x == 0 && (y == 0 || y == 2)) {
				generator.setTemplateModelSource(modelC);
			} else if (y == 1 || y == 3) {
				generator.setTemplateModelSource(modelB);
			} else {
				generator.setTemplateModelSource(modelD);
			}

			generator.setSpriteSheetX(x);
			generator.setSpriteSheetY(y);

			// We want to generate one set of models, so we'll tell the resource manager to run the generator once.
			resourceManager->addToGroup("", false);

			if (resourceManager->loadGroup("characters") != Aela::ResourceManager::Status::OK) {
				std::cerr << "Failed to load a resource from group \"characters\": " << resourceManager->getNewInvalidResourceKeys()[0] << "\n";
				break;
			}
		}
	}
}

bool Game::CharacterProvider::addCharacter(Character* character, size_t* id) {
	if (getCharacterByName(character->getName()) != nullptr || getCharacterByLocation(*character->getLocation()) != nullptr) {
		AelaErrorHandling::consoleWindowError("Character already exists by name (" + character->getName() + ") or at the location.");
		return false;
	}
	Location* location = character->getLocation();
	characters[nextCharacterID] = character;
	charactersByName[character->getName()] = nextCharacterID;
	charactersByLocation[location->getWorld()][location->getChunk()][location->getTileGroup()] = nextCharacterID;
	
	if (id != nullptr) {
		*id = nextCharacterID;
	}

	generateCharacterModel(nextCharacterID);

	nextCharacterID++;
	return true;
}

bool Game::CharacterProvider::addCharacter(Character* character) {
	return addCharacter(character, nullptr);
}

bool Game::CharacterProvider::removeCharacterByID(size_t id) {
	if (getCharacterByID(id) == nullptr) {
		return false;
	}
	Character* character = characters[id];
	Location* location = character->getLocation();
	charactersByName.erase(character->getName());
	charactersByLocation[location->getWorld()][location->getChunk()].erase(location->getTileGroup());
	characters.erase(id);

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

Game::Character* Game::CharacterProvider::getCharacterByID(size_t id) {
	auto iter = characters.find(id);
	if (iter == characters.end()) {
		return nullptr;
	}
	return iter->second;
}

Game::Character* Game::CharacterProvider::getCharacterByName(std::string name) {
	auto iter = charactersByName.find(name);
	if (iter != charactersByName.end()) {
		return characters[iter->second];
	}
	return nullptr;
}

Game::Character* Game::CharacterProvider::getCharacterByLocation(const Location& location) {
	auto iter1 = charactersByLocation.find(location.getWorld());
	if (iter1 == charactersByLocation.end()) {
		return nullptr;
	}
	auto iter2 = iter1->second.find(location.getChunk());
	if (iter2 == iter1->second.end()) {
		return nullptr;
	}
	auto iter3 = iter2->second.find(location.getTileGroup());
	if (iter3 == iter2->second.end()) {
		return nullptr;
	}
	return characters[iter3->second];
}

std::unordered_map<glm::ivec3, size_t, IVec3HashMapFunctions, IVec3HashMapFunctions>*
	Game::CharacterProvider::getCharactersInChunk(size_t world, glm::ivec2 chunk) {
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

void Game::CharacterProvider::saveDataToSaveState(SaveState* saveState) {
	auto* characterInformation = new std::unordered_map<size_t, CharacterInformationBlock>();
	
	for (auto& pair : characters) {
		(*characterInformation)[pair.first] = pair.second->getCharacterInformationBlock();
	}

	saveState->addData("characters", characterInformation);
}

void Game::CharacterProvider::loadDataFromSaveState(SaveState* saveState) {
	std::unordered_map<size_t, CharacterInformationBlock>* characterInformation = (std::unordered_map<size_t, CharacterInformationBlock>*) saveState->getData("characters");
	
	for (auto& pair : *characterInformation) {
		characters[pair.first]->setPropertiesUsingCharacterInformationBlock(&pair.second);
	}
}

void Game::CharacterProvider::characterWasMoved(std::string name, Location* oldLocation, Location* newLocation) {
	charactersByLocation[oldLocation->getWorld()][oldLocation->getChunk()].erase(oldLocation->getTileGroup());
	charactersByLocation[newLocation->getWorld()][newLocation->getChunk()][newLocation->getTileGroup()] = charactersByName[name];
}

void Game::CharacterProvider::generateCharacterModel(size_t character) {
	ResourceManager* resourceManager = GameObjectProvider::getResourceManager();
	CharacterModelGenerator generator;
	std::unordered_map<size_t, Character*> map;
	map[character] = characters[character];
	generator.setCharacters(&map);
	std::string groupName = "characters_" + std::to_string(character);

	std::string modelA = (std::string) DEFAULT_MODEL_PATH + "character_A.obj";
	std::string modelB = (std::string) DEFAULT_MODEL_PATH + "character_B.obj";
	std::string modelC = (std::string) DEFAULT_MODEL_PATH + "character_C.obj";
	std::string modelD = (std::string) DEFAULT_MODEL_PATH + "character_D.obj";

	resourceManager->bindLoader(&generator);
	resourceManager->bindGroup(groupName);

	for (unsigned int y = 0; y < 4; y++) {
		for (unsigned int x = 0; x < 3; x++) {
			if (x == 0 && (y == 1 || y == 3)) {
				generator.setTemplateModelSource(modelA);
			} else if (x == 0 && (y == 0 || y == 2)) {
				generator.setTemplateModelSource(modelC);
			} else if (y == 1 || y == 3) {
				generator.setTemplateModelSource(modelB);
			} else {
				generator.setTemplateModelSource(modelD);
			}

			generator.setSpriteSheetX(x);
			generator.setSpriteSheetY(y);

			// We want to generate one set of models, so we'll tell the resource manager to run the generator once.
			resourceManager->addToGroup("", false);

			if (resourceManager->loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
				std::cerr << "Failed to load a resource from group \"" + groupName + "\": " << resourceManager->getNewInvalidResourceKeys()[0] << "\n";
				break;
			}
		}
	}
}
