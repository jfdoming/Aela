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
#include "../../Displays/Hints/HintDisplay.h"
#include "../../Displays/Tiles/TileInventoryDisplay.h"
#include "../../Enemies/EnemyProvider.h"
#include "../../Enemies/Turret.h"
#include "../ScriptManager.h"

#define BUTTON_PRESSING_TIME 500

using namespace Scripts;

namespace Scripts {
	bool aelaTalkedToPlayerInDialogue1 = false;

}

void Scripts::showDialogue(std::string name, std::string avatar, std::string text, std::string scriptToRunOnceComplete) {
	dialogueDisplay->showDialogue(name, avatar, text, scriptToRunOnceComplete);
}

void Scripts::showTwoOptions(DialogueOption option1, DialogueOption option2) {
	dialogueDisplay->showOptions(&option1, &option2);
}

void Scripts::showThreeOptions(DialogueOption option1, DialogueOption option2, DialogueOption option3) {
	dialogueDisplay->showOptions(&option1, &option2, &option3);
}

void Scripts::showFourOptions(DialogueOption option1, DialogueOption option2, DialogueOption option3, DialogueOption option4) {
	dialogueDisplay->showOptions(&option1, &option2, &option3, &option4);
}

void Scripts::showHint(std::string text, HintDisplayDuration duration) {
	hintDisplay->displayHint(text, duration);
}

void Scripts::hideDialogue() {
	dialogueDisplay->closeDialog();
}

void Scripts::setupCharacters(int stage, int level) {
	switch (stage) {
		case 1:
			switch (level) {
				case 1: {
					Character* worker_1 = new Character("worker_1", &Location(1, -3, 0, 4, 1, 9));
					worker_1->setTextureName("scientist_1");
					characterProvider->addCharacter(worker_1);
					break;
				}
				case 2: {
					Character* worker_1 = characterProvider->getCharacterByName("worker_1");
					if (worker_1 != nullptr) {
						worker_1->teleportImmediately(&Location(1, -10, 3, 11, 1, 14));
					}

					Character* soldier_1 = new Character("soldier_1", &Location(1, -9, 4, 11, -4, 5), 0.00275f, 0);
					soldier_1->setTextureName("soldier_2");
					characterProvider->addCharacter(soldier_1);

					Character* soldier_2 = new Character("soldier_2", &Location(1, -9, 4, 12, -4, 4), 0.00275f, 0);
					soldier_2->setTextureName("soldier_1");
					characterProvider->addCharacter(soldier_2);

					Character* soldier_3 = new Character("soldier_3", &Location(1, -9, 4, 10, -4, 2), 0.00275f, 0);
					soldier_3->setTextureName("soldier_1");
					characterProvider->addCharacter(soldier_3);

					Character* soldier_4 = new Character("soldier_4", &Location(1, -9, 4, 11, -4, 1), 0.00275f, 0);
					soldier_4->setTextureName("soldier_2");
					characterProvider->addCharacter(soldier_4);

					Character* scientist_1 = new Character("scientist_1", &Location(1, -9, 4, 11, -4, 4), 0.00275f, 0);
					scientist_1->setTextureName("scientist_1");
					characterProvider->addCharacter(scientist_1);

					Character* scientist_2 = new Character("scientist_2", &Location(1, -9, 4, 11, -4, 3), 0.00275f, 0);
					scientist_2->setTextureName("scientist_1");
					characterProvider->addCharacter(scientist_2);

					Character* scientist_3 = new Character("scientist_3", &Location(1, -9, 4, 11, -4, 2), 0.00275f, 0);
					scientist_3->setTextureName("scientist_1");
					characterProvider->addCharacter(scientist_3);
					break;
				}
				case 3: {
					Character* looker = new Character("looker", &Location(1, -22, 1, 7, 2, 2));

					Turret* turret = new Turret("turret_1", &Location(1, -15, 0, 0, 0, 11), 2);
					turret->setTextureName("turret_1");
					turret->setDetectionAngles(false, false, true, false);
					turret->turnImmediately(TileDirection::LEFT);
					characterProvider->addCharacter(turret);
					enemyProvider->addTurret(turret);

					Turret* turret_2 = new Turret("turret_2", &Location(1, -16, 0, 9, 0, 9), 6);
					turret_2->setTextureName("turret_2");
					turret_2->setDetectionAngles(true, false, false, false);
					turret_2->turnImmediately(TileDirection::RIGHT);
					characterProvider->addCharacter(turret_2);
					enemyProvider->addTurret(turret_2);

					Turret* turret_3 = new Turret("turret_3", &Location(1, -16, 0, 1, 0, 8), 6);
					turret_3->setTextureName("turret_3");
					turret_3->setDetectionAngles(true, false, false, false);
					turret_3->turnImmediately(TileDirection::RIGHT);
					characterProvider->addCharacter(turret_3);
					enemyProvider->addTurret(turret_3);

					Turret* turret_4 = new Turret("turret_4", &Location(1, -16, 0, 1, 0, 10), 6);
					turret_4->setTextureName("turret_4");
					turret_4->setDetectionAngles(true, false, false, false);
					turret_4->turnImmediately(TileDirection::RIGHT);
					characterProvider->addCharacter(turret_4);
					enemyProvider->addTurret(turret_4);

					Turret* turret_5 = new Turret("turret_5", &Location(1, -18, 0, 12, 0, 13), 4);
					turret_5->setTextureName("turret_5");
					turret_5->setDetectionAngles(false, true, false, false);
					turret_5->turnImmediately(TileDirection::FORWARD);
					characterProvider->addCharacter(turret_5);
					enemyProvider->addTurret(turret_5);

					Turret* turret_6 = new Turret("turret_6", &Location(1, -18, 1, 6, 0, 3), 4);
					turret_6->setTextureName("turret_6");
					turret_6->setDetectionAngles(false, false, false, true);
					turret_6->turnImmediately(TileDirection::BACKWARD);
					characterProvider->addCharacter(turret_6);
					enemyProvider->addTurret(turret_6);

					Turret* turret_7 = new Turret("turret_7", &Location(1, -19, 0, 6, 0, 13), 4);
					turret_7->setTextureName("turret_7");
					turret_7->setDetectionAngles(false, true, false, false);
					turret_7->turnImmediately(TileDirection::FORWARD);
					characterProvider->addCharacter(turret_7);
					enemyProvider->addTurret(turret_7);

					Turret* turret_8 = new Turret("turret_8", &Location(1, -20, 0, 4, 0, 13), 4);
					turret_8->setTextureName("turret_8");
					turret_8->setDetectionAngles(true, false, false, false);
					turret_8->turnImmediately(TileDirection::FORWARD);
					characterProvider->addCharacter(turret_8);
					enemyProvider->addTurret(turret_8);
					break;
				}
			}
	}
}

void Scripts::setupTeleporters(int stage, int level) {
	switch (stage) {
		case 1:
			switch (level) {
				case 2:
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
				case 3:
					worldManager->addTeleporterToFloorTile(&Location(1, -17, 1, 7, 0, 2), &Location(1, -17, 3, 8, 0, 3));
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
						openDoor("door_1_0", 165, Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 2)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 2)))->removeTile(197);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 2)))->addTile(198);
					};

					auto onClose_1_0 = []() {
						closeDoor("door_1_0", 165, Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 2)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 2)))->removeTile(198);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 2)))->addTile(197);
					};

					Door door_1_0(1);
					setupDoor(&door_1_0, "door_1_0", onOpen_1_0, onClose_1_0);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 1)), "door_1_0", "door_1_0 sensor_0", 9, 0);

					// World 1 Door 1
					auto onOpen_1_1 = []() {
						openDoor("door_1_1", 169, Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 5)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 5)))->removeTile(207);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 5)))->addTile(208);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 5)))->removeTile(209);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 5)))->addTile(210);
					};

					auto onClose_1_1 = []() {
						closeDoor("door_1_1", 169, Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 5)));
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
						openDoor("door_1_2", 173, Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 8)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 8)))->removeTile(217);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(11, 0, 8)))->addTile(218);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 8)))->removeTile(221);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(9, 0, 8)))->addTile(222);
					};

					auto onClose_1_2 = []() {
						closeDoor("door_1_2", 173, Location(1, glm::ivec2(-1, 0), glm::ivec3(10, 0, 8)));
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
						openDoor("door_1_3", 177, Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 5)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(2, 0, 5)))->removeTile(231);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(2, 0, 5)))->addTile(232);
					};

					auto onClose_1_3 = []() {
						closeDoor("door_1_3", 177, Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 5)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(2, 0, 5)))->addTile(231);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(2, 0, 5)))->removeTile(232);
					};

					Door door_1_3(1);
					setupDoor(&door_1_3, "door_1_3", onOpen_1_3, onClose_1_3);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(2, 0, 4)), "door_1_3", "door_1_3 sensor_0", 11, 0);

					// World 1 Door 4
					auto onOpen_1_4 = []() {
						openDoor("door_1_4", 181, Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 5)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 8)))->removeTile(237);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 8)))->addTile(238);
					};

					auto onClose_1_4 = []() {
						closeDoor("door_1_4", 181, Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 5)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 8)))->addTile(237);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 8)))->removeTile(238);
					};

					Door door_1_4(1);
					setupDoor(&door_1_4, "door_1_4", onOpen_1_4, onClose_1_4);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 7)), "door_1_4", "door_1_4 sensor_0", 9, 0);

					// World 1 Door 5
					auto onOpen_1_5 = []() {
						openDoor("door_1_5", 185, Location(1, glm::ivec2(-1, -1), glm::ivec3(1, 0, 14)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 8)))->removeTile(247);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 8)))->addTile(248);
					};

					auto onClose_1_5 = []() {
						closeDoor("door_1_5", 185, Location(1, glm::ivec2(-1, -1), glm::ivec3(1, 0, 14)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 8)))->addTile(247);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 8)))->removeTile(248);
					};

					Door door_1_5(1);
					setupDoor(&door_1_5, "door_1_5", onOpen_1_5, onClose_1_5);
					setupDoorSensor(&Location(1, glm::ivec2(-2, 0), glm::ivec3(13, 0, 7)), "door_1_5", "door_1_5 sensor_0", 9, 0);

					// World 1 Door 6
					auto onOpen_1_6 = []() {
						openDoor("door_1_6", 189, Location(1, glm::ivec2(-2, 0), glm::ivec3(10, 0, 1)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 1)))->removeTile(261);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 1)))->addTile(262);
					};

					auto onClose_1_6 = []() {
						closeDoor("door_1_6", 189, Location(1, glm::ivec2(-2, 0), glm::ivec3(10, 0, 1)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 1)))->addTile(261);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 1)))->removeTile(262);
					};

					Door door_1_6(1);
					setupDoor(&door_1_6, "door_1_6", onOpen_1_6, onClose_1_6);
					setupDoorSensor(&Location(1, glm::ivec2(-1, 0), glm::ivec3(1, 0, 0)), "door_1_6", "door_1_6 sensor_0", 11, 0);

					// World 1 Door 7
					auto onOpen_1_7 = []() {
						openDoor("door_1_7", 193, Location(1, glm::ivec2(-2, 0), glm::ivec3(5, 0, 7)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 8)))->removeTile(267);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 8)))->addTile(268);
					};

					auto onClose_1_7 = []() {
						closeDoor("door_1_7", 193, Location(1, glm::ivec2(-2, 0), glm::ivec3(5, 0, 7)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 8)))->addTile(267);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 8)))->removeTile(268);
					};

					Door door_1_7(1);
					setupDoor(&door_1_7, "door_1_7", onOpen_1_7, onClose_1_7);
					setupDoorSensor(&Location(1, glm::ivec2(-2, 0), glm::ivec3(8, 0, 7)), "door_1_7", "door_1_7 sensor_0", 9, 0);

					// World 1 Door 8
					auto onOpen_1_8 = []() {
						openDoor("door_1_8", 165, Location(1, glm::ivec2(-3, 0), glm::ivec3(9, 0, 7)));
					};

					auto onClose_1_8 = []() {
						closeDoor("door_1_8", 165, Location(1, glm::ivec2(-3, 0), glm::ivec3(9, 0, 7)));
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
						openDoor("door_1_9", 169, Location(1, glm::ivec2(-4, 0), glm::ivec3(13, 0, 7)));
					};

					auto onClose_1_9 = []() {
						closeDoor("door_1_9", 169, Location(1, glm::ivec2(-4, 0), glm::ivec3(13, 0, 7)));
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
						openDoor("door_1_10_1", 173, Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 8)));
						openDoor("door_1_10_2", 173, Location(1, glm::ivec2(-4, 0), glm::ivec3(4, 0, 8)));
					};

					auto onClose_1_10 = []() {
						closeDoor("door_1_10_1", 173, Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 8)));
						closeDoor("door_1_10_2", 173, Location(1, glm::ivec2(-4, 0), glm::ivec3(4, 0, 8)));
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
						openDoor("door_1_11", 177, Location(1, glm::ivec2(-5, 0), glm::ivec3(13, 0, 10)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 11)))->removeTile(227);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 11)))->addTile(228);
					};

					auto onClose_1_11 = []() {
						closeDoor("door_1_11", 177, Location(1, glm::ivec2(-5, 0), glm::ivec3(13, 0, 10)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 11)))->addTile(227);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 11)))->removeTile(228);
					};

					Door door_1_11(1);
					setupDoor(&door_1_11, "door_1_11", onOpen_1_11, onClose_1_11);
					setupDoorSensor(&Location(1, glm::ivec2(-4, 0), glm::ivec3(3, 0, 10)), "door_1_11", "door_1_11 sensor_0", 9, 0);

					// World 1 Door 12
					auto onOpen_1_12 = []() {
						closeDoor("door_1_12_1", 165, Location(1, glm::ivec2(-7, 0), glm::ivec3(13, 0, 13)));
						openDoor("door_1_12_2", 165, Location(1, glm::ivec2(-7, 0), glm::ivec3(5, 0, 13)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(9, 0, 0)))->removeTile(199);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(9, 0, 0)))->addTile(200);
					};

					auto onClose_1_12 = []() {
						openDoor("door_1_12_1", 165, Location(1, glm::ivec2(-7, 0), glm::ivec3(13, 0, 13)));
						closeDoor("door_1_12_2", 165, Location(1, glm::ivec2(-7, 0), glm::ivec3(5, 0, 13)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(9, 0, 0)))->addTile(199);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(9, 0, 0)))->removeTile(200);
					};

					Door door_1_12(1);
					setupDoor(&door_1_12, "door_1_12", onOpen_1_12, onClose_1_12);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 0), glm::ivec3(9, 0, 15)), "door_1_12", "door_1_12 sensor_0", 10, 0);

					// World 1 Door 13
					auto onOpen_1_13 = []() {
						openDoor("door_1_0", 169, Location(1, glm::ivec2(-7, 0), glm::ivec3(9, 0, 13)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(12, 0, 0)))->removeTile(209);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(12, 0, 0)))->addTile(210);
					};

					auto onClose_1_13 = []() {
						closeDoor("door_1_0", 169, Location(1, glm::ivec2(-7, 0), glm::ivec3(9, 0, 13)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(12, 0, 0)))->addTile(209);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(12, 0, 0)))->removeTile(210);
					};

					Door door_1_13(1);
					setupDoor(&door_1_13, "door_1_13", onOpen_1_13, onClose_1_13);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 0), glm::ivec3(12, 0, 15)), "door_1_13", "door_1_13 sensor_0", 10, 0);

					// World 1 Door 14
					auto onOpen_1_14 = []() {
						openDoor("door_1_14", 173, Location(1, glm::ivec2(-7, 0), glm::ivec3(1, 0, 13)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(14, 0, 0)))->removeTile(219);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(14, 0, 0)))->addTile(220);
					};

					auto onClose_1_14 = []() {
						closeDoor("door_1_14", 173, Location(1, glm::ivec2(-7, 0), glm::ivec3(1, 0, 13)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(14, 0, 0)))->addTile(219);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(14, 0, 0)))->removeTile(220);
					};

					Door door_1_14(1);
					setupDoor(&door_1_14, "door_1_14", onOpen_1_14, onClose_1_14);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 0), glm::ivec3(14, 0, 15)), "door_1_14", "door_1_14 sensor_0", 11, 0);

					// World 1 Door 15
					auto onClose_1_15 = []() {
						closeDoor("door_1_15", 181, Location(1, glm::ivec2(-7, 1), glm::ivec3(5, 0, 6)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(1, 0, 11)))->addTile(241);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(1, 0, 11)))->removeTile(242);
					};

					auto onTimerCompletion_1_15 = [onClose_1_15]() {
						onClose_1_15();
						doorProvider->getDoor("door_1_15")->lock(0);
					};

					auto onOpen_1_15 = [onTimerCompletion_1_15]() {
						openDoor("door_1_15", 181, Location(1, glm::ivec2(-7, 1), glm::ivec3(5, 0, 6)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(1, 0, 11)))->removeTile(241);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 1), glm::ivec3(1, 0, 11)))->addTile(242);
						timer->scheduleEventInSeconds(3, onTimerCompletion_1_15);
					};

					Door door_1_15(1);
					setupDoor(&door_1_15, "door_1_15", onOpen_1_15, nullptr);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 1), glm::ivec3(1, 0, 10)), "door_1_15", "door_1_15 sensor_0", 11, 0);

					// World 1 Door 16
					auto onOpen_1_16 = []() {
						closeDoor("door_1_16_1", 169, Location(1, glm::ivec2(-7, 2), glm::ivec3(4, 0, 5)));
						openDoor("door_1_16_2", 169, Location(1, glm::ivec2(-7, 2), glm::ivec3(8, 0, 5)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(9, 0, 5)))->addTile(210);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(9, 0, 5)))->removeTile(209);
					};

					auto onClose_1_16 = []() {
						openDoor("door_1_16_1", 169, Location(1, glm::ivec2(-7, 2), glm::ivec3(4, 0, 5)));
						closeDoor("door_1_16_2", 169, Location(1, glm::ivec2(-7, 2), glm::ivec3(8, 0, 5)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(9, 0, 5)))->addTile(209);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-7, 2), glm::ivec3(9, 0, 5)))->removeTile(210);
					};

					Door door_1_16({false});
					setupDoor(&door_1_16, "door_1_16", onOpen_1_16, onClose_1_16);
					setupDoorSensor(&Location(1, glm::ivec2(-7, 2), glm::ivec3(9, 0, 4)), "door_1_16", "door_1_16 sensor_0", 10, 0);

					// World 1 Door 17
					auto onClose_1_17 = []() {
						closeDoor("door_1_17_1", 185, Location(1, glm::ivec2(-7, 1), glm::ivec3(5, 0, 14)));
						closeDoor("door_1_17_2", 185, Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 0)));
						closeDoor("door_1_17_3", 185, Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 8)));
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
						openDoor("door_1_17_1", 185, Location(1, glm::ivec2(-7, 1), glm::ivec3(5, 0, 14)));
						openDoor("door_1_17_2", 185, Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 0)));
						openDoor("door_1_17_3", 185, Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 8)));
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
						openDoor("door_1_18", 165, Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 4)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 4)))->addTile(202);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 4)))->removeTile(201);
					};

					auto onClose_1_18 = []() {
						closeDoor("door_1_18", 165, Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 4)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 4)))->removeTile(202);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 4)))->addTile(201);
					};

					Door door_1_18(1);
					setupDoor(&door_1_18, "door_1_18", onOpen_1_18, onClose_1_18);
					setupDoorSensor(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 3)), "door_1_18", "door_1_18 sensor_0", 11, 0);

					// World 1 Door 19
					auto onOpen_1_19 = []() {
						openDoor("door_1_19", 189, Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 8)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 8)))->removeTile(189);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(8, 0, 8)))->addTile(266);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(8, 0, 8)))->removeTile(265);
					};

					auto onClose_1_19 = []() {
						closeDoor("door_1_19", 189, Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 8)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(8, 0, 8)))->removeTile(266);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(8, 0, 8)))->addTile(265);
					};

					Door door_1_19(1);
					setupDoor(&door_1_19, "door_1_19", onOpen_1_19, onClose_1_19);
					setupDoorSensor(&Location(1, glm::ivec2(-8, 2), glm::ivec3(8, 0, 7)), "door_1_19", "door_1_19 sensor_0", 13, 0);

					// World 1 Door 20
					auto onOpen_1_20 = []() {
						openDoor("door_1_20", 173, Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 12)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 12)))->addTile(222);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 12)))->removeTile(221);
					};

					auto onClose_1_20 = []() {
						closeDoor("door_1_20", 173, Location(1, glm::ivec2(-8, 2), glm::ivec3(9, 0, 12)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 12)))->removeTile(222);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 12)))->addTile(221);
					};

					Door door_1_20(1);
					setupDoor(&door_1_20, "door_1_20", onOpen_1_20, onClose_1_20);
					setupDoorSensor(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 11)), "door_1_20", "door_1_20 sensor_0", 11, 0);

					// World 1 Door 21
					auto onOpen_1_21 = []() {
						openDoor("door_1_21", 193, Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 15)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 15)))->addTile(270);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 15)))->removeTile(269);
					};

					auto onClose_1_21 = []() {
						closeDoor("door_1_21", 193, Location(1, glm::ivec2(-8, 2), glm::ivec3(13, 0, 15)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 15)))->removeTile(270);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 15)))->addTile(269);
					};

					Door door_1_21(1);
					setupDoor(&door_1_21, "door_1_21", onOpen_1_21, onClose_1_21);
					setupDoorSensor(&Location(1, glm::ivec2(-8, 2), glm::ivec3(12, 0, 14)), "door_1_21", "door_1_21 sensor_0", 10, 0);

					// World 1 Door 22
					auto onOpen_1_22 = []() {
						openDoor("door_1_22", 165, Location(1, glm::ivec2(-11, 3), glm::ivec3(11, 0, 14)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(12, 0, 14)))->addTile(200);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(12, 0, 14)))->removeTile(199);
					};

					auto onClose_1_22 = []() {
						closeDoor("door_1_22", 165, Location(1, glm::ivec2(-11, 3), glm::ivec3(11, 0, 14)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(12, 0, 14)))->removeTile(200);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 3), glm::ivec3(12, 0, 14)))->addTile(199);
					};

					Door door_1_22(1);
					setupDoor(&door_1_22, "door_1_22", onOpen_1_22, onClose_1_22);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 3), glm::ivec3(12, 0, 13)), "door_1_22", "door_1_22 sensor_0", 10, 0);

					// World 1 Door 23
					auto onOpen_1_23 = []() {
						openDoor("door_1_23_1", 169, Location(1, glm::ivec2(-11, 3), glm::ivec3(2, 0, 14)));
						closeDoor("door_1_23_2", 169, Location(1, glm::ivec2(-11, 3), glm::ivec3(7, 0, 14)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(11, 0, 2)))->addTile(214);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(11, 0, 2)))->removeTile(213);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(2, 0, 2)))->addTile(208);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(2, 0, 2)))->removeTile(207);
					};

					auto onClose_1_23 = []() {
						closeDoor("door_1_23_1", 169, Location(1, glm::ivec2(-11, 3), glm::ivec3(2, 0, 14)));
						openDoor("door_1_23_2", 169, Location(1, glm::ivec2(-11, 3), glm::ivec3(7, 0, 14)));
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
						openDoor("door_1_24", 173, Location(1, glm::ivec2(-11, 4), glm::ivec3(7, 0, 1)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(7, 0, 1)))->removeTile(173);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(6, 0, 1)))->addTile(218);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(6, 0, 1)))->removeTile(217);
					};

					auto onClose_1_24 = []() {
						closeDoor("door_1_24", 173, Location(1, glm::ivec2(-11, 4), glm::ivec3(7, 0, 1)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(6, 0, 1)))->removeTile(218);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 4), glm::ivec3(6, 0, 1)))->addTile(217);
					};

					Door door_1_24(1);
					setupDoor(&door_1_24, "door_1_24", onOpen_1_24, onClose_1_24);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 4), glm::ivec3(6, 0, 0)), "door_1_24", "door_1_24 sensor_0", 9, 0);

					// World 1 Door 25
					auto onOpen_1_25 = []() {
						openDoor("door_1_25", 177, Location(1, glm::ivec2(-12, 3), glm::ivec3(13, 0, 14)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(13, 0, 14)))->removeTile(177);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 14)))->addTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 14)))->removeTile(231);
					};

					auto onClose_1_25 = []() {
						closeDoor("door_1_25", 177, Location(1, glm::ivec2(-12, 3), glm::ivec3(13, 0, 14)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 14)))->removeTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 14)))->addTile(231);
					};

					Door door_1_25(1);
					setupDoor(&door_1_25, "door_1_25", onOpen_1_25, onClose_1_25);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 13)), "door_1_25", "door_1_25 sensor_0", 11, 0);

					// World 1 Door 26
					auto onOpen_1_26 = []() {
						openDoor("door_1_26", 165, Location(1, glm::ivec2(-12, 2), glm::ivec3(15, 0, 3)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(0, 0, 3)))->addTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(0, 0, 3)))->removeTile(203);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(14, 0, 3)))->addTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 2), glm::ivec3(14, 0, 3)))->removeTile(203);
					};

					auto onClose_1_26 = []() {
						closeDoor("door_1_26", 165, Location(1, glm::ivec2(-12, 2), glm::ivec3(15, 0, 3)));
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
						openDoor("door_1_27_1", 173, Location(1, glm::ivec2(-11, 2), glm::ivec3(4, 0, 3)));
						openDoor("door_1_27_2", 173, Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 0, 3)));
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
						closeDoor("door_1_27_1", 173, Location(1, glm::ivec2(-11, 2), glm::ivec3(4, 0, 3)));
						closeDoor("door_1_27_2", 173, Location(1, glm::ivec2(-12, 2), glm::ivec3(10, 0, 3)));
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
						openDoor("door_1_28", 181, Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 11)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(10, 0, 11)))->addTile(238);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(10, 0, 11)))->removeTile(237);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(8, 0, 11)))->addTile(240);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(8, 0, 11)))->removeTile(239);
					};

					auto onClose_1_28 = []() {
						closeDoor("door_1_28", 181, Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 11)));
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
						openDoor("door_1_29", 185, Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 9)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 15)))->addTile(256);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 15)))->removeTile(255);
					};

					auto onClose_1_29 = []() {
						closeDoor("door_1_29", 185, Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 9)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 15)))->removeTile(256);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 15)))->addTile(255);
					};

					Door door_1_29(1);
					setupDoor(&door_1_29, "door_1_29", onOpen_1_29, onClose_1_29);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 1), glm::ivec3(15, 0, 14)), "door_1_29", "door_1_29 sensor_0", 13, 0);

					// World 1 Door 30
					auto onOpen_1_30 = []() {
						openDoor("door_1_30", 165, Location(1, glm::ivec2(-10, 2), glm::ivec3(5, 0, 15)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(6, 0, 15)))->addTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(6, 0, 15)))->removeTile(203);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(4, 0, 15)))->addTile(204);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(4, 0, 15)))->removeTile(203);
					};

					auto onClose_1_30 = []() {
						closeDoor("door_1_30", 165, Location(1, glm::ivec2(-10, 2), glm::ivec3(5, 0, 15)));
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
						openDoor("door_1_31", 181, Location(1, glm::ivec2(-11, 2), glm::ivec3(15, 0, 4)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(14, 0, 4)))->addTile(244);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(14, 0, 4)))->removeTile(243);
					};

					auto onClose_1_31 = []() {
						closeDoor("door_1_31", 181, Location(1, glm::ivec2(-11, 2), glm::ivec3(15, 0, 4)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(14, 0, 4)))->removeTile(244);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 2), glm::ivec3(14, 0, 4)))->addTile(243);
					};

					Door door_1_31(1);
					setupDoor(&door_1_31, "door_1_31", onOpen_1_31, onClose_1_31);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 2), glm::ivec3(14, 0, 3)), "door_1_31", "door_1_31 sensor_0", 12, 0);

					// World 1 Door 32
					auto onOpen_1_32 = []() {
						openDoor("door_1_32", 185, Location(1, glm::ivec2(-11, 2), glm::ivec3(15, 0, 10)));
					};

					auto onClose_1_32 = []() {
						closeDoor("door_1_32", 185, Location(1, glm::ivec2(-11, 2), glm::ivec3(15, 0, 10)));
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
						openDoor("door_1_33", 189, Location(1, glm::ivec2(-10, 2), glm::ivec3(5, 0, 7)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(7, 0, 11)))->addTile(266);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(15, 0, 15)))->removeTile(265);
					};

					auto onClose_1_33 = []() {
						closeDoor("door_1_33", 189, Location(1, glm::ivec2(-10, 2), glm::ivec3(5, 0, 7)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(7, 0, 11)))->removeTile(266);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 2), glm::ivec3(15, 0, 15)))->addTile(265);
					};

					Door door_1_33(1);
					setupDoor(&door_1_33, "door_1_33", onOpen_1_33, onClose_1_33);
					setupDoorSensor(&Location(1, glm::ivec2(-10, 2), glm::ivec3(7, 0, 10)), "door_1_33", "door_1_33 sensor_0", 13, 0);

					// World 1 Door 34
					auto onOpen_1_34 = []() {
						openDoor("door_1_34_1", 169, Location(1, glm::ivec2(-11, 1), glm::ivec3(14, 0, 7)));
						closeDoor("door_1_34_2", 169, Location(1, glm::ivec2(-10, 1), glm::ivec3(2, 0, 7)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(15, 0, 10)))->addTile(212);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(15, 0, 10)))->removeTile(211);
					};

					auto onClose_1_34 = []() {
						closeDoor("door_1_34_1", 169, Location(1, glm::ivec2(-11, 1), glm::ivec3(14, 0, 7)));
						openDoor("door_1_34_2", 169, Location(1, glm::ivec2(-10, 1), glm::ivec3(2, 0, 7)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(15, 0, 10)))->removeTile(212);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(15, 0, 10)))->addTile(211);
					};

					Door door_1_34({false});
					setupDoor(&door_1_34, "door_1_34", onOpen_1_34, onClose_1_34);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 1), glm::ivec3(15, 0, 9)), "door_1_34", "door_1_34 sensor_0", 11, 0);

					// World 1 Door 35
					auto onOpen_1_35 = []() {
						openDoor("door_1_35", 173, Location(1, glm::ivec2(-10, 1), glm::ivec3(6, 0, 10)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(5, 0, 10)))->addTile(222);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(5, 0, 10)))->removeTile(221);
					};

					auto onClose_1_35 = []() {
						closeDoor("door_1_35", 173, Location(1, glm::ivec2(-10, 1), glm::ivec3(6, 0, 10)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(5, 0, 10)))->removeTile(222);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(5, 0, 10)))->addTile(221);
					};

					Door door_1_35(1);
					setupDoor(&door_1_35, "door_1_35", onOpen_1_35, onClose_1_35);
					setupDoorSensor(&Location(1, glm::ivec2(-10, 1), glm::ivec3(5, 0, 9)), "door_1_35", "door_1_35 sensor_0", 11, 0);

					// World 1 Door 36
					auto onOpen_1_36 = []() {
						openDoor("door_1_36", 177, Location(1, glm::ivec2(-10, 1), glm::ivec3(10, 0, 10)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(9, 0, 10)))->addTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(9, 0, 10)))->removeTile(231);
					};

					auto onClose_1_36 = []() {
						closeDoor("door_1_36", 177, Location(1, glm::ivec2(-10, 1), glm::ivec3(10, 0, 10)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(9, 0, 10)))->removeTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-10, 1), glm::ivec3(9, 0, 10)))->addTile(231);
					};

					Door door_1_36(1);
					setupDoor(&door_1_36, "door_1_36", onOpen_1_36, onClose_1_36);
					setupDoorSensor(&Location(1, glm::ivec2(-10, 1), glm::ivec3(9, 0, 9)), "door_1_36", "door_1_36 sensor_0", 11, 0);

					// World 1 Door 37
					auto onOpen_1_37 = []() {
						openDoor("door_1_37", 177, Location(1, glm::ivec2(-11, 1), glm::ivec3(5, 0, 11)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 15)))->addTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 15)))->removeTile(231);
					};

					auto onClose_1_37 = []() {
						closeDoor("door_1_37", 177, Location(1, glm::ivec2(-11, 1), glm::ivec3(5, 0, 11)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 15)))->removeTile(232);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 15)))->addTile(231);
					};

					Door door_1_37(1);
					setupDoor(&door_1_37, "door_1_37", onOpen_1_37, onClose_1_37);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 1), glm::ivec3(9, 0, 14)), "door_1_37", "door_1_37 sensor_0", 11, 0);

					// World 1 Door 38
					auto onOpen_1_38 = []() {
						openDoor("door_1_38", 169, Location(1, glm::ivec2(-12, 2), glm::ivec3(15, 0, 11)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 0)))->addTile(216);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 0)))->removeTile(215);
					};

					auto onClose_1_38 = []() {
						closeDoor("door_1_38", 169, Location(1, glm::ivec2(-12, 2), glm::ivec3(15, 0, 11)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 0)))->removeTile(216);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-12, 3), glm::ivec3(12, 0, 0)))->addTile(215);
					};

					Door door_1_38(1);
					setupDoor(&door_1_38, "door_1_38", onOpen_1_38, onClose_1_38);
					setupDoorSensor(&Location(1, glm::ivec2(-12, 2), glm::ivec3(12, 0, 15)), "door_1_38", "door_1_38 sensor_0", 13, 0);

					// World 1 Door 39
					auto onOpen_1_39 = []() {
						openDoor("door_1_39", 193, Location(1, glm::ivec2(-11, 1), glm::ivec3(14, 0, 10)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(13, 0, 15)))->addTile(272);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(13, 0, 15)))->removeTile(271);
					};

					auto onClose_1_39 = []() {
						closeDoor("door_1_39", 193, Location(1, glm::ivec2(-11, 1), glm::ivec3(14, 0, 10)));
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(13, 0, 15)))->removeTile(272);
						worldManager->getTileGroup(&Location(1, glm::ivec2(-11, 1), glm::ivec3(13, 0, 15)))->addTile(271);
					};

					Door door_1_39(1);
					setupDoor(&door_1_39, "door_1_39", onOpen_1_39, onClose_1_39);
					setupDoorSensor(&Location(1, glm::ivec2(-11, 1), glm::ivec3(13, 0, 14)), "door_1_39", "door_1_39 sensor_0", 11, 0);

					/*auto onOpen_1_40 = []() {
						setupLevel(1, 2);
						fadeTeleportPlayer(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 0));
						player->getCharacter()->moveIfPossible(TileDirection::FORWARD);
						player->getCharacter()->turn(TileDirection::BACKWARD);
					};

					scriptManager->addScript("door_1_40", onOpen_1_40);
					worldManager->addPromptedScript("door_1_40", &Location(1, -5, 0, 14, 0, 13));*/
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
					scriptManager->addScript("dialogue_0_1", std::bind(&showDialogue, "Aela", "", "Hi! My name is Aela! I will be your assisstant throughout your tests.", "dialogue_0_2"));
					scriptManager->addScript("dialogue_0_2", std::bind(&showDialogue, "Aela", "", "As you know, Oxylabs has changed its testing protocols.", "dialogue_0_3"));
					scriptManager->addScript("dialogue_0_3", std::bind(&showDialogue, "Aela", "", "According to the new protocols, testing can no longer be outsourced.", "dialogue_0_4"));
					scriptManager->addScript("dialogue_0_4", std::bind(&showDialogue, "Aela", "", "So while our \"more intelligent\" higher ups look for testers to hire,", "dialogue_0_5"));
					scriptManager->addScript("dialogue_0_5", std::bind(&showDialogue, "Aela", "", "us scientists are now responsible for testing each others' projects.", "dialogue_0_6"));
					scriptManager->addScript("dialogue_0_6", std::bind(&showDialogue, "Aela", "", "Only our management could be this moronic, right?", "dialogue_0_7"));
					scriptManager->addScript("dialogue_0_7", std::bind(&showDialogue, "Aela", "", "It says here that you, Player, are a computer engineer working on Project Foxtrot.", "dialogue_0_8"));
					scriptManager->addScript("dialogue_0_8", std::bind(&showDialogue, "Aela", "", "I myself was a mechanical engineer under Project Bravo.", "dialogue_0_9"));
					scriptManager->addScript("dialogue_0_9", std::bind(&showDialogue, "Aela", "", "Nevertheless, we've been forced to waste our time on the testing of Project Echo!", "dialogue_0_10"));
					scriptManager->addScript("dialogue_0_10", std::bind(&showDialogue, "Aela", "", "Looks like your elevator has arrived.", "dialogue_0_11"));
					scriptManager->addScript("dialogue_0_11", std::bind(&showDialogue, "Aela", "", "Once you leave the elevator, please walk to the kiosk marked with an \"i\".", "dialogue_0_12"));

					auto dialogue_0_12 = []() {
						dialogueDisplay->closeDialog();
						hintDisplay->displayHint("You can use W, A, S and D to walk!", HintDisplayDuration::LONG);
						hintDisplay->displayHint("Walk to the exit!", HintDisplayDuration::LONG);
					};

					scriptManager->addScript("dialogue_0_12", dialogue_0_12);

					auto walked_on_0_0 = []() {
						fadeTeleportPlayer(1, glm::ivec2(0, 0), glm::ivec3(14, 0, 6));
						hintDisplay->clear();
						worldManager->removeWalkedOnScript(&Location(0, glm::ivec2(0, -1), glm::ivec3(1, 0, 15)));
					};

					scriptManager->addScript("walked_on_0_0", walked_on_0_0);
					worldManager->addWalkedOnScript("walked_on_0_0", &Location(0, glm::ivec2(0, -1), glm::ivec3(1, 0, 15)));

					auto walked_on_0_1 = []() {
						hintDisplay->displayHint("You can interact with objects, such as kiosks.", HintDisplayDuration::LONG);
						hintDisplay->displayHint("Press ENTER while facing an object to interact with it!", HintDisplayDuration::LONG);
					};

					scriptManager->addScript("walked_on_0_1", walked_on_0_1);
					worldManager->addWalkedOnScript("walked_on_0_1", &Location(1, glm::ivec2(0, 0), glm::ivec3(14, 0, 6)));

					auto dialogue_1_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, 0, 0, 10, 0, 6));
							hintDisplay->clear();
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/0.png", "WELCOME TO LEVEL 1 OF THE PROJECT ECHO TESTING FACILITY.", "dialogue_1_1");
						}
					};

					scriptManager->addScript("dialogue_1_0", dialogue_1_0);
					worldManager->addPromptedScript("dialogue_1_0", &Location(1, glm::ivec2(0, 0), glm::ivec3(10, 0, 6)));

					scriptManager->addScript("dialogue_1_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "PROJECT ECHO IS ALL ABOUT HIGH-SPEED TRANSPORTATION OF MATTER.", "dialogue_1_2"));
					scriptManager->addScript("dialogue_1_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "YOUR TESING WILL REVOLVE AROUND SEVERAL MATTER-PROPULSION DEVICES.", "dialogue_1_3"));
					scriptManager->addScript("dialogue_1_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "HOWEVER, THE FOCUS OF YOUR TESTS WILL BE THE PROPULSION GUN.", "dialogue_1_4"));
					scriptManager->addScript("dialogue_1_4", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "YOUR PROPULSION GUN HAS BEEN DESIGNED TO PICK UP MATTER BELOW YOUR WAIST.", "dialogue_1_5"));
					scriptManager->addScript("dialogue_1_5", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "FOR EXAMPLE, YOU CAN USE YOUR PROPULSION GUN ON THE FLOOR IN FRONT OF YOU.", "dialogue_1_6"));

					auto dialogue_1_6 = []() {
						game->getTileSwitchGun()->setActive(true);
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/0.png", "HERE IS YOUR PROPULSION GUN.", "dialogue_1_7");
						tileInventoryDisplay->show();
					};

					scriptManager->addScript("dialogue_1_6", dialogue_1_6);

					auto dialogue_1_7 = []() {
						turnDisplayOff(Location(1, 0, 0, 10, 0, 6));
						if (!aelaTalkedToPlayerInDialogue1) {
							aelaTalkedToPlayerInDialogue1 = true;
							dialogueDisplay->showDialogue("Aela", "", "Player!", "dialogue_1_8");
						} else {
							dialogueDisplay->showDialogue("Aela", "", "Are you still wondering what to do, Player?", "dialogue_1_12");
						}
					};

					scriptManager->addScript("dialogue_1_7", dialogue_1_7);

					scriptManager->addScript("dialogue_1_8", std::bind(&showDialogue, "Aela", "", "Your propulsion gun has three slots, and each slot can hold a bit of matter.", "dialogue_1_9"));
					scriptManager->addScript("dialogue_1_9", std::bind(&showDialogue, "Aela", "", "Currently, your three slots contain flooring.", "dialogue_1_10"));
					scriptManager->addScript("dialogue_1_10", std::bind(&showDialogue, "Aela", "", "When you use your gun, you'll replace the matter in front of you with the matter in the slot.", "dialogue_1_11"));
					scriptManager->addScript("dialogue_1_11", std::bind(&showDialogue, "Aela", "", "It's actually really simple stuff, so don't panic!", "dialogue_1_12"));
					scriptManager->addScript("dialogue_1_12", std::bind(&showDialogue, "Aela", "", "Try using your gun to make a bridge across the water to your right!", "dialogue_1_13"));

					auto dialogue_1_13 = []() {
						hideDialogue();
						hintDisplay->displayHint("Press BACKSLASH to use the Propulsion Gun.", HintDisplayDuration::LONG);
						hintDisplay->displayHint("Use the LEFT and RIGHT arrow keys to select a slot.", HintDisplayDuration::LONG);
					};

					scriptManager->addScript("dialogue_1_13", dialogue_1_13);

					auto dialogue_2_0 = []() {
						turnDisplayOn(Location(1, -3, 0, 15, 0, 5));
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/0.png", "DID YOU KNOW?", "dialogue_2_1");
					};

					scriptManager->addScript("dialogue_2_0", dialogue_2_0);
					worldManager->addPromptedScript("dialogue_2_0", &Location(1, -3, 0, 15, 0, 5));

					scriptManager->addScript("dialogue_2_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "LAVA IS ACTUALLY HOT, MELTED ROCK!", "dialogue_2_1"));
					scriptManager->addScript("dialogue_2_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "WHEN LAVA INTERACTS WITH WATER, IT COOLS OFF AND BECOMES SOLID!", "dialogue_2_2"));		

					auto dialogue_2_3 = []() {
						turnDisplayOff(Location(1, -3, 0, 15, 0, 5));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_2_3", dialogue_2_3);

					auto dialogue_3_0 = []() {
						turnDisplayOn(Location(1, -3, 0, 5, 0, 5));
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/0.png", "DID YOU KNOW?", "dialogue_3_1");
					};

					scriptManager->addScript("dialogue_3_0", dialogue_3_0);
					worldManager->addPromptedScript("dialogue_3_0", &Location(1, -3, 0, 5, 0, 5));

					auto dialogue_3_1 = []() {
						// Insert that one animation here!
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/0.png", "THE FOUNDER OF OXYLABS, ARTHUR WRIGHT, MIGHT HAVE BEEN AN ENTREPRENEUR.", "dialogue_3_2");
					};

					scriptManager->addScript("dialogue_3_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "HOWEVER, IN HIS HEART, HE WAS AN ENTHUSIASTIC SCIENTIST.", "dialogue_3_3"));
					scriptManager->addScript("dialogue_3_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "THE CURRENT PROJECT YOU ARE TESTING, PROJECT ECHO, WAS HIS LAST.", "dialogue_3_4"));
					scriptManager->addScript("dialogue_3_4", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "HIS DAUGHTER TOOK OVER AND STARTED OXYLABS' MOST FAMOUS DEVELOPMENT, PROJECT FOXTROT.", "dialogue_3_5"));

					auto dialogue_3_5 = []() {
						turnDisplayOff(Location(1, -3, 0, 5, 0, 5));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_3_5", dialogue_3_5);

					auto dialogue_4_0 = []() {
						turnDisplayOn(Location(1, -4, 0, 3, 0, 2));
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/0.png", "DID YOU KNOW?", "dialogue_4_1");
					};

					scriptManager->addScript("dialogue_4_0", dialogue_4_0);
					worldManager->addPromptedScript("dialogue_4_0", &Location(1, -4, 0, 3, 0, 2));

					scriptManager->addScript("dialogue_4_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "THE PROPULSION GUN USES STATE OF THE ART TECHNOLOGY.", "dialogue_4_2"));
					scriptManager->addScript("dialogue_4_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "THIS TECHNOLOGY IS BASED ON MANY RECENT BREAKTHROUGHS IN PHYSICS.", "dialogue_4_3"));
					scriptManager->addScript("dialogue_4_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "THE MOST IMPORTANT BREAKTHROUGH IS MR. READ'S OWN EXPANSION OF MEMERSTOTLE'S THEORY OF THE POLYGON.", "dialogue_4_4"));

					auto dialogue_4_4 = []() {
						turnDisplayOff(Location(1, -4, 0, 3, 0, 2));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_4_4", dialogue_4_4);

					scriptManager->addScript("hint_1_0", std::bind(&showHint, (std::string) "You can press R to go load to a previous checkpoint.", HintDisplayDuration::LONG));
					worldManager->addWalkedOnScript("hint_1_0", &Location(1, -2, 0, 5, 0, 8));

					scriptManager->addScript("hint_2_0", std::bind(&showHint, (std::string) "Pressing ENTER will also interact with buttons.", HintDisplayDuration::LONG));
					worldManager->addWalkedOnScript("hint_2_0", &Location(1, -5, 0, 13, 0, 10));

					auto button_0_1 = []() {
						Character* character = player->getCharacter();
						character->allowNewMovements(false);
						pressButton(Location(1, glm::ivec2(-5, 0), glm::ivec3(14, 0, 13)));

						auto event2 = []() {
							openDoor("metal door", 345, Location(1, glm::ivec2(-5, 0), glm::ivec3(13, 0, 13)));
						};

						auto event3 = [character]() {
							character->allowNewMovements(true);
							character->turn(TileDirection::RIGHT);
							character->moveIfPossible(TileDirection::RIGHT);
							character->turn(TileDirection::FORWARD);
							character->moveIfPossible(TileDirection::FORWARD);
							character->teleportWithAnimation(&Location(0, 0, 0, 1, 0, 0), TeleportationAnimation::FADE);
							character->moveIfPossible(TileDirection::FORWARD);
							character->turn(TileDirection::BACKWARD);
							character->allowNewMovements(false);
							setupLevel(1, 2);
						};

						timer->scheduleEventInMillis(1000, event2);
						timer->scheduleEventInMillis(1500, event3);
					};

					scriptManager->addScript("button_0_1", button_0_1);
					worldManager->addPromptedScript("button_0_1", &Location(1, glm::ivec2(-5, 0), glm::ivec3(14, 0, 13)));
					break;
				}
				case 2: {
					auto dialogue_8_0 = []() {
						player->getCharacter()->allowNewMovements(true);
						dialogueDisplay->showDialogue("Aela", "", "Congratulations on making it through the first level!", "dialogue_8_1");
					};
					scriptManager->addScript("dialogue_8_0", dialogue_8_0);
					worldManager->addWalkedOnScript("dialogue_8_0", &Location(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 1)));

					scriptManager->addScript("dialogue_8_1", std::bind(&showDialogue, "Aela", "", "Was it easy?", "dialogue_8_2"));

					auto dialogue_8_2 = []() {
						dialogueDisplay->showOptions(&DialogueOption("Yes", "dialogue_8_3_1"), &DialogueOption("No", "dialogue_8_3_2"));
						hintDisplay->displayHint("You can use W/S or A/D to select how to respond.", HintDisplayDuration::LONG);
					};

					scriptManager->addScript("dialogue_8_3_1", std::bind(&showDialogue, "Aela", "", "Well, I'm sure it was for you!", "dialogue_8_4"));
					scriptManager->addScript("dialogue_8_3_2", std::bind(&showDialogue, "Aela", "", "Really? Well you made it look easy.", "dialogue_8_4"));
					scriptManager->addScript("dialogue_8_4", std::bind(&showDialogue, "Aela", "", "Apparently, the next level ups the difficulty. We'll see if that's true.", "dialogue_8_5"));

					auto dialogue_8_5 = []() {
						// End elevator animation here
						hideDialogue();
						worldManager->removeWalkedOnScript(&Location(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 1)));
					};

					scriptManager->addScript("dialogue_8_5", dialogue_8_5);

					scriptManager->addScript("walked_on_1_0", std::bind(&fadeTeleportPlayer, 1, glm::ivec2(-7, 0), glm::ivec3(14, 0, 1)), true);
					worldManager->addWalkedOnScript("walked_on_1_0", &Location(0, glm::ivec2(0, -1), glm::ivec3(1, 0, 15)));

					auto dialogue_5_0 = []() {
						turnDisplayOn(Location(1, -6, 0, 2, 0, 6));
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/0.png", "WELCOME TO LEVEL 2 OF THE PROJECT ECHO TESTING FACILITY.", "dialogue_5_1");
					};

					scriptManager->addScript("dialogue_5_0", dialogue_5_0);
					worldManager->addPromptedScript("dialogue_5_0", &Location(1, -6, 0, 2, 0, 6));

					scriptManager->addScript("dialogue_5_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "THIS LEVEL HAS BEEN DESIGNED SO THAT YOUR TESTING IS MORE ENJOYABLE.", "dialogue_5_2"));
					scriptManager->addScript("dialogue_5_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "YOU WILL FIND PUZZLES THAT FORCE YOU TO THINK BEFORE YOU ACT.", "dialogue_5_3"));
					scriptManager->addScript("dialogue_5_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "GOOD LUCK!", "dialogue_5_4"));

					auto dialogue_5_4 = []() {
						turnDisplayOff(Location(1, -6, 0, 2, 0, 6));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_5_4", dialogue_5_4);

					auto dialogue_6_0 = []() {
						turnDisplayOn(Location(1, -10, 4, 3, 0, 2));
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/0.png", "DID YOU KNOW?", "dialogue_6_1");
					};

					scriptManager->addScript("dialogue_6_0", dialogue_6_0);
					worldManager->addPromptedScript("dialogue_6_0", &Location(1, -10, 4, 3, 0, 2));

					scriptManager->addScript("dialogue_6_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "THE PADS YOU HAVE USED TO REACH THIS KIOSK ARE SIMPLY CALLED \"TELEPORTERS\".", "dialogue_6_2"));
					scriptManager->addScript("dialogue_6_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "YOUR PROPULSION GUN USES A MINIMIZED VERSION OF THE TECHNOLOGY IN TELEPORTERS.", "dialogue_6_3"));
					scriptManager->addScript("dialogue_6_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "CURRENTLY, OXYLAB ENGINEERS ARE TRYING TO ADAPT THIS TECHNOLOGY FOR MAINSTREAM USE.", "dialogue_6_4"));

					auto dialogue_6_4 = []() {
						turnDisplayOff(Location(1, -10, 4, 3, 0, 2));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_6_4", dialogue_6_4);

					auto walked_on_3_0 = []() {
						worldManager->removeWalkedOnScript(&Location(1, -9, 3, 11, 0, 2));
						worldManager->removeWalkedOnScript(&Location(1, -9, 3, 11, 0, 1));

						Character* character = player->getCharacter();
						character->turn(TileDirection::FORWARD);
						character->moveIfPossible(TileDirection::FORWARD);
						character->allowNewMovements(false);

						std::list<Movement> movements;

						for (size_t i = 0; i < 30; i++) {
							movements.push_back(Movement(TileDirection::BACKWARD));
						}

						// Move other characters
						Character* soldier = characterProvider->getCharacterByName("soldier_1");
						soldier->moveIfPossible(&movements);

						soldier = characterProvider->getCharacterByName("soldier_2");
						soldier->moveIfPossible(&movements);

						soldier = characterProvider->getCharacterByName("soldier_3");
						soldier->moveIfPossible(&movements);

						soldier = characterProvider->getCharacterByName("soldier_4");
						soldier->moveIfPossible(&movements);

						Character* scientist = characterProvider->getCharacterByName("scientist_1");
						scientist->moveIfPossible(&movements);

						scientist = characterProvider->getCharacterByName("scientist_2");
						scientist->moveIfPossible(&movements);

						scientist = characterProvider->getCharacterByName("scientist_3");
						scientist->moveIfPossible(&movements);

						auto walked_on_3_1 = [character]() {
							character->allowNewMovements(true);
							showDialogue("Aela", "", "What on earth?", "walked_on_3_2");
						};

						timer->scheduleEventInSeconds(4, walked_on_3_1);
						scriptManager->addScript("walked_on_3_2", std::bind(&showDialogue, "Aela", "", "What are those soldiers doing with those scientists?", "walked_on_3_3"));
						scriptManager->addScript("walked_on_3_3", std::bind(&showDialogue, "Aela", "", "I might have to tell a supervisor about this.", "hide_dialogue"));
					};

					scriptManager->addScript("walked_on_3_0", walked_on_3_0);
					worldManager->addWalkedOnScript("walked_on_3_0", &Location(1, -9, 3, 11, 0, 2));
					worldManager->addWalkedOnScript("walked_on_3_0", &Location(1, -9, 3, 11, 0, 1));

					auto button_1_1 = []() {
						Character* character = player->getCharacter();
						character->allowNewMovements(false);
						pressButton(Location(1, glm::ivec2(-10, 3), glm::ivec3(6, 0, 3)));

						auto event2 = []() {
							openDoor("metal door", 345, Location(1, glm::ivec2(-10, 3), glm::ivec3(5, 0, 3)));
						};

						auto event3 = [character]() {
							character->allowNewMovements(true);
							character->turn(TileDirection::RIGHT);
							character->moveIfPossible(TileDirection::RIGHT);
							character->turn(TileDirection::FORWARD);
							character->moveIfPossible(TileDirection::FORWARD);
							character->teleportWithAnimation(&Location(0, 0, 0, 1, 0, 0), TeleportationAnimation::FADE);
							character->moveIfPossible(TileDirection::FORWARD);
							character->turn(TileDirection::BACKWARD);
							character->allowNewMovements(false);
							setupLevel(1, 3);
						};

						timer->scheduleEventInMillis(1000, event2);
						timer->scheduleEventInMillis(1500, event3);
					};

					scriptManager->addScript("button_1_1", button_1_1);
					worldManager->addPromptedScript("button_1_1", &Location(1, glm::ivec2(-10, 3), glm::ivec3(6, 0, 3)));

					break;
				}
				case 3: {
					auto dialogue_9_0 = []() {
						worldManager->removeWalkedOnScript(&Location(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 1)));
						player->getCharacter()->allowNewMovements(true);
						dialogueDisplay->showDialogue("Aela", "", "Apparently, all employees are being called down for some emergency meeting.", "dialogue_9_1");
					};
					scriptManager->addScript("dialogue_9_0", dialogue_9_0);
					worldManager->addWalkedOnScript("dialogue_9_0", &Location(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 1)));

					scriptManager->addScript("dialogue_9_1", std::bind(&showDialogue, "Aela", "", "The only way for you to exit these testing facilities is to finish this upcoming level.", "dialogue_9_2"));
					scriptManager->addScript("dialogue_9_2", std::bind(&showDialogue, "Aela", "", "I'm in a surveillance room that's close to the meeting point.", "dialogue_9_3"));
					scriptManager->addScript("dialogue_9_3", std::bind(&showDialogue, "Aela", "", "When the meeting is over, I'll try and come back to fill you in on the details.", "dialogue_9_4"));
					scriptManager->addScript("dialogue_9_4", std::bind(&showDialogue, "Aela", "", "Good luck with the level!", "hide_dialogue"));

					scriptManager->addScript("walked_on_2_0", std::bind(&fadeTeleportPlayer, 1, glm::ivec2(-15, 0), glm::ivec3(13, 0, 14)), true);
					worldManager->addWalkedOnScript("walked_on_2_0", &Location(1, glm::ivec2(0, -1), glm::ivec3(1, 0, 15)));

					auto dialogue_7_0 = []() {
						turnDisplayOn(Location(1, -15, 0, 10, 0, 3));
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/0.png", "WELCOME TO LEVEL 3 OF THE PROJECT ECHO TESTING FACILITY.", "dialogue_7_1");
					};

					scriptManager->addScript("dialogue_7_0", dialogue_7_0);
					worldManager->addPromptedScript("dialogue_7_0", &Location(1, -15, 0, 10, 0, 3));

					scriptManager->addScript("dialogue_7_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "REMEMBER THE ADVICE \"THINK BEFORE YOUR ACT\"?", "dialogue_7_2"));
					scriptManager->addScript("dialogue_7_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "WELL NOW YOU MAY NOT EVEN HAVE THE TIME TO THINK!", "dialogue_7_3"));
					scriptManager->addScript("dialogue_7_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/0.png", "ANY MISTAKE OF YOURS COULD BE LETHAL, SO TRY YOUR BEST!", "dialogue_7_4"));

					auto dialogue_7_4 = []() {
						turnDisplayOff(Location(1, -15, 0, 10, 0, 3));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_7_4", dialogue_7_4);

					scriptManager->addScript("dialogue_10_0", std::bind(&showDialogue, "Plaque", "", "\"Hills outside of the first Oxylabs facility - 1905\"", "hide_dialogue"));
					worldManager->addPromptedScript("dialogue_10_0", &Location(1, -16, 1, 13, 0, 2));

					scriptManager->addScript("dialogue_11_0", std::bind(&showDialogue, "Plaque", "", "\"The Beauty of Memester Bay - 1908\"", "dialogue_11_1"));
					worldManager->addPromptedScript("dialogue_11_0", &Location(1, -19, 1, 15, 0, 6));
					scriptManager->addScript("dialogue_11_1", std::bind(&showDialogue, "Plaque", "", "The following depicts Memester Bay - a rocket launch site of Oxylabs rocketry.", "hide_dialogue"));

					scriptManager->addScript("dialogue_12_0", std::bind(&showDialogue, "Plaque", "", "\"Firstname Lastname - 1907\"", "dialogue_12_1"));
					worldManager->addPromptedScript("dialogue_12_0", &Location(1, -21, 1, 14, 0, 5));
					scriptManager->addScript("dialogue_12_1", std::bind(&showDialogue, "Plaque", "", "This is a portrait of Arthur Wright's wife.", "dialogue_12_2"));
					scriptManager->addScript("dialogue_12_2", std::bind(&showDialogue, "Plaque", "", "She was supportive of Mr. Wright's hard-working spirit.", "dialogue_12_3"));
					scriptManager->addScript("dialogue_12_3", std::bind(&showDialogue, "Plaque", "", "Unfortunately, she died alongside Mr. Wright due to an automative accident in 1909.", "hide_dialogue"));
					
					scriptManager->addScript("dialogue_13_0", std::bind(&showDialogue, "Plaque", "", "\"A Picture of Mr. Wright on Kozack's first colour camera - 1909\"", "dialogue_11_1"));
					worldManager->addPromptedScript("dialogue_13_0", &Location(1, -17, 3, 9, 0, 11));
					scriptManager->addScript("dialogue_13_1", std::bind(&showDialogue, "Plaque", "", "During early development of Project Echo, a scientist set two teleporters to teleport to one another.", "dialogue_13_2"));
					scriptManager->addScript("dialogue_13_2", std::bind(&showDialogue, "Plaque", "", "One of his unsuspecting coworkers walked into one of those teleporters.", "dialogue_13_3"));
					scriptManager->addScript("dialogue_13_3", std::bind(&showDialogue, "Plaque", "", "The coworker ended up teleporting in an endless loop.", "dialogue_13_4"));
					scriptManager->addScript("dialogue_13_4", std::bind(&showDialogue, "Plaque", "", "The following photo depicts Mr. Wright's reaction to the mess.", "hide_dialogue"));

					scriptManager->addScript("dialogue_14_0", std::bind(&showDialogue, "Plaque", "", "Mr. Wright got the idea for Project Echo after watching the film this poster promotes.", "hide_dialogue"));
					worldManager->addPromptedScript("dialogue_14_0", &Location(1, -17, 3, 4, 0, 11));

					scriptManager->addScript("dialogue_15_0", std::bind(&showDialogue, "Plaque", "", "The following is an early prototype for a teleporter.", "dialogue_15_1"));
					worldManager->addPromptedScript("dialogue_15_0", &Location(1, -17, 3, 9, 0, 14));
					scriptManager->addScript("dialogue_15_1", std::bind(&showDialogue, "Plaque", "", "Many prototypes had to be tethered to a large computer.", "hide_dialogue"));

					scriptManager->addScript("dialogue_16_0", std::bind(&showDialogue, "Plaque", "", "This is what a teleporter looks like before its paint is applied.", "hide_dialogue"));
					worldManager->addPromptedScript("dialogue_16_0", &Location(1, -17, 3, 4, 0, 14));

					scriptManager->addScript("dialogue_17_0", std::bind(&showDialogue, "Plaque", "", "Museum of the teleporter, right this way! Includes free cake!", "hide_dialogue"));
					worldManager->addPromptedScript("dialogue_17_0", &Location(1, -17, 0, 8, 0, 13));

					scriptManager->addScript("dialogue_18_0", std::bind(&showDialogue, "Plaque", "", "Museum of the teleporter, right this way! Fun for the whole extended family!", "hide_dialogue"));
					worldManager->addPromptedScript("dialogue_18_0", &Location(1, -17, 0, 6, 0, 13));
					break;
				}
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
	dialogueDisplay->showDialogue("???", "", "Welcome to the OXYLABS Test Facility.", "dialogue_0_1");
	tileInventoryDisplay->hide();
}

void Scripts::continueGame() {}

void Scripts::editMap() {
	game->getTileSwitchGun()->setActive(true);
	player->getCharacter()->teleportWithoutAnimation(&Location(MAP_BEING_EDITED_WORLD_ID, glm::ivec2(0, 0), glm::ivec3(0, 0, 0)));
}

void Scripts::setupLevel(int stage, int level) {
	setupTeleporters(stage, level);
	setupDoors(stage, level);
	setupCheckPoints(stage, level);
	setupCharacters(stage, level);
	setupGeneralEvents(stage, level);

	switch (stage) {
		case 1:
			if (level < 4) {
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

void Scripts::openDoor(std::string doorID, size_t tileType, Location location) {
	clearDoorAnimations(doorID);

	TileGroup* tileGroup = worldManager->getTileGroup(&location);
	tileGroup->removeTile(tileType);
	tileGroup->addTile(tileType + 1);

	auto frame1 = [tileGroup, tileType]() {
		tileGroup->removeTile(tileType + 1);
		tileGroup->addTile(tileType + 2);
	};

	auto frame2 = [tileGroup, tileType]() {
		tileGroup->removeTile(tileType + 2);
		tileGroup->addTile(tileType + 3);
	};

	auto frame3 = [tileGroup, tileType]() {
		tileGroup->removeTile(tileType + 3);
	};

	timer->scheduleEventInNanos(doorID + "_1", TIME_BETWEEN_DOOR_ANIMATION_FRAMES, frame1);
	timer->scheduleEventInNanos(doorID + "_2", TIME_BETWEEN_DOOR_ANIMATION_FRAMES * 2ll, frame2);
	timer->scheduleEventInNanos(doorID + "_3", TIME_BETWEEN_DOOR_ANIMATION_FRAMES * 3ll, frame3);
}

void Scripts::closeDoor(std::string doorID, size_t tileType, Location location) {
	clearDoorAnimations(doorID);

	TileGroup* tileGroup = worldManager->getTileGroup(&location);
	tileGroup->addTile(tileType + 3);

	auto frame1 = [tileGroup, tileType]() {
		tileGroup->removeTile(tileType + 3);
		tileGroup->addTile(tileType + 2);
	};

	auto frame2 = [tileGroup, tileType]() {
		tileGroup->removeTile(tileType + 2);
		tileGroup->addTile(tileType + 1);
	};

	auto frame3 = [tileGroup, tileType]() {
		tileGroup->removeTile(tileType + 1);
		tileGroup->addTile(tileType);
	};

	timer->scheduleEventInNanos(doorID + "_1", TIME_BETWEEN_DOOR_ANIMATION_FRAMES, frame1);
	timer->scheduleEventInNanos(doorID + "_2", TIME_BETWEEN_DOOR_ANIMATION_FRAMES * 2ll, frame2);
	timer->scheduleEventInNanos(doorID + "_3", TIME_BETWEEN_DOOR_ANIMATION_FRAMES * 3ll, frame3);

	Character* character = characterProvider->getCharacterByLocation(&location);
	if (character != nullptr) {
		character->kill();
	}
}

void Scripts::clearDoorAnimations(std::string doorID) {
	timer->removeEvent(doorID + "_1");
	timer->removeEvent(doorID + "_2");
	timer->removeEvent(doorID + "_3");
}

void Scripts::turnDisplayOn(Location location) {
	TileGroup* tileGroup = worldManager->getTileGroup(&location);
	tileGroup->removeTile(73);
	tileGroup->addTile(74);
}

void Scripts::turnDisplayOff(Location location) {
	TileGroup* tileGroup = worldManager->getTileGroup(&location);
	tileGroup->addTile(73);
	tileGroup->removeTile(74);
}

void Scripts::pressButton(Location location) {
	TileGroup* tileGroup = worldManager->getTileGroup(&location);
	tileGroup->removeTile(343);
	tileGroup->addTile(344);

	auto event = [tileGroup]() {
		tileGroup->addTile(343);
		tileGroup->removeTile(344);
	};

	timer->scheduleEventInMillis(BUTTON_PRESSING_TIME, event);
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

		timer->scheduleEventInNanos((size_t) (1.0 / player->getCharacter()->getCurrentSpeed()), std::bind(&GameSaver::save, gameSaver, "save_0"));
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

		timer->scheduleEventInNanos((size_t) (1.0 / player->getCharacter()->getCurrentSpeed()), std::bind(&GameSaver::save, gameSaver, "save_0"));
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

		timer->scheduleEventInNanos((size_t) (1.0 / player->getCharacter()->getCurrentSpeed()), std::bind(&GameSaver::save, gameSaver, "save_0"));
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