#include "SaveState.h"
#include "../Character/CharacterProvider.h"
#include "../Doors/DoorProvider.h"
#include "../Worlds/WorldManager.h"
#include "../Displays/Tiles/TileInventoryDisplay.h"

Game::SaveState::SaveState() {}

void Game::SaveState::save() {
	for (auto& pair : data) {
		delete pair.second;
	}

	data.clear();

	WorldManager* worldManager = GameObjectProvider::getWorldManager();
	CharacterProvider* characterProvider = GameObjectProvider::getCharacterProvider();
	DoorProvider* doorProvider = GameObjectProvider::getDoorProvider();

	// This changes some player-related properties.
	player = *GameObjectProvider::getPlayer();
	Character* character = player.getCharacter();
	bool allowNewMovements = character->areNewMovementsAllowed();
	character->allowNewMovements(false);
	player.getCharacter()->clearAllMovements();

	worldManager->saveDataToSaveState(this);
	characterProvider->saveDataToSaveState(this);
	doorProvider->saveDataToSaveState(this);

	character->allowNewMovements(allowNewMovements);
	GameObjectProvider::getTileInventoryDisplay()->refreshSubMenu();
}

void Game::SaveState::load() {
	WorldManager* worldManager = GameObjectProvider::getWorldManager();
	CharacterProvider* characterProvider = GameObjectProvider::getCharacterProvider();
	DoorProvider* doorProvider = GameObjectProvider::getDoorProvider();

	(*GameObjectProvider::getPlayer()) = player;
	characterProvider->loadDataFromSaveState(this);
	worldManager->loadDataFromSaveState(this);
	doorProvider->loadDataFromSaveState(this);

	Character* character = player.getCharacter();

	// In case the player was dead, revive them. Allow them to move, too.
	character->revive();
	character->allowNewMovements(true);

	GameObjectProvider::getTileInventoryDisplay()->refreshSubMenu();
}

void Game::SaveState::addData(std::string tag, void* dataPtr) {
	data[tag] = dataPtr;
}

void* Game::SaveState::getData(std::string tag) {
	return data[tag];
}
