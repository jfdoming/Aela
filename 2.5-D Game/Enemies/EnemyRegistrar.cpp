#include "EnemyRegistrar.h"

Game::EnemyRegistrar::EnemyRegistrar() {}

void Game::EnemyRegistrar::updateRegisteredEnemies() {
	for (Turret* turret : turrets) {
		turret->update(player->getCharacter(), time->getCurrentTimeInNanos());
		if (turret->hasRecentlyAttacked()) {
			turret->addBulletEffects(scene, resourceManager, time);
		}
	}
}

size_t Game::EnemyRegistrar::registerTurret(Turret* turret) {
	turrets.push_back(turret);
	return turrets.size() - 1;
}

Game::Turret* Game::EnemyRegistrar::getTurret(size_t id) {
	return turrets[id];
}

void Game::EnemyRegistrar::setAelaObjects(Engine* engine) {
	this->time = engine->getTime();
	this->resourceManager = engine->getResourceManager();
}

void Game::EnemyRegistrar::setPlayer(Player* player) {
	this->player = player;
}

void Game::EnemyRegistrar::setGameplayScene(Scene* scene) {
	this->scene = scene;
}
