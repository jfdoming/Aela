#include "EnemyProvider.h"
#include "../Worlds/WorldManager.h"

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

void Game::EnemyProvider::returnWasPressed() {
	WorldManager* worldManager = GameObjectProvider::getWorldManager();
	Character* character = player->getCharacter();
	TileDirection direction = character->getDirectionFacing();
	Location location = *character->getLocation();
	CharacterProvider* characterProvider = GameObjectProvider::getCharacterProvider();
	glm::ivec2 chunk = location.getChunk();
	glm::ivec3 tile = location.getTileGroup();

	worldManager->getCoordinateOfNeighbouringTile(tile, chunk, direction);
	Character* enemyCharacter = characterProvider->getCharacterByLocation(Location(location.getWorld(), chunk, tile));

	if (enemyCharacter == nullptr) {
		return;
	}

	auto* enemy = dynamic_cast<Enemy*>(enemyCharacter);
	if (enemy == nullptr) {
		return;
	}

	if (enemy->getDirectionFacing() == character->getDirectionFacing()) {
		// The player can turn enemies such as turrets off!
		enemy->toggleHostility();
	}
}

size_t Game::EnemyProvider::addTurret(Turret* turret) {
	turrets.push_back(turret);
	return turrets.size() - 1;
}

Game::Turret* Game::EnemyProvider::getTurret(size_t id) {
	return turrets[id];
}