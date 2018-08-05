#include "GameSaver.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Worlds/WorldManager.h"
#include "../Character/CharacterProvider.h"

Game::GameSaver::GameSaver() {}

void Game::GameSaver::save(std::string saveStateName) {
	saveStates[saveStateName] = SaveState();
	saveStates[saveStateName].save();
}

bool Game::GameSaver::load(std::string saveStateName) {
	auto pos = saveStates.find(saveStateName);
	if (pos == saveStates.end()) {
		return false;
	}

	pos->second.load();

	auto mapRebuild = []() {
		GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
	};
	GameObjectProvider::getTimer()->scheduleEventInMillis(1, mapRebuild);

	return true;
}