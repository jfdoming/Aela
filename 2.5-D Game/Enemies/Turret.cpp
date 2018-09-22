#include <utility>

#include <utility>

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
#include "../Character/CharacterProvider.h"

Game::Turret::Turret() {
	setDetectionAngles(true, true, true, true);
}

Game::Turret::Turret(std::string name, const Location& location, unsigned int detectionRange) : Enemy(std::move(name), location) {
	setDetectionAngles(true, true, true, true);
	this->detectionRange = detectionRange;
}

Game::Turret::Turret(std::string name, const Location& location, unsigned int detectionRange, int strength, long long attackCooldown) : Turret(
		std::move(name), location, detectionRange) {
	this->strength = strength;
	this->attackCooldown = attackCooldown;
}

Game::Turret::Turret(std::string name, const Location& location, unsigned int detectionRange, bool detectRight, bool detectForward, bool detectLeft, bool detectBackward, int strength,
	long long attackCooldown) : Turret(std::move(name), location, detectionRange, strength, attackCooldown) {
	setDetectionAngles(detectRight, detectForward, detectLeft, detectBackward);
}

Game::Turret::~Turret() {
}

void Game::Turret::update() {
	Enemy::update();

	for (size_t i = 0; i < targets.size(); i++) {
		Character* character = GameObjectProvider::getCharacterProvider()->getCharacterByID(targets[i]);

		if (character == nullptr) {
			targets.erase(targets.begin() + i);
			continue;
		}

		if (!character->isAlive()) {
			continue;
		}

		const bool isInSight = (isInLineOfSight(character->getLocationBeforeAnimation(), TileDirection::RIGHT, detectionRange)
		&& detectionAngles[TileDirection::RIGHT])
		|| (isInLineOfSight(character->getLocationBeforeAnimation(), TileDirection::FORWARD, detectionRange)
			&& detectionAngles[TileDirection::FORWARD])
		|| (isInLineOfSight(character->getLocationBeforeAnimation(), TileDirection::LEFT, detectionRange)
			&& detectionAngles[TileDirection::LEFT])
		|| (isInLineOfSight(character->getLocationBeforeAnimation(), TileDirection::BACKWARD, detectionRange)
			&& detectionAngles[TileDirection::BACKWARD]);

		if (hostile && isInSight) {
			if (attackIfPossible(character)) {
				addBulletEffects(GameObjectProvider::getGameplayScene(), GameObjectProvider::getResourceManager(), GameObjectProvider::getTime());
			}
		}
	}
}

void Game::Turret::setDetectionRange(int detectionRange) {
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

Game::EnemyType Game::Turret::getType() {
	return EnemyType::TURRET;
}

void Game::Turret::addBulletEffects(Scene* gameplayScene, ResourceManager* resourceManager, Clock* time) {
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
		auto* particleEmitter = new LaserParticleEmitter(time);
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
