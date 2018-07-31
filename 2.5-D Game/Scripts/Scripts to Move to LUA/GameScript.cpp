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
#include "../../Resources/ResourceInfo.h"
#include "../ScriptManager.h"

using namespace Scripts;

void dialogue_0_1() {
	dialogueDisplay->showDialogue("Now go!", "hide_dialoguealogue");
}

void dialogue_1_0() {
	worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->removeTile(73);
	worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->addTile(74);
	dialogueDisplay->showDialogue("Yes, this is an info kiosk.", "dialogue_1_1");
}

void dialogue_1_1() {
	game->getTileSwitchGun()->setActive(true);
	dialogueDisplay->showDialogue("Here is your gun.", "dialogue_1_2");
}

void dialogue_1_2() {
	worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->removeTile(74);
	worldManager->getTileGroup(&Location(1, 0, 0, 10, 0, 6))->addTile(73);
	hideDialogue();
}

void ac_1_0() {

}

void Scripts::hideDialogue() {
	dialogueDisplay->closeDialog();
}

void Scripts::setupCharacters() {}

void Scripts::setupTeleporters() {
	// worldManager->addTeleporterToFloorTile(&Location(1, 0, 0, 12, 0, 6), &Location(1, 0, 0, 14, 0, 6));
}

void Scripts::setupDoors() {
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

	Door door_1_0(1, true);
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

	Door door_1_1(2, true, true);
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

	Door door_1_2(2, true, true);
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

	Door door_1_3(1, true);
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

	Door door_1_4(1, true);
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

	Door door_1_5(1, true);
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

	Door door_1_6(1, true);
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

	Door door_1_7(1, true);
	setupDoor(&door_1_7, "door_1_7", onOpen_1_7, onClose_1_7);
	setupDoorSensor(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 7)), "door_1_7", "door_1_7 sensor_0", 9, 0);

	// World 1 Door 8
	auto onOpen_1_8 = []() {
		worldManager->getTileGroup(&Location(1, glm::ivec2(-3, 0), glm::ivec3(9, 0, 7)))->removeTile(165);
		AelaErrorHandling::windowWarning("YAY!");
	};

	auto onClose_1_8 = []() {
		worldManager->getTileGroup(&Location(1, glm::ivec2(-3, 0), glm::ivec3(9, 0, 7)))->addTile(165);
		AelaErrorHandling::windowWarning("NAY!");
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
		std::cout << "YES!\n";
	};

	auto onClose_1_10 = []() {
		worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 8)))->addTile(173);
		worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(4, 0, 8)))->addTile(173);
		std::cout << "NO!\n";
	};

	std::cout << "start lol\n";
	Door door_1_10;
	door_1_10.setOnOpen(&((std::function<void()>) onOpen_1_10));
	door_1_10.setOnClose(&((std::function<void()>) onClose_1_10));
	TileAmountLock* door_1_10_lock_0 = new TileAmountLock();
	door_1_10_lock_0->specifyRegionAsCuboid(&Location(1, -4, 0, 1, 0, 3), 6, 1, 5);
	door_1_10_lock_0->addTileTypeToCheckFor(9, 9);
	door_1_10_lock_0->addTileTypeToCheckFor(10, 9);
	door_1_10_lock_0->addTileTypeToCheckFor(11, 3);
	door_1_10_lock_0->addTileTypeToCheckFor(13, 9);
	door_1_10.addLock((Lock*) &door_1_10_lock_0);
	setupDoorTileAmountSensors(door_1_10_lock_0, "door_1_10 sensor_0");
	doorProvider->addDoor("door_1_10", &door_1_10);
	door_1_10_lock_0->setIDInDoor(0);
	door_1_10_lock_0->setDoor(doorProvider->getDoor("door_1_10"));
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

	scriptManager->runScript("setup_stage_1_level_1");
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
			if (level < 5) {
				loadLogoMaterialAnimation(level);
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

void Scripts::setupDoorTileAmountSensors(TileAmountLock* lock, std::string sensorScriptName) {
	auto script = [lock]() {
		lock->checkIfUnlocked();
	};
	scriptManager->addScript(sensorScriptName, script);

	for (auto& tileGroup : *lock->getRegion()) {
		tileGroup->setSwitchScript(sensorScriptName);
	}
}

void Scripts::triggerDoorSensor(Location location, size_t correctTileType, std::string doorTag, size_t lockID) {
	if (worldManager->getTileGroup(&location)->getTile(correctTileType) != nullptr) {
		doorProvider->getDoor(doorTag)->unlock(lockID);
	} else {
		doorProvider->getDoor(doorTag)->lock(lockID);
	}
}

void Scripts::addScriptsToGameWorld() {
	scriptManager->addScript("setup_characters", &setupCharacters);
	scriptManager->addScript("setup_teleporters", &setupTeleporters);
	scriptManager->addScript("setup_doors", &setupDoors);
	scriptManager->addScript("hide_dialoguealogue", &hideDialogue);
	scriptManager->addScript("start_new_game", &startNewGame);
	scriptManager->addScript("continue_game", &continueGame);
	scriptManager->addScript("edit_map", &editMap);
	scriptManager->addScript("add_tile_behaviours", &addTileBehaviours);

	scriptManager->addScript("setup_stage_1_level_1", std::bind(&setupLevel, 1, 1));
	scriptManager->addScript("setup_stage_1_level_2", std::bind(&setupLevel, 1, 2));
	scriptManager->addScript("setup_stage_1_level_3", std::bind(&setupLevel, 1, 3));
	scriptManager->addScript("setup_stage_1_level_4", std::bind(&setupLevel, 1, 4));
	scriptManager->addScript("setup_stage_1_level_5", std::bind(&setupLevel, 1, 5));

	scriptManager->addScript("walked_on_0_0", std::bind(&fadeTeleportPlayer, 1, glm::ivec2(0, 0), glm::ivec3(14, 0, 6)));
	worldManager->addWalkedOnScript("walked_on_0_0", &Location(0, glm::ivec2(0, -1), glm::ivec3(1, 0, 15)));

	scriptManager->addScript("dialogue_0_1", &dialogue_0_1);

	scriptManager->addScript("dialogue_1_0", &dialogue_1_0);
	worldManager->addPromptedScript("dialogue_1_0", &Location(1, glm::ivec2(0, 0), glm::ivec3(10, 0, 6)));

	scriptManager->addScript("dialogue_1_1", &dialogue_1_1);
	scriptManager->addScript("dialogue_1_2", &dialogue_1_2);
}

void Scripts::setupScripts() {
	addScriptsToGameWorld();
}

void Scripts::runGameStartingScripts() {
	scriptManager->runScript("setup_characters");
	scriptManager->runScript("setup_teleporters");
	scriptManager->runScript("setup_doors");
	scriptManager->runScript("add_tile_behaviours");
}