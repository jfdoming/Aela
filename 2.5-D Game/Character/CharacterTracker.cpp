
/*
* Class: Character Tracker
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage characters.
*/

#include "stdafx.h"
#include "CharacterTracker.h"
#include "../../Project Aela/Aela_Engine.h"
#include "../Scripts/ScriptManager.h"
#include "Resource Management/ResourceManager.h"
#include "Animation/Animator.h"
#include "Animation/AnimationLooper.h"
#include "3D/Animation/AnimationTrack3D.h"
#include "3D/Camera/Camera3D.h"
#include "CharacterModelGenerator.h"
#include "Resource Management/ResourcePaths.h"
#include "../../Project Aela/3D/Animation/AnimationTrackMaterial.h"
#include "../../Project Aela/Utilities/enumut.h"
#include "../Utilities/GameConstants.h"
#include "../Particles/TileSwitchParticleEmitter.h"
#include "../Particles/CharacterTeleportParticleEmitter.h"
#include "../Resources/ResourceInfo.h"

using namespace Game;

Game::CharacterTracker::CharacterTracker() {
	animator = GameObjectProvider::getAnimator();
	camera = GameObjectProvider::getCamera();
	resourceManager = GameObjectProvider::getResourceManager();
	time = GameObjectProvider::getTime();
}

Game::CharacterTracker::~CharacterTracker() {
	for (auto pair : characters) {
		delete pair.second;
	}
}

void Game::CharacterTracker::setup() {
	scriptManager = GameObjectProvider::getScriptManager();
}

void Game::CharacterTracker::update() {
	for (auto pair : characters) {
		size_t id = pair.first;
		Character* character = characters[id];

		if (character->health <= 0) {
			animateCharacterDeath(character);
			if (character->getName() == PLAYER_NAME) {
				character->allowNewMovements(false);
				playerIsDead = true;
				character->getLocation()->setWorld(-1);
				character->getLocationBeforeAnimation()->setWorld(-1);
			}
			charactersToDelete.push_back(id);
			continue;
		}

		character->update();

		if (!character->isMoving()) {
			std::pair<Movement, std::string>* translation = character->getNextTranslation();
			if (translation != nullptr) {
				processCharacterMovement(character, &translation->first, translation->second);
				character->removeNextTranslation();
			}
		} else if (!animator->trackWithTagExists(character->getName() + "/mv")) {
			std::pair<Movement, std::string>* translation = character->getNextTranslation();
			if (translation != nullptr) {
				processCharacterMovement(character, &translation->first, translation->second);
				character->removeNextTranslation();
			} else {
				character->moving = false;
			}
		}

		character->animationHadJustEnded = false;
	}

	// I was getting memory access violatiosn even though I was deleting the way you're meant to
	// in an iterative loop like above. So I made this instead:
	for (auto iter : charactersToDelete) {
		deleteCharacterByID(iter);
	}
	charactersToDelete.clear();

	if (!camera->isUsingKeyboardControls() && lockingCameraToPlayer && !playerIsDead) {
		// The camera has not been taken over by the user using a cheat-code.
		resetCameraPosition();
	}
}

void Game::CharacterTracker::scenesWereSetUp() {
	gameplayScene = GameObjectProvider::getGameplayScene();
}

void Game::CharacterTracker::generateCharacterModels(ResourceManager* resourceManager) {
	CharacterModelGenerator generator;
	generator.setCharacters(&characters);

	std::string modelA = (std::string) RESOURCE_ROOT + DEFAULT_MODEL_PATH + "character_A.obj";
	std::string modelB = (std::string) RESOURCE_ROOT + DEFAULT_MODEL_PATH + "character_B.obj";
	std::string modelTemp = (std::string) RESOURCE_ROOT + DEFAULT_MODEL_PATH + "floor.obj";

	resourceManager->bindLoader(&generator);
	resourceManager->bindGroup("characters");

	for (unsigned int y = 0; y < 4; y++) {
		for (unsigned int x = 0; x < 3; x++) {
			if (x == 0 && (y == 1 || y == 3)) {
				generator.setTemplateModelSource(modelA);
			} else if (y == 0 || y == 2) {
				generator.setTemplateModelSource(modelTemp);
			} else {
				generator.setTemplateModelSource(modelB);
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

bool Game::CharacterTracker::trackCharacter(Character* character, size_t* id) {
	if (getCharacterByName(character->getName()) != nullptr || getCharacterByLocation(character->getLocation()) != nullptr) {
		AelaErrorHandling::consoleWindowError("Character already exists by name (" + character->getName() + ") or at the location.");
		return false;
	}
	Location* location = character->getLocation();
	characters[nextCharacterID] = character;
	charactersByName[character->getName()] = nextCharacterID;
	charactersByLocation[location->getWorld()][location->getChunk()][location->getTileGroup()] = nextCharacterID;
	*id = nextCharacterID;
	nextCharacterID++;
	return true;
}

bool Game::CharacterTracker::deleteCharacterByID(size_t id) {
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

Game::Character* Game::CharacterTracker::getCharacterByID(size_t id) {
	auto iter = characters.find(id);
	if (iter == characters.end()) {
		return nullptr;
	}
	return iter->second;
}

Game::Character* Game::CharacterTracker::getCharacterByName(std::string name) {
	auto iter = charactersByName.find(name);
	if (iter != charactersByName.end()) {
		return characters[iter->second];
	}
	return nullptr;
}

Game::Character* Game::CharacterTracker::getCharacterByLocation(Location* location) {
	auto iter1 = charactersByLocation.find(location->getWorld());
	if (iter1 == charactersByLocation.end()) {
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
	return characters[iter3->second];
}

std::unordered_map<glm::ivec3, size_t, IVec3HashMapFunctions, IVec3HashMapFunctions>*
	Game::CharacterTracker::getCharactersInChunk(size_t world, glm::ivec2 chunk) {
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

void Game::CharacterTracker::turnTrackedCharacter(Character* character, TileDirection direction) {
	character->turnSimple(direction);

	Model* model;
	if (resourceManager->obtain<Model>("ch/" + character->getTextureName() + "/0/" + std::to_string(enumToInteger(direction)) + "/mo", model)) {
		character->setModel(model);
		character->getEntity()->setModel(model);
		mapNeedsToBeRebuilt = true;
	}
}

void Game::CharacterTracker::turnTrackedCharacter(size_t id, TileDirection direction) {
		Character* character = getCharacterByID(id);
		if (character == nullptr) {
			return;
		}
		turnTrackedCharacter(character, direction);
}

void Game::CharacterTracker::turnTrackedCharacter(std::string name, TileDirection direction) {
	auto iter = charactersByName.find(name);
	if (iter == charactersByName.end()) {
		return;
	}

	Character* character = characters[iter->second];
	turnTrackedCharacter(character, direction);
}

void Game::CharacterTracker::moveTrackedCharacter(Character* character, Movement* movement, std::string scriptOnCompletion) {
	character->moveSimple(movement, scriptOnCompletion);
	turnTrackedCharacter(character, movement->getDirection());
}

void Game::CharacterTracker::moveTrackedCharacter(size_t id, Movement* movement, std::string scriptOnCompletion) {
	Character* character = getCharacterByID(id);
	if (character == nullptr) {
		return;
	}
	moveTrackedCharacter(character, movement, scriptOnCompletion);
}

void Game::CharacterTracker::moveTrackedCharacter(std::string name, Movement* movement, std::string scriptOnCompletion) {
	auto iter = charactersByName.find(name);
	if (iter == charactersByName.end()) {
		return;
	}

	Character* character = characters[iter->second];
	moveTrackedCharacter(character, movement, scriptOnCompletion);
}

void Game::CharacterTracker::teleportTrackedCharacter(Character* character, Location* location, bool animation) {
	Movement teleportation(location, &glm::vec3(), character->getDirectionFacing(), true);
	if (!animation) {
		teleportation.setAnimated(false);
	}
	moveTrackedCharacter(character, &teleportation, "");
}

void Game::CharacterTracker::teleportTrackedCharacter(size_t id, Location* location, bool animation) {
	Character* character = getCharacterByID(id);
	if (character == nullptr) {
		return;
	}
	teleportTrackedCharacter(character, location, animation);
}

void Game::CharacterTracker::teleportTrackedCharacter(std::string name, Location* location, bool animation) {
	auto iter = charactersByName.find(name);
	if (iter == charactersByName.end()) {
		return;
	}

	Character* character = characters[iter->second];
	teleportTrackedCharacter(character, location, animation);
}

bool Game::CharacterTracker::doesMapNeedToBeRebuilt() {
	return mapNeedsToBeRebuilt;
}

void Game::CharacterTracker::mapWasRebuilt() {
	mapNeedsToBeRebuilt = false;
}

void Game::CharacterTracker::setPlayer(size_t id) {
	playerID = id;
}

bool Game::CharacterTracker::isPlayerDead() {
	return playerIsDead;
}

void Game::CharacterTracker::setGameplayMenuItems(std::shared_ptr<RectComponent> deathRect, std::shared_ptr<Label> deathText) {
	this->deathRect = deathRect;
	this->deathText = deathText;
}

void Game::CharacterTracker::processCharacterMovement(Character* character, Movement* movement, std::string scriptOnCompletion) {
	character->moving = true;
	Location oldLocation = *character->getLocation();
	charactersByLocation[oldLocation.getWorld()][oldLocation.getChunk()].erase(oldLocation.getTileGroup());

	Location* newLocation = movement->getDestination();
	charactersByLocation[newLocation->getWorld()][newLocation->getChunk()][newLocation->getTileGroup()] = charactersByName[character->getName()];
	character->setLocation(newLocation);

	if (movement->isATeleportation()) {
		if (movement->isAnimated()) {
			character->allowNewMovements(true);
			lockingCameraToPlayer = false;

			CharacterTeleportParticleEmitter* particleEmitter = new CharacterTeleportParticleEmitter(time);
			particleEmitter->setBaseDistance(oldLocation.getWorldSpaceLocation().y + 10);
			particleEmitter->setBaseSpeed(0.00000001f);
			particleEmitter->setPathOffset(oldLocation.getWorldSpaceLocation().y);
			particleEmitter->setLifeTime(1500000000);
			particleEmitter->setCharacter(character);
			particleEmitter->setLocations(&oldLocation, newLocation);
			particleEmitter->setSecondaryPathOffset(newLocation->getWorldSpaceLocation().y + 8);

			std::vector<GLTexture*> textures;
			GLTexture* texture = static_cast<GLTexture*>(characters[playerID]->getEntity()->getModel()->getSubModels()->at(0).getMaterial()->getTexture());
			textures.push_back(texture);

			auto halfLifeAction = [this, character]() {
				mapNeedsToBeRebuilt = true;
				lockingCameraToPlayer = true;
			};

			auto onEndAction = [this, character]() {
				character->allowNewMovements(false);
			};

			particleEmitter->setActionOnHalfLife(halfLifeAction);
			particleEmitter->setActionOnEnd(onEndAction);
			particleEmitter->setupParticles(&textures, 1, 1, 1);
			gameplayScene->putParticleEmitter(particleEmitter);
		} else {
			mapNeedsToBeRebuilt = true;
		}
		return;
	}

	glm::vec3 translationForAnimation = *movement->getWorldspaceTranslation();

	long long timeToAdvanceTrackBy;

	if (character->animationHadJustEnded) {
		timeToAdvanceTrackBy = character->timePassedAfterAnimationEnd;
	} else {
		timeToAdvanceTrackBy = 0;
	}

	if (character->getName() == PLAYER_NAME) {
		// std::cout << "Starting a character animation.\n";
		animateCamera(translationForAnimation, character->getCurrentSpeed(), timeToAdvanceTrackBy);
	}

	AnimationTrack3D track3D;

	// "mv" means "movement". The name is shortened to an abbreviation to save memory.
	track3D.setTag(character->getName() + "/mv");

	KeyFrame3D frame;
	frame.setObject(character->getEntity());
	glm::vec3 characterTranslation = *character->getEntity()->getPosition() + translationForAnimation;
	frame.setTranslation(&characterTranslation);
	auto action = [character, scriptOnCompletion, this]() {
		character->animationHasEnded();
		scriptManager->runScript(scriptOnCompletion);
	};

	frame.setEndingAction(std::bind(action));

	track3D.addKeyFrame((size_t) (1000000.0f / character->getCurrentSpeed()), &frame);
	// frame.start();
	// track.updatePositionInTrack(timeToAdvanceTrackBy);
	// std::cout << timeToAdvanceTrackBy << " - " << track.getPositionInTrack() << " is the time.\n";
	animator->addAnimationTrack3D(&track3D);

	AnimationTrackModel modelTrack;

	// "st" means "stepping". The name is shortened to an abbreviation to save memory.
	modelTrack.setTag(character->getName() + "/st");

	std::string step = std::to_string(enumToInteger(character->getCurrentStep()) + 1);

	std::string direction = std::to_string(enumToInteger(character->getDirectionFacing()));
	Model* model1 = nullptr, *model2 = nullptr;
	if (!resourceManager->obtain<Model>("ch/" + character->getTextureName() + "/" + step + "/" + direction + "/mo", model1)
		|| !resourceManager->obtain<Model>("ch/" + character->getTextureName() + "/0/" + direction + "/mo", model2)) {
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

void Game::CharacterTracker::animateCamera(glm::vec3 translation, float speed, long long timeToAdvanceTrackBy) {
	//glm::vec3 cameraTrans = *camera->getPosition() + translation;
	//AnimationTrack3D track;
	//track.setTag("camera_mv");
	//KeyFrame3D frame;
	//frame.setObject(camera);
	//frame.setTranslation(&cameraTrans);
	//track.addKeyFrame((long long) (1000000.0f / speed), &frame);
	//frame.start();
	//// track.updatePositionInTrack(timeToAdvanceTrackBy);
	//animator->addAnimationTrack3D(&track);
}

void Game::CharacterTracker::animateCharacterDeath(Character* character) {
	TileSwitchParticleEmitter* particleEmitter = new TileSwitchParticleEmitter(time);
	glm::vec3 worldSpacePosition = character->getLocationBeforeAnimation()->getWorldSpaceLocation();
	particleEmitter->setBaseDistance(worldSpacePosition.y + 10);
	particleEmitter->setBaseSpeed(0.00000002f);
	particleEmitter->setPathOffset(worldSpacePosition.y);
	particleEmitter->setLifeTime(500000000);
	particleEmitter->setupDimensions(&Rect<float>(worldSpacePosition.x, worldSpacePosition.z, 1, 1));

	if (character->getName() == PLAYER_NAME) {
		auto endingAction = [this]() {
			animatePlayerDeathScreen();
		};

		particleEmitter->setActionOnEnd(endingAction);
	}

	std::vector<GLTexture*> textures;
	GLTexture* texture = static_cast<GLTexture*>(character->getEntity()->getModel()->getSubModels()->at(0).getMaterial()->getTexture());
	textures.push_back(texture);

	particleEmitter->setupParticles(&textures, 1, 1, 1);
	gameplayScene->putParticleEmitter(particleEmitter);

	character->setVisibility(false);
	character->allowNewMovements(false);
}

void Game::CharacterTracker::animatePlayerDeathScreen() {
	AnimationTrack2D rectTrack2D;
	KeyFrame2D rectFrame;
	rectFrame.setObject(deathRect);
	rectFrame.setTint(&ColourRGBA(1, 1, 1, 1));
	rectTrack2D.addKeyFrameUsingMillis(1000, &rectFrame);
	animator->addAnimationTrack2D(&rectTrack2D);

	AnimationTrack2D textTrack2D;
	KeyFrame2D textFrame;
	textFrame.setObject(deathText);
	textFrame.setTint(&ColourRGBA(1, 1, 1, 1));
	textTrack2D.addKeyFrameUsingMillis(1000, &textFrame);
	animator->addAnimationTrack2D(&textTrack2D);
}

void Game::CharacterTracker::resetCameraPosition() {
	Character* character = characters[playerID];
	glm::vec3 entityPosition = *character->getEntity()->getPosition();
	camera->setPosition(entityPosition + glm::vec3(0, sin(PLAYER_CAMERA_ANGLE)
		* PLAYER_CAMERA_DISTANCE, -cos(PLAYER_CAMERA_ANGLE) * PLAYER_CAMERA_DISTANCE));
	camera->setRotation(0, -PLAYER_CAMERA_ANGLE, 0);
}
