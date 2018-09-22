#include <utility>

#include "Enemy.h"
#include "../Chunks/Chunk.h"
Game::Enemy::Enemy() = default;

Game::Enemy::Enemy(std::string name, const Location& location) : Character(std::move(name), location) {
	
}

Game::Enemy::Enemy(int strength, long long attackCooldown) {
	this->strength = strength;
	this->attackCooldown = attackCooldown;
}

Game::Enemy::Enemy(std::string name, const Location& location, int strength, long long attackCooldown) : Character(
		std::move(name), location) {
	this->strength = strength;
	this->attackCooldown = attackCooldown;
}

Game::Enemy::~Enemy() {
}

Game::EnemyType Game::Enemy::getType() {
	return EnemyType::GENERIC;
}

void Game::Enemy::setStrength(int strength) {
	this->strength = strength;
}

int Game::Enemy::getStrength() {
	return strength;
}

void Game::Enemy::setAttackCooldown(long long attackCooldown) {
	this->attackCooldown = attackCooldown;
}

bool Game::Enemy::attackIfPossible(Character* character) {
	const long long currentTime = GameObjectProvider::getTime()->getCurrentTimeInNanos();
	if (currentTime < timeOnLastAttack + attackCooldown) {
		return false;
	}
	attack(character);
	timeOnLastAttack = currentTime;
	return true;
}

bool Game::Enemy::isHostile() {
	return hostile;
}

void Game::Enemy::setHostile(bool hostile) {
	this->hostile = hostile;
}

void Game::Enemy::toggleHostility() {
	hostile = !hostile;
}

void Game::Enemy::addTarget(size_t characterID) {
	auto pos = std::find(targets.begin(), targets.end(), characterID);

	if (pos == targets.end()) {
		targets.push_back(characterID);
	}
}

void Game::Enemy::removeTarget(size_t characterID) {
	auto pos = std::find(targets.begin(), targets.end(), characterID);

	if (pos != targets.end()) {
		targets.erase(pos);
	}
}

void Game::Enemy::clearTargets() {
	targets.clear();
}

void Game::Enemy::update() {
	Character::update();
}

void Game::Enemy::attack(Character* character) {
	if (hostile) {
		character->decreaseHealth(strength);
	}
}

bool Game::Enemy::isInLineOfSight(Location* location, TileDirection direction, int detectionRange) {
	if (location->getWorld() != this->location.getWorld()) {
		return false;
	}

	if (location->getTileGroup().x == this->location.getTileGroup().x && this->location.getChunk().x == location->getChunk().x) {
		int differenceInZ = this->location.getChunk().y * CHUNK_LENGTH + this->location.getTileGroup().z
			- location->getChunk().y * CHUNK_LENGTH - location->getTileGroup().z;
		if ((differenceInZ > 0 && direction == TileDirection::BACKWARD && differenceInZ <= detectionRange) || (differenceInZ < 0 &&
			direction == TileDirection::FORWARD && differenceInZ >= -detectionRange)) {
			return true;
		}
	}

	if (location->getTileGroup().z == this->location.getTileGroup().z && this->location.getChunk().y == location->getChunk().y) {
		int differenceInX = this->location.getChunk().x * CHUNK_WIDTH + this->location.getTileGroup().x
			- location->getChunk().x * CHUNK_WIDTH - location->getTileGroup().x;
		if ((differenceInX > 0 && direction == TileDirection::RIGHT && differenceInX <= detectionRange) || (differenceInX < 0 &&
			direction == TileDirection::LEFT && differenceInX >= -detectionRange)) {
			return true;
		}
	}

	return false;
}
