#include "TileBehaviourExecuter.h"
#include "../Worlds/WorldManager.h"

Game::TileBehaviourExecuter::TileBehaviourExecuter() {}

void Game::TileBehaviourExecuter::setup() {
	worldManager = GameObjectProvider::getWorldManager();
}

void Game::TileBehaviourExecuter::addBehaviour(size_t tileTypeID, std::function<void(Location*)>* behaviour) {
	behaviours[tileTypeID] = *behaviour;
}

void Game::TileBehaviourExecuter::runBehaviour(Location* location, size_t tileTypeID) {
	TileGroup* tileGroup = worldManager->getTileGroup(location);

	auto pos = behaviours.find(tileTypeID);
	if (pos != behaviours.end()) {
		pos->second(location);
	}
}
