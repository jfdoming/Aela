/*
* Name: Game Scripts
* Author: Robert Ciborowski
* Date: 03/01/2018
* Description: A file which contains gameplay-related scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once
#include "../../Game Object Provider/GameObjectProvider.h"
#include "../../Location/Location.h"
#include "../../Doors/Door.h"
#include "../../Doors/TileAmountLock.h"

namespace Scripts {
	using namespace Game;

	void hideDialogue();

	// This sets up game elements.
	void setupCharacters();
	void setupTeleporters();
	void setupDoors();

	void startNewGame();
	void continueGame();
	void editMap();

	void setupLevel(int stage, int level);

	void addTileBehaviours();

	void fadeTeleportPlayer(size_t world, glm::ivec2 chunk, glm::ivec3 tile);
	void riseTeleportPlayer(size_t world, glm::ivec2 chunk, glm::ivec3 tile);
	
	void unlockDoorLock(std::string tag, size_t lockID);
	void setupDoor(Door* door, std::string name, std::function<void()> onOpen, std::function<void()> onClose);
	void setupDoorSensor(Location* location, std::string doorName, std::string sensorScriptName, size_t tileType, size_t lockID);
	void setupDoorTileAmountSensors(TileAmountLock* lock, std::string sensorScriptName);
	void triggerDoorSensor(Location location, size_t correctTileType, std::string doorTag, size_t lockID);

	// This adds all gameplay scripts to the ScriptManager and also binds tile-related scripts to their tiles.
	void addScriptsToGameWorld();

	// This sets up necessary items for gameplay scripts.
	void setupScripts();

	// This runs all setup scripts.
	void runGameStartingScripts();
}