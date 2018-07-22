/*
* Class: Character
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent an character.
*/

#include "Character.h"
#include "../Worlds/WorldManager.h"
#include "../Particles/TileSwitchParticleEmitter.h"
#include "../Particles/CharacterTeleportParticleEmitter.h"
#include "../Player/Player.h"
#include "../Aela Game/AelaGame.h"
#include "../Resources/ResourceInfo.h"
#include "../Scripts/ScriptManager.h"
#include "../Camera/CameraController.h"
#include "../Displays/Hint Display/HintDisplay.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include "../../Project Aela/Utilities/enumut.h"
#include <iostream>

using namespace Game;

Game::Character::Character() {
	// According to Platinum, it should be 0.00375.
	walkingSpeed = 0.00375f;
	// According to Platinum, it should be 0.0075, but that feels too fast.
	runningSpeed = 0.0075f;
	directionFacing = TileDirection::BACKWARD;
	health = 1;
	maxHealth = 1;
}

Game::Character::Character(std::string name) : Character() {
	this->name = name;
}

void Game::Character::setLocation(Location* location) {
	this->location = *location;
}

void Game::Character::animateDeath() {
	TileSwitchParticleEmitter* particleEmitter = new TileSwitchParticleEmitter(GameObjectProvider::getTime());
	glm::vec3 worldSpacePosition = locationBeforeAnimation.getWorldSpaceLocation();
	particleEmitter->setBaseDistance(worldSpacePosition.y + 10);
	particleEmitter->setBaseSpeed(0.00000002f);
	particleEmitter->setPathOffset(worldSpacePosition.y);
	particleEmitter->setLifeTime(500000000);
	particleEmitter->setupDimensions(&Rect<float>(worldSpacePosition.x, worldSpacePosition.z, 1, 1));

	if (this == GameObjectProvider::getPlayer()->getCharacter()) {
		auto endingAction = [this]() {
			GameObjectProvider::getGame()->animatePlayerDeathScreen();
		};

		particleEmitter->setActionOnEnd(endingAction);
	}

	std::vector<GLTexture*> textures;
	GLTexture* texture = static_cast<GLTexture*>(entity->getModel()->getSubModels()->at(0).getMaterial()->getTexture());
	textures.push_back(texture);

	particleEmitter->setupParticles(&textures, 1, 1, 1);
	GameObjectProvider::getGameplayScene()->putParticleEmitter(particleEmitter);

	newMovementsAreAllowed = false;
}

void Game::Character::generateModel(ResourceManager* resourceManager) {
	std::unordered_map<size_t, Character*> characterList;
	characterList[0] = this;

	CharacterModelGenerator generator;
	generator.setCharacters(&characterList);

	std::string modelA = (std::string) RESOURCE_ROOT + DEFAULT_MODEL_PATH + "character_A.obj";
	std::string modelB = (std::string) RESOURCE_ROOT + DEFAULT_MODEL_PATH + "character_B.obj";
	std::string modelTemp = (std::string) RESOURCE_ROOT + DEFAULT_MODEL_PATH + "floor.obj";

	resourceManager->bindLoader(&generator);
	resourceManager->bindGroup("ch/" + name);

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

Game::Location* Game::Character::getLocation() {
	return &location;
}

Location* Game::Character::getLocationBeforeAnimation() {
	return &locationBeforeAnimation;
}

void Game::Character::setModel(Model* model) {
	baseModel = model;
}

Model* Game::Character::getModel() {
	return baseModel;
}

void Game::Character::setName(std::string name) {
	this->name = name;
}

std::string Game::Character::getName() {
	return name;
}

void Game::Character::setWalkingSpeed(float walkingSpeed) {
	this->walkingSpeed = walkingSpeed;
}

float Game::Character::getWalkingSpeed() {
	return walkingSpeed;
}

void Game::Character::setRunningSpeed(float runningSpeed) {
	this->runningSpeed = runningSpeed;
}

float Game::Character::getRunningSpeed() {
	return runningSpeed;
}

float Game::Character::getCurrentSpeed() {
	if (running) {
		return runningSpeed;
	} else {
		return walkingSpeed;
	}
}

void Game::Character::setRunning(bool running) {
	this->running = running;
}

bool Game::Character::getRunning() {
	return running;
}

void Game::Character::setTextureName(std::string textureName) {
	this->textureName = textureName;
}

std::string Game::Character::getTextureName() {
	return textureName;
}

Game::TileDirection Game::Character::getDirectionFacing() {
	return directionFacing;
}

Aela::ModelEntity* Game::Character::getEntity() {
	return entity;
}

void Game::Character::setEntity(ModelEntity* entity) {
	this->entity = entity;
}

Game::CharacterStep Game::Character::getCurrentStep() {
	return currentStep;
}

void Game::Character::switchStep() {
	if (currentStep == CharacterStep::LEFT) {
		currentStep = CharacterStep::RIGHT;
	} else {
		currentStep = CharacterStep::LEFT;
	}
}

bool Game::Character::isMoving() {
	return moving;
}

void Game::Character::animationHasEnded() {
	// Moving gets sets to false by the character manager! It does this since it performs some actions
	// when the character is done moving!
	moving = false;
	locationBeforeAnimation = location;
	animationHadJustEnded = true;
	timePassedAfterAnimationEnd = entity->getTimePassedAfterAnimationEnd();
}

bool Game::Character::animationHasJustEnded() {
	return animationHadJustEnded;
}

void Game::Character::turn(TileDirection direction) {
	if (!newMovementsAreAllowed) {
		return;
	}

	if (directionFacing != direction) {
		timePassedAfterAnimationEnd = 0;
		animationHadJustEnded = false;
	}

	directionFacing = direction;

	Model* model;
	if (GameObjectProvider::getResourceManager()->obtain<Model>("ch/" + textureName + "/0/" + std::to_string(enumToInteger(direction)) + "/mo", model)) {
		baseModel = model;
		entity->setModel(model);
		GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
	}
}

void Game::Character::move(Movement* movement, std::string scriptOnCompletion) {
	addTranslation(movement, scriptOnCompletion);
	turn(movement->getDirection());

	if (movement->isATeleportation() && movement->isAnimated()) {
		newMovementsAreAllowed = false;
	}
}

void Game::Character::moveIfPossible(TileDirection direction) {
	if (newMovementsAreAllowed) {
		possibleMovementsToProcess.push_back(direction);
	}
}

void Game::Character::moveIfPossible(std::list<TileDirection> directions) {
	if (newMovementsAreAllowed) {
		possibleMovementsToProcess.splice(possibleMovementsToProcess.end(), directions);
	}
}

void Game::Character::clearFutureMovements() {
	possibleMovementsToProcess.clear();
}

void Game::Character::teleportWithoutAnimation(Location * location) {
	Movement teleportation(location, &glm::vec3(), directionFacing, true, false);
	move(&teleportation, "");
}

void Game::Character::teleportWithAnimation(Location* location, TeleportationAnimation animation) {
	newMovementsAreAllowed = false;
	Movement teleportation(location, &glm::vec3(), directionFacing, true, animation);
	move(&teleportation, "");
}

void Game::Character::kill() {
	alive = false;
	animateDeath();
	GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
}

void Game::Character::revive() {
	alive = true;
}

bool Game::Character::isAlive() {
	return alive;
}

void Game::Character::stopMoving() {
	moving = false;
}

void Game::Character::setHealth(int health) {
	this->health = health;
	if (health > maxHealth) {
		health = maxHealth;
	}
	if (health < 0) {
		health = 0;
		kill();
	}
}

int Game::Character::getHealth() {
	return health;
}

void Game::Character::increaseHealth(int amount) {
	health += amount;
	if (health > maxHealth) {
		health = maxHealth;
	}
}

void Game::Character::decreaseHealth(int amount) {
	health -= amount;
	if (health < 0) {
		health = 0;
		kill();
	}
}

void Game::Character::setMaxHealth(int maxHealth) {
	this->maxHealth = maxHealth;
	if (maxHealth < 1) {
		maxHealth = 1;
	}
	if (health > maxHealth) {
		health = maxHealth;
	}
}

int Game::Character::getMaxHealth() {
	return maxHealth;
}

void Game::Character::increaseMaxHealth(int amount) {
	maxHealth += amount;
}

void Game::Character::decreaseMaxHealth(int amount) {
	maxHealth -= amount;
	if (maxHealth < 1) {
		maxHealth = 1;
	}
	if (health > maxHealth) {
		health = maxHealth;
	}
}

void Game::Character::setVisibility(bool visible) {
	this->visible = visible;
	entity->setVisibility(visible);
}

bool Game::Character::isVisible() {
	return visible;
}

void Game::Character::allowNewMovements(bool newMovementsAreAllowed) {
	this->newMovementsAreAllowed = newMovementsAreAllowed;
}

bool Game::Character::areNewMovementsAllowed() {
	return newMovementsAreAllowed;
}

void Game::Character::update() {
	animationHadJustEnded = false;

	if (!newMovementsAreAllowed) {
		possibleMovementsToProcess.clear();
	} else if (!moving && possibleMovementsToProcess.size() > 0) {
		processPossibleMovement(possibleMovementsToProcess.front());
		possibleMovementsToProcess.pop_front();
	}

	if (!moving) {
		if (translations.size() == 0) {
			return;
		}

		std::pair<Movement, std::string> translation = translations[0];
		processMovement(&translation.first, translation.second);
		translations.erase(translations.begin());
	} else if (!GameObjectProvider::getAnimator()->trackWithTagExists(name + "/mv")) {
		if (translations.size() == 0) {
			moving = false;
			return;
		}

		std::pair<Movement, std::string> translation = translations[0];
		processMovement(&translation.first, translation.second);
		translations.erase(translations.begin());
	}
}

void Game::Character::addTranslation(Movement* movement, std::string scriptOnceComplete) {
	translations.push_back(std::pair<Movement, std::string>(*movement, scriptOnceComplete));
}

void Game::Character::processMovement(Movement* movement, std::string scriptOnCompletion) {
	moving = true;
	switchStep();

	CharacterProvider* characterProvider = GameObjectProvider::getCharacterProvider();
	Location oldLocation = location;
	Location newLocation = *movement->getDestination();

	if (!movement->isAnimated()) {
		characterProvider->characterWasMoved(name, &oldLocation, &newLocation);
		location = newLocation;
		GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
		animationHasEnded();
		return;
	}

	if (movement->isATeleportation()) {
		TeleportationAnimation teleportationAnimation = movement->getTeleportationAnimation();

		// This already occurs elsewhere:
		// newMovementsAreAllowed = false;

		if (teleportationAnimation == TeleportationAnimation::RISE) {
			characterProvider->characterWasMoved(name, &oldLocation, &newLocation);
			location = newLocation;

			CameraController* cameraController = GameObjectProvider::getCameraController();
			cameraController->setLockCameraToPlayer(false);

			float offset = GameObjectProvider::getWorldManager()->getCharacterYOffsetInWorldspace();

			CharacterTeleportParticleEmitter* particleEmitter = new CharacterTeleportParticleEmitter(GameObjectProvider::getTime());
			particleEmitter->setBaseDistance(oldLocation.getWorldSpaceLocation().y + offset + 10);
			particleEmitter->setBaseSpeed(0.00000001f);
			particleEmitter->setPathOffset(oldLocation.getWorldSpaceLocation().y + offset);
			particleEmitter->setLifeTime(1500000000);
			particleEmitter->setCharacter(this);
			particleEmitter->setLocations(&oldLocation, &newLocation);
			particleEmitter->setSecondaryPathOffset(newLocation.getWorldSpaceLocation().y - offset + 8);

			std::vector<GLTexture*> textures;
			GLTexture* texture = static_cast<GLTexture*>(entity->getModel()->getSubModels()->at(0).getMaterial()->getTexture());
			textures.push_back(texture);

			auto halfLifeAction = [this, cameraController]() {
				GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
				cameraController->setLockCameraToPlayer(true);
			};

			auto onEndAction = [this]() {
				newMovementsAreAllowed = true;
			};

			particleEmitter->setActionOnHalfLife(halfLifeAction);
			particleEmitter->setActionOnEnd(onEndAction);
			particleEmitter->setupParticles(&textures, 1, 1, 1);
			GameObjectProvider::getGameplayScene()->putParticleEmitter(particleEmitter);
		} else if (teleportationAnimation == TeleportationAnimation::FADE) {
			auto halfwayTeleportAction = [this, oldLocation, newLocation]() {
				Location oldLocation2 = oldLocation;
				Location newLocation2 = newLocation;
				GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
				GameObjectProvider::getCharacterProvider()->characterWasMoved(name, &oldLocation2, &newLocation2);
				location = newLocation;
			};

			auto teleportationEnd = [this]() {
				newMovementsAreAllowed = true;
			};

			GameObjectProvider::getTimer()->scheduleEventInMillis(750, halfwayTeleportAction);
			GameObjectProvider::getTimer()->scheduleEventInMillis(1500, teleportationEnd);
			GameObjectProvider::getGame()->animateFadeTeleport();
		}
		return;
	}

	characterProvider->characterWasMoved(name, &oldLocation, &newLocation);
	location = newLocation;
	glm::vec3 translationForAnimation = *movement->getWorldspaceTranslation();
	long long timeToAdvanceTrackBy;

	if (animationHadJustEnded) {
		timeToAdvanceTrackBy = timePassedAfterAnimationEnd;
	} else {
		timeToAdvanceTrackBy = 0;
	}

	AnimationTrack3D track3D;

	// "mv" means "movement". The name is shortened to an abbreviation to save memory.
	track3D.setTag(name + "/mv");

	KeyFrame3D frame;
	frame.setObject(entity);
	glm::vec3 characterTranslation = *entity->getPosition() + translationForAnimation;
	frame.setTranslation(&characterTranslation);
	auto action = [this, scriptOnCompletion]() {
		animationHasEnded();
		GameObjectProvider::getScriptManager()->runScript(scriptOnCompletion);
	};

	frame.setEndingAction(std::bind(action));

	track3D.addKeyFrame((size_t) (1000000.0f / getCurrentSpeed()), &frame);

	Animator* animator = GameObjectProvider::getAnimator();
	animator->addAnimationTrack3D(&track3D);

	AnimationTrackModel modelTrack;

	// "st" means "stepping". The name is shortened to an abbreviation to save memory.
	modelTrack.setTag(name + "/st");

	std::string step = std::to_string(enumToInteger(currentStep) + 1);

	std::string direction = std::to_string(enumToInteger(directionFacing));
	Model* model1 = nullptr, *model2 = nullptr;

	ResourceManager* resourceManager = GameObjectProvider::getResourceManager();
	if (!resourceManager->obtain<Model>("ch/" + textureName + "/" + step + "/" + direction + "/mo", model1)
		|| !resourceManager->obtain<Model>("ch/" + textureName + "/0/" + direction + "/mo", model2)) {
		return;
	}

	auto action1 = [this, model1]() {
		setModel(model1);
	};

	auto action2 = [this, model2]() {
		setModel(model2);
	};

	KeyFrameModel step1Frame, step2Frame;

	step1Frame.setModel(model1);
	step1Frame.setModelEntity(entity);
	step1Frame.setEndingAction(action1);
	modelTrack.addKeyFrame(1, &step1Frame);

	step2Frame.setModel(model2);
	step2Frame.setModelEntity(entity);
	step2Frame.setEndingAction(action2);
	modelTrack.addKeyFrame((long long) (1000000.0f / getCurrentSpeed() / 2), &step2Frame);

	frame.setEndingAction(std::bind(action));
	animator->addAnimationTrackModel(&modelTrack);
}

void Game::Character::processPossibleMovement(TileDirection direction) {
	if (direction != directionFacing) {
		turn(direction);
		return;
	}

	Location newLocation = location;
	glm::ivec2 chunkCoord = newLocation.getChunk();
	glm::ivec3 tileCoord = newLocation.getTileGroup();
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

	newLocation.setChunk(chunkCoord);
	newLocation.setTile(tileCoord);

	WorldManager* worldManager = GameObjectProvider::getWorldManager();
	CharacterProvider* characterProvider = GameObjectProvider::getCharacterProvider();
	AelaGame* game = GameObjectProvider::getGame();

	if (newLocation.getWorld() < worldManager->getNumberOfWorlds()) {
		Chunk* chunk = worldManager->getChunk(&newLocation);
		GameMode mode = game->getGameMode();
		Character* playerCharacter = GameObjectProvider::getPlayer()->getCharacter();

		if (mode == GameMode::GAMEPLAY) {
			if (chunk != nullptr) {
				TileGroup* tileGroup = worldManager->getTileGroup(&newLocation);

				if (tileGroup == nullptr || tileGroup->isCollidable(GameObjectProvider::getTileAtlas())) {
					// If the movement is impossible, don't do it!
					turn(direction);
					return; 
				}

				if (characterProvider->getCharacterByLocation(&newLocation) != nullptr) {
					// If the movement is impossible, don't do it!
					turn(direction);
					return;
				}

				std::string script = *tileGroup->getWalkedOnScriptID();
				Movement movement(&newLocation, &translation, direction, false);
				move(&movement, script);

				Teleporter* teleporter = worldManager->getTeleporter(&newLocation);
				if (teleporter != nullptr) {
					Movement teleportation(teleporter->getDestination(), &glm::vec3(), directionFacing, true,
						TeleportationAnimation::RISE);
					move(&teleportation, script);
				}
			}
		} else if (mode == GameMode::MAP_EDITOR) {
			if (chunk != nullptr) {
				TileGroup* tileGroup = worldManager->getTileGroup(&newLocation);

				if (tileGroup == nullptr) {
					if (this == playerCharacter) {
						GameObjectProvider::getHintDisplay()->displayHint("(Does not exist yet) Chunk: ("
							+ std::to_string(chunkCoord.x) + ", " + std::to_string(chunkCoord.y) + ") Tile: ("
							+ std::to_string(tileCoord.x) + ", " + std::to_string(tileCoord.y) + ", "
							+ std::to_string(tileCoord.z) + ")", HintDisplayDuration::FOREVER);
					}
				} else if (this == playerCharacter) {
					GameObjectProvider::getHintDisplay()->displayHint("Chunk: (" + std::to_string(chunkCoord.x) + ", "
						+ std::to_string(chunkCoord.y) + ") Tile: (" + std::to_string(tileCoord.x) + ", "
						+ std::to_string(tileCoord.y) + ", " + std::to_string(tileCoord.z) + ")", HintDisplayDuration::FOREVER);
				}

				Movement movement(&newLocation, &translation, direction, false);
				move(&movement, "");
			} else {
				if (this == playerCharacter) {
					GameObjectProvider::getHintDisplay()->displayHint("(Does not exist yet) Chunk: ("
						+ std::to_string(chunkCoord.x) + ", " + std::to_string(chunkCoord.y) + ") Tile: ("
						+ std::to_string(tileCoord.x) + ", " + std::to_string(tileCoord.y) + ", "
						+ std::to_string(tileCoord.z) + ")", HintDisplayDuration::FOREVER);
				}

				Movement movement(&newLocation, &translation, direction, false);
				move(&movement, "");
			}
		}
	}
}