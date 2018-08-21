#include "GameSaver.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Worlds/WorldManager.h"
#include "../Character/CharacterProvider.h"
#include "../Player/Player.h"
#include "../Character/Character.h"

Game::GameSaver::GameSaver() {}

bool Game::GameSaver::save(std::string saveStateName) {
	if (!GameObjectProvider::getPlayer()->getCharacter()->isAlive()) {
		return false;
	}

	saveStates[saveStateName] = SaveState();
	saveStates[saveStateName].save();
	return true;
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