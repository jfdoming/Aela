/*
* Class: Turret
* Author: Robert Ciborowski
* Date: 06/04/2018
* Description: A class which represents an enemy that attacks if
*              the player is within its line of sight.
*/

#include "Turret.h"
#include "../Chunks/Chunk.h"
#include "../Particles/LaserParticleEmitter.h"
#include "../Particles/TileSwitchParticleEmitter.h"

Game::Turret::Turret() {
	setDetectionAngles(true, true, true, true);
}

Game::Turret::Turret(std::string name, Location* location, int detectionRange) : Enemy(name, location) {
	setDetectionAngles(true, true, true, true);
	this->detectionRange = detectionRange;
}

Game::Turret::Turret(std::string name, Location* location, int detectionRange, int strength, long long attackCooldown) : Turret(name, location, detectionRange) {
	this->strength = strength;
	this->attackCooldown = attackCooldown;
}

Game::Turret::Turret(std::string name, Location* location, int detectionRange, bool detectRight, bool detectForward, bool detectLeft, bool detectBackward, int strength,
	long long attackCooldown) : Turret(name, location, detectionRange, strength, attackCooldown) {
	setDetectionAngles(detectRight, detectForward, detectLeft, detectBackward);
}

void Game::Turret::update(Character* playerCharacter, long long time) {
	recentlyAttacked = false;
	if (hostile && isInLineOfSight(playerCharacter->getLocationBeforeAnimation())) {
		if (attackIfPossible(playerCharacter, time)) {
			recentlyAttacked = true;
		}
	}
}

bool Game::Turret::isInLineOfSight(Location* location) {
	if (location->getWorld() != this->location.getWorld()) {
		return false;
	}

	if (location->getTileGroup().x == this->location.getTileGroup().x && this->location.getChunk().x == location->getChunk().x) {
		int differenceInZ = this->location.getChunk().y * CHUNK_LENGTH + this->location.getTileGroup().z
			- location->getChunk().y * CHUNK_LENGTH - location->getTileGroup().z;
		if ((differenceInZ > 0 && detectionAngles[TileDirection::BACKWARD] && differenceInZ <= detectionRange) || (differenceInZ < 0 &&
			detectionAngles[TileDirection::FORWARD] && differenceInZ >= -detectionRange)) {
			return true;
		}
	}

	if (location->getTileGroup().z == this->location.getTileGroup().z && this->location.getChunk().y == location->getChunk().y) {
		int differenceInX = this->location.getChunk().x * CHUNK_WIDTH + this->location.getTileGroup().x
			- location->getChunk().x * CHUNK_WIDTH - location->getTileGroup().x;
		if ((differenceInX > 0 && detectionAngles[TileDirection::RIGHT] && differenceInX <= detectionRange) || (differenceInX < 0 &&
			detectionAngles[TileDirection::LEFT] && differenceInX >= -detectionRange)) {
			return true;
		}
	}

	return false;
}

void Game::Turret::setDetectionRange(unsigned int detectionRange) {
	this->detectionRange = detectionRange;
}

void Game::Turret::setDetectionAngle(TileDirection direction, bool detect) {
	detectionAngles[direction] = detect;
}

void Game::Turret::setDetectionAngles(bool detectRight, bool detectForward, bool detectLeft, bool detectBackward) {
	detectionAngles[TileDirection::RIGHT] = detectRight;
	detectionAngles[TileDirection::FORWARD] = detectForward;
	detectionAngles[TileDirection::LEFT] = detectLeft;
	detectionAngles[TileDirection::BACKWARD] = detectBackward;
}

bool Game::Turret::hasRecentlyAttacked() {
	return recentlyAttacked;
}

Game::EnemyType Game::Turret::getType() {
	return EnemyType::TURRET;
}

void Game::Turret::update() {
		
}

void Game::Turret::addBulletEffects(Scene* gameplayScene, ResourceManager* resourceManager, Time* time) {
	std::cout << "Getting\n";
	std::vector<GLTexture*> textures;
	GLTexture* texture;
	if (!resourceManager->obtain<GLTexture>((std::string) "res/particles/laser.dds", texture)) {
		AelaErrorHandling::consoleWindowError("Could not obtain res/particles/laser.dds");
		return;
	}
	textures.push_back(texture);

	for (auto direction : detectionAngles) {
		if (!direction.second) {
			continue;
		}
		LaserParticleEmitter* particleEmitter = new LaserParticleEmitter(time);
		particleEmitter->setBaseDistance(10);
		particleEmitter->setBaseSpeed(0.00000002f);
		particleEmitter->setPathOffset(0);
		particleEmitter->setLifeTime(attackCooldown);
		particleEmitter->setPosition(location.getWorldSpaceLocation());
		particleEmitter->setDirection(direction.first);

		particleEmitter->setupParticles(&textures, 1, 1, detectionRange);
		gameplayScene->putParticleEmitter(particleEmitter);

		/*glm::vec3 worldSpacePosition = location.getWorldSpaceLocation();
		TileSwitchParticleEmitter* particleEmitter = new TileSwitchParticleEmitter(time);
		particleEmitter->setBaseDistance(10);
		particleEmitter->setBaseSpeed(0.00000002f);
		particleEmitter->setPathOffset(worldSpacePosition.y);
		particleEmitter->setLifeTime(500000000);
		particleEmitter->setupDimensions(&Rect<float>(worldSpacePosition.x, worldSpacePosition.z, 1, 1));

		particleEmitter->setupParticles(&textures, 1, 1, 1);
		gameplayScene->putParticleEmitter(particleEmitter);*/
	}
}
