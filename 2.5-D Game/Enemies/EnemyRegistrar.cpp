#include "EnemyRegistrar.h"
#include "../Player/Player.h"

using namespace Game;

Game::EnemyRegistrar::EnemyRegistrar() {
	time = GameObjectProvider::getTime();
	resourceManager = GameObjectProvider::getResourceManager();
}

void Game::EnemyRegistrar::setup() {
	player = GameObjectProvider::getPlayer();
}

void Game::EnemyRegistrar::scenesWereSetUp() {
	gameplayScene = GameObjectProvider::getGameplayScene();
}

void Game::EnemyRegistrar::updateRegisteredEnemies() {
	for (Turret* turret : turrets) {
		turret->update(player->getCharacter(), time->getCurrentTimeInNanos());
		if (turret->hasRecentlyAttacked()) {
			turret->addBulletEffects(gameplayScene, resourceManager, time);
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