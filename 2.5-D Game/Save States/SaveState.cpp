#include "SaveState.h"
#include "../Character/CharacterProvider.h"
#include "../Worlds/WorldManager.h"

Game::SaveState::SaveState() {}

void Game::SaveState::save() {
	WorldManager* worldManager = GameObjectProvider::getWorldManager();
	CharacterProvider* characterProvider = GameObjectProvider::getCharacterProvider();

	player = *GameObjectProvider::getPlayer();
	Character* character = player.getCharacter();
	bool allowNewMovements = character->areNewMovementsAllowed();
	character->allowNewMovements(false);
	player.getCharacter()->clearAllMovements();
	world = *worldManager->getWorld(player.getCharacter()->getLocation()->getWorld());
	for (auto& pair : *characterProvider->getAllCharacters()) {
		characterInformation[pair.first] = pair.second->getCharacterInformationBlock();
	}
	character->allowNewMovements(allowNewMovements);
}

void Game::SaveState::load() {
	WorldManager* worldManager = GameObjectProvider::getWorldManager();
	CharacterProvider* characterProvider = GameObjectProvider::getCharacterProvider();

	(*GameObjectProvider::getPlayer()) = player;

	for (auto& pair : *characterProvider->getAllCharacters()) {
		pair.second->setPropertiesUsingCharacterInformationBlock(&characterInformation[pair.first]);
	}

	(*worldManager->getWorld(player.getCharacter()->getLocation()->getWorld())) = world;
	// worldManager->rebuildMapNextUpdate();
}
