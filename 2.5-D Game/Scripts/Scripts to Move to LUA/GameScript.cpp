#include "GameScript.h"
#include "ScriptObjects.h"
#include "AnimationScript.h"
#include "../../Player/Player.h"
#include "../../Aela Game/AelaGame.h"
#include "../../Displays/Dialogue/DialogueDisplay.h"
#include "../../Worlds/WorldManager.h"
#include "../../Tiles/TileBehaviourExecuter.h"
#include "../../Doors/DoorProvider.h"
#include "../../Doors/TileAmountLock.h"
#include "../../Save States/GameSaver.h"
#include "../../Resources/ResourceInfo.h"
#include "../ScriptManager.h"

using namespace Scripts;

void Scripts::showDialogue(std::string text, std::string scriptToRunOnceComplete) {
	dialogueDisplay->showDialogue(text, scriptToRunOnceComplete);
}

void Scripts::hideDialogue() {
	dialogueDisplay->closeDialog();
}

void Scripts::setupCharacters() {}

void Scripts::setupTeleporters(int stage, int level) {
	switch (stage) {
		case 1:
			switch (level) {
				case 1:
					worldManager->addTeleporterToFloorTile(&Location(1, -7, 2, 6, 0, 1), &Location(1, -8, 2, 8, 0, 3));
					worldManager->addTeleporterToFloorTile(&Location(1, -10, 3, 7, 0, 13), &Location(1, -10, 3, 5, 0, 13));
					worldManager->addTeleporterToFloorTile(&Location(1, -10, 4, 3, 0, 1), &Location(1, -10, 3, 5, 0, 13));
					worldManager->addTeleporterToFloorTile(&Location(1, -10, 3, 2, 0, 15), &Location(1, -10, 4, 2, 0, 1));
					worldManager->addTeleporterToFloorTile(&Location(1, -10, 3, 2, 0, 10), &Location(1, -10, 3, 3, 0, 7));
					worldManager->addTeleporterToFloorTile(&Location(1, -11, 4, 3, 0, 1), &Location(1, -11, 3, 12, 0, 13));
					worldManager->addTeleporterToFloorTile(&Location(1, -12, 4, 10, 0, 1), &Location(1, -12, 4, 8, 0, 1));
					worldManager->addTeleporterToFloorTile(&Location(1, -12, 2, 9, 0, 6), &Location(1, -11, 1, 0, 0, 14));
					worldManager->addTeleporterToFloorTile(&Location(1, -12, 2, 15, 0, 6), &Location(1, -12, 2, 11, 0, 15));
					worldManager->addTeleporterToFloorTile(&Location(1, -12, 1, 14, 0, 8), &Location(1, -12, 1, 8, 0, 10));
					worldManager->addTeleporterToFloorTile(&Location(1, -11, 2, 11, 0, 15), &Location(1, -11, 2, 14, 0, 3));
					worldManager->addTeleporterToFloorTile(&Location(1, -10, 2, 4, 0, 5), &Location(1, -11, 2, 14, 0, 9));
					worldManager->addTeleporterToFloorTile(&Location(1, -10, 1, 1, 0, 14), &Location(1, -10, 1, 7, 0, 9));
					worldManager->addTeleporterToFloorTile(&Location(1, -10, 1, 5, 0, 14), &Location(1, -10, 1, 11, 0, 9));
					worldManager->addTeleporterToFloorTile(&Location(1, -10, 1, 9, 0, 14), &Location(1, -11, 1, 15, 0, 14));
					break;
			}
			break;
	}
}

void Scripts::setupDoors(int stage, int level) {
	switch (stage) {
		case 1:
			switch (level) {
				case 1:
					// World 1 Door 0
					auto onOpen_1_0 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 2)))->removeTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 2)))->removeTile(197);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 2)))->addTile(198);
					};

					auto onClose_1_0 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 2)))->addTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 2)))->removeTile(198);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 2)))->addTile(197);
					};

					Door door_1_0(1);
					setupDoor(&door_1_0, "door_1_0", onOpen_1_0, onClose_1_0);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 1)), "door_1_0", "door_1_0 sensor_0", 9, 0);

					// World 1 Door 1
					auto onOpen_1_1 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 5)))->removeTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 5)))->removeTile(207);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 5)))->addTile(208);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 5)))->removeTile(209);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 5)))->addTile(210);
					};

					auto onClose_1_1 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 5)))->addTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 5)))->addTile(207);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 5)))->removeTile(208);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 5)))->addTile(209);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 5)))->removeTile(210);
					};

					Door door_1_1(2);
					setupDoor(&door_1_1, "door_1_1", onOpen_1_1, onClose_1_1);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 4)), "door_1_1", "door_1_1 sensor_0", 9, 0);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 4)), "door_1_1", "door_1_1 sensor_1", 10, 1);

					// World 1 Door 2
					auto onOpen_1_2 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 8)))->removeTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 8)))->removeTile(217);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 8)))->addTile(218);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 8)))->removeTile(221);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 8)))->addTile(222);
					};

					auto onClose_1_2 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 8)))->addTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 8)))->addTile(217);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 8)))->removeTile(218);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 8)))->addTile(221);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 8)))->removeTile(222);
					};

					Door door_1_2(2);
					setupDoor(&door_1_2, "door_1_2", onOpen_1_2, onClose_1_2);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 7)), "door_1_2", "door_1_2 sensor_0", 9, 0);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 7)), "door_1_2", "door_1_2 sensor_1", 11, 1);

					// World 1 Door 3
					auto onOpen_1_3 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 5)))->removeTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(2, 0, 5)))->removeTile(231);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(2, 0, 5)))->addTile(232);
					};

					auto onClose_1_3 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 5)))->addTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(2, 0, 5)))->addTile(231);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(2, 0, 5)))->removeTile(232);
					};

					Door door_1_3(1);
					setupDoor(&door_1_3, "door_1_3", onOpen_1_3, onClose_1_3);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(2, 0, 4)), "door_1_3", "door_1_3 sensor_0", 11, 0);

					// World 1 Door 4
					auto onOpen_1_4 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 5)))->removeTile(181);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 8)))->removeTile(237);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 8)))->addTile(238);
					};

					auto onClose_1_4 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 5)))->addTile(181);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 8)))->addTile(237);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 8)))->removeTile(238);
					};

					Door door_1_4(1);
					setupDoor(&door_1_4, "door_1_4", onOpen_1_4, onClose_1_4);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 7)), "door_1_4", "door_1_4 sensor_0", 9, 0);

					// World 1 Door 5
					auto onOpen_1_5 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, -1), glm::ivec3(1, 0, 14)))->removeTile(185);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 8)))->removeTile(247);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 8)))->addTile(248);
					};

					auto onClose_1_5 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, -1), glm::ivec3(1, 0, 14)))->addTile(185);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 8)))->addTile(247);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 8)))->removeTile(248);
					};

					Door door_1_5(1);
					setupDoor(&door_1_5, "door_1_5", onOpen_1_5, onClose_1_5);
					setupDoorSensor(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 7)), "door_1_5", "door_1_5 sensor_0", 9, 0);

					// World 1 Door 6
					auto onOpen_1_6 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(10, 0, 1)))->removeTile(189);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 1)))->removeTile(261);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 1)))->addTile(262);
					};

					auto onClose_1_6 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(10, 0, 1)))->addTile(189);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 1)))->addTile(261);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 1)))->removeTile(262);
					};

					Door door_1_6(1);
					setupDoor(&door_1_6, "door_1_6", onOpen_1_6, onClose_1_6);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 0)), "door_1_6", "door_1_6 sensor_0", 11, 0);

					// World 1 Door 7
					auto onOpen_1_7 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(5, 0, 7)))->removeTile(193);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 8)))->removeTile(267);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 8)))->addTile(268);
					};

					auto onClose_1_7 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(5, 0, 7)))->addTile(193);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 8)))->addTile(267);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 8)))->removeTile(268);
					};

					Door door_1_7(1);
					setupDoor(&door_1_7, "door_1_7", onOpen_1_7, onClose_1_7);
					setupDoorSensor(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 7)), "door_1_7", "door_1_7 sensor_0", 9, 0);

					// World 1 Door 8
					auto onOpen_1_8 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-3, 0), glm::ivec3(9, 0, 7)))->removeTile(165);
					};

					auto onClose_1_8 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-3, 0), glm::ivec3(9, 0, 7)))->addTile(165);
					};

					Door door_1_8(16);
					setupDoor(&door_1_8, "door_1_8", onOpen_1_8, onClose_1_8);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(14, 0, 4)), "door_1_8", "door_1_8 sensor_0", 13, 0);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(13, 0, 4)), "door_1_8", "door_1_8 sensor_1", 9, 1);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(12, 0, 4)), "door_1_8", "door_1_8 sensor_2", 9, 2);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(11, 0, 4)), "door_1_8", "door_1_8 sensor_3", 13, 3);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(14, 0, 3)), "door_1_8", "door_1_8 sensor_4", 9, 4);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(13, 0, 3)), "door_1_8", "door_1_8 sensor_5", 9, 5);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(12, 0, 3)), "door_1_8", "door_1_8 sensor_6", 9, 6);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(11, 0, 3)), "door_1_8", "door_1_8 sensor_7", 9, 7);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(14, 0, 2)), "door_1_8", "door_1_8 sensor_8", 13, 8);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(13, 0, 2)), "door_1_8", "door_1_8 sensor_9", 9, 9);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(12, 0, 2)), "door_1_8", "door_1_8 sensor_10", 9, 10);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(11, 0, 2)), "door_1_8", "door_1_8 sensor_11", 13, 11);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(14, 0, 1)), "door_1_8", "door_1_8 sensor_12", 9, 12);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(13, 0, 1)), "door_1_8", "door_1_8 sensor_13", 13, 13);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(12, 0, 1)), "door_1_8", "door_1_8 sensor_14", 13, 14);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(11, 0, 1)), "door_1_8", "door_1_8 sensor_15", 9, 15);

					// World 1 Door 9
					auto onOpen_1_9 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(13, 0, 7)))->removeTile(169);
					};

					auto onClose_1_9 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(13, 0, 7)))->addTile(169);
					};

					Door door_1_9(16);
					setupDoor(&door_1_9, "door_1_9", onOpen_1_9, onClose_1_9);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(3, 0, 4)), "door_1_9", "door_1_9 sensor_0", 13, 0);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(2, 0, 4)), "door_1_9", "door_1_9 sensor_1", 13, 1);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(1, 0, 4)), "door_1_9", "door_1_9 sensor_2", 9, 2);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(0, 0, 4)), "door_1_9", "door_1_9 sensor_3", 9, 3);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(3, 0, 3)), "door_1_9", "door_1_9 sensor_4", 9, 4);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(2, 0, 3)), "door_1_9", "door_1_9 sensor_5", 9, 5);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(1, 0, 3)), "door_1_9", "door_1_9 sensor_6", 13, 6);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(0, 0, 3)), "door_1_9", "door_1_9 sensor_7", 9, 7);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(3, 0, 2)), "door_1_9", "door_1_9 sensor_8", 13, 8);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(2, 0, 2)), "door_1_9", "door_1_9 sensor_9", 13, 9);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(1, 0, 2)), "door_1_9", "door_1_9 sensor_10", 11, 10);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(0, 0, 2)), "door_1_9", "door_1_9 sensor_11", 11, 11);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(3, 0, 1)), "door_1_9", "door_1_9 sensor_12", 10, 12);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(2, 0, 1)), "door_1_9", "door_1_9 sensor_13", 10, 13);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(1, 0, 1)), "door_1_9", "door_1_9 sensor_14", 10, 14);
					setupDoorSensor(&Location(1, glm::ivec2(-3, 0), glm::ivec3(0, 0, 1)), "door_1_9", "door_1_9 sensor_15", 10, 15);

					// World 1 Door 10
					auto onOpen_1_10 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 8)))->removeTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(4, 0, 8)))->removeTile(173);
					};

					auto onClose_1_10 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 8)))->addTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(4, 0, 8)))->addTile(173);
					};

					Door door_1_10;
					door_1_10.setOnOpen(&((std::function<void()>) onOpen_1_10));
					door_1_10.setOnClose(&((std::function<void()>) onClose_1_10));
					TileAmountLock* door_1_10_lock_0 = new TileAmountLock();
					door_1_10_lock_0->specifyRegionAsCuboid(&Location(1, -4, 0, 1, 0, 3), 6, 1, 5);
					door_1_10_lock_0->addTileTypeToCheckFor(9, 9);
					door_1_10_lock_0->addTileTypeToCheckFor(10, 9);
					door_1_10_lock_0->addTileTypeToCheckFor(11, 3);
					door_1_10_lock_0->addTileTypeToCheckFor(13, 9);
					Lock* door_1_10_lock_0_base = door_1_10_lock_0;
					door_1_10.addLock(door_1_10_lock_0_base);
					setupDoorTileAmountSensors(door_1_10_lock_0, "door_1_10 sensor_0");
					doorProvider->addDoor("door_1_10", &door_1_10);
					door_1_10_lock_0->setIDInDoor(0);
					door_1_10_lock_0->setDoor(doorProvider->getDoor("door_1_10"));

					// World 1 Door 11
					auto onOpen_1_11 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-5, 0), glm::ivec3(14, 0, 10)))->removeTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 11)))->removeTile(227);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 11)))->addTile(228);
					};

					auto onClose_1_11 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-5, 0), glm::ivec3(14, 0, 10)))->addTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 11)))->addTile(227);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 11)))->removeTile(228);
					};

					Door door_1_11(1);
					setupDoor(&door_1_11, "door_1_11", onOpen_1_11, onClose_1_11);
					setupDoorSensor(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 10)), "door_1_11", "door_1_11 sensor_0", 9, 0);

					// World 1 Door 12
					auto onOpen_1_12 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 0), glm::ivec3(13, 0, 13)))->addTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 0), glm::ivec3(5, 0, 13)))->removeTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(9, 0, 0)))->removeTile(199);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(9, 0, 0)))->addTile(200);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(9, 0, 0)))->removeTile(201);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(5, 0, 0)))->addTile(202);
					};

					auto onClose_1_12 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 0), glm::ivec3(13, 0, 13)))->removeTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 0), glm::ivec3(5, 0, 13)))->addTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(9, 0, 0)))->addTile(199);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(9, 0, 0)))->removeTile(200);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(9, 0, 0)))->addTile(201);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(5, 0, 0)))->removeTile(202);
					};

					Door door_1_12(1);
					setupDoor(&door_1_12, "door_1_12", onOpen_1_12, onClose_1_12);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 0), glm::ivec3(9, 0, 15)), "door_1_12", "door_1_12 sensor_0", 10, 0);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 0), glm::ivec3(5, 0, 15)), "door_1_12", "door_1_12 sensor_1", 11, 1);

					// World 1 Door 13
					auto onOpen_1_13 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 0), glm::ivec3(9, 0, 13)))->removeTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(12, 0, 0)))->removeTile(209);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(12, 0, 0)))->addTile(210);
					};

					auto onClose_1_13 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 0), glm::ivec3(9, 0, 13)))->addTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(12, 0, 0)))->addTile(209);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(12, 0, 0)))->removeTile(210);
					};

					Door door_1_13(1);
					setupDoor(&door_1_13, "door_1_13", onOpen_1_13, onClose_1_13);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 0), glm::ivec3(12, 0, 15)), "door_1_13", "door_1_13 sensor_0", 10, 0);

					// World 1 Door 14
					auto onOpen_1_14 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 0), glm::ivec3(1, 0, 13)))->removeTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(14, 0, 0)))->removeTile(219);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(14, 0, 0)))->addTile(220);
					};

					auto onClose_1_14 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 0), glm::ivec3(1, 0, 13)))->addTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(14, 0, 0)))->addTile(219);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(14, 0, 0)))->removeTile(220);
					};

					Door door_1_14(1);
					setupDoor(&door_1_14, "door_1_14", onOpen_1_14, onClose_1_14);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 0), glm::ivec3(14, 0, 15)), "door_1_14", "door_1_14 sensor_0", 11, 0);

					// World 1 Door 15
					auto onClose_1_15 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(5, 0, 6)))->addTile(181);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(1, 0, 11)))->addTile(241);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(1, 0, 11)))->removeTile(242);
					};

					auto onTimerCompletion_1_15 = [onClose_1_15]() {
						onClose_1_15();
						doorProvider->getDoor("door_1_15")->lock(0);
					};

					auto onOpen_1_15 = [onTimerCompletion_1_15]() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(5, 0, 6)))->removeTile(181);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(1, 0, 11)))->removeTile(241);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(1, 0, 11)))->addTile(242);
						timer->scheduleEventInSeconds(3, onTimerCompletion_1_15);
					};

					Door door_1_15(1);
					setupDoor(&door_1_15, "door_1_15", onOpen_1_15, nullptr);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 1), glm::ivec3(1, 0, 10)), "door_1_15", "door_1_15 sensor_0", 11, 0);

					// World 1 Door 16
					auto onOpen_1_16 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(4, 0, 5)))->addTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(8, 0, 5)))->removeTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(9, 0, 5)))->removeTile(210);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(9, 0, 5)))->addTile(209);
					};

					auto onClose_1_16 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(4, 0, 5)))->removeTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(8, 0, 5)))->addTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(9, 0, 5)))->addTile(209);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(9, 0, 5)))->removeTile(210);
					};

					Door door_1_16({false});
					setupDoor(&door_1_16, "door_1_16", onOpen_1_16, onClose_1_16);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 2), glm::ivec3(9, 0, 4)), "door_1_16", "door_1_16 sensor_0", 10, 0);

					// World 1 Door 17
					auto onClose_1_17 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(5, 0, 14)))->addTile(185);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 0)))->addTile(185);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 8)))->addTile(185);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(3, 0, 9)))->addTile(249);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(3, 0, 9)))->removeTile(250);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(4, 0, 2)))->addTile(247);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(4, 0, 2)))->removeTile(248);
					};

					auto onTimerCompletion_1_17 = [onClose_1_17]() {
						onClose_1_17();
						doorProvider->getDoor("door_1_17")->lock(0);
						doorProvider->getDoor("door_1_17")->lock(1);
					};

					auto onOpen_1_17 = [onTimerCompletion_1_17]() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(5, 0, 14)))->removeTile(185);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 0)))->removeTile(185);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 8)))->removeTile(185);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(3, 0, 9)))->removeTile(249);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(3, 0, 9)))->addTile(250);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(4, 0, 2)))->removeTile(247);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(4, 0, 2)))->addTile(248);
						timer->scheduleEventInSeconds(5, onTimerCompletion_1_17);
					};

					Door door_1_17(1);
					setupDoor(&door_1_17, "door_1_17", onOpen_1_17, nullptr);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 2), glm::ivec3(3, 0, 8)), "door_1_17", "door_1_17 sensor_0", 10, 0);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 2), glm::ivec3(4, 0, 1)), "door_1_17", "door_1_17 sensor_1", 9, 1);

					// World 1 Door 18
					auto onOpen_1_18 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 4)))->removeTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 4)))->addTile(202);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 4)))->removeTile(201);
					};

					auto onClose_1_18 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 4)))->addTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 4)))->removeTile(202);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 4)))->addTile(201);
					};

					Door door_1_18(1);
					setupDoor(&door_1_18, "door_1_18", onOpen_1_18, onClose_1_18);
					setupDoorSensor(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 3)), "door_1_18", "door_1_18 sensor_0", 11, 0);

					// World 1 Door 19
					auto onOpen_1_19 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 8)))->removeTile(189);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(8, 0, 8)))->addTile(266);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(8, 0, 8)))->removeTile(265);
					};

					auto onClose_1_19 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 8)))->addTile(189);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(8, 0, 8)))->removeTile(266);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(8, 0, 8)))->addTile(265);
					};

					Door door_1_19(1);
					setupDoor(&door_1_19, "door_1_19", onOpen_1_19, onClose_1_19);
					setupDoorSensor(&Location(1, glm::ivec2(-8, 2), glm::ivec3(8, 0, 7)), "door_1_19", "door_1_19 sensor_0", 13, 0);

					// World 1 Door 20
					auto onOpen_1_20 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 12)))->removeTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 12)))->addTile(222);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 12)))->removeTile(221);
					};

					auto onClose_1_20 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 12)))->addTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 12)))->removeTile(222);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 12)))->addTile(221);
					};

					Door door_1_20(1);
					setupDoor(&door_1_20, "door_1_20", onOpen_1_20, onClose_1_20);
					setupDoorSensor(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 11)), "door_1_20", "door_1_20 sensor_0", 11, 0);

					// World 1 Door 21
					auto onOpen_1_21 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 15)))->removeTile(193);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 15)))->addTile(270);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 15)))->removeTile(269);
					};

					auto onClose_1_21 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 15)))->addTile(193);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 15)))->removeTile(270);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 15)))->addTile(269);
					};

					Door door_1_21(1);
					setupDoor(&door_1_21, "door_1_21", onOpen_1_21, onClose_1_21);
					setupDoorSensor(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 14)), "door_1_21", "door_1_21 sensor_0", 10, 0);

					// World 1 Door 22
					auto onOpen_1_22 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(11, 0, 14)))->removeTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(12, 0, 14)))->addTile(200);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(12, 0, 14)))->removeTile(199);
					};

					auto onClose_1_22 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(11, 0, 14)))->addTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(12, 0, 14)))->removeTile(200);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(12, 0, 14)))->addTile(199);
					};

					Door door_1_22(1);
					setupDoor(&door_1_22, "door_1_22", onOpen_1_22, onClose_1_22);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 3), glm::ivec3(12, 0, 13)), "door_1_22", "door_1_22 sensor_0", 10, 0);

					// World 1 Door 23
					auto onOpen_1_23 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(2, 0, 14)))->removeTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(11, 0, 2)))->addTile(214);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(11, 0, 2)))->removeTile(213);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(2, 0, 2)))->addTile(208);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(2, 0, 2)))->removeTile(207);
					};

					auto onClose_1_23 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(2, 0, 14)))->addTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(11, 0, 2)))->removeTile(214);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(11, 0, 2)))->addTile(213);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(2, 0, 2)))->removeTile(208);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(2, 0, 2)))->addTile(207);
					};

					Door door_1_23({true, false});
					setupDoor(&door_1_23, "door_1_23", onOpen_1_23, onClose_1_23);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 4), glm::ivec3(11, 0, 1)), "door_1_23", "door_1_23 sensor_0", 12, 0);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 4), glm::ivec3(2, 0, 1)), "door_1_23", "door_1_23 sensor_1", 9, 1);

					// World 1 Door 24
					auto onOpen_1_24 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(7, 0, 1)))->removeTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(6, 0, 1)))->addTile(218);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(6, 0, 1)))->removeTile(217);
					};

					auto onClose_1_24 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(7, 0, 1)))->addTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(6, 0, 1)))->removeTile(218);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(6, 0, 1)))->addTile(217);
					};

					Door door_1_24(1);
					setupDoor(&door_1_24, "door_1_24", onOpen_1_24, onClose_1_24);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 4), glm::ivec3(6, 0, 0)), "door_1_24", "door_1_24 sensor_0", 9, 0);

					// World 1 Door 25
					auto onOpen_1_25 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(13, 0, 14)))->removeTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 14)))->addTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 14)))->removeTile(231);
					};

					auto onClose_1_25 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(13, 0, 14)))->addTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 14)))->removeTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 14)))->addTile(231);
					};

					Door door_1_25(1);
					setupDoor(&door_1_25, "door_1_25", onOpen_1_25, onClose_1_25);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 13)), "door_1_25", "door_1_25 sensor_0", 11, 0);

					// World 1 Door 26
					auto onOpen_1_26 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(15, 0, 3)))->removeTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(0, 0, 3)))->addTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(0, 0, 3)))->removeTile(203);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(14, 0, 3)))->addTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(14, 0, 3)))->removeTile(203);
					};

					auto onClose_1_26 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(15, 0, 3)))->addTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(0, 0, 3)))->removeTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(0, 0, 3)))->addTile(203);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(14, 0, 3)))->removeTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(14, 0, 3)))->addTile(203);
					};

					Door door_1_26(2);
					setupDoor(&door_1_26, "door_1_26", onOpen_1_26, onClose_1_26);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 2), glm::ivec3(0, 0, 2)), "door_1_26", "door_1_26 sensor_0", 12, 0);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 2), glm::ivec3(14, 0, 2)), "door_1_26", "door_1_26 sensor_1", 12, 1);

					// World 1 Door 27
					auto onOpen_1_27 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(4, 0, 3)))->removeTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 0, 3)))->removeTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(4, 0, 7)))->addTile(218);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(4, 0, 7)))->removeTile(217);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 0, 7)))->addTile(222);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 0, 7)))->removeTile(221);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 1, 7)))->addTile(224);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 1, 7)))->removeTile(223);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(5, 0, 15)))->addTile(226);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(5, 0, 15)))->removeTile(225);
					};

					auto onClose_1_27 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(4, 0, 3)))->addTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 0, 3)))->addTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(4, 0, 7)))->removeTile(218);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(4, 0, 7)))->addTile(217);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 0, 7)))->removeTile(222);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 0, 7)))->addTile(221);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 1, 7)))->removeTile(224);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 1, 7)))->addTile(223);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(5, 0, 15)))->removeTile(226);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(5, 0, 15)))->addTile(225);
					};

					Door door_1_27({false, false, true});
					setupDoor(&door_1_27, "door_1_27", onOpen_1_27, onClose_1_27);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 2), glm::ivec3(4, 0, 6)), "door_1_27", "door_1_27 sensor_0", 9, 0);
					setupMultiTypeDoorSensor(&Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 0, 6)), "door_1_27", "door_1_27 sensor_1", {11, 12}, 1);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 1), glm::ivec3(5, 0, 14)), "door_1_27", "door_1_27 sensor_2", 13, 2);

					// World 1 Door 28
					auto onOpen_1_28 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 11)))->removeTile(181);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(10, 0, 11)))->addTile(238);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(10, 0, 11)))->removeTile(237);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(8, 0, 11)))->addTile(240);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(8, 0, 11)))->removeTile(239);
					};

					auto onClose_1_28 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 11)))->addTile(181);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(10, 0, 11)))->removeTile(238);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(10, 0, 11)))->addTile(237);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(8, 0, 11)))->removeTile(240);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(8, 0, 11)))->addTile(239);
					};

					Door door_1_28(2);
					setupDoor(&door_1_28, "door_1_28", onOpen_1_28, onClose_1_28);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 1), glm::ivec3(10, 0, 10)), "door_1_28", "door_1_28 sensor_0", 9, 0);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 1), glm::ivec3(8, 0, 10)), "door_1_28", "door_1_28 sensor_1", 10, 1);

					// World 1 Door 29
					auto onOpen_1_29 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 9)))->removeTile(185);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 15)))->addTile(256);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 15)))->removeTile(255);
					};

					auto onClose_1_29 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 9)))->addTile(185);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 15)))->removeTile(256);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 15)))->addTile(255);
					};

					Door door_1_29(1);
					setupDoor(&door_1_29, "door_1_29", onOpen_1_29, onClose_1_29);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 14)), "door_1_29", "door_1_29 sensor_0", 13, 0);

					// World 1 Door 30
					auto onOpen_1_30 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(5, 0, 15)))->removeTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(6, 0, 15)))->addTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(6, 0, 15)))->removeTile(203);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(4, 0, 15)))->addTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(4, 0, 15)))->removeTile(203);
					};

					auto onClose_1_30 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(5, 0, 15)))->addTile(165);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(6, 0, 15)))->removeTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(6, 0, 15)))->addTile(203);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(4, 0, 15)))->removeTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(4, 0, 15)))->addTile(203);
					};

					Door door_1_30(2);
					setupDoor(&door_1_30, "door_1_30", onOpen_1_30, onClose_1_30);
					setupDoorSensor(&Location(1, glm::ivec2(-10, 2), glm::ivec3(6, 0, 14)), "door_1_30", "door_1_30 sensor_0", 12, 0);
					setupDoorSensor(&Location(1, glm::ivec2(-10, 2), glm::ivec3(4, 0, 14)), "door_1_30", "door_1_30 sensor_1", 12, 1);

					// World 1 Door 31
					auto onOpen_1_31 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(15, 0, 4)))->removeTile(181);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(14, 0, 4)))->addTile(244);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(14, 0, 4)))->removeTile(243);
					};

					auto onClose_1_31 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(15, 0, 4)))->addTile(181);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(14, 0, 4)))->removeTile(244);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(14, 0, 4)))->addTile(243);
					};

					Door door_1_31(1);
					setupDoor(&door_1_31, "door_1_31", onOpen_1_31, onClose_1_31);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 2), glm::ivec3(14, 0, 3)), "door_1_31", "door_1_31 sensor_0", 12, 0);

					// World 1 Door 32
					auto onOpen_1_32 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(15, 0, 10)))->removeTile(185);
					};

					auto onClose_1_32 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(15, 0, 10)))->addTile(185);
					};

					Door door_1_32;
					door_1_32.setOnOpen(&((std::function<void()>) onOpen_1_32));
					door_1_32.setOnClose(&((std::function<void()>) onClose_1_32));
					TileAmountLock* door_1_32_lock_0 = new TileAmountLock();
					door_1_32_lock_0->specifyRegionAsCuboid(&Location(1, -11, 2, 14, 0, 6), 3, 1, 4);
					door_1_32_lock_0->addTileTypeToCheckFor(9, 4);
					door_1_32_lock_0->addTileTypeToCheckFor(10, 3);
					door_1_32_lock_0->addTileTypeToCheckFor(13, 5);
					Lock* door_1_32_lock_0_base = door_1_32_lock_0;
					door_1_32.addLock(door_1_32_lock_0_base);
					setupDoorTileAmountSensors(door_1_32_lock_0, "door_1_32 sensor_0");
					doorProvider->addDoor("door_1_32", &door_1_32);
					door_1_32_lock_0->setIDInDoor(0);
					door_1_32_lock_0->setDoor(doorProvider->getDoor("door_1_32"));

					// World 1 Door 33
					auto onOpen_1_33 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(5, 0, 7)))->removeTile(189);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(7, 0, 11)))->addTile(266);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(15, 0, 15)))->removeTile(265);
					};

					auto onClose_1_33 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(5, 0, 7)))->addTile(189);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(7, 0, 11)))->removeTile(266);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(15, 0, 15)))->addTile(265);
					};

					Door door_1_33(1);
					setupDoor(&door_1_33, "door_1_33", onOpen_1_33, onClose_1_33);
					setupDoorSensor(&Location(1, glm::ivec2(-10, 2), glm::ivec3(7, 0, 10)), "door_1_33", "door_1_33 sensor_0", 13, 0);

					// World 1 Door 34
					auto onOpen_1_34 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(14, 0, 7)))->removeTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(2, 0, 7)))->addTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(15, 0, 10)))->addTile(212);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(15, 0, 10)))->removeTile(211);
					};

					auto onClose_1_34 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(14, 0, 7)))->addTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(2, 0, 7)))->removeTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(15, 0, 10)))->removeTile(212);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(15, 0, 10)))->addTile(211);
					};

					Door door_1_34({false});
					setupDoor(&door_1_34, "door_1_34", onOpen_1_34, onClose_1_34);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 1), glm::ivec3(15, 0, 9)), "door_1_34", "door_1_34 sensor_0", 11, 0);

					// World 1 Door 35
					auto onOpen_1_35 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(6, 0, 10)))->removeTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(5, 0, 10)))->addTile(222);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(5, 0, 10)))->removeTile(221);
					};

					auto onClose_1_35 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(6, 0, 10)))->addTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(5, 0, 10)))->removeTile(222);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(5, 0, 10)))->addTile(221);
					};

					Door door_1_35(1);
					setupDoor(&door_1_35, "door_1_35", onOpen_1_35, onClose_1_35);
					setupDoorSensor(&Location(1, glm::ivec2(-10, 1), glm::ivec3(5, 0, 9)), "door_1_35", "door_1_35 sensor_0", 11, 0);

					// World 1 Door 36
					auto onOpen_1_36 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(10, 0, 10)))->removeTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(9, 0, 10)))->addTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(9, 0, 10)))->removeTile(231);
					};

					auto onClose_1_36 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(10, 0, 10)))->addTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(9, 0, 10)))->removeTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(9, 0, 10)))->addTile(231);
					};

					Door door_1_36(1);
					setupDoor(&door_1_36, "door_1_36", onOpen_1_36, onClose_1_36);
					setupDoorSensor(&Location(1, glm::ivec2(-10, 1), glm::ivec3(9, 0, 9)), "door_1_36", "door_1_36 sensor_0", 11, 0);

					// World 1 Door 37
					auto onOpen_1_37 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(5, 0, 11)))->removeTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 15)))->addTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 15)))->removeTile(231);
					};

					auto onClose_1_37 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(5, 0, 11)))->addTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 15)))->removeTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 15)))->addTile(231);
					};

					Door door_1_37(1);
					setupDoor(&door_1_37, "door_1_37", onOpen_1_37, onClose_1_37);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 14)), "door_1_37", "door_1_37 sensor_0", 11, 0);

					// World 1 Door 38
					auto onOpen_1_38 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(15, 0, 11)))->removeTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 0)))->addTile(210);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 0)))->removeTile(209);
					};

					auto onClose_1_38 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(15, 0, 11)))->addTile(169);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 0)))->removeTile(210);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 0)))->addTile(209);
					};

					Door door_1_38(1);
					setupDoor(&door_1_38, "door_1_38", onOpen_1_38, onClose_1_38);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 2), glm::ivec3(12, 0, 15)), "door_1_38", "door_1_38 sensor_0", 13, 0);

					// World 1 Door 39
					auto onOpen_1_39 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(14, 0, 10)))->removeTile(193);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(13, 0, 15)))->addTile(272);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(13, 0, 15)))->removeTile(271);
					};

					auto onClose_1_39 = []() {
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(14, 0, 10)))->addTile(193);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(13, 0, 15)))->removeTile(272);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(13, 0, 15)))->addTile(271);
					};

					Door door_1_39(1);
					setupDoor(&door_1_39, "door_1_39", onOpen_1_39, onClose_1_39);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 1), glm::ivec3(13, 0, 14)), "door_1_39", "door_1_39 sensor_0", 11, 0);

					auto onOpen_1_40 = []() {
						setupLevel(1, 2);
						fadeTeleportPlayer(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 0));
						player->getCharacter()->moveIfPossible(TileDirection::FORWARD);
						player->getCharacter()->turn(TileDirection::BACKWARD);
					};

					scriptManager->addScript("door_1_40", onOpen_1_40);
					worldManager->addPromptedScript("door_1_40", &Location(1, -5, 0, 14, 0, 13));
					break;
			}
			break;
	}
}

void Scripts::setupCheckPoints(int stage, int level) {
	switch (stage) {
		case 1:
			switch (level) {
				case 1:
					TileDirection right = TileDirection::RIGHT;
					TileDirection forward = TileDirection::FORWARD;
					TileDirection left = TileDirection::LEFT;
					TileDirection backward = TileDirection::BACKWARD;

					setupCheckPointWithLeftSaveSign("check_point_1_0", Location(1, -2, 0, 5, 0, 7), Location(1, -2, 0, 6, 0, 7), forward);
					setupCheckPointWithLeftSaveSign("check_point_1_1", Location(1, -3, 0, 9, 0, 7), Location(1, -3, 0, 10, 0, 7), forward);
					setupCheckPointWithLeftSaveSign("check_point_1_2", Location(1, -4, 0, 13, 0, 7), Location(1, -4, 0, 14, 0, 7), forward);
					setupCheckPointWithLeftSaveSign("check_point_1_3", Location(1, -2, 0, 5, 0, 7), Location(1, -2, 0, 6, 0, 7), forward);
					setupCheckPointWithBothSaveSigns("check_point_1_4", Location(1, -7, 1, 1, 0, 1), Location(1, -7, 1, 2, 0, 1), Location(1, -7, 1, 0, 0, 1), forward);
					setupCheckPointWithBothSaveSigns("check_point_1_5", Location(1, -8, 2, 9, 0, 15), Location(1, -8, 2, 10, 0, 15), Location(1, -8, 2, 8, 0, 15), forward);
					setupCheckPointWithLeftSaveSign("check_point_1_6", Location(1, -11, 3, 13, 0, 9), Location(1, -11, 3, 14, 0, 9), forward);
					setupCheckPointWithBothSaveSigns("check_point_1_7", Location(1, -12, 3, 4, 0, 0), Location(1, -12, 3, 5, 0, 0), Location(1, -12, 3, 3, 0, 0), backward);
					setupCheckPointWithBothSaveSigns("check_point_1_8", Location(1, -12, 2, 15, 0, 11), Location(1, -11, 2, 0, 0, 11), Location(1, -12, 2, 14, 0, 11), forward);
					setupCheckPointWithBothSaveSigns("check_point_1_9", Location(1, -15, 0, 1, 0, 14), Location(1, -15, 0, 2, 0, 14), Location(1, -15, 0, 0, 0, 14), forward);
					break;
			}
			break;
	}
}

void Scripts::setupGeneralEvents(int stage, int level) {
	switch (stage) {
		case 1:
			switch (level) {
				case 1: {
					scriptManager->addScript("dialogue_0_1", std::bind(&showDialogue, "Now go!", "hide_dialogue"));

					scriptManager->addScript("walked_on_0_0", std::bind(&fadeTeleportPlayer, 1, glm::ivec2(0, 0), glm::ivec3(14, 0, 6)));
					worldManager->addWalkedOnScript("walked_on_0_0", &Location(0, glm::ivec2(0, -1), glm::ivec3(1, 0, 15)));

					auto dialogue_1_0 = []() {
						worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->removeTile(73);
						worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->addTile(74);
						dialogueDisplay->showDialogue("Yes, this is an info kiosk.", "dialogue_1_1");
					};

					scriptManager->addScript("dialogue_1_0", dialogue_1_0);
					worldManager->addPromptedScript("dialogue_1_0", &Location(1, glm::ivec2(0, 0), glm::ivec3(10, 0, 6)));

					auto dialogue_1_1 = []() {
						game->getTileSwitchGun()->setActive(true);
						dialogueDisplay->showDialogue("Here is your gun.", "dialogue_1_2");
					};

					scriptManager->addScript("dialogue_1_1", dialogue_1_1);

					auto dialogue_1_2 = []() {
						worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->removeTile(74);
						worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->addTile(73);
						hideDialogue();
					};

					scriptManager->addScript("dialogue_1_2", dialogue_1_2);
					break;
				}
				case 2:
					scriptManager->addScript("walked_on_0_0", std::bind(&fadeTeleportPlayer, 1, glm::ivec2(-7, 0), glm::ivec3(14, 0, 1)), true);
					worldManager->addWalkedOnScript("walked_on_0_0", &Location(0, glm::ivec2(0, -1), glm::ivec3(1, 0, 15)));
					break;
			}
			break;
	}
}

void Scripts::startNewGame() {
	player->getCharacter()->teleportWithoutAnimation(&Location(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 2)));		
	// worldManager->setCurrentWorld(0);

	// This sets up the player's tile inventory for a new game.
	for (int i = 0; i < 3; i++) {
		player->getTileInventory()->addTile(&Tile(3));
	}

	// The tile switch gun is inactive by default.
	// game->getTileSwitchGun()->setActive(false);

	setupLevel(1, 1);
	dialogueDisplay->showDialogue("Blah, blah, blah, plot, plot, plot.", "dialogue_0_1");
}

void Scripts::continueGame() {}

void Scripts::editMap() {
	game->getTileSwitchGun()->setActive(true);
	player->getCharacter()->teleportWithoutAnimation(&Location(MAP_BEING_EDITED_WORLD_ID, glm::ivec2(0, 0), glm::ivec3(0, 0, 0)));
}

void Scripts::setupLevel(int stage, int level) {
	switch (stage) {
		case 1:
			if (level < 4) {
				loadLogoMaterialAnimation(level);
				setupTeleporters(stage, level);
				setupDoors(stage, level);
				setupCheckPoints(stage, level);
				setupGeneralEvents(stage, level);
			}
			break;
	}
}

void onWaterPlace(Location* location) {
	std::vector<glm::ivec2> chunkCoords;
	std::vector<glm::ivec3> tileCoords;

	for (int i = 0; i < 4; i++) {
		chunkCoords.push_back(location->getChunk());
		tileCoords.push_back(location->getTileGroup());
	}

	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[0], chunkCoords[0], TileDirection::RIGHT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[1], chunkCoords[1], TileDirection::FORWARD);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[2], chunkCoords[2], TileDirection::LEFT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[3], chunkCoords[3], TileDirection::BACKWARD);

	for (int i = 0; i < 4; i++) {
		TileGroup* group = Scripts::worldManager->getTileGroup(&Location(location->getWorld(), chunkCoords[i], tileCoords[i]));
		if (group != nullptr) {
			Tile* tile = group->getTile(10);
			if (tile == nullptr) {
				continue;
			}
			group->removeTile(tile->getType());
			group->addTile(13);
			worldManager->tileWasPlaced(&Location(location->getWorld(), chunkCoords[i], tileCoords[i]), 13);
		}
	}
}

void onLavaPlace(Location* location) {
	std::vector<glm::ivec2> chunkCoords;
	std::vector<glm::ivec3> tileCoords;

	for (int i = 0; i < 4; i++) {
		chunkCoords.push_back(location->getChunk());
		tileCoords.push_back(location->getTileGroup());
	}

	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[0], chunkCoords[0], TileDirection::RIGHT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[1], chunkCoords[1], TileDirection::FORWARD);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[2], chunkCoords[2], TileDirection::LEFT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[3], chunkCoords[3], TileDirection::BACKWARD);

	for (int i = 0; i < 4; i++) {
		TileGroup* waterTileGroup = Scripts::worldManager->getTileGroup(&Location(location->getWorld(), chunkCoords[i], tileCoords[i]));
		if (waterTileGroup != nullptr) {
			if (!waterTileGroup->containsTile(9)) {
				continue;
			}
			TileGroup* lavaTileGroup = Scripts::worldManager->getTileGroup(location);
			if (!lavaTileGroup->removeTile(10)) {
				AelaErrorHandling::windowError("A tilegroup is... corrupt or something?!");
				continue;
			}
			lavaTileGroup->addTile(13);
			worldManager->tileWasPlaced(location, 13);
			break;
		}
	}
}

void Scripts::addTileBehaviours() {
	std::function<void(Location*)> water = &onWaterPlace;
	tileBehaviourExecuter->addBehaviour(9, &water);
	std::function<void(Location*)> lava = &onLavaPlace;
	tileBehaviourExecuter->addBehaviour(10, &lava);
}

void Scripts::fadeTeleportPlayer(size_t world, glm::ivec2 chunk, glm::ivec3 tile) {
	player->getCharacter()->teleportWithAnimation(&Location(world, chunk, tile), TeleportationAnimation::FADE);
}

void Scripts::riseTeleportPlayer(size_t world, glm::ivec2 chunk, glm::ivec3 tile) {
	player->getCharacter()->teleportWithAnimation(&Location(world, chunk, tile), TeleportationAnimation::RISE);
}

void Scripts::unlockDoorLock(std::string tag, size_t lockID) {
	doorProvider->getDoor(tag)->unlock(lockID);
}

void Scripts::setupDoor(Door* door, std::string name, std::function<void()> onOpen, std::function<void()> onClose) {
	door->setOnOpen(&onOpen);
	door->setOnClose(&onClose);
	doorProvider->addDoor(name, door);
}

void Scripts::setupDoorSensor(Location* location, std::string doorName, std::string sensorScriptName, size_t tileType, size_t lockID) {
	scriptManager->addScript(sensorScriptName, std::bind(triggerDoorSensor, *location, tileType, doorName, lockID));
	worldManager->addTileSwitchScript(sensorScriptName, location);
}

void Scripts::setupMultiTypeDoorSensor(Location* location, std::string doorName, std::string sensorScriptName, std::vector<size_t> correctTileTypes, size_t lockID) {
	scriptManager->addScript(sensorScriptName, std::bind(triggerMultiTypeDoorSensor, *location, correctTileTypes, doorName, lockID));
	worldManager->addTileSwitchScript(sensorScriptName, location);
}

void Scripts::setupDoorTileAmountSensors(TileAmountLock* lock, std::string sensorScriptName) {
	auto script = [lock]() {
		lock->checkIfUnlocked();
	};
	scriptManager->addScript(sensorScriptName, script);

	for (auto& tileGroup : *lock->getRegion()) {
		tileGroup->setSwitchScript(sensorScriptName);
	}
}

void Scripts::setupCheckPointWithRightSaveSign(std::string name, Location checkPointLocation, Location rightSaveSignLocation,
	TileDirection directionToMovePlayer) {
	auto event = [name, rightSaveSignLocation, checkPointLocation, directionToMovePlayer]() mutable {
		TileGroup* tileGroup = worldManager->getTileGroup(&rightSaveSignLocation);
		tileGroup->removeTile(161);
		tileGroup->addTile(162);

		player->getCharacter()->moveIfPossible(directionToMovePlayer);
		player->getTileInventory()->clear(3);

		worldManager->getTileGroup(&checkPointLocation)->addTile(307);

		gameSaver->save("save_0");
	};

	scriptManager->addScript(name, event);
	worldManager->addWalkedOnScript(name, &checkPointLocation);
}

void Scripts::setupCheckPointWithLeftSaveSign(std::string name, Location checkPointLocation, Location leftSaveSignLocation,
	TileDirection directionToMovePlayer) {
	auto event = [name, leftSaveSignLocation, checkPointLocation, directionToMovePlayer]() mutable {
		TileGroup* tileGroup = worldManager->getTileGroup(&leftSaveSignLocation);
		tileGroup->removeTile(163);
		tileGroup->addTile(164);

		player->getCharacter()->moveIfPossible(directionToMovePlayer);
		player->getTileInventory()->clear(3);

		worldManager->getTileGroup(&checkPointLocation)->addTile(307);

		gameSaver->save("save_0");
	};

	scriptManager->addScript(name, event);
	worldManager->addWalkedOnScript(name, &checkPointLocation);
}

void Scripts::setupCheckPointWithBothSaveSigns(std::string name, Location checkPointLocation, Location leftSaveSignLocation,
	Location rightSaveSignLocation, TileDirection directionToMovePlayer) {
	auto event = [name, leftSaveSignLocation, rightSaveSignLocation, checkPointLocation, directionToMovePlayer]() mutable {
		TileGroup* leftTileGroup = worldManager->getTileGroup(&leftSaveSignLocation);
		leftTileGroup->removeTile(163);
		leftTileGroup->addTile(164);

		TileGroup* rightTileGroup = worldManager->getTileGroup(&rightSaveSignLocation);
		rightTileGroup->removeTile(161);
		rightTileGroup->addTile(162);

		player->getCharacter()->moveIfPossible(directionToMovePlayer);
		player->getTileInventory()->clear(3);

		worldManager->getTileGroup(&checkPointLocation)->addTile(307);

		gameSaver->save("save_0");
	};

	scriptManager->addScript(name, event);
	worldManager->addWalkedOnScript(name, &checkPointLocation);
}

void Scripts::triggerDoorSensor(Location location, size_t correctTileType, std::string doorTag, size_t lockID) {
	if (worldManager->getTileGroup(&location)->getTile(correctTileType) != nullptr) {
		doorProvider->getDoor(doorTag)->unlock(lockID);
	} else {
		doorProvider->getDoor(doorTag)->lock(lockID);
	}
}

void Scripts::triggerMultiTypeDoorSensor(Location location, std::vector<size_t> correctTileTypes, std::string doorTag, size_t lockID) {
	for (size_t type : correctTileTypes) {
		if (worldManager->getTileGroup(&location)->getTile(type) != nullptr) {
			doorProvider->getDoor(doorTag)->unlock(lockID);
			return;
		}
	}
	doorProvider->getDoor(doorTag)->lock(lockID);
}

void Scripts::addImportantScripts() {
	scriptManager->addScript("setup_characters", &setupCharacters);
	scriptManager->addScript("hide_dialogue", &hideDialogue);
	scriptManager->addScript("start_new_game", &startNewGame);
	scriptManager->addScript("continue_game", &continueGame);
	scriptManager->addScript("edit_map", &editMap);
	scriptManager->addScript("add_tile_behaviours", &addTileBehaviours);
}

void Scripts::setupScripts() {
	addImportantScripts();
}

void Scripts::runGameStartingScripts() {
	scriptManager->runScript("add_tile_behaviours");
}