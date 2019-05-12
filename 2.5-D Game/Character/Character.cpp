#include "Character.h"
#include "../Worlds/WorldManager.h"
#include "../Particles/TileSwitchParticleEmitter.h"
#include "../Particles/CharacterTeleportParticleEmitter.h"
#include "../Particles/CharacterDeathParticleEmitter.h"
#include "../Player/Player.h"
#include "../Aela Game/AelaGame.h"
#include "CharacterModelGenerator.h"
#include "../Scripts/ScriptManager.h"
#include "../Displays/Hints/HintDisplay.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include "../../Project Aela/Utilities/enumut.h"
#include "../../Project Aela/Time/Timer/Timer.h"
#include <iostream>
#include <utility>
#include "../Movement/WalkingMovement.h"
#include "../Movement/TurningMovement.h"
#include "../Movement/TeleportationMovement.h"
#include "../Movement/TileMovement.h"
#include "CharacterHealthEvent.h"

using namespace Game;

Game::Character::Character() {
	// According to Platinum, it should be 0.00375.
	walkingSpeed = 0.00375f;
	// According to Platinum, it should be 0.0075, but that feels too fast.
	runningSpeed = 0.00575f;
	directionFacing = TileDirection::BACKWARD;
	health = 1;
	maxHealth = 1;
}

Game::Character::Character(std::string name) : Character() {
	this->name = std::move(name);
}

Game::Character::Character(std::string name, const Location& location) : Character() {
	this->name = std::move(name);
	this->location = location;
}

Game::Character::Character(std::string name, const Location& location, float walkingSpeed, float runningSpeed) : Character(
		std::move(name), location) {
	this->walkingSpeed = walkingSpeed;
	this->runningSpeed = runningSpeed;
}

Character::~Character() {
}

void Character::changeLocation(Location& location) {
	GameObjectProvider::getCharacterProvider()->characterWasMoved(name, &this->location, &location);
	this->location = location;
}

void Game::Character::turnFreely(TileDirection direction) {
	if (movementQueue.empty() && newMovementsAreAllowed) {
		turn(direction);
	}
}

void Character::turn(TileDirection direction) {
	// if (newMovementsAreAllowed) {
	if (direction != TileDirection::UP && direction != TileDirection::DOWN && direction != TileDirection::NONE) {
		directionFacing = direction;
		changeModel();
	}

	// }
}

void Character::startMovingInDirection(TileDirection direction) {
	if (movementQueue.empty()) {
		movingFreely = true;
		startMovement(direction);
	}
}

void Character::addTileMovementToQueue(TileDirection direction) {
	addTileMovementToQueue(direction, false);
}

void Game::Character::addTileMovementToQueue(TileDirection direction, bool running) {
	TileMovement* movement = new TileMovement(direction, true, running);
	movementQueue.push((Movement*) movement);
}

void Character::addTileMovementsToQueue(std::queue<TileDirection>& directions) {
	addTileMovementsToQueue(directions, false);
}

void Game::Character::addTileMovementsToQueue(std::queue<TileDirection>& directions, bool running) {
	std::queue<TileDirection> directions2 = directions;
	while (!directions2.empty()) {
		TileMovement* movement = new TileMovement(directions2.front(), true, running);
		movementQueue.push((Movement*) movement);
		directions2.pop();
	}
}

void Character::addMovementToQueue(TileDirection direction, float magnitude) {
	addMovementToQueue(direction, magnitude, false);
}

void Game::Character::addMovementToQueue(TileDirection direction, float magnitude, bool running) {
	WalkingMovement* movement = new WalkingMovement(magnitude, direction, true, running);
	movementQueue.push((Movement*) movement);
}

void Character::addMovementsToQueue(std::queue<TileDirection>& directions, std::queue<float>& magnitudes) {
	addMovementsToQueue(directions, magnitudes, false);
}

void Game::Character::addMovementsToQueue(std::queue<TileDirection>& directions, std::queue<float>& magnitudes, bool running) {
	std::queue<TileDirection> directions2 = directions;
	std::queue<float> magnitudes2 = magnitudes;
	while (!directions2.empty()) {
		WalkingMovement* movement = new WalkingMovement(magnitudes2.front(), directions2.front(), true, running);
		movementQueue.push((Movement*) movement);
		directions2.pop();
		magnitudes2.pop();
	}
}

void Character::addNoCollisionTileMovementToQueue(TileDirection direction) {
	addNoCollisionTileMovementToQueue(direction, false);
}

void Game::Character::addNoCollisionTileMovementToQueue(TileDirection direction, bool running) {
	TileMovement* movement = new TileMovement(direction, false, running);
	movementQueue.push((Movement*) movement);
}

void Character::addNoCollisionMovementToQueue(TileDirection direction, float magnitude) {
	addNoCollisionMovementToQueue(direction, magnitude, false);
}

void Game::Character::addNoCollisionMovementToQueue(TileDirection direction, float magnitude, bool running) {
	WalkingMovement* movement = new WalkingMovement(magnitude, direction, false, running);
	movementQueue.push((Movement*) movement);
}

void Character::addTurnToQueue(TileDirection direction) {
	TurningMovement* movement = new TurningMovement(direction);
	movementQueue.push((Movement*) movement);
}

void Character::addTeleportToQueue(Location& location, TeleportationAnimation animation) {
	addTeleportToQueue(location, animation, "");
}

void Character::addTeleportToQueue(Location& location, TeleportationAnimation animation, std::string scriptOnHalfLife) {
	TeleportationMovement* movement = new TeleportationMovement(location, animation);
	movement->setScriptOnHalfLife(scriptOnHalfLife);
	movementQueue.push((Movement*) movement);
}

void Character::clearMovementQueue() {
	while (!movementQueue.empty()) {
		delete movementQueue.front();
		movementQueue.pop();
	}

	stopMoving();
}

void Game::Character::stopMoving() {
	movementDirection = TileDirection::NONE;
	moving = false;
	movingFreely = false;
	bothFeetOnGround();
}

// If I were to use the version below, the soldiers and scientists wont move.
/*void Character::clearMovementQueue() {
	while (!movementQueue.empty()) {
		delete movementQueue.front();
		movementQueue.pop();
	}

	if (!movingFreely) {
		movementDirection = TileDirection::NONE;
		moving = false;
		movingFreely = false;
		bothFeetOnGround();
	}
}

void Game::Character::stopMoving() {
	movementDirection = TileDirection::NONE;
	moving = false;
	movingFreely = false;
	bothFeetOnGround();

	while (!movementQueue.empty()) {
		delete movementQueue.front();
		movementQueue.pop();
	}
}*/

void Character::startMovement(TileDirection direction) {
	if (newMovementsAreAllowed) {
		movementDirection = direction;
		moving = true;
		timeSinceLastMovement = GameObjectProvider::getTime()->getCurrentTimeInMillis();

		// This will make sure the step changes next time update() is called.
		timeSinceLastCharacterStep = 0;
	}
}

void Character::translate(glm::vec3 vector) {
	Location location2 = location;
	location2.translateByAmount(vector);
	glm::ivec3 locationTileGroupAsInts = location.getTileGroup();
	glm::vec3 locationTileGroup = location.getTileGroup();
	glm::vec3 location2TileGroup = location2.getTileGroup();
	// float location2X = abs(location2Z);
	// float location2Z = abs(location2TileGroup.z);
	float xRemainder = location2TileGroup.x - floor(location2TileGroup.x);
	float zRemainder = location2TileGroup.z - floor(location2TileGroup.z);
	unsigned long long currentTime = GameObjectProvider::getTime()->getCurrentTimeInMillis();
	WorldManager* worldManager = GameObjectProvider::getWorldManager();

	if (collidable) {
		// This checks to see if the leftmost collision point has entered a new tile. If so, we perform a collision check.
		if (movementDirection == TileDirection::LEFT && (int) floor((location2TileGroup + collisionPoints[0]).x) != locationTileGroupAsInts.x) {
			for (auto collisionVector : collisionPoints) {
				Location location3 = location2;
				location3 += collisionVector;
				if (collisionVector != collisionPoints[0]) {
					location3 += collisionPoints[0];
				}

				TileGroup* tileGroup = worldManager->getTileGroup(location3);
				if (tileGroup == nullptr) {
					continue;
				}

				if ((collisionVector == collisionPoints[2] && tileGroup->containsCollidableNonSwitchableTile() || collisionVector != collisionPoints[2]) && collisionCheck(location3)) {
					// vector.x -= collisionPoints[0].x + xRemainder - 0.999f;
					vector.x = floor(locationTileGroup.x) - locationTileGroup.x + 0.999f - collisionPoints[0].x;
					location += vector;
					entity->setPosition(*entity->getPosition() + vector);
					timeSinceLastMovement = currentTime;
					stopMoving();
					return;
				}
			}
		}

		// This checks to see if the rightmost collision point has entered a new tile. If so, we perform a collision check.
		if (movementDirection == TileDirection::RIGHT && (int) floor((location2TileGroup + collisionPoints[1]).x) != locationTileGroupAsInts.x) {
			// This doesn't get triggered until its too late???
			for (auto collisionVector : collisionPoints) {
				Location location3 = location2;
				location3 += collisionVector;
				if (collisionVector != collisionPoints[1]) {
					location3 += collisionPoints[1];
				}

				TileGroup* tileGroup = worldManager->getTileGroup(location3);
				if (tileGroup == nullptr) {
					continue;
				}

				if ((collisionVector == collisionPoints[2] && tileGroup->containsCollidableNonSwitchableTile() || collisionVector != collisionPoints[2]) && collisionCheck(location3)) {
					vector.x = floor(locationTileGroup.x) - locationTileGroup.x - collisionPoints[1].x + 0.001f;
					location += vector;
					entity->setPosition(*entity->getPosition() + vector);
					timeSinceLastMovement = currentTime;
					stopMoving();
					return;
				}
			}
		}

		// This checks to see if the furthest (greatest z value) collision point has entered a new tile. If so, we perform a collision check.
		if (movementDirection == TileDirection::FORWARD && (int) floor((location2TileGroup + collisionPoints[2]).z) != locationTileGroupAsInts.z) {
			for (auto collisionVector : collisionPoints) {
				Location location3 = location2;
				location3 += collisionVector;
				if (collisionVector != collisionPoints[2]) {
					location3 += collisionPoints[2];
				}

				TileGroup* tileGroup = worldManager->getTileGroup(location3);
				if (tileGroup == nullptr) {
					continue;
				}

				// (collisionVector == collisionPoints[2] && tileGroup->containsCollidableNonSwitchableTile() || collisionVector != collisionPoints[2])
				if (tileGroup->containsCollidableNonSwitchableTile() && collisionCheck(location3)) {
					// vector.z -= collisionPoints[2].z + zRemainder - 0.999f;
					vector.z = floor(locationTileGroup.z) - locationTileGroup.z + 0.999f - collisionPoints[2].z;
					location += vector;
					entity->setPosition(*entity->getPosition() + vector);
					timeSinceLastMovement = currentTime;
					stopMoving();
					return;
				}

				location3 -= collisionPoints[2];

				if (collisionVector != collisionPoints[2] && collisionCheck(location3)) {
					vector.z = floor(locationTileGroup.z) - locationTileGroup.z + 0.999f - collisionVector.z;
					location += vector;
					entity->setPosition(*entity->getPosition() + vector);
					timeSinceLastMovement = currentTime;
					stopMoving();
					return;
				}
			}

			if (collisionCheck(location2)) {
				glm::vec3 tileGroup = location.getTileGroup();
				tileGroup.z = floor(tileGroup.z) + 0.999f;
				entity->setPosition(*entity->getPosition() + tileGroup - location.getTileGroup());
				location.setTileGroup(tileGroup);
				timeSinceLastMovement = currentTime;
				stopMoving();
				return;
			}
		}

		// This checks to see if the closest (least z value) collision point has entered a new tile. If so, we perform a collision check.
		if (movementDirection == TileDirection::BACKWARD && (int) floor((location2TileGroup + collisionPoints[3]).z) != locationTileGroupAsInts.z) {
			for (auto collisionVector : collisionPoints) {
				Location location3 = location2;
				location3 += collisionVector;
				if (collisionVector != collisionPoints[3]) {
					location3 += collisionPoints[3];
				}

				TileGroup* tileGroup = worldManager->getTileGroup(location3);
				if (tileGroup == nullptr) {
					continue;
				}

				if ((collisionVector == collisionPoints[2] && tileGroup->containsCollidableNonSwitchableTile() || collisionVector != collisionPoints[2]) && collisionCheck(location3)) {
					vector.z = floor(locationTileGroup.z) - locationTileGroup.z - collisionPoints[3].z + 0.001f;
					location += vector;
					entity->setPosition(*entity->getPosition() + vector);
					timeSinceLastMovement = currentTime;
					stopMoving();
					return;
				}
			}
		}
	}

	// The following is used to see if the character's center entered a new tile.
	glm::ivec3 locationTileGroupAsIntegers = location.getTileGroup();
	glm::ivec3 location2TileGroupAsIntegers = location2.getTileGroup();
	Teleporter* teleporter = nullptr;

	// This switches the character step, if necessary.
	if (running && currentTime > timeSinceLastCharacterStep + TIME_BETWEEN_CHARACTER_STEPS_RUNNING
		|| !running && currentTime > timeSinceLastCharacterStep + TIME_BETWEEN_CHARACTER_STEPS_WALKING) {
		switchStep();
	}

	Location oldLocation = location;
	location = location2;
	entity->setPosition(*entity->getPosition() + vector);
	timeSinceLastMovement = currentTime;

	std::cout << name << " is at " << location << "\n";

	if (locationTileGroupAsIntegers != location2TileGroupAsIntegers) {
		// Since the character changed what tile they are on, we must let the CharacterProvider know.
		GameObjectProvider::getCharacterProvider()->characterWasMoved(name, &oldLocation, &location2);
		GameObjectProvider::getWorldManager()->runWalkedScriptOfTile(location2);
	}

	teleporter = worldManager->getTeleporter(location2);
	if (teleporter != nullptr) {
		float remainderX = location2.getTileGroup().x - floor(location2.getTileGroup().x);
		float remainderZ = location2.getTileGroup().z - floor(location2.getTileGroup().z);

		if (remainderX > TELEPORTER_ACTIVATION_MIN_X && remainderX < TELEPORTER_ACTIVATION_MAX_X
			&& remainderZ > TELEPORTER_ACTIVATION_MIN_Z && remainderZ < TELEPORTER_ACTIVATION_MAX_Z) {
			addTeleportToQueue(*teleporter->getDestination(), TeleportationAnimation::RISE);
		}
	}
}

void Game::Character::setLocation(Location* location) {
	this->location = *location;
}

void Game::Character::animateDeath() {
	float offset = GameObjectProvider::getWorldManager()->getCharacterYOffsetInWorldspace();

	auto* particleEmitter = new CharacterDeathParticleEmitter(GameObjectProvider::getTime());
	particleEmitter->setBaseDistance(location.getWorldSpaceLocation().y + offset + 1);
	particleEmitter->setBaseSpeed(0.00000001f);
	particleEmitter->setPathOffset(location.getWorldSpaceLocation().y + offset);
	particleEmitter->setLifeTime(100000000);
	particleEmitter->setCharacter(this);
	particleEmitter->setLocation(&location);

	std::vector<GLTexture*> textures;
	auto* texture = dynamic_cast<GLTexture*>(entity->getModel()->getSubModels()->at(0).getMaterial()->getTexture());
	textures.push_back(texture);

	if (this == GameObjectProvider::getPlayer()->getCharacter()) {
		auto endingAction = [this]() {
			GameObjectProvider::getGame()->animatePlayerDeathScreen();
		};

		particleEmitter->setActionOnEnd(endingAction);
	}

	particleEmitter->setupParticles(&textures, 1, 1, 1);
	GameObjectProvider::getGameplayScene()->putParticleEmitter(particleEmitter);

	newMovementsAreAllowed = false;
}

void Game::Character::generateModel() {
	ResourceManager* resourceManager = GameObjectProvider::getResourceManager();
	std::string groupName = "ch/" + std::to_string((long long) this) + "/" + textureName;

	if (resourceManager->groupExists(groupName)) {
		return;
	}

	if (this == GameObjectProvider::getPlayer()->getCharacter()) {
		generatePlayerModel();
		return;
	}
	
	std::unordered_map<size_t, Character*> characterList;
	characterList[0] = this;

	CharacterModelGenerator generator;
	generator.setCharacters(&characterList);
	
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
				std::cerr << "Failed to load a resource from group \"characters\": " << resourceManager->getNewInvalidResourceKeys()[0] << "\n";
				break;
			}
		}
	}
}

void Character::generatePlayerModel() {
	ResourceManager* resourceManager = GameObjectProvider::getResourceManager();
	std::unordered_map<size_t, Character*> characterList;
	characterList[0] = this;

	CharacterModelGenerator generator;
	generator.setCharacters(&characterList);

	std::string groupName = "ch/" + name + "/" + textureName;
	std::string modelA = (std::string) DEFAULT_MODEL_PATH + "character_A.obj";
	std::string modelB = (std::string) DEFAULT_MODEL_PATH + "character_B.obj";
	std::string modelC = (std::string) DEFAULT_MODEL_PATH + "character_C.obj";
	std::string modelD = (std::string) DEFAULT_MODEL_PATH + "character_D.obj";

	resourceManager->bindLoader(&generator);
	resourceManager->bindGroup(groupName);

	for (unsigned int y = 0; y < 4; y++) {
		for (unsigned int x = 0; x < 3; x++) {
			std::string modelName = (std::string) DEFAULT_MODEL_PATH + "player_" + std::to_string(x) + "_" + std::to_string(y) + ".obj";
			generator.setTemplateModelSource(modelName);

			generator.setSpriteSheetX(x);
			generator.setSpriteSheetY(y);

			// We want to generate one set of models, so we'll tell the resource manager to run the generator once.
			resourceManager->addToGroup("", false);

			if (resourceManager->loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
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
	this->name = std::move(name);
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

bool Game::Character::isRunning() {
	return running;
}

void Game::Character::setTexture(std::string textureName) {
	this->textureName = std::move(textureName);
	changeModel();
}

std::string Game::Character::getTextureName() {
	return textureName;
}

Game::TileDirection Game::Character::getDirectionFacing() {
	return directionFacing;
}

TileDirection Character::getOppositeDirectionFacing() {
	switch (directionFacing) {
		case TileDirection::FORWARD:
			return TileDirection::BACKWARD;
		case TileDirection::BACKWARD:
			return TileDirection::FORWARD;
		case TileDirection::LEFT:
			return TileDirection::RIGHT;
		case TileDirection::RIGHT:
			return TileDirection::LEFT;
		default:
			return TileDirection::BACKWARD;
	}
}

TileDirection Character::getMovementDirection() {
	return movementDirection;
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
	switch (currentStep) {
		case CharacterStep::LEFT:
			currentStep = CharacterStep::POST_LEFT;
			break;
		case CharacterStep::POST_LEFT:
			currentStep = CharacterStep::RIGHT;
			break;
		case CharacterStep::RIGHT:
			currentStep = CharacterStep::POST_RIGHT;
			break;
		case CharacterStep::POST_RIGHT:
			currentStep = CharacterStep::LEFT;
			break;
	}

	playFootstepSound();
	changeModel();
	timeSinceLastCharacterStep = GameObjectProvider::getTime()->getCurrentTimeInMillis();
}

void Character::bothFeetOnGround() {
	switch (currentStep) {
		case CharacterStep::LEFT:
			currentStep = CharacterStep::POST_LEFT;
			break;
		case CharacterStep::RIGHT:
			currentStep = CharacterStep::POST_RIGHT;
			break;
		default:
			break;
	}

	// playFootstepSound();
	changeModel();
	timeSinceLastCharacterStep = GameObjectProvider::getTime()->getCurrentTimeInMillis();
}

void Character::playFootstepSound() {
	TileGroup* tileGroup = GameObjectProvider::getWorldManager()->getTileGroup(location);
	Tile* tile;
	if (tileGroup != nullptr) {
		tile = tileGroup->getFloorTile();
		if (tile != nullptr) {
			GameObjectProvider::getFootstepAudioPlayer()->playSound(tile->getType());
		}
	}
}

bool Game::Character::isMoving() {
	return moving;
}

bool Character::isMovingFreely() {
	return movingFreely;
}

bool Character::hasMovementsInQueue() {
	return !movementQueue.empty();
}

void Game::Character::kill() {
	alive = false;

	if (GameObjectProvider::getGame()->isAnimatingDeaths()) {
		animateDeath();
	}

	if (this == GameObjectProvider::getPlayer()->getCharacter()) {
		GameObjectProvider::getAudioLooper()->stopAllLoops();
	}

	try {
		onKill();
	} catch (std::bad_function_call e) {
		// If you reach this point, onKill probably wasn't set.
		// Q: Why don't I just do "if (onKill)" instead of a try-catch?
		// A: Because it doesn't seem to work.
	}

	GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
}

void Character::killWithoutAnimation() {
	alive = false;

	if (this == GameObjectProvider::getPlayer()->getCharacter()) {
		GameObjectProvider::getAudioLooper()->stopAllLoops();
	}

	try {
		onKill();
	} catch (std::bad_function_call e) {
		// If you reach this point, onKill probably wasn't set.
		// Q: Why don't I just do "if (onKill)" instead of a try-catch?
		// A: Because it doesn't seem to work.
	}

	GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
}

void Game::Character::revive() {
	alive = true;
	health = maxHealth;

	if (this == GameObjectProvider::getPlayer()->getCharacter()) {
		GameObjectProvider::getGame()->hideDeathScreen();
	}

	GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
	GameObjectProvider::getEventHandler()->fireEvent(new CharacterHealthEvent(this));
}

void Character::setOnKill(std::function<void()> onKill) {
	this->onKill = onKill;
}

bool Game::Character::isAlive() {
	return alive;
}

void Game::Character::setPropertiesUsingCharacterInformationBlock(CharacterInformationBlock* block) {
	entity = nullptr;
	// clearAllMovements();
	alive = block->alive;
	// directionFacing = block->directionFacing;
	turn(block->directionFacing);
	changeLocation(block->location);
	running = false;
	runningSpeed = block->runningSpeed;
	visible = block->visible;
	walkingSpeed = block->walkingSpeed;
	moving = false;
	newMovementsAreAllowed = false;

	// This creates a pause for a moment, letting the player breathe and realise what their new surroundings are.
	// This delay will be moved elsewhere once menus work.
	auto allowNewMovements = [this]() {
		this->allowNewMovements(true);
	};
	GameObjectProvider::getTimer()->scheduleEventInMillis(1000, allowNewMovements);
}

CharacterInformationBlock Game::Character::getCharacterInformationBlock() {
	CharacterInformationBlock block;
	block.alive = alive;
	block.directionFacing = directionFacing;
	block.location = location;
	block.moving = moving;
	block.newMovementsAreAllowed = newMovementsAreAllowed;
	// block.possibleMovementsToProcess = possibleMovementsToProcess;
	block.running = running;
	block.runningSpeed = runningSpeed;
	// block.translations = translations;
	block.visible = visible;
	block.walkingSpeed = walkingSpeed;
	return block;
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

float Character::getHealthLeftAsFraction() {
	return (float) health / maxHealth;
}

void Character::setMoney(int money) {
	this->money = money;
}

int Character::getMoney() {
	return money;
}

bool Character::increaseMoney(int amount) {
	money += amount;

	if (money < 0) {
		money = 0;
		return false;
	}

	return true;
}

bool Character::decreaseMoney(int amount) {
	return increaseMoney(-amount);
}

bool Character::makePurchase(int cost) {
	if (money - cost >= 0) {
		money -= cost;
		return true;
	}
	return false;
}

void Game::Character::increaseHealth(int amount) {
	health += amount;
	if (health > maxHealth) {
		health = maxHealth;
	}
	GameObjectProvider::getEventHandler()->fireEvent(new CharacterHealthEvent(this));
}

void Game::Character::decreaseHealth(int amount) {
	if (alive) {
		health -= amount;
		if (health < 0) {
			health = 0;
			kill();
		}
	}
	GameObjectProvider::getEventHandler()->fireEvent(new CharacterHealthEvent(this));
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
	if (entity != nullptr) {
		entity->setVisibility(visible);
	}
}

bool Game::Character::isVisible() {
	return visible;
}

void Game::Character::allowNewMovements(bool newMovementsAreAllowed) {
	this->newMovementsAreAllowed = newMovementsAreAllowed;

	if (!newMovementsAreAllowed) {
		stopMoving();
	}
}

bool Game::Character::areNewMovementsAllowed() {
	return newMovementsAreAllowed;
}

void Game::Character::setCollidable(bool collidable) {
	this->collidable = collidable;
}

bool Game::Character::isCollidable() {
	return collidable;
}

void Game::Character::toggleCollidability() {
	collidable = !collidable;
}

bool Character::hasAModel() {
	return baseModel != nullptr;
}

void Game::Character::update() {
	if (moving) {
		if (movingFreely && !movementQueue.empty()) {
			// If the character is being moved freely but has movementQueue in its movement queue, it should
			// stop moving freely and process the movementQueue in the queue.
			stopMoving();
		}
		
		if (movingFreely) {
			// If this code is reached, the character is being moved by startMovingInDirection() and
			// is most likely being controlled by the player.
			unsigned long long currentTime = GameObjectProvider::getTime()->getCurrentTimeInMillis();
			glm::vec3 translation;

			switch (movementDirection) {
				case TileDirection::RIGHT:
					translation.x -= (currentTime - timeSinceLastMovement) * getCurrentSpeed();
					break;
				case TileDirection::FORWARD:
					translation.z += (currentTime - timeSinceLastMovement) * getCurrentSpeed();
					break;
				case TileDirection::LEFT:
					translation.x += (currentTime - timeSinceLastMovement) * getCurrentSpeed();
					break;
				case TileDirection::BACKWARD:
					translation.z -= (currentTime - timeSinceLastMovement) * getCurrentSpeed();
					break;
			}

			if (translation == glm::vec3()) {
				return;
			}

			translate(translation);
		}
	}
	
	if (!movementQueue.empty()) {
		// The character is being moved by a movement in the movement queue.
		Movement* movement = movementQueue.front();

		if (!movement->isStarted()) {
			movement->start(this);
			startMovement(movement->getDirection());
			moving = true;
		}

		if (movement->moveCharacter(this)) {
			// The movement has just ended!
			delete movement;
			movementQueue.pop();
			stopMoving();
		}
	} else {
	}	
}

void Character::changeModel() {
	Model* model;

	std::string step;
	if (currentStep == CharacterStep::LEFT) {
		step = "1";
	} else if (currentStep == CharacterStep::RIGHT) {
		step = "2";
	} else {
		step = "0";
	}

	std::string modelName = "ch/" + textureName + "/" + step + "/" + std::to_string(enumToInteger(directionFacing)) + "/mo";

	if (!GameObjectProvider::getResourceManager()->obtain<Model>(modelName, model)) {
		// The model has not been constructed yet.
		generateModel();

		if (!GameObjectProvider::getResourceManager()->obtain<Model>(modelName, model)) {
			// How did you get here? Something must have went terribly wrong!
			return;
		}
	}

	try {
		baseModel = model;

		if (entity != nullptr) {
			entity->setModel(model);
		}
	} catch (...) {
		// I don't know why, but the code above may cause a non-sensical exception when loading.
		entity->setModel(nullptr);
	}

	GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
}

glm::vec3 Character::getWorldSpaceTranslation(TileDirection direction) {
	switch (direction) {
		case TileDirection::RIGHT:
			return glm::vec3(-1, 0, 0);
		case TileDirection::FORWARD:
			return glm::vec3(0, 0, 1);
		case TileDirection::LEFT:
			return glm::vec3(1, 0, 0);
		case TileDirection::BACKWARD:
			return glm::vec3(0, 0, -1);
		case TileDirection::UP:
			return glm::vec3(0, 1, 0);
		case TileDirection::DOWN:
			return glm::vec3(0, -1, 0);
		default:
			return glm::vec3();
	}
}

bool Character::collisionCheck(Location& location) {
	if (!collidable) {
		return false;
	}

	WorldManager* worldManager = GameObjectProvider::getWorldManager();
	if (location.getWorld() < worldManager->getNumberOfWorlds()) {
		Chunk* chunk = worldManager->getChunk(location);
		GameMode mode = GameObjectProvider::getGame()->getGameMode();

		if (mode == GameMode::GAMEPLAY) {
			if (chunk != nullptr) {
				TileGroup* tileGroup = worldManager->getTileGroup(location);

				if (tileGroup == nullptr || (/*movement->isCollidable() && */ tileGroup->isCollidable())) {
					// If the movement is impossible, don't do it!
					return true; 
				}

				Character* character = GameObjectProvider::getCharacterProvider()->getCharacterByLocation(location);
				if (character != nullptr && character != this && character->isCollidable() && character->isAlive()) {
					// If the movement is impossible, don't do it!
					return true; 
				}
			} else {
				return true;
			}
		}
	}
	return false;
}
