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
#include "../../Displays/Hints/HintDisplayDuration.h"
#include "../../Displays/Dialogue/DialogueOption.h"

namespace Scripts {
	using namespace Game;

	void showDialogue(std::string name, std::string avatar, std::string text, std::string scriptToRunOnceComplete);
	void showTwoOptions(DialogueOption option1, DialogueOption option2);
	void showThreeOptions(DialogueOption option1, DialogueOption option2, DialogueOption option3);
	void showFourOptions(DialogueOption option1, DialogueOption option2, DialogueOption option3, DialogueOption option4);
	void showHint(std::string text, HintDisplayDuration duration);
	void hideDialogue();

	// This sets up game elements.
	void setupCharacters(int stage, int level);
	void setupTeleporters(int stage, int level);
	void setupDoors(int stage, int level);
	void setupCheckPoints(int stage, int level);
	void setupGeneralEvents(int stage, int level);

	void startNewGame();
	void continueGame();
	void editMap();

	void setupLevel(int stage, int level);

	void addTileBehaviours();

	void fadeTeleportPlayer(size_t world, glm::ivec2 chunk, glm::ivec3 tile);
	void riseTeleportPlayer(size_t world, glm::ivec2 chunk, glm::ivec3 tile);

	void unlockDoorLock(std::string doorID, size_t lockID);
	void openDoor(std::string doorID, size_t tileType, Location location);
	void closeDoor(std::string doorID, size_t tileType, Location location);
	void clearDoorAnimations(std::string doorID);
	void turnDisplayOn(Location location);
	void turnDisplayOff(Location location);
	void pressButton(Location location);

	void setupDoor(Door* door, std::string name, std::function<void()> onOpen, std::function<void()> onClose);
	void setupDoorSensor(const Location& location, std::string doorName, std::string sensorScriptName, size_t tileType, size_t lockID);
	void setupMultiTypeDoorSensor(const Location& location, std::string doorName, std::string sensorScriptName, std::vector<size_t> correctTileTypes, size_t lockID);
	void setupDoorTileAmountSensors(TileAmountLock* lock, std::string sensorScriptName);
	void setupCheckPointWithRightSaveSign(std::string name, Location checkPointLocation, Location rightSaveSignLocation, TileDirection directionToMovePlayer);
	void setupCheckPointWithLeftSaveSign(std::string name, Location checkPointLocation, Location leftSaveSignLocation, TileDirection directionToMovePlayer);
	void setupCheckPointWithBothSaveSigns(std::string name, Location checkPointLocation, Location leftSaveSignLocation, Location rightSaveSignLocation, TileDirection directionToMovePlayer);
	
	void triggerDoorSensor(Location location, size_t correctTileType, std::string doorTag, size_t lockID);
	void triggerMultiTypeDoorSensor(Location location, std::vector<size_t> correctTileTypes, std::string doorTag, size_t lockID);

	// This adds all gameplay scripts to the ScriptManager and also binds tile-related scripts to their tiles.
	void addImportantScripts();

	// This sets up necessary items for gameplay scripts.
	void setupScripts();

	// This runs all setup scripts.
	void runGameStartingScripts();
}