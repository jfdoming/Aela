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
	for (Enemy* enemy : enemies) {
		enemy->update();
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

size_t Game::EnemyProvider::addEnemy(Enemy* enemy) {
	enemies.push_back(enemy);
	return enemies.size() - 1;
}

Game::Enemy* Game::EnemyProvider::getEnemy(size_t id) {
	return enemies[id];
}

Enemy* EnemyProvider::getEnemy(std::string name) {
	for (auto* ptr : enemies) {
		if (ptr->getName() == name) {
			return ptr;
		}
	}

	return nullptr;
}
