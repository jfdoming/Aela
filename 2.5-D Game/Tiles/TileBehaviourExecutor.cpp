#include "TileBehaviourExecutor.h"
#include "../Worlds/WorldManager.h"

Game::TileBehaviourExecutor::TileBehaviourExecutor() = default;

void Game::TileBehaviourExecutor::setup() {
	worldManager = GameObjectProvider::getWorldManager();
}

void Game::TileBehaviourExecutor::addBehaviour(size_t tileTypeID, std::function<void(const Location&)>* behaviour) {
	behaviours[tileTypeID] = *behaviour;
}

void Game::TileBehaviourExecutor::runBehaviour(const Location& location, size_t tileTypeID) {
	TileGroup* tileGroup = worldManager->getTileGroup(location);

	auto pos = behaviours.find(tileTypeID);
	if (pos != behaviours.end()) {
		pos->second(location);
	}
}