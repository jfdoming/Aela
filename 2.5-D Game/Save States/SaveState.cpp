#include "SaveState.h"
#include "../Character/CharacterProvider.h"
#include "../Doors/DoorProvider.h"
#include "../Worlds/WorldManager.h"
#include "../Displays/Tiles/TileInventoryDisplay.h"

Game::SaveState::SaveState() = default;

void Game::SaveState::save() {
	WorldManager* worldManager = GameObjectProvider::getWorldManager();
	CharacterProvider* characterProvider = GameObjectProvider::getCharacterProvider();
	DoorProvider* doorProvider = GameObjectProvider::getDoorProvider();

	// This changes some player-related properties.
	player = GameObjectProvider::getPlayer();
	Character* character = player->getCharacter();
	bool allowNewMovements = character->areNewMovementsAllowed();
	character->allowNewMovements(false);
	player->getCharacter()->clearMovementQueue();

	// These are the main things to be saved.
	worldManager->saveDataToSaveState(this);
	characterProvider->saveDataToSaveState(this);
	doorProvider->saveDataToSaveState(this);
	player->saveDataToSaveState(this);

	// This saves other random things.
	delete (std::string*) getData("looping track");
	std::string* track = new std::string("");

	delete (AudioLoopData*) getData("looping track data");
	AudioLoopData* loopData = new AudioLoopData;

	if (GameObjectProvider::getAudioLooper()->getNumberOfCurrentLoops() != 0) {
		*track = GameObjectProvider::getAudioLooper()->getFirstLoop()->getSrc();
		*loopData = *GameObjectProvider::getAudioLooper()->getFirstLoopData();
	}

	addData("looping track", track);
	addData("looping track data", loopData);

	character->allowNewMovements(allowNewMovements);
	GameObjectProvider::getTileInventoryDisplay()->refreshSubMenu();
}

void Game::SaveState::load() {
	std::cout << "Starting to load.\n";
	GameObjectProvider::getTimer()->removeAllEvents();
	GameObjectProvider::getPlayer()->loadDataFromSaveState(this);
	GameObjectProvider::getCharacterProvider()->loadDataFromSaveState(this);
	GameObjectProvider::getWorldManager()->loadDataFromSaveState(this);
	GameObjectProvider::getDoorProvider()->loadDataFromSaveState(this);

	Character* character = player->getCharacter();
	bool playerWasAlive = character->isAlive();

	// In case the player was dead, revive them. Allow them to move, too.
	character->revive();
	character->allowNewMovements(true);
	character->setRunning(true);

	// This loads some other stuff.
	std::string* track = (std::string*) getData("looping track");
	AudioLooper* looper = GameObjectProvider::getAudioLooper();
	// std::cout << (*track != "") << " " << (looper->getNumberOfCurrentLoops() != 0) << " " << (*track == looper->getFirstLoop()->getSrc()) << " " << playerWasAlive << "\n";
	if (*track != "" && !(looper->getNumberOfCurrentLoops() != 0 && *track == looper->getFirstLoop()->getSrc() && playerWasAlive)) {
		looper->stopAllLoops();
		AudioLoopData* data = (AudioLoopData*) getData("looping track data");
		GameObjectProvider::getResourceManager()->useResourceRoot(false);
		looper->loopAudio(*track, data->numberOfLoops, data->locationOfLoopEnd, 0);
		GameObjectProvider::getResourceManager()->useResourceRoot(true);
	}

	GameObjectProvider::getTileInventoryDisplay()->refreshSubMenu();
}

void Game::SaveState::addData(std::string tag, void* dataPtr) {
	data[tag] = dataPtr;
}

void* Game::SaveState::getData(std::string tag) {
	auto pair = data.find(tag);

	if (pair == data.end()) {
		return nullptr;
	}

	return pair->second;
}
