#include "GameSaver.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Worlds/WorldManager.h"
#include "../Character/CharacterProvider.h"
#include "../Player/Player.h"
#include "../Character/Character.h"
#include "SaveExporter.h"
#include "../Resources/ResourceInfo.h"
#include "SaveLoader.h"
#include "../Worlds/WorldExporter.h"
#include "../Worlds/WorldLoader.h"
#include "../Displays/Hints/HintDisplay.h"
#include "../Displays/Timer/TimerDisplay.h"
#include <Shlobj.h>
#include "../../Project Aela/Utilities/dirut.h"
#include "../Camera/CameraController.h"
#include "../Displays/Tiles/TileInventoryDisplay.h"

Game::GameSaver::GameSaver() {
	WCHAR path[MAX_PATH];
	if (!SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
		AelaErrorHandling::windowError("Game Saver", "Could not get user path, such as C:\\Users\\Name! This means that save files won't work. Aborting!");
		GameObjectProvider::getWindow()->quit();
		return;
	}

	std::wstring wString = std::wstring(path);
	userPath = std::string(wString.begin(), wString.end());

	// if (!directoryExists(pathString)) {
		makeDirectory(userPath + "/Documents/");
		makeDirectory(userPath + "/Documents/Neo-Zero");
		makeDirectory(userPath + "/Documents/Neo-Zero/saves");
	// }
}

bool Game::GameSaver::save(std::string saveStateName) {
	std::cout << "Saving.\n";
	if (!GameObjectProvider::getPlayer()->getCharacter()->isAlive()) {
		return false;
	}

	if (saveStates.find(saveStateName) == saveStates.end()) {
		saveStates[saveStateName] = SaveState();
	}

	saveStates[saveStateName].save();
	SaveExporter exporter;
	exporter.exportSaveState(userPath + SAVE_LOCATION + saveStateName + ".txt", &saveStates[saveStateName]);
	GameObjectProvider::getWorldExporter()->exportWorld(userPath + SAVE_LOCATION + saveStateName + "_world.txt",
		GameObjectProvider::getWorldManager()->getWorld(GameObjectProvider::getPlayer()->getCharacter()->getLocation()->getWorld()));
	return true;
}

bool Game::GameSaver::load(std::string saveStateName) {
	if (!allowLoading) {
		GameObjectProvider::getHintDisplay()->clearAndDisplayHint("There is no savepoint to load.", HintDisplayDuration::SHORT);
		return false;
	}

	auto pos = saveStates.find(saveStateName);
	if (pos == saveStates.end()) {
		return false;
	}

	pos->second.load();
	
	// GameObjectProvider::getPlayer()->getTileInventory()->setCurrentTile(0);
	GameObjectProvider::getGameplayScene()->clearParticleEmitters();
	GameObjectProvider::getTimer()->endAllEventsNextUpdate();
	GameObjectProvider::getHintDisplay()->clear();
	GameObjectProvider::getTimerDisplay()->clear();
	GameObjectProvider::getTileInventoryDisplay()->refreshSubMenu();
	// GameObjectProvider::getTileInventoryDisplay()->animateSelectorBox();
	GameObjectProvider::getPlayer()->getCharacter()->clearMovementQueue();
	GameObjectProvider::getCameraController()->setLockCameraToPlayer(true);

	auto mapRebuild = []() {
		GameObjectProvider::getWorldManager()->rebuildMapNextUpdate();
	};
	GameObjectProvider::getTimer()->scheduleEventInMillis(1, mapRebuild);

	return true;
}

bool Game::GameSaver::loadFromFile(std::string saveStateName) {
	if (!allowLoading) {
		GameObjectProvider::getHintDisplay()->clearAndDisplayHint("There is no savepoint to load.", HintDisplayDuration::SHORT);
		return false;
	}

	if (saveStates.find(saveStateName) == saveStates.end()) {
		saveStates[saveStateName] = SaveState();
	}

	bool success;
	SaveLoader loader;
	success = loader.loadSaveState(userPath + SAVE_LOCATION + saveStateName + ".txt", saveStates[saveStateName]);

	if (!success) {
		return false;
	}

	WorldManager* worldManager = GameObjectProvider::getWorldManager();
	WorldLoader worldLoader;
	World world;
	success = worldLoader.loadWorld(userPath + SAVE_LOCATION + saveStateName + "_world.txt", world);

	if (!success) {
		return false;
	}

	worldManager->addWorld(&world, GameObjectProvider::getPlayer()->getCharacter()->getLocation()->getWorld());

	// save(saveStateName);
	return true;
}

void Game::GameSaver::disableLoading() {
	allowLoading = false;
}

void Game::GameSaver::enableLoading() {
	allowLoading = true;
}
