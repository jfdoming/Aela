#include "Enemy.h"

Game::Enemy::Enemy() {}

Game::Enemy::Enemy(int strength, long long attackCooldown) {
	this->strength = strength;
	this->attackCooldown = attackCooldown;
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

bool Game::Enemy::attackIfPossible(Character* character, long long currentTime) {
	if (currentTime < timeOnLastAttack + attackCooldown) {
		return false;
	}
	character->decreaseHealth(strength);
	timeOnLastAttack = currentTime;
	std::cout << "Health: " << character->getHealth() << "\n";
	return true;
}

void Game::Enemy::update() {}
