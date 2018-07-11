#include "EnemyProvider.h"
#include "../Player/Player.h"

using namespace Game;

Game::EnemyProvider::EnemyProvider() {
	time = GameObjectProvider::getTime();
	resourceManager = GameObjectProvider::getResourceManager();
}

void Game::EnemyProvider::setup() {
	player = GameObjectProvider::getPlayer();
}

void Game::EnemyProvider::scenesWereSetUp() {
	gameplayScene = GameObjectProvider::getGameplayScene();
}

void Game::EnemyProvider::updateRegisteredEnemies() {
	for (Turret* turret : turrets) {
		turret->update(player->getCharacter(), time->getCurrentTimeInNanos());
		if (turret->hasRecentlyAttacked()) {
			turret->addBulletEffects(gameplayScene, resourceManager, time);
		}
	}
}

size_t Game::EnemyProvider::addTurret(Turret* turret) {
	turrets.push_back(turret);
	return turrets.size() - 1;
}

Game::Turret* Game::EnemyProvider::getTurret(size_t id) {
	return turrets[id];
}