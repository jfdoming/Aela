#include <utility>
#include "GameScript.h"
#include "ScriptObjects.h"
#include "AnimationScript.h"
#include "SoundScript.h"
#include "../../Player/Player.h"
#include "../../Aela Game/AelaGame.h"
#include "../../Displays/Dialogue/DialogueDisplay.h"
#include "../../Worlds/WorldManager.h"
#include "../Tiles/TileBehaviourExecutor.h"
#include "../../Doors/DoorProvider.h"
#include "../../Doors/TileAmountLock.h"
#include "../../Save States/GameSaver.h"
#include "../../Resources/ResourceInfo.h"
#include "../../Displays/Hints/HintDisplay.h"
#include "../../Displays/Tiles/TileInventoryDisplay.h"
#include "../../Enemies/Turret.h"
#include "../ScriptManager.h"
#include "../../Location/Location.h"
#include "../../../Project Aela/Window/Window.h"
#include "MapScript.h"
#include "../../Displays/Timer/LockTimer.h"
#include "../../Menus/FontSizes.h"
#define BUTTON_PRESSING_TIME 500

using namespace Scripts;

namespace Scripts {
	bool aelaTalkedToPlayerInDialogue1 = false;
	bool playerCalledBrotherInTelecom0 = false;
}

void Scripts::showDialogue(std::string name, std::string avatar, std::string text, std::string scriptToRunOnceComplete) {
	dialogueDisplay->showDialogue(std::move(name), std::move(avatar), std::move(text), std::move(scriptToRunOnceComplete));
}

void Scripts::showBlinkingDialogue(std::string name, std::string avatar, std::string blinkingAvatar, std::string text, std::string scriptToRunOnceComplete) {
	dialogueDisplay->showDialogue(std::move(name), std::move(avatar), std::move(blinkingAvatar), std::move(text), std::move(scriptToRunOnceComplete));
}

void Scripts::showTwoOptions(bool showAvatar, DialogueOption option1, DialogueOption option2) {
	dialogueDisplay->showOptions(&option1, &option2);

	if (!showAvatar) {
		dialogueDisplay->stopShowingAvatar();
	}
}

void Scripts::showThreeOptions(bool showAvatar, DialogueOption option1, DialogueOption option2, DialogueOption option3) {
	dialogueDisplay->showOptions(&option1, &option2, &option3);

	if (!showAvatar) {
		dialogueDisplay->stopShowingAvatar();
	}
}

void Scripts::showFourOptions(bool showAvatar, DialogueOption option1, DialogueOption option2, DialogueOption option3, DialogueOption option4) {
	dialogueDisplay->showOptions(&option1, &option2, &option3, &option4);

	if (!showAvatar) {
		dialogueDisplay->stopShowingAvatar();
	}
}

void Scripts::showHint(std::string text, HintDisplayDuration duration) {
	hintDisplay->displayHint(std::move(text), duration);
}

void Scripts::hideDialogue() {
	dialogueDisplay->closeDialog();
}

void Scripts::setupCharacters(int stage, int level) {
	switch (stage) {
		case 1:
			switch (level) {
				case 1: {
					// TEMPORARY!
					/*Character* ferdinand = new Character("ferdinand", Location(0, -2, 0, 14.5f, 0, 5.5f));
					ferdinand->setTexture("Ferdinand");
					characterProvider->addCharacter(ferdinand);*/

					// ACTUAL:
					Character* worker_1 = new Character("worker_1", Location(1, -3, 0, 4.5f, 1, 9.5f));
					worker_1->setTexture("scientist_8");
					characterProvider->addCharacter(worker_1);

					Character* worker_2 = new Character("worker_2", Location(1, -3, 0, 1.5f, 1, 9.5f));
					worker_2->setTexture("scientist_6");
					characterProvider->addCharacter(worker_2);

					Character* scientist_4 = new Character("scientist_4", Location(0, -5, 0, 13.5f, 0, 1.5f), 0.00275f, 0);
					scientist_4->setTexture("scientist_1");
					scientist_4->addTurnToQueue(TileDirection::FORWARD);
					characterProvider->addCharacter(scientist_4);

					Character* scientist_5 = new Character("scientist_5", Location(0, -5, 0, 10.5f, 0, 2.5f), 0.00275f, 0);
					scientist_5->setTexture("scientist_2");
					scientist_5->addTurnToQueue(TileDirection::FORWARD);
					characterProvider->addCharacter(scientist_5);

					Character* alfie = new Character("Alfie", Location(0, -5, 0, 12.5f, 0, 15.5f), 0.00275f, 0);
					alfie->setTexture("scientist_10");
					alfie->addTurnToQueue(TileDirection::BACKWARD);
					characterProvider->addCharacter(alfie);
					break;
				}
				case 2: {
					/*Character* annabelle = new Character("annabelle", Location(2, 0, 1, 8.5f, 0, 0.5f));
					annabelle->setTexture("Annabelle");
					characterProvider->addCharacter(annabelle);*/

					Character* worker_1 = characterProvider->getCharacterByName("worker_2");
					if (worker_1 != nullptr) {
						worker_1->addTurnToQueue(TileDirection::BACKWARD);
					}

					Character* soldier_1 = new Character("soldier_1", Location(1, -9, 4, 11.5f, -4, 5.5f), 0.00275f, 0);
					soldier_1->setTexture("soldier_2");
					characterProvider->addCharacter(soldier_1);

					Character* soldier_2 = new Character("soldier_2", Location(1, -9, 4, 12.5f, -4, 4.5f), 0.00275f, 0);
					soldier_2->setTexture("soldier_1");
					characterProvider->addCharacter(soldier_2);

					Character* soldier_3 = new Character("soldier_3", Location(1, -9, 4, 10.5f, -4, 2.5f), 0.00275f, 0);
					soldier_3->setTexture("soldier_1");
					characterProvider->addCharacter(soldier_3);

					Character* soldier_4 = new Character("soldier_4", Location(1, -9, 4, 11.5f, -4, 1.5f), 0.00275f, 0);
					soldier_4->setTexture("soldier_2");
					characterProvider->addCharacter(soldier_4);

					Character* soldier_5 = new Character("soldier_5", Location(1, -10, 3, 10.5f, 1, 14.5f), 0.00275f, 0);
					soldier_5->setTexture("soldier_1");
					soldier_5->addTurnToQueue(TileDirection::LEFT);
					characterProvider->addCharacter(soldier_5);

					Character* soldier_6 = new Character("soldier_6", Location(1, -10, 3, 12.5f, 1, 14.5f), 0.00275f, 0);
					soldier_6->setTexture("soldier_1");
					soldier_6->addTurnToQueue(TileDirection::RIGHT);
					characterProvider->addCharacter(soldier_6);

					Character* soldier_7 = new Character("soldier_7", Location(1, -11, 3, 4.5f, 0, 0.5f), 0.00275f, 0);
					soldier_7->setTexture("soldier_1");
					soldier_7->addTurnToQueue(TileDirection::LEFT);
					characterProvider->addCharacter(soldier_7);

					Character* soldier_8 = new Character("soldier_8", Location(1, -11, 3, 5.5f, 0, 0.5f), 0.00275f, 0);
					soldier_8->setTexture("soldier_1");
					soldier_8->addTurnToQueue(TileDirection::RIGHT);
					characterProvider->addCharacter(soldier_8);

					Character* soldier_10 = new Character("soldier_10", Location(1, -7, 1, 1.5f, 0, 0.5f), 0.00275f, 0);
					soldier_10->setTexture("soldier_2");
					soldier_10->addTurnToQueue(TileDirection::BACKWARD);
					characterProvider->addCharacter(soldier_10);

					Character* soldier_11 = new Character("soldier_11", Location(1, -11, 3, 13.5f, 0, 8.5f), 0.00275f, 0);
					soldier_11->setTexture("soldier_2");
					soldier_11->addTurnToQueue(TileDirection::BACKWARD);
					characterProvider->addCharacter(soldier_11);

					Character* scientist_1 = new Character("scientist_1", Location(1, -9, 4, 11.5f, -4, 4.5f), 0.00275f, 0);
					scientist_1->setTexture("scientist_1");
					characterProvider->addCharacter(scientist_1);

					Character* scientist_2 = new Character("scientist_2", Location(1, -9, 4, 11.5f, -4, 3.5f), 0.00275f, 0);
					scientist_2->setTexture("scientist_4");
					characterProvider->addCharacter(scientist_2);

					Character* scientist_3 = new Character("scientist_3", Location(1, -9, 4, 11.5f, -4, 2.5f), 0.00275f, 0);
					scientist_3->setTexture("scientist_6");
					characterProvider->addCharacter(scientist_3);

					Character* scientist_4_2 = new Character("scientist_4_2", Location(1, -7, 0, 12.5f, 0, 3.5f), 0.00275f, 0);
					scientist_4_2->setTexture("scientist_1");
					scientist_4_2->addTurnToQueue(TileDirection::FORWARD);
					characterProvider->addCharacter(scientist_4_2);

					Character* scientist_5_2 = new Character("scientist_5_2", Location(1, -7, 0, 9.5f, 0, 4.5f), 0.00275f, 0);
					scientist_5_2->setTexture("scientist_2");
					scientist_5_2->addTurnToQueue(TileDirection::FORWARD);
					characterProvider->addCharacter(scientist_5_2);

					Character* tutorial_scientist = new Character("tutorial_scientist", Location(0, -4, 0, 0.5f, 0, 0.5f), 0.00275f, 0);
					tutorial_scientist->setTexture("scientist_9");
					characterProvider->addCharacter(tutorial_scientist);
					tutorial_scientist->addTurnToQueue(TileDirection::BACKWARD);
					break;
				}
				case 3: {
					Character* robert_2 = new Character("Robert_2", Location(0, -2, 0, 13.5f, 0, 11.5f));
					robert_2->setTexture("Robert");
					characterProvider->addCharacter(robert_2);

					// Character* robert = new Character("Robert", Location(0, -2, 0, 14.5f, 0, 11.5f));
					Character* robert = new Character("Robert", Location(1, -22, 1, 1.5f, 0, 7.5f));
					robert->setTexture("Robert");
					characterProvider->addCharacter(robert);

					Character* soldier_9 = new Character("soldier_9", Location(1, -17, 0, 4.5f, 0, 13.5f), 0.00275f, 0);
					soldier_9->setTexture("soldier_1");
					soldier_9->addTurnToQueue(TileDirection::FORWARD);
					characterProvider->addCharacter(soldier_9);

					Character* dr_cephei = new Character("dr_cephei", Location(1, -22, 5, 6.5f, 1, 8.5f), 0.00275f, 0);
					dr_cephei->setTexture("dr_cephei");
					dr_cephei->addTurnToQueue(TileDirection::BACKWARD);
					characterProvider->addCharacter(dr_cephei);

					Character* robot_1 = new Character("robot_1", Location(1, -16, 0, 15.5f, 0, 11.5f), 0.00275f, 0);
					robot_1->setTexture("robot_1");
					robot_1->addTurnToQueue(TileDirection::BACKWARD);
					characterProvider->addCharacter(robot_1);

					Character* robot_2 = new Character("robot_2", Location(1, -17, 3, 7.5f, 0, 7.5f), 0.00275f, 0);
					robot_2->setTexture("robot_1");
					robot_2->addTurnToQueue(TileDirection::BACKWARD);
					characterProvider->addCharacter(robot_2);

					Character* robot_3 = new Character("robot_3", Location(1, -23, 1, 13.5f, 0, 1.5f), 0.00275f, 0);
					robot_3->setTexture("robot_2");
					robot_3->addTurnToQueue(TileDirection::BACKWARD);
					characterProvider->addCharacter(robot_3);

					Character* robot_4 = new Character("robot_4", Location(1, -23, 1, 4.5f, 0, 4.5f), 0.00275f, 0);
					robot_4->setTexture("robot_1");
					robot_4->addTurnToQueue(TileDirection::BACKWARD);
					characterProvider->addCharacter(robot_4);

					Character* soldier_12 = new Character("soldier_12", Location(1, -22, 1, 5.5f, 0, 13.5f), 0.00275f, 0);
					soldier_12->setTexture("soldier_1");
					soldier_12->addTurnToQueue(TileDirection::LEFT);
					characterProvider->addCharacter(soldier_12);

					Character* soldier_13 = new Character("soldier_13", Location(1, -22, 1, 7.5f, -1.5, 13.5f), 0.00275f, 0);
					soldier_13->setTexture("soldier_1");
					soldier_13->addTurnToQueue(TileDirection::RIGHT);
					characterProvider->addCharacter(soldier_13);

					Character* soldier_14 = new Character("soldier_14", Location(1, -17, 0, 7.5f, 0, 13.5f), 0.00275f, 0);
					soldier_14->setTexture("soldier_2");
					soldier_14->addTurnToQueue(TileDirection::FORWARD);
					characterProvider->addCharacter(soldier_14);

					Character* soldier_15 = new Character("soldier_15", Location(1, -22, 5, 1.5f, 0, 13.5f), 0.00275f, 0);
					soldier_15->setTexture("soldier_3");
					soldier_15->addTurnToQueue(TileDirection::BACKWARD);
					characterProvider->addCharacter(soldier_15);
					break;
				}
				default:
					break;
			}
		case 2:
			switch (level) {
				case 1:
					Character* scientist_4 = new Character("scientist_4", Location(0, -3, 0, 1, 0, 8));
					scientist_4->setTexture("scientist_2");
					characterProvider->addCharacter(scientist_4);

					Character* scientist_5 = new Character("scientist_5", Location(0, -3, 0, 2, 0, 8));
					scientist_5->setTexture("scientist_3");
					characterProvider->addCharacter(scientist_5);

					Character* scientist_6 = new Character("scientist_6", Location(0, -3, 0, 3, 0, 8));
					scientist_6->setTexture("scientist_4");
					characterProvider->addCharacter(scientist_6);

					Character* scientist_7 = new Character("scientist_7", Location(0, -3, 0, 1, 0, 7));
					scientist_7->setTexture("scientist_5");
					characterProvider->addCharacter(scientist_7);

					Character* aela = new Character("Aela", Location(0, -3, 0, 2, 0, 7));
					aela->setTexture("Aela");
					characterProvider->addCharacter(aela);

					Character* ferdinand = new Character("Ferdinand", Location(0, -3, 0, 2, 0, 5));
					ferdinand->setTexture("Ferdinand");
					ferdinand->addTurnToQueue(TileDirection::FORWARD);
					characterProvider->addCharacter(ferdinand);
					break;
			}
		default:
			break;
	}
}

void Scripts::setupTeleporters(int stage, int level) {
	switch (stage) {
		case 1:
			switch (level) {
				case 2: {
					worldManager->addTeleporterToFloorTile(Location(1, -7, 2, 6, 0, 1), Location(1, -8, 2, 8.5f, 0, 3.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -10, 3, 7, 0, 13), Location(1, -10, 3, 5.5f, 0, 13.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -10, 4, 3, 0, 1), Location(1, -10, 3, 5.5f, 0, 13.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -10, 3, 2, 0, 15), Location(1, -10, 4, 2.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -10, 3, 2, 0, 10), Location(1, -10, 3, 3.5f, 0, 7.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -11, 4, 3, 0, 1), Location(1, -11, 3, 12.5f, 0, 13.5f));

					worldManager->addTeleporterToFloorTile(Location(1, -12, 4, 10, 0, 1), Location(1, -12, 4, 8.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -12, 4, 11, 0, 1), Location(1, -12, 4, 6.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -12, 4, 10, 0, 0), Location(1, -12, 4, 7.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -12, 4, 11, 0, 0), Location(1, -12, 4, 5.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -12, 4, 7, 0, 1), Location(1, -12, 4, 6.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -12, 4, 5, 0, 1), Location(1, -12, 4, 7.5f, 0, 1.5f));

					worldManager->addTeleporterToFloorTile(Location(1, -12, 2, 9, 0, 6), Location(1, -11, 1, 0.5f, 0, 14.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -12, 2, 15, 0, 6),
														   Location(1, -12, 2, 11.5f, 0, 15.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -12, 1, 14, 0, 8), Location(1, -12, 1, 8.5f, 0, 10.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -11, 2, 11, 0, 15),
														   Location(1, -11, 2, 14.5f, 0, 3.5f));
					// worldManager->addTeleporterToFloorTile(Location(1, -10, 2, 4, 0, 5), Location(1, -11, 2, 14.5f, 0, 9.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -10, 1, 1, 0, 14), Location(1, -10, 1, 7.5f, 0, 9.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -10, 1, 5, 0, 14), Location(1, -10, 1, 11.5f, 0, 9.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -10, 1, 9, 0, 14), Location(1, -11, 1, 15.5f, 0, 14.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -12, 2, 15, 0, 6), Location(1, -12, 2, 12.5f, 0, 10.5f));
					break;
				}
				case 3: {
					worldManager->addTeleporterToFloorTile(Location(1, -17, 1, 7, 0, 2), Location(1, -17, 3, 8.5f, 0, 3.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -18, 3, 8, 0, 13), Location(1, -19, 3, 2.5f, 0, 12.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -19, 3, 2, 0, 12), Location(1, -18, 3, 8.5f, 0, 13.5f));
					
					worldManager->addTeleporterToFloorTile(Location(1, -16, 3, 5, 0, 13), Location(1, -21, 1, 11.5f, 0, 0.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -16, 3, 6, 0, 13), Location(1, -18, 3, 8.5f, 0, 13.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -16, 3, 7, 0, 13), Location(1, -18, 3, 8.5f, 0, 13.5f));
					
					// worldManager->addTeleporterToFloorTile(Location(1, -16, 0, 5, 0, 5), Location(1, -16, 0, 5, 0, 5));
					worldManager->addTeleporterToFloorTile(Location(1, -17, 1, 14, 0, 1), Location(1, -17, 0, 15.5f, 0, 9.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -17, 1, 13, 0, 1), Location(1, -17, 0, 10.5f, 0, 9.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -16, 0, 0, 0, 13), Location(1, -17, 1, 13.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -17, 0, 10, 0, 14), Location(1, -16, 0, 0.5f, 0, 14.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -16, 0, 0, 0, 9), Location(1, -17, 0, 11.5f, 0, 14.5f));
					
					worldManager->addTeleporterToFloorTile(Location(1, -21, 0, 1, 0, 15), Location(1, -22, 1, 15.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -21, 0, 1, 0, 11), Location(1, -22, 1, 15.5f, 0, 1.5f));
					// worldManager->addTeleporterToFloorTile(Location(1, -21, 0, 1, 0, 11), Location(1, -22, 1, 13.5f, 0, 0.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -21, 1, 1, 0, 1), Location(1, -22, 1, 11.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 1, 13, 0, 0), Location(1, -22, 1, 15.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 1, 13, 0, 1), Location(1, -22, 1, 11.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -21, 0, 1, 0, 9), Location(1, -22, 1, 11.5f, 0, 1.5f));
					// worldManager->addTeleporterToFloorTile(Location(1, -22, 0, 13, 0, 15), Location(1, -22, 1, 7.5f, 0, 1.5f));
					// worldManager->addTeleporterToFloorTile(Location(1, -22, 0, 13, 0, 11), Location(1, -22, 1, 7.5f, 0, 1.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 0, 9, 0, 15), Location(1, -22, 0, 15.5f, 0, 11.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 1, 9, 0, 1), Location(1, -21, 0, 3.5f, 0, 11.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 0, 5, 0, 15), Location(1, -21, 0, 3.5f, 0, 11.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 1, 9, 0, 1), Location(1, -21, 0, 3.5f, 0, 11.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 0, 13, 0, 9), Location(1, -21, 0, 3.5f, 0, 11.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 0, 9, 0, 15), Location(1, -22, 0, 15.5f, 0, 11.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 0, 13, 0, 10), Location(1, -22, 0, 3.5f, 0, 13.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 0, 13, 0, 15), Location(1, -21, 1, 3.5f, 0, 1.5f));

					worldManager->addTeleporterToFloorTile(Location(1, -22, 3, 7, 0, 10), Location(1, -22, 3, 3.5f, 0, 11.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 4, 4, 0, 1), Location(1, -22, 3, 7.5f, 0, 11.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 4, 8, 0, 0), Location(1, -22, 4, 4.5f, 0, 1.5f));

					worldManager->addTeleporterToFloorTile(Location(1, -22, 4, 6, 0, 9), Location(1, -22, 4, 7.5f, 0, 14.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 4, 9, 0, 12), Location(1, -22, 4, 8.5f, 0, 9.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 4, 8, 0, 12), Location(1, -22, 4, 3.5f, 0, 14.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 4, 7, 0, 12), Location(1, -22, 5, 3.5f, 0, 3.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 4, 3, 0, 12), Location(1, -22, 5, 3.5f, 0, 3.5f));
					worldManager->addTeleporterToFloorTile(Location(1, -22, 5, 3, 0, 1), Location(1, -22, 5, 7.5f, 0, 3.5f));

					break;
				}
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void Scripts::setupDoors(int stage, int level) {
	doorProvider->removeAllDoors();

	switch (stage) {
		case 1:
			switch (level) {
				case 1: {
					// World 1 Door 0
					auto onOpen_1_0 = []() {
						openDoor("door_1_0", 165, Location(1, -1, 0, 10, 0, 2));
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 2))->removeTile(
								197);
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 2))->addTile(198);
						hintDisplay->clear();
					};

					auto onClose_1_0 = []() {
						closeDoor("door_1_0", 165, Location(1, -1, 0, 10, 0, 2));
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 2))->removeTile(
								198);
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 2))->addTile(197);
					};

					Door door_1_0(1);
					setupDoor(&door_1_0, "door_1_0", onOpen_1_0, onClose_1_0);
					setupDoorSensor(Location(1, -1, 0, 11, 0, 1), "door_1_0",
									"door_1_0 sensor_0", 9, 0);

					// World 1 Door 1
					auto onOpen_1_1 = []() {
						openDoor("door_1_1", 169, Location(1, -1, 0, 10, 0, 5));
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 5))->removeTile(
								207);
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 5))->addTile(208);
						worldManager->getTileGroup(Location(1, -1, 0, 9, 0, 5))->removeTile(
								211);
						worldManager->getTileGroup(Location(1, -1, 0, 9, 0, 5))->addTile(212);
					};

					auto onClose_1_1 = []() {
						closeDoor("door_1_1", 169, Location(1, -1, 0, 10, 0, 5));
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 5))->addTile(207);
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 5))->removeTile(
								208);
						worldManager->getTileGroup(Location(1, -1, 0, 9, 0, 5))->addTile(211);
						worldManager->getTileGroup(Location(1, -1, 0, 9, 0, 5))->removeTile(
								212);
					};

					Door door_1_1(2);
					setupDoor(&door_1_1, "door_1_1", onOpen_1_1, onClose_1_1);
					setupDoorSensor(Location(1, -1, 0, 11, 0, 4), "door_1_1",
									"door_1_1 sensor_0", 9, 0);
					setupDoorSensor(Location(1, -1, 0, 9, 0, 4), "door_1_1",
									"door_1_1 sensor_1", 11, 1);

					// World 1 Door 2
					/*auto onOpen_1_2 = []() {
						openDoor("door_1_2", 173, Location(1, -1, 0, 10, 0, 8));
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 8))->removeTile(
								217);
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 8))->addTile(218);
						worldManager->getTileGroup(Location(1, -1, 0, 9, 0, 8))->removeTile(
								217);
						worldManager->getTileGroup(Location(1, -1, 0, 9, 0, 8))->addTile(218);
					};

					auto onClose_1_2 = []() {
						closeDoor("door_1_2", 173, Location(1, -1, 0, 10, 0, 8));
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 8))->addTile(217);
						worldManager->getTileGroup(Location(1, -1, 0, 11, 0, 8))->removeTile(
								218);
						worldManager->getTileGroup(Location(1, -1, 0, 9, 0, 8))->addTile(217);
						worldManager->getTileGroup(Location(1, -1, 0, 9, 0, 8))->removeTile(
								218);
					};

					Door door_1_2(2);
					setupDoor(&door_1_2, "door_1_2", onOpen_1_2, onClose_1_2);
					setupDoorSensor(Location(1, -1, 0, 11, 0, 7), "door_1_2",
									"door_1_2 sensor_0", 9, 0);
					setupDoorSensor(Location(1, -1, 0, 9, 0, 7), "door_1_2",
									"door_1_2 sensor_1", 11, 1);*/

					// World 1 Door 3
					auto onOpen_1_3 = []() {
						openDoor("door_1_3", 177, Location(1, -1, 0, 1, 0, 5));
						worldManager->getTileGroup(Location(1, -1, 0, 2, 0, 5))->removeTile(
								231);
						worldManager->getTileGroup(Location(1, -1, 0, 2, 0, 5))->addTile(232);
					};

					auto onClose_1_3 = []() {
						closeDoor("door_1_3", 177, Location(1, -1, 0, 1, 0, 5));
						worldManager->getTileGroup(Location(1, -1, 0, 2, 0, 5))->addTile(231);
						worldManager->getTileGroup(Location(1, -1, 0, 2, 0, 5))->removeTile(
								232);
					};

					Door door_1_3(1);
					setupDoor(&door_1_3, "door_1_3", onOpen_1_3, onClose_1_3);
					setupDoorSensor(Location(1, -1, 0, 2, 0, 4), "door_1_3",
									"door_1_3 sensor_0", 11, 0);

					// World 1 Door 4
					auto onOpen_1_4 = []() {
						openDoor("door_1_4", 181, Location(1, -2, 0, 13, 0, 5));
						worldManager->getTileGroup(Location(1, -1, 0, 1, 0, 8))->removeTile(
								239);
						worldManager->getTileGroup(Location(1, -1, 0, 1, 0, 8))->addTile(240);
					};

					auto onClose_1_4 = []() {
						closeDoor("door_1_4", 181, Location(1, -2, 0, 13, 0, 5));
						worldManager->getTileGroup(Location(1, -1, 0, 1, 0, 8))->addTile(239);
						worldManager->getTileGroup(Location(1, -1, 0, 1, 0, 8))->removeTile(
								240);
					};

					Door door_1_4(1);
					setupDoor(&door_1_4, "door_1_4", onOpen_1_4, onClose_1_4);
					setupDoorSensor(Location(1, -1, 0, 1, 0, 7), "door_1_4",
									"door_1_4 sensor_0", 10, 0);

					// World 1 Door 5
					auto onOpen_1_5 = []() {
						openDoor("door_1_5", 185, Location(1, -1, -1, 1, 0, 14));
						worldManager->getTileGroup(Location(1, -2, 0, 13, 0, 8))->removeTile(
								249);
						worldManager->getTileGroup(Location(1, -2, 0, 13, 0, 8))->addTile(250);
					};

					auto onClose_1_5 = []() {
						closeDoor("door_1_5", 185, Location(1, -1, -1, 1, 0, 14));
						worldManager->getTileGroup(Location(1, -2, 0, 13, 0, 8))->addTile(249);
						worldManager->getTileGroup(Location(1, -2, 0, 13, 0, 8))->removeTile(
								250);
					};

					Door door_1_5(1);
					setupDoor(&door_1_5, "door_1_5", onOpen_1_5, onClose_1_5);
					setupDoorSensor(Location(1, -2, 0, 13, 0, 7), "door_1_5",
									"door_1_5 sensor_0", 10, 0);

					// World 1 Door 6
					auto onOpen_1_6 = []() {
						openDoor("door_1_6", 189, Location(1, -2, 0, 10, 0, 1));
						worldManager->getTileGroup(Location(1, -1, 0, 1, 0, 1))->removeTile(
								261);
						worldManager->getTileGroup(Location(1, -1, 0, 1, 0, 1))->addTile(262);
					};

					auto onClose_1_6 = []() {
						closeDoor("door_1_6", 189, Location(1, -2, 0, 10, 0, 1));
						worldManager->getTileGroup(Location(1, -1, 0, 1, 0, 1))->addTile(261);
						worldManager->getTileGroup(Location(1, -1, 0, 1, 0, 1))->removeTile(
								262);
					};

					Door door_1_6(1);
					setupDoor(&door_1_6, "door_1_6", onOpen_1_6, onClose_1_6);
					setupDoorSensor(Location(1, -1, 0, 1, 0, 0), "door_1_6",
									"door_1_6 sensor_0", 11, 0);

					// World 1 Door 7
					auto onOpen_1_7 = []() {
						openDoor("door_1_7", 193, Location(1, -2, 0, 5, 0, 7));
						worldManager->getTileGroup(Location(1, -2, 0, 8, 0, 8))->removeTile(
								267);
						worldManager->getTileGroup(Location(1, -2, 0, 8, 0, 8))->addTile(268);
					};

					auto onClose_1_7 = []() {
						closeDoor("door_1_7", 193, Location(1, -2, 0, 5, 0, 7));
						worldManager->getTileGroup(Location(1, -2, 0, 8, 0, 8))->addTile(267);
						worldManager->getTileGroup(Location(1, -2, 0, 8, 0, 8))->removeTile(
								268);
					};

					Door door_1_7(1);
					setupDoor(&door_1_7, "door_1_7", onOpen_1_7, onClose_1_7);
					setupDoorSensor(Location(1, -2, 0, 8, 0, 7), "door_1_7",
									"door_1_7 sensor_0", 9, 0);

					// World 1 Door 8
					auto onOpen_1_8 = []() {
						openDoor("door_1_8", 165, Location(1, -3, 0, 9, 0, 7));
					};

					auto onClose_1_8 = []() {
						closeDoor("door_1_8", 165, Location(1, -3, 0, 9, 0, 7));
					};

					Door door_1_8(16);
					setupDoor(&door_1_8, "door_1_8", onOpen_1_8, onClose_1_8);
					setupDoorSensor(Location(1, -3, 0, 14, 0, 4), "door_1_8",
									"door_1_8 sensor_0", 13, 0);
					setupDoorSensor(Location(1, -3, 0, 13, 0, 4), "door_1_8",
									"door_1_8 sensor_1", 9, 1);
					setupDoorSensor(Location(1, -3, 0, 12, 0, 4), "door_1_8",
									"door_1_8 sensor_2", 9, 2);
					setupDoorSensor(Location(1, -3, 0, 11, 0, 4), "door_1_8",
									"door_1_8 sensor_3", 13, 3);
					setupDoorSensor(Location(1, -3, 0, 14, 0, 3), "door_1_8",
									"door_1_8 sensor_4", 9, 4);
					setupDoorSensor(Location(1, -3, 0, 13, 0, 3), "door_1_8",
									"door_1_8 sensor_5", 9, 5);
					setupDoorSensor(Location(1, -3, 0, 12, 0, 3), "door_1_8",
									"door_1_8 sensor_6", 9, 6);
					setupDoorSensor(Location(1, -3, 0, 11, 0, 3), "door_1_8",
									"door_1_8 sensor_7", 9, 7);
					setupDoorSensor(Location(1, -3, 0, 14, 0, 2), "door_1_8",
									"door_1_8 sensor_8", 13, 8);
					setupDoorSensor(Location(1, -3, 0, 13, 0, 2), "door_1_8",
									"door_1_8 sensor_9", 9, 9);
					setupDoorSensor(Location(1, -3, 0, 12, 0, 2), "door_1_8",
									"door_1_8 sensor_10", 9, 10);
					setupDoorSensor(Location(1, -3, 0, 11, 0, 2), "door_1_8",
									"door_1_8 sensor_11", 13, 11);
					setupDoorSensor(Location(1, -3, 0, 14, 0, 1), "door_1_8",
									"door_1_8 sensor_12", 9, 12);
					setupDoorSensor(Location(1, -3, 0, 13, 0, 1), "door_1_8",
									"door_1_8 sensor_13", 13, 13);
					setupDoorSensor(Location(1, -3, 0, 12, 0, 1), "door_1_8",
									"door_1_8 sensor_14", 13, 14);
					setupDoorSensor(Location(1, -3, 0, 11, 0, 1), "door_1_8",
									"door_1_8 sensor_15", 9, 15);

					// World 1 Door 9
					auto onOpen_1_9 = []() {
						openDoor("door_1_9", 169, Location(1, -4, 0, 13, 0, 7));
					};

					auto onClose_1_9 = []() {
						closeDoor("door_1_9", 169, Location(1, -4, 0, 13, 0, 7));
					};

					Door door_1_9(16);
					setupDoor(&door_1_9, "door_1_9", onOpen_1_9, onClose_1_9);
					setupDoorSensor(Location(1, -3, 0, 3, 0, 4), "door_1_9",
									"door_1_9 sensor_0", 13, 0);
					setupDoorSensor(Location(1, -3, 0, 2, 0, 4), "door_1_9",
									"door_1_9 sensor_1", 13, 1);
					setupDoorSensor(Location(1, -3, 0, 1, 0, 4), "door_1_9",
									"door_1_9 sensor_2", 9, 2);
					setupDoorSensor(Location(1, -3, 0, 0, 0, 4), "door_1_9",
									"door_1_9 sensor_3", 9, 3);
					setupDoorSensor(Location(1, -3, 0, 3, 0, 3), "door_1_9",
									"door_1_9 sensor_4", 9, 4);
					setupDoorSensor(Location(1, -3, 0, 2, 0, 3), "door_1_9",
									"door_1_9 sensor_5", 9, 5);
					setupDoorSensor(Location(1, -3, 0, 1, 0, 3), "door_1_9",
									"door_1_9 sensor_6", 13, 6);
					setupDoorSensor(Location(1, -3, 0, 0, 0, 3), "door_1_9",
									"door_1_9 sensor_7", 9, 7);
					setupDoorSensor(Location(1, -3, 0, 3, 0, 2), "door_1_9",
									"door_1_9 sensor_8", 13, 8);
					setupDoorSensor(Location(1, -3, 0, 2, 0, 2), "door_1_9",
									"door_1_9 sensor_9", 13, 9);
					setupDoorSensor(Location(1, -3, 0, 1, 0, 2), "door_1_9",
									"door_1_9 sensor_10", 11, 10);
					setupDoorSensor(Location(1, -3, 0, 0, 0, 2), "door_1_9",
									"door_1_9 sensor_11", 11, 11);
					setupDoorSensor(Location(1, -3, 0, 3, 0, 1), "door_1_9",
									"door_1_9 sensor_12", 10, 12);
					setupDoorSensor(Location(1, -3, 0, 2, 0, 1), "door_1_9",
									"door_1_9 sensor_13", 10, 13);
					setupDoorSensor(Location(1, -3, 0, 1, 0, 1), "door_1_9",
									"door_1_9 sensor_14", 10, 14);
					setupDoorSensor(Location(1, -3, 0, 0, 0, 1), "door_1_9",
									"door_1_9 sensor_15", 10, 15);

					// World 1 Door 10
					auto onOpen_1_10 = []() {
						openDoor("door_1_10_1", 173, Location(1, -4, 0, 3, 0, 8));
						openDoor("door_1_10_2", 173, Location(1, -4, 0, 4, 0, 8));
					};

					auto onClose_1_10 = []() {
						closeDoor("door_1_10_1", 173, Location(1, -4, 0, 3, 0, 8));
						closeDoor("door_1_10_2", 173, Location(1, -4, 0, 4, 0, 8));
					};

					Door door_1_10;
					auto function3 = (std::function<void()>) onOpen_1_10;
					door_1_10.setOnOpen(&function3);
					auto function4 = (std::function<void()>) onClose_1_10;
					door_1_10.setOnClose(&function4);
					auto* door_1_10_lock_0 = new TileAmountLock();
					door_1_10_lock_0->specifyRegionAsCuboid(Location(1, -4, 0, 1, 0, 5), 6, 1, 3);
					door_1_10_lock_0->addTileTypeToCheckFor(9, 3);
					door_1_10_lock_0->addTileTypeToCheckFor(10, 6);
					door_1_10_lock_0->addTileTypeToCheckFor(11, 3);
					door_1_10_lock_0->addTileTypeToCheckFor(13, 6);
					Lock* door_1_10_lock_0_base = door_1_10_lock_0;
					door_1_10.addLock(door_1_10_lock_0_base);
					setupDoorTileAmountSensors(door_1_10_lock_0, "door_1_10 sensor_0");
					doorProvider->addDoor("door_1_10", &door_1_10);
					door_1_10_lock_0->setIDInDoor(0);
					door_1_10_lock_0->setDoorByID("door_1_10");

					// World 1 Door 11
					auto onOpen_1_11 = []() {
						openDoor("door_1_11", 177, Location(1, -5, 0, 13, 0, 10));
						worldManager->getTileGroup(Location(1, -4, 0, 3, 0, 11))->removeTile(
								235);
						worldManager->getTileGroup(Location(1, -4, 0, 3, 0, 11))->addTile(236);
					};

					auto onClose_1_11 = []() {
						closeDoor("door_1_11", 177, Location(1, -5, 0, 13, 0, 10));
						worldManager->getTileGroup(Location(1, -4, 0, 3, 0, 11))->addTile(235);
						worldManager->getTileGroup(Location(1, -4, 0, 3, 0, 11))->removeTile(
								236);
					};

					Door door_1_11(1);
					setupDoor(&door_1_11, "door_1_11", onOpen_1_11, onClose_1_11);
					setupDoorSensor(Location(1, -4, 0, 3, 0, 10), "door_1_11",
									"door_1_11 sensor_0", 13, 0);
					break;
				}
				case 2: {
					doorProvider->removeAllDoors();

					// World 1 Door 12
					auto onOpen_1_12 = []() {
						closeDoor("door_1_12_1", 165, Location(1, -7, 0, 13, 0, 13));
						openDoor("door_1_12_2", 165, Location(1, -7, 0, 5, 0, 13));
						worldManager->getTileGroup(Location(1, -7, 1, 9, 0, 0))->removeTile(
								199);
						worldManager->getTileGroup(Location(1, -7, 1, 9, 0, 0))->addTile(200);
					};

					auto onClose_1_12 = []() {
						openDoor("door_1_12_1", 165, Location(1, -7, 0, 13, 0, 13));
						closeDoor("door_1_12_2", 165, Location(1, -7, 0, 5, 0, 13));
						worldManager->getTileGroup(Location(1, -7, 1, 9, 0, 0))->addTile(199);
						worldManager->getTileGroup(Location(1, -7, 1, 9, 0, 0))->removeTile(
								200);
					};

					Door door_1_12(1);
					setupDoor(&door_1_12, "door_1_12", onOpen_1_12, onClose_1_12);
					setupDoorSensor(Location(1, -7, 0, 9, 0, 15), "door_1_12",
									"door_1_12 sensor_0", 10, 0);

					// World 1 Door 13
					auto onOpen_1_13 = []() {
						openDoor("door_1_0", 169, Location(1, -7, 0, 9, 0, 13));
						worldManager->getTileGroup(Location(1, -7, 1, 12, 0, 0))->removeTile(
								209);
						worldManager->getTileGroup(Location(1, -7, 1, 12, 0, 0))->addTile(210);
					};

					auto onClose_1_13 = []() {
						closeDoor("door_1_0", 169, Location(1, -7, 0, 9, 0, 13));
						worldManager->getTileGroup(Location(1, -7, 1, 12, 0, 0))->addTile(209);
						worldManager->getTileGroup(Location(1, -7, 1, 12, 0, 0))->removeTile(
								210);
					};

					Door door_1_13(1);
					setupDoor(&door_1_13, "door_1_13", onOpen_1_13, onClose_1_13);
					setupDoorSensor(Location(1, -7, 0, 12, 0, 15), "door_1_13",
									"door_1_13 sensor_0", 10, 0);

					// World 1 Door 14
					auto onOpen_1_14 = []() {
						openDoor("door_1_14", 173, Location(1, -7, 0, 1, 0, 13));
						worldManager->getTileGroup(Location(1, -7, 1, 14, 0, 0))->removeTile(
								221);
						worldManager->getTileGroup(Location(1, -7, 1, 14, 0, 0))->addTile(222);
					};

					auto onClose_1_14 = []() {
						closeDoor("door_1_14", 173, Location(1, -7, 0, 1, 0, 13));
						worldManager->getTileGroup(Location(1, -7, 1, 14, 0, 0))->addTile(221);
						worldManager->getTileGroup(Location(1, -7, 1, 14, 0, 0))->removeTile(
								222);
					};

					Door door_1_14(1);
					setupDoor(&door_1_14, "door_1_14", onOpen_1_14, onClose_1_14);
					setupDoorSensor(Location(1, -7, 0, 14, 0, 15), "door_1_14",
									"door_1_14 sensor_0", 11, 0);

					// World 1 Door 15
					auto onClose_1_15 = []() {
						closeDoor("door_1_15", 181, Location(1, -7, 1, 5, 0, 6));
						worldManager->getTileGroup(Location(1, -7, 1, 1, 0, 11))->addTile(241);
						worldManager->getTileGroup(Location(1, -7, 1, 1, 0, 11))->removeTile(
								242);
					};

					auto onTimerCompletion_1_15 = [onClose_1_15]() {
						doorProvider->getDoor("door_1_15")->lock(0);
					};

					auto onOpen_1_15 = [onTimerCompletion_1_15]() {
						openDoor("door_1_15", 181, Location(1, -7, 1, 5, 0, 6));
						worldManager->getTileGroup(Location(1, -7, 1, 1, 0, 11))->removeTile(
								241);
						worldManager->getTileGroup(Location(1, -7, 1, 1, 0, 11))->addTile(242);
						LockTimer* lockTimer = new LockTimer(4500, TimerColour::TEAL, onTimerCompletion_1_15);
						lockTimer->start();
					};

					Door door_1_15(1);
					setupDoor(&door_1_15, "door_1_15", onOpen_1_15, onClose_1_15);
					setupTimedDoorSensor(Location(1, -7, 1, 1, 0, 10), "door_1_15",
									"door_1_15 sensor_0", 11, 0);

					// World 1 Door 16
					auto onOpen_1_16 = []() {
						closeDoor("door_1_16_1", 169, Location(1, -7, 2, 4, 0, 5));
						openDoor("door_1_16_2", 169, Location(1, -7, 2, 8, 0, 5));
						worldManager->getTileGroup(Location(1, -7, 2, 9, 0, 5))->addTile(210);
						worldManager->getTileGroup(Location(1, -7, 2, 9, 0, 5))->removeTile(
								209);
					};

					auto onClose_1_16 = []() {
						openDoor("door_1_16_1", 169, Location(1, -7, 2, 4, 0, 5));
						closeDoor("door_1_16_2", 169, Location(1, -7, 2, 8, 0, 5));
						worldManager->getTileGroup(Location(1, -7, 2, 9, 0, 5))->addTile(209);
						worldManager->getTileGroup(Location(1, -7, 2, 9, 0, 5))->removeTile(
								210);
					};

					Door door_1_16({false});
					setupDoor(&door_1_16, "door_1_16", onOpen_1_16, onClose_1_16);
					setupDoorSensor(Location(1, -7, 2, 9, 0, 4), "door_1_16",
									"door_1_16 sensor_0", 10, 0);

					// World 1 Door 17
					auto onClose_1_17 = []() {
						closeDoor("door_1_17_1", 185, Location(1, -7, 1, 5, 0, 14));
						closeDoor("door_1_17_2", 185, Location(1, -8, 2, 13, 0, 0));
						closeDoor("door_1_17_3", 185, Location(1, -8, 2, 13, 0, 8));
						worldManager->getTileGroup(Location(1, -7, 2, 3, 0, 9))->addTile(249);
						worldManager->getTileGroup(Location(1, -7, 2, 3, 0, 9))->removeTile(
								250);
					};

					auto onTimerCompletion_1_17 = [onClose_1_17]() {
						doorProvider->getDoor("door_1_17")->lock(0);
						doorProvider->getDoor("door_1_17")->lock(1);
					};

					auto onOpen_1_17 = [onTimerCompletion_1_17]() {
						openDoor("door_1_17_1", 185, Location(1, -7, 1, 5, 0, 14));
						openDoor("door_1_17_2", 185, Location(1, -8, 2, 13, 0, 0));
						openDoor("door_1_17_3", 185, Location(1, -8, 2, 13, 0, 8));
						worldManager->getTileGroup(Location(1, -7, 2, 3, 0, 9))->removeTile(
								249);
						worldManager->getTileGroup(Location(1, -7, 2, 3, 0, 9))->addTile(250);
						LockTimer* lockTimer = new LockTimer(12000, TimerColour::RED, onTimerCompletion_1_17);
						lockTimer->start();
					};

					Door door_1_17(1);
					setupDoor(&door_1_17, "door_1_17", onOpen_1_17, onClose_1_17);
					setupTimedDoorSensor(Location(1, -7, 2, 3, 0, 8), "door_1_17",
									"door_1_17 sensor_0", 10, 0);

					// World 1 Door 18
					auto onOpen_1_18 = []() {
						openDoor("door_1_18", 165, Location(1, -8, 2, 9, 0, 4));
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 4))->addTile(202);
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 4))->removeTile(
								201);
					};

					auto onClose_1_18 = []() {
						closeDoor("door_1_18", 165, Location(1, -8, 2, 9, 0, 4));
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 4))->removeTile(
								202);
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 4))->addTile(201);
					};

					Door door_1_18(1);
					setupDoor(&door_1_18, "door_1_18", onOpen_1_18, onClose_1_18);
					setupDoorSensor(Location(1, -8, 2, 12, 0, 3), "door_1_18",
									"door_1_18 sensor_0", 11, 0);

					// World 1 Door 19
					/*auto onOpen_1_19 = []() {
						openDoor("door_1_19", 189, Location(1, -8, 2, 9, 0, 8));
						worldManager->getTileGroup(Location(1, -8, 2, 9, 0, 8))->removeTile(
								189);
						worldManager->getTileGroup(Location(1, -8, 2, 8, 0, 8))->addTile(266);
						worldManager->getTileGroup(Location(1, -8, 2, 8, 0, 8))->removeTile(
								265);
					};

					auto onClose_1_19 = []() {
						closeDoor("door_1_19", 189, Location(1, -8, 2, 9, 0, 8));
						worldManager->getTileGroup(Location(1, -8, 2, 8, 0, 8))->removeTile(
								266);
						worldManager->getTileGroup(Location(1, -8, 2, 8, 0, 8))->addTile(265);
					};

					Door door_1_19(1);
					setupDoor(&door_1_19, "door_1_19", onOpen_1_19, onClose_1_19);
					setupDoorSensor(Location(1, -8, 2, 8, 0, 7), "door_1_19",
									"door_1_19 sensor_0", 13, 0);*/

					// World 1 Door 20
					auto onOpen_1_20 = []() {
						openDoor("door_1_20", 173, Location(1, -8, 2, 9, 0, 12));
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 12))->addTile(222);
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 12))->removeTile(
								221);
					};

					auto onClose_1_20 = []() {
						closeDoor("door_1_20", 173, Location(1, -8, 2, 9, 0, 12));
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 12))->removeTile(
								222);
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 12))->addTile(221);
					};

					Door door_1_20(1);
					setupDoor(&door_1_20, "door_1_20", onOpen_1_20, onClose_1_20);
					setupDoorSensor(Location(1, -8, 2, 12, 0, 11), "door_1_20",
									"door_1_20 sensor_0", 11, 0);

					// World 1 Door 21
					auto onOpen_1_21 = []() {
						openDoor("door_1_21", 193, Location(1, -8, 2, 13, 0, 15));
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 15))->addTile(270);
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 15))->removeTile(
								269);
					};

					auto onClose_1_21 = []() {
						closeDoor("door_1_21", 193, Location(1, -8, 2, 13, 0, 15));
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 15))->removeTile(
								270);
						worldManager->getTileGroup(Location(1, -8, 2, 12, 0, 15))->addTile(269);
					};

					Door door_1_21(1);
					setupDoor(&door_1_21, "door_1_21", onOpen_1_21, onClose_1_21);
					setupDoorSensor(Location(1, -8, 2, 12, 0, 14), "door_1_21",
									"door_1_21 sensor_0", 10, 0);

					// World 1 Door 22
					auto onOpen_1_22 = []() {
						openDoor("door_1_22", 165, Location(1, -11, 3, 11, 0, 14));
						worldManager->getTileGroup(Location(1, -11, 3, 12, 0, 14))->addTile(
								200);
						worldManager->getTileGroup(Location(1, -11, 3, 12, 0, 14))->removeTile(
								199);
					};

					auto onClose_1_22 = []() {
						closeDoor("door_1_22", 165, Location(1, -11, 3, 11, 0, 14));
						worldManager->getTileGroup(Location(1, -11, 3, 12, 0, 14))->removeTile(
								200);
						worldManager->getTileGroup(Location(1, -11, 3, 12, 0, 14))->addTile(
								199);
					};

					Door door_1_22(1);
					setupDoor(&door_1_22, "door_1_22", onOpen_1_22, onClose_1_22);
					setupDoorSensor(Location(1, -11, 3, 12, 0, 13), "door_1_22",
									"door_1_22 sensor_0", 10, 0);

					// World 1 Door 23
					auto onOpen_1_23 = []() {
						openDoor("door_1_23_1", 169, Location(1, -11, 3, 2, 0, 14));
						closeDoor("door_1_23_2", 169, Location(1, -11, 3, 7, 0, 14));
						worldManager->getTileGroup(Location(1, -11, 4, 11, 0, 2))->addTile(214);
						worldManager->getTileGroup(Location(1, -11, 4, 11, 0, 2))->removeTile(
								213);
						worldManager->getTileGroup(Location(1, -11, 4, 2, 0, 2))->addTile(208);
						worldManager->getTileGroup(Location(1, -11, 4, 2, 0, 2))->removeTile(
								207);
					};

					auto onClose_1_23 = []() {
						closeDoor("door_1_23_1", 169, Location(1, -11, 3, 2, 0, 14));
						openDoor("door_1_23_2", 169, Location(1, -11, 3, 7, 0, 14));
						worldManager->getTileGroup(Location(1, -11, 4, 11, 0, 2))->removeTile(
								214);
						worldManager->getTileGroup(Location(1, -11, 4, 11, 0, 2))->addTile(213);
						worldManager->getTileGroup(Location(1, -11, 4, 2, 0, 2))->removeTile(
								208);
						worldManager->getTileGroup(Location(1, -11, 4, 2, 0, 2))->addTile(207);
					};

					Door door_1_23({true, false});
					setupDoor(&door_1_23, "door_1_23", onOpen_1_23, onClose_1_23);
					setupDoorSensor(Location(1, -11, 4, 11, 0, 1), "door_1_23",
									"door_1_23 sensor_0", 12, 0);
					setupDoorSensor(Location(1, -11, 4, 2, 0, 1), "door_1_23",
									"door_1_23 sensor_1", 9, 1);

					// World 1 Door 24
					auto onOpen_1_24 = []() {
						openDoor("door_1_24", 173, Location(1, -11, 4, 7, 0, 1));
						worldManager->getTileGroup(Location(1, -11, 4, 7, 0, 1))->removeTile(
								173);
						worldManager->getTileGroup(Location(1, -11, 4, 6, 0, 1))->addTile(218);
						worldManager->getTileGroup(Location(1, -11, 4, 6, 0, 1))->removeTile(
								217);
					};

					auto onClose_1_24 = []() {
						closeDoor("door_1_24", 173, Location(1, -11, 4, 7, 0, 1));
						worldManager->getTileGroup(Location(1, -11, 4, 6, 0, 1))->removeTile(
								218);
						worldManager->getTileGroup(Location(1, -11, 4, 6, 0, 1))->addTile(217);
					};

					Door door_1_24(1);
					setupDoor(&door_1_24, "door_1_24", onOpen_1_24, onClose_1_24);
					setupDoorSensor(Location(1, -11, 4, 6, 0, 0), "door_1_24",
									"door_1_24 sensor_0", 9, 0);

					// World 1 Door 25
					auto onOpen_1_25 = []() {
						openDoor("door_1_25", 177, Location(1, -12, 3, 13, 0, 14));
						worldManager->getTileGroup(Location(1, -12, 3, 13, 0, 14))->removeTile(
								177);
						worldManager->getTileGroup(Location(1, -12, 3, 12, 0, 14))->addTile(
								232);
						worldManager->getTileGroup(Location(1, -12, 3, 12, 0, 14))->removeTile(
								231);
					};

					auto onClose_1_25 = []() {
						closeDoor("door_1_25", 177, Location(1, -12, 3, 13, 0, 14));
						worldManager->getTileGroup(Location(1, -12, 3, 12, 0, 14))->removeTile(
								232);
						worldManager->getTileGroup(Location(1, -12, 3, 12, 0, 14))->addTile(
								231);
					};

					Door door_1_25(1);
					setupDoor(&door_1_25, "door_1_25", onOpen_1_25, onClose_1_25);
					setupDoorSensor(Location(1, -12, 3, 12, 0, 13), "door_1_25",
									"door_1_25 sensor_0", 11, 0);

					// World 1 Door 26
					auto onOpen_1_26 = []() {
						openDoor("door_1_26", 165, Location(1, -12, 2, 15, 0, 3));
						worldManager->getTileGroup(Location(1, -11, 2, 0, 0, 3))->addTile(204);
						worldManager->getTileGroup(Location(1, -11, 2, 0, 0, 3))->removeTile(
								203);
						worldManager->getTileGroup(Location(1, -12, 2, 14, 0, 3))->addTile(204);
						worldManager->getTileGroup(Location(1, -12, 2, 14, 0, 3))->removeTile(
								203);
					};

					auto onClose_1_26 = []() {
						closeDoor("door_1_26", 165, Location(1, -12, 2, 15, 0, 3));
						worldManager->getTileGroup(Location(1, -11, 2, 0, 0, 3))->removeTile(
								204);
						worldManager->getTileGroup(Location(1, -11, 2, 0, 0, 3))->addTile(203);
						worldManager->getTileGroup(Location(1, -12, 2, 14, 0, 3))->removeTile(
								204);
						worldManager->getTileGroup(Location(1, -12, 2, 14, 0, 3))->addTile(203);
					};

					Door door_1_26(2);
					setupDoor(&door_1_26, "door_1_26", onOpen_1_26, onClose_1_26);
					setupDoorSensor(Location(1, -11, 2, 0, 0, 2), "door_1_26",
									"door_1_26 sensor_0", 12, 0);
					setupDoorSensor(Location(1, -12, 2, 14, 0, 2), "door_1_26",
									"door_1_26 sensor_1", 12, 1);

					// World 1 Door 27
					auto onOpen_1_27 = []() {
						// openDoor("door_1_27_1", 173, Location(1, -11, 2, 4, 0, 3));
						openDoor("door_1_27_2", 173, Location(1, -12, 2, 10, 0, 3));
						worldManager->getTileGroup(Location(1, -11, 2, 4, 0, 7))->addTile(218);
						worldManager->getTileGroup(Location(1, -11, 2, 4, 0, 7))->removeTile(
								217);
						worldManager->getTileGroup(Location(1, -12, 2, 10, 0, 7))->addTile(222);
						worldManager->getTileGroup(Location(1, -12, 2, 10, 0, 7))->removeTile(
								221);
						worldManager->getTileGroup(Location(1, -12, 2, 10, 1, 7))->addTile(224);
						worldManager->getTileGroup(Location(1, -12, 2, 10, 1, 7))->removeTile(
								223);
						worldManager->getTileGroup(Location(1, -11, 1, 5, 0, 15))->addTile(226);
						worldManager->getTileGroup(Location(1, -11, 1, 5, 0, 15))->removeTile(
								225);
					};

					auto onClose_1_27 = []() {
						// closeDoor("door_1_27_1", 173, Location(1, -11, 2, 4, 0, 3));
						closeDoor("door_1_27_2", 173, Location(1, -12, 2, 10, 0, 3));
						worldManager->getTileGroup(Location(1, -11, 2, 4, 0, 7))->removeTile(
								218);
						worldManager->getTileGroup(Location(1, -11, 2, 4, 0, 7))->addTile(217);
						worldManager->getTileGroup(Location(1, -12, 2, 10, 0, 7))->removeTile(
								222);
						worldManager->getTileGroup(Location(1, -12, 2, 10, 0, 7))->addTile(221);
						worldManager->getTileGroup(Location(1, -12, 2, 10, 1, 7))->removeTile(
								224);
						worldManager->getTileGroup(Location(1, -12, 2, 10, 1, 7))->addTile(223);
						worldManager->getTileGroup(Location(1, -11, 1, 5, 0, 15))->removeTile(
								226);
						worldManager->getTileGroup(Location(1, -11, 1, 5, 0, 15))->addTile(225);
					};

					Door door_1_27({false, true, true});
					setupDoor(&door_1_27, "door_1_27", onOpen_1_27, onClose_1_27);
					setupDoorSensor(Location(1, -11, 2, 4, 0, 6), "door_1_27",
									"door_1_27 sensor_0", 9, 0);
					setupMultiTypeDoorSensor(Location(1, -12, 2, 10, 0, 6), "door_1_27",
											 "door_1_27 sensor_1", {11, 12}, 1);
					setupDoorSensor(Location(1, -11, 1, 5, 0, 14), "door_1_27",
									"door_1_27 sensor_2", 13, 2);

					// World 1 Door 28
					auto onOpen_1_28 = []() {
						openDoor("door_1_28", 181, Location(1, -12, 1, 9, 0, 11));
						worldManager->getTileGroup(Location(1, -12, 1, 10, 0, 11))->addTile(
								238);
						worldManager->getTileGroup(Location(1, -12, 1, 10, 0, 11))->removeTile(
								237);
						worldManager->getTileGroup(Location(1, -12, 1, 8, 0, 11))->addTile(240);
						worldManager->getTileGroup(Location(1, -12, 1, 8, 0, 11))->removeTile(
								239);
					};

					auto onClose_1_28 = []() {
						closeDoor("door_1_28", 181, Location(1, -12, 1, 9, 0, 11));
						worldManager->getTileGroup(Location(1, -12, 1, 10, 0, 11))->removeTile(
								238);
						worldManager->getTileGroup(Location(1, -12, 1, 10, 0, 11))->addTile(
								237);
						worldManager->getTileGroup(Location(1, -12, 1, 8, 0, 11))->removeTile(
								240);
						worldManager->getTileGroup(Location(1, -12, 1, 8, 0, 11))->addTile(239);
					};

					Door door_1_28(2);
					setupDoor(&door_1_28, "door_1_28", onOpen_1_28, onClose_1_28);
					setupDoorSensor(Location(1, -12, 1, 10, 0, 10), "door_1_28",
									"door_1_28 sensor_0", 9, 0);
					setupDoorSensor(Location(1, -12, 1, 8, 0, 10), "door_1_28",
									"door_1_28 sensor_1", 10, 1);

					// World 1 Door 29
					auto onOpen_1_29 = []() {
						openDoor("door_1_29", 185, Location(1, -12, 1, 15, 0, 9));
						worldManager->getTileGroup(Location(1, -12, 1, 15, 0, 15))->addTile(
								256);
						worldManager->getTileGroup(Location(1, -12, 1, 15, 0, 15))->removeTile(
								255);
					};

					auto onClose_1_29 = []() {
						closeDoor("door_1_29", 185, Location(1, -12, 1, 15, 0, 9));
						worldManager->getTileGroup(Location(1, -12, 1, 15, 0, 15))->removeTile(
								256);
						worldManager->getTileGroup(Location(1, -12, 1, 15, 0, 15))->addTile(
								255);
					};

					Door door_1_29(1);
					setupDoor(&door_1_29, "door_1_29", onOpen_1_29, onClose_1_29);
					setupDoorSensor(Location(1, -12, 1, 15, 0, 14), "door_1_29",
									"door_1_29 sensor_0", 13, 0);

					// World 1 Door 30
					auto onOpen_1_30 = []() {
						openDoor("door_1_30", 165, Location(1, -10, 2, 5, 0, 15));
						worldManager->getTileGroup(Location(1, -10, 2, 6, 0, 15))->addTile(204);
						worldManager->getTileGroup(Location(1, -10, 2, 6, 0, 15))->removeTile(
								203);
						worldManager->getTileGroup(Location(1, -10, 2, 4, 0, 15))->addTile(204);
						worldManager->getTileGroup(Location(1, -10, 2, 4, 0, 15))->removeTile(
								203);
					};

					auto onClose_1_30 = []() {
						closeDoor("door_1_30", 165, Location(1, -10, 2, 5, 0, 15));
						worldManager->getTileGroup(Location(1, -10, 2, 6, 0, 15))->removeTile(
								204);
						worldManager->getTileGroup(Location(1, -10, 2, 6, 0, 15))->addTile(203);
						worldManager->getTileGroup(Location(1, -10, 2, 4, 0, 15))->removeTile(
								204);
						worldManager->getTileGroup(Location(1, -10, 2, 4, 0, 15))->addTile(203);
					};

					Door door_1_30(2);
					setupDoor(&door_1_30, "door_1_30", onOpen_1_30, onClose_1_30);
					setupDoorSensor(Location(1, -10, 2, 6, 0, 14), "door_1_30",
									"door_1_30 sensor_0", 12, 0);
					setupDoorSensor(Location(1, -10, 2, 4, 0, 14), "door_1_30",
									"door_1_30 sensor_1", 12, 1);

					// World 1 Door 31
					auto onOpen_1_31 = []() {
						openDoor("door_1_31", 181, Location(1, -11, 2, 15, 0, 4));
						worldManager->getTileGroup(Location(1, -11, 2, 14, 0, 4))->addTile(244);
						worldManager->getTileGroup(Location(1, -11, 2, 14, 0, 4))->removeTile(
								243);
					};

					auto onClose_1_31 = []() {
						closeDoor("door_1_31", 181, Location(1, -11, 2, 15, 0, 4));
						worldManager->getTileGroup(Location(1, -11, 2, 14, 0, 4))->removeTile(
								244);
						worldManager->getTileGroup(Location(1, -11, 2, 14, 0, 4))->addTile(243);
					};

					Door door_1_31(1);
					setupDoor(&door_1_31, "door_1_31", onOpen_1_31, onClose_1_31);
					setupDoorSensor(Location(1, -11, 2, 14, 0, 3), "door_1_31",
									"door_1_31 sensor_0", 12, 0);

					// World 1 Door 32
					auto onOpen_1_32 = []() {
						openDoor("door_1_32", 185, Location(1, -11, 2, 15, 0, 10));
					};

					auto onClose_1_32 = []() {
						closeDoor("door_1_32", 185, Location(1, -11, 2, 15, 0, 10));
					};

					Door door_1_32;
					auto function1 = (std::function<void()>) onOpen_1_32;
					door_1_32.setOnOpen(&function1);
					auto function2 = (std::function<void()>) onClose_1_32;
					door_1_32.setOnClose(&function2);
					auto* door_1_32_lock_0 = new TileAmountLock();
					door_1_32_lock_0->specifyRegionAsCuboid(Location(1, -11, 2, 14, 0, 9), 3, 1, 1);
					door_1_32_lock_0->addTileTypeToCheckFor(11, 1);
					door_1_32_lock_0->addTileTypeToCheckFor(13, 2);
					Lock* door_1_32_lock_0_base = door_1_32_lock_0;
					door_1_32.addLock(door_1_32_lock_0_base);
					setupDoorTileAmountSensors(door_1_32_lock_0, "door_1_32 sensor_0");
					doorProvider->addDoor("door_1_32", &door_1_32);
					door_1_32_lock_0->setIDInDoor(0);
					door_1_32_lock_0->setDoorByID("door_1_32");

					// World 1 Door 33
					auto onOpen_1_33 = []() {
						openDoor("door_1_33", 189, Location(1, -10, 2, 5, 0, 7));
						worldManager->getTileGroup(Location(1, -10, 2, 7, 0, 11))->addTile(266);
						worldManager->getTileGroup(Location(1, -10, 2, 15, 0, 15))->removeTile(
								265);
					};

					auto onClose_1_33 = []() {
						closeDoor("door_1_33", 189, Location(1, -10, 2, 5, 0, 7));
						worldManager->getTileGroup(Location(1, -10, 2, 7, 0, 11))->removeTile(
								266);
						worldManager->getTileGroup(Location(1, -10, 2, 15, 0, 15))->addTile(
								265);
					};

					Door door_1_33(1);
					setupDoor(&door_1_33, "door_1_33", onOpen_1_33, onClose_1_33);
					setupDoorSensor(Location(1, -10, 2, 7, 0, 10), "door_1_33",
									"door_1_33 sensor_0", 13, 0);

					// World 1 Door 34
					auto onOpen_1_34 = []() {
						openDoor("door_1_34_1", 169, Location(1, -11, 1, 14, 0, 7));
						closeDoor("door_1_34_2", 169, Location(1, -10, 1, 2, 0, 7));
						worldManager->getTileGroup(Location(1, -11, 1, 15, 0, 10))->addTile(
								212);
						worldManager->getTileGroup(Location(1, -11, 1, 15, 0, 10))->removeTile(
								211);
					};

					auto onClose_1_34 = []() {
						closeDoor("door_1_34_1", 169, Location(1, -11, 1, 14, 0, 7));
						openDoor("door_1_34_2", 169, Location(1, -10, 1, 2, 0, 7));
						worldManager->getTileGroup(Location(1, -11, 1, 15, 0, 10))->removeTile(
								212);
						worldManager->getTileGroup(Location(1, -11, 1, 15, 0, 10))->addTile(
								211);
					};

					Door door_1_34({false});
					setupDoor(&door_1_34, "door_1_34", onOpen_1_34, onClose_1_34);
					setupDoorSensor(Location(1, -11, 1, 15, 0, 9), "door_1_34",
									"door_1_34 sensor_0", 11, 0);

					// World 1 Door 35
					auto onOpen_1_35 = []() {
						openDoor("door_1_35", 173, Location(1, -10, 1, 6, 0, 10));
						worldManager->getTileGroup(Location(1, -10, 1, 5, 0, 10))->addTile(222);
						worldManager->getTileGroup(Location(1, -10, 1, 5, 0, 10))->removeTile(
								221);
					};

					auto onClose_1_35 = []() {
						closeDoor("door_1_35", 173, Location(1, -10, 1, 6, 0, 10));
						worldManager->getTileGroup(Location(1, -10, 1, 5, 0, 10))->removeTile(
								222);
						worldManager->getTileGroup(Location(1, -10, 1, 5, 0, 10))->addTile(221);
					};

					Door door_1_35(1);
					setupDoor(&door_1_35, "door_1_35", onOpen_1_35, onClose_1_35);
					setupDoorSensor(Location(1, -10, 1, 5, 0, 9), "door_1_35",
									"door_1_35 sensor_0", 11, 0);

					// World 1 Door 36
					auto onOpen_1_36 = []() {
						openDoor("door_1_36", 177, Location(1, -10, 1, 10, 0, 10));
						worldManager->getTileGroup(Location(1, -10, 1, 9, 0, 10))->addTile(232);
						worldManager->getTileGroup(Location(1, -10, 1, 9, 0, 10))->removeTile(
								231);
					};

					auto onClose_1_36 = []() {
						closeDoor("door_1_36", 177, Location(1, -10, 1, 10, 0, 10));
						worldManager->getTileGroup(Location(1, -10, 1, 9, 0, 10))->removeTile(
								232);
						worldManager->getTileGroup(Location(1, -10, 1, 9, 0, 10))->addTile(231);
					};

					Door door_1_36(1);
					setupDoor(&door_1_36, "door_1_36", onOpen_1_36, onClose_1_36);
					setupDoorSensor(Location(1, -10, 1, 9, 0, 9), "door_1_36",
									"door_1_36 sensor_0", 11, 0);

					// World 1 Door 37
					auto onOpen_1_37 = []() {
						openDoor("door_1_37", 177, Location(1, -11, 1, 5, 0, 11));
						worldManager->getTileGroup(Location(1, -12, 1, 9, 0, 15))->addTile(232);
						worldManager->getTileGroup(Location(1, -12, 1, 9, 0, 15))->removeTile(
								231);
					};

					auto onClose_1_37 = []() {
						closeDoor("door_1_37", 177, Location(1, -11, 1, 5, 0, 11));
						worldManager->getTileGroup(Location(1, -12, 1, 9, 0, 15))->removeTile(
								232);
						worldManager->getTileGroup(Location(1, -12, 1, 9, 0, 15))->addTile(231);
					};

					Door door_1_37(1);
					setupDoor(&door_1_37, "door_1_37", onOpen_1_37, onClose_1_37);
					setupDoorSensor(Location(1, -12, 1, 9, 0, 14), "door_1_37",
									"door_1_37 sensor_0", 11, 0);

					// World 1 Door 38
					/*auto onOpen_1_38 = []() {
						openDoor("door_1_38", 169, Location(1, -12, 2, 15, 0, 11));
						worldManager->getTileGroup(Location(1, -12, 3, 12, 0, 0))->addTile(216);
						worldManager->getTileGroup(Location(1, -12, 3, 12, 0, 0))->removeTile(
								215);
					};

					auto onClose_1_38 = []() {
						closeDoor("door_1_38", 169, Location(1, -12, 2, 15, 0, 11));
						worldManager->getTileGroup(Location(1, -12, 3, 12, 0, 0))->removeTile(
								216);
						worldManager->getTileGroup(Location(1, -12, 3, 12, 0, 0))->addTile(215);
					};

					Door door_1_38(1);
					setupDoor(&door_1_38, "door_1_38", onOpen_1_38, onClose_1_38);
					setupDoorSensor(Location(1, -12, 2, 13, 0, 10), "door_1_38",
									"door_1_38 sensor_0", 13, 0);*/

					// World 1 Door 39
					auto onOpen_1_39 = []() {
						openDoor("door_1_39", 193, Location(1, -11, 1, 14, 0, 10));
						worldManager->getTileGroup(Location(1, -11, 1, 13, 0, 15))->addTile(
								272);
						worldManager->getTileGroup(Location(1, -11, 1, 13, 0, 15))->removeTile(
								271);
					};

					auto onClose_1_39 = []() {
						closeDoor("door_1_39", 193, Location(1, -11, 1, 14, 0, 10));
						worldManager->getTileGroup(Location(1, -11, 1, 13, 0, 15))->removeTile(
								272);
						worldManager->getTileGroup(Location(1, -11, 1, 13, 0, 15))->addTile(
								271);
					};

					Door door_1_39(1);
					setupDoor(&door_1_39, "door_1_39", onOpen_1_39, onClose_1_39);
					setupDoorSensor(Location(1, -11, 1, 13, 0, 14), "door_1_39",
									"door_1_39 sensor_0", 11, 0);
					
					// World 1 Skip Door 0
					auto onOpen_skip_1_0 = []() {
						openDoor("door_skip_1_0", 345, Location(1, -12, 3, 7, 0, 6));
					};

					auto onClose_skip_1_0 = []() {
						closeDoor("door_skip_1_0", 345, Location(1, -12, 3, 7, 0, 6));
					};

					Door door_skip_1_0(1);
					setupDoor(&door_skip_1_0, "door_skip_1_0", onOpen_skip_1_0, onClose_skip_1_0);

					// World 1 Skip Door 1
					auto onOpen_skip_1_1 = []() {
						openDoor("door_skip_1_1", 345, Location(1, -11, 3, 0, 0, 0));
					};

					auto onClose_skip_1_1 = []() {
						closeDoor("door_skip_1_1", 345, Location(1, -11, 3, 0, 0, 0));
					};

					Door door_skip_1_1(1);
					setupDoor(&door_skip_1_1, "door_skip_1_1", onOpen_skip_1_1, onClose_skip_1_1);

					// World 1 Door 56
					auto onOpen_1_56 = []() {
						openDoor("door_1_56_1", 345, Location(1, -9, 4, 1, 0, 9));
					};

					auto onClose_1_56 = []() {
						closeDoor("door_1_56_1", 345, Location(1, -9, 4, 1, 0, 9));
					};

					Door door_1_56(1);
					setupDoor(&door_1_56, "door_1_56", onOpen_1_56, onClose_1_56);

					// World 1 Door 57
					auto onOpen_1_57 = []() {
						openDoor("door_1_57_1", 345, Location(1, -8, 4, 6, 0, 9));
					};

					auto onClose_1_57 = []() {
						closeDoor("door_1_57_1", 345, Location(1, -8, 4, 6, 0, 9));
					};

					Door door_1_57(1);
					setupDoor(&door_1_57, "door_1_57", onOpen_1_57, onClose_1_57);
					break;
				}
				case 3: {
					// World 1 Door 41
					auto onClose_1_41 = []() {
						closeDoor("door_1_41_1", 165, Location(1, -16, 0, 7, 0, 8));
						closeDoor("door_1_41_2", 165, Location(1, -16, 0, 3, 0, 11));
						worldManager->getTileGroup(Location(1, -16, 0, 8, 0, 6))->addTile(197);
						worldManager->getTileGroup(Location(1, -16, 0, 8, 0, 6))->removeTile(
								198);
					};

					auto onTimerCompletion_1_41 = [onClose_1_41]() {
						doorProvider->getDoor("door_1_41")->lock(0);
					};

					auto onOpen_1_41 = [onTimerCompletion_1_41]() {
						openDoor("door_1_41_1", 165, Location(1, -16, 0, 7, 0, 8));
						openDoor("door_1_41_2", 165, Location(1, -16, 0, 3, 0, 11));
						worldManager->getTileGroup(Location(1, -16, 0, 8, 0, 6))->addTile(198);
						worldManager->getTileGroup(Location(1, -16, 0, 8, 0, 6))->removeTile(
								197);
						LockTimer* lockTimer = new LockTimer(4000, TimerColour::BLUE, onTimerCompletion_1_41);
						lockTimer->start();
					};

					Door door_1_41(1);
					setupDoor(&door_1_41, "door_1_41", onOpen_1_41, onClose_1_41);
					setupTimedDoorSensor(Location(1, -16, 0, 8, 0, 5), "door_1_41",
									"door_1_41 sensor_0", 9, 0);

					// World 1 Door 42
					auto onClose_1_42 = []() {
						closeDoor("door_1_42_1", 169, Location(1, -16, 0, 7, 0, 11));
						closeDoor("door_1_42_2", 169, Location(1, -16, 0, 3, 0, 8));
						worldManager->getTileGroup(Location(1, -16, 0, 2, 0, 6))->addTile(207);
						worldManager->getTileGroup(Location(1, -16, 0, 2, 0, 6))->removeTile(
								208);
					};

					auto onTimerCompletion_1_42 = [onClose_1_42]() {
						doorProvider->getDoor("door_1_42")->lock(0);
					};

					auto onOpen_1_42 = [onTimerCompletion_1_42]() {
						openDoor("door_1_42_1", 169, Location(1, -16, 0, 7, 0, 11));
						openDoor("door_1_42_2", 169, Location(1, -16, 0, 3, 0, 8));
						worldManager->getTileGroup(Location(1, -16, 0, 2, 0, 6))->addTile(208);
						worldManager->getTileGroup(Location(1, -16, 0, 2, 0, 6))->removeTile(
								207);
						LockTimer* lockTimer = new LockTimer(4000, TimerColour::GREEN, onTimerCompletion_1_42);
						lockTimer->start();
					};

					Door door_1_42(1);
					setupDoor(&door_1_42, "door_1_42", onOpen_1_42, onClose_1_42);
					setupTimedDoorSensor(Location(1, -16, 0, 2, 0, 5), "door_1_42",
									"door_1_42 sensor_1", 9, 0);

					// World 1 Door 43
					auto onClose_1_43 = []() {
						closeDoor("door_1_43_1", 173, Location(1, -16, 0, 5, 0, 14));
						worldManager->getTileGroup(Location(1, -16, 0, 7, 0, 14))->addTile(217);
						worldManager->getTileGroup(Location(1, -16, 0, 7, 0, 14))->removeTile(
								218);
						worldManager->getTileGroup(Location(1, -16, 0, 3, 0, 14))->addTile(217);
						worldManager->getTileGroup(Location(1, -16, 0, 3, 0, 14))->removeTile(
								218);
					};

					auto onOpen_1_43 = []() {
						openDoor("door_1_43_1", 173, Location(1, -16, 0, 5, 0, 14));
						worldManager->getTileGroup(Location(1, -16, 0, 7, 0, 14))->addTile(218);
						worldManager->getTileGroup(Location(1, -16, 0, 7, 0, 14))->removeTile(
								217);
						worldManager->getTileGroup(Location(1, -16, 0, 3, 0, 14))->addTile(218);
						worldManager->getTileGroup(Location(1, -16, 0, 3, 0, 14))->removeTile(
								217);
					};

					Door door_1_43(2);
					setupDoor(&door_1_43, "door_1_43", onOpen_1_43, onClose_1_43);
					setupDoorSensor(Location(1, -16, 0, 7, 0, 13), "door_1_43",
									"door_1_43 sensor_0", 9, 0);
					setupDoorSensor(Location(1, -16, 0, 3, 0, 13), "door_1_43",
									"door_1_43 sensor_1", 9, 1);

					// World 1 Door 44
					auto onClose_1_44 = []() {
						closeDoor("door_1_44_1", 173, Location(1, -17, 0, 13, 0, 10));
						worldManager->getTileGroup(Location(1, -17, 0, 15, 0, 15))->addTile(225);
						worldManager->getTileGroup(Location(1, -17, 0, 15, 0, 15))->removeTile(
								226);
					};

					auto onTimerCompletion_1_44 = [onClose_1_44]() {
						doorProvider->getDoor("door_1_44")->lock(0);
					};

					auto onOpen_1_44 = [onTimerCompletion_1_44]() {
						openDoor("door_1_44_1", 173, Location(1, -17, 0, 13, 0, 10));
						worldManager->getTileGroup(Location(1, -17, 0, 15, 0, 15))->addTile(226);
						worldManager->getTileGroup(Location(1, -17, 0, 15, 0, 15))->removeTile(
								225);
						LockTimer* lockTimer = new LockTimer(13000, TimerColour::YELLOW, onTimerCompletion_1_44);
						lockTimer->start();
					};

					Door door_1_44(1);
					setupDoor(&door_1_44, "door_1_44", onOpen_1_44, onClose_1_44);
					setupTimedDoorSensor(Location(1, -17, 0, 15, 0, 14), "door_1_44",
									"door_1_44 sensor_0", 13, 0);

					// World 1 Door 45
					auto onClose_1_45 = []() {
						closeDoor("door_1_45_1", 177, Location(1, -17, 0, 13, 0, 7));
						worldManager->getTileGroup(Location(1, -17, 0, 11, 0, 10))->addTile(231);
						worldManager->getTileGroup(Location(1, -17, 0, 11, 0, 10))->removeTile(
								232);
					};

					auto onTimerCompletion_1_45 = [onClose_1_45]() {
						doorProvider->getDoor("door_1_45")->lock(0);
					};

					auto onOpen_1_45 = [onTimerCompletion_1_45]() {
						openDoor("door_1_45_1", 177, Location(1, -17, 0, 13, 0, 7));
						worldManager->getTileGroup(Location(1, -17, 0, 11, 0, 10))->addTile(232);
						worldManager->getTileGroup(Location(1, -17, 0, 11, 0, 10))->removeTile(
								231);
						LockTimer* lockTimer = new LockTimer(13000, TimerColour::ORANGE, onTimerCompletion_1_45);
						lockTimer->start();
					};

					Door door_1_45(1);
					setupDoor(&door_1_45, "door_1_45", onOpen_1_45, onClose_1_45);
					setupTimedDoorSensor(Location(1, -17, 0, 11, 0, 9), "door_1_45",
									"door_1_45 sensor_0", 11, 0);

					// World 1 Door 46
					auto onClose_1_46 = []() {
						closeDoor("door_1_46_1", 185, Location(1, -22, 0, 2, 0, 14));
						worldManager->getTileGroup(Location(1, -21, 0, 8, 0, 12))->addTile(251);
						worldManager->getTileGroup(Location(1, -21, 0, 8, 0, 12))->removeTile(
								252);
					};

					auto onTimerCompletion_1_46 = [onClose_1_46]() {
						doorProvider->getDoor("door_1_46")->lock(0);
					};

					auto onOpen_1_46 = [onTimerCompletion_1_46]() {
						openDoor("door_1_46_1", 185, Location(1, -22, 0, 2, 0, 14));
						worldManager->getTileGroup(Location(1, -21, 0, 8, 0, 12))->addTile(252);
						worldManager->getTileGroup(Location(1, -21, 0, 8, 0, 12))->removeTile(
								251);
						LockTimer* lockTimer = new LockTimer(5000, TimerColour::RED, onTimerCompletion_1_46);
						lockTimer->start();
					};

					Door door_1_46(1);
					setupDoor(&door_1_46, "door_1_46", onOpen_1_46, onClose_1_46);
					setupTimedDoorSensor(Location(1, -21, 0, 8, 0, 11), "door_1_46",
									"door_1_46 sensor_0", 11, 0);

					// World 1 Door 47
					auto onClose_1_47 = []() {
						closeDoor("door_1_47_1", 177, Location(1, -22, 2, 4, 0, 15));
						closeDoor("door_1_47_2", 177, Location(1, -22, 2, 4, 0, 12));
						worldManager->getTileGroup(Location(1, -22, 3, 8, 0, 4))->addTile(231);
						worldManager->getTileGroup(Location(1, -22, 3, 8, 0, 4))->removeTile(
								232);
					};

					auto onTimerCompletion_1_47 = [onClose_1_47]() {
						doorProvider->getDoor("door_1_47")->lock(0);
					};

					auto onOpen_1_47 = [onTimerCompletion_1_47]() {
						openDoor("door_1_47_1", 177, Location(1, -22, 2, 4, 0, 15));
						openDoor("door_1_47_2", 177, Location(1, -22, 2, 4, 0, 12));
						worldManager->getTileGroup(Location(1, -22, 3, 8, 0, 4))->addTile(232);
						worldManager->getTileGroup(Location(1, -22, 3, 8, 0, 4))->removeTile(
								231);
						LockTimer* lockTimer = new LockTimer(5000, TimerColour::ORANGE, onTimerCompletion_1_47);
						lockTimer->start();
					};

					Door door_1_47(1);
					setupDoor(&door_1_47, "door_1_47", onOpen_1_47, onClose_1_47);
					setupTimedDoorSensor(Location(1, -22, 3, 8, 0, 3), "door_1_47",
									"door_1_47 sensor_0", 11, 0);

					// World 1 Door 48
					auto onOpen_1_48 = []() {
						openDoor("door_1_48_1", 165, Location(1, -22, 2, 1, 0, 12));
						worldManager->getTileGroup(Location(1, -22, 2, 5, 0, 12))->removeTile(
								197);
						worldManager->getTileGroup(Location(1, -22, 2, 5, 0, 12))->addTile(198);
					};

					auto onClose_1_48 = []() {
						closeDoor("door_1_48_1", 165, Location(1, -22, 2, 1, 0, 12));
						worldManager->getTileGroup(Location(1, -22, 2, 5, 0, 12))->removeTile(
								198);
						worldManager->getTileGroup(Location(1, -22, 2, 5, 0, 12))->addTile(197);
					};

					Door door_1_48(1);
					setupDoor(&door_1_48, "door_1_48", onOpen_1_48, onClose_1_48);
					setupDoorSensor(Location(1, -22, 2, 5, 0, 11), "door_1_48",
									"door_1_48 sensor_0", 9, 0);

					// World 1 Door 49
					auto onOpen_1_49 = []() {
						openDoor("door_1_49_1", 169, Location(1, -22, 2, 1, 0, 15));
						worldManager->getTileGroup(Location(1, -22, 2, 5, 0, 15))->removeTile(
								207);
						worldManager->getTileGroup(Location(1, -22, 2, 5, 0, 15))->addTile(208);
					};

					auto onClose_1_49 = []() {
						closeDoor("door_1_49_1", 169, Location(1, -22, 2, 1, 0, 15));
						worldManager->getTileGroup(Location(1, -22, 2, 5, 0, 15))->removeTile(
								208);
						worldManager->getTileGroup(Location(1, -22, 2, 5, 0, 15))->addTile(207);
					};

					Door door_1_49(1);
					setupDoor(&door_1_49, "door_1_49", onOpen_1_49, onClose_1_49);
					setupDoorSensor(Location(1, -22, 2, 5, 0, 14), "door_1_49",
									"door_1_49 sensor_0", 9, 0);

					// World 1 Door 50
					auto onOpen_1_50 = []() {
						openDoor("door_1_50_1", 181, Location(1, -22, 3, 4, 0, 2));
						worldManager->getTileGroup(Location(1, -22, 3, 5, 0, 2))->removeTile(
								241);
						worldManager->getTileGroup(Location(1, -22, 3, 5, 0, 2))->addTile(242);
					};

					auto onClose_1_50 = []() {
						closeDoor("door_1_50_1", 181, Location(1, -22, 3, 4, 0, 2));
						worldManager->getTileGroup(Location(1, -22, 3, 5, 0, 2))->removeTile(
								242);
						worldManager->getTileGroup(Location(1, -22, 3, 5, 0, 2))->addTile(241);
					};

					Door door_1_50(1);
					setupDoor(&door_1_50, "door_1_50", onOpen_1_50, onClose_1_50);
					setupDoorSensor(Location(1, -22, 3, 5, 0, 1), "door_1_50",
									"door_1_50 sensor_0", 11, 0);

					// World 1 Door 51
					auto onOpen_1_51 = []() {
						openDoor("door_1_51_1", 173, Location(1, -22, 3, 1, 0, 5));
						worldManager->getTileGroup(Location(1, -22, 3, 2, 0, 5))->removeTile(
								217);
						worldManager->getTileGroup(Location(1, -22, 3, 2, 0, 5))->addTile(218);
					};

					auto onClose_1_51 = []() {
						closeDoor("door_1_51_1", 173, Location(1, -22, 3, 1, 0, 5));
						worldManager->getTileGroup(Location(1, -22, 3, 2, 0, 5))->removeTile(
								218);
						worldManager->getTileGroup(Location(1, -22, 3, 2, 0, 5))->addTile(217);
					};

					Door door_1_51(1);
					setupDoor(&door_1_51, "door_1_51", onOpen_1_51, onClose_1_51);
					setupDoorSensor(Location(1, -22, 3, 2, 0, 4), "door_1_51",
									"door_1_51 sensor_0", 9, 0);

					// World 1 Door 52
					auto onOpen_1_52 = []() {
						openDoor("door_1_52_1", 189, Location(1, -22, 4, 1, 0, 2));
						worldManager->getTileGroup(Location(1, -22, 3, 8, 0, 12))->removeTile(
								257);
						worldManager->getTileGroup(Location(1, -22, 3, 8, 0, 12))->addTile(258);
						worldManager->getTileGroup(Location(1, -22, 3, 4, 0, 12))->removeTile(
								259);
						worldManager->getTileGroup(Location(1, -22, 3, 4, 0, 12))->addTile(260);
					};

					auto onClose_1_52 = []() {
						closeDoor("door_1_52_1", 189, Location(1, -22, 4, 1, 0, 2));
						worldManager->getTileGroup(Location(1, -22, 3, 8, 0, 12))->removeTile(
								258);
						worldManager->getTileGroup(Location(1, -22, 3, 8, 0, 12))->addTile(257);
						worldManager->getTileGroup(Location(1, -22, 3, 4, 0, 12))->removeTile(
								260);
						worldManager->getTileGroup(Location(1, -22, 3, 4, 0, 12))->addTile(259);
					};

					Door door_1_52(2);
					setupDoor(&door_1_52, "door_1_52", onOpen_1_52, onClose_1_52);
					setupDoorSensor(Location(1, -22, 3, 8, 0, 11), "door_1_52",
									"door_1_52 sensor_0", 9, 0);
					setupDoorSensor(Location(1, -22, 3, 4, 0, 11), "door_1_52",
									"door_1_52 sensor_1", 10, 1);

					// World 1 Door 53
					auto onOpen_1_53 = []() {
						openDoor("door_1_53_1", 185, Location(1, -22, 3, 6, 0, 15));
						closeDoor("door_1_53_2", 185, Location(1, -22, 3, 1, 0, 15));
						worldManager->getTileGroup(Location(1, -22, 4, 6, 0, 2))->addTile(248);
						worldManager->getTileGroup(Location(1, -22, 4, 6, 0, 2))->removeTile(
								247);
						worldManager->getTileGroup(Location(1, -22, 4, 6, 1, 2))->addTile(256);
						worldManager->getTileGroup(Location(1, -22, 4, 6, 1, 2))->removeTile(
								255);
					};

					auto onClose_1_53 = []() {
						closeDoor("door_1_53_1", 185, Location(1, -22, 3, 6, 0, 15));
						openDoor("door_1_53_2", 185, Location(1, -22, 3, 1, 0, 15));
						worldManager->getTileGroup(Location(1, -22, 4, 6, 0, 2))->addTile(247);
						worldManager->getTileGroup(Location(1, -22, 4, 6, 0, 2))->removeTile(
								248);
						worldManager->getTileGroup(Location(1, -22, 4, 6, 1, 2))->addTile(255);
						worldManager->getTileGroup(Location(1, -22, 4, 6, 1, 2))->removeTile(
								256);
					};

					Door door_1_53({false});
					setupDoor(&door_1_53, "door_1_53", onOpen_1_53, onClose_1_53);
					setupMultiTypeDoorSensor(Location(1, -22, 4, 6, 0, 1), "door_1_53",
											 "door_1_53 sensor_1", {9, 13}, 0);


					// World 1 Door 54
					auto onOpen_1_54 = []() {
						openDoor("door_1_54_1", 169, Location(1, -22, 5, 5, 0, 4));
						worldManager->getTileGroup(Location(1, -22, 4, 8, 0, 15))->removeTile(
								207);
						worldManager->getTileGroup(Location(1, -22, 4, 8, 0, 15))->addTile(208);
						worldManager->getTileGroup(Location(1, -22, 4, 2, 0, 15))->removeTile(
								207);
						worldManager->getTileGroup(Location(1, -22, 4, 2, 0, 15))->addTile(208);
					};

					auto onClose_1_54 = []() {
						closeDoor("door_1_54_1", 169, Location(1, -22, 5, 5, 0, 4));
						worldManager->getTileGroup(Location(1, -22, 4, 8, 0, 15))->removeTile(
								208);
						worldManager->getTileGroup(Location(1, -22, 4, 8, 0, 15))->addTile(207);
						worldManager->getTileGroup(Location(1, -22, 4, 2, 0, 15))->removeTile(
								208);
						worldManager->getTileGroup(Location(1, -22, 4, 2, 0, 15))->addTile(207);
					};

					Door door_1_54(2);
					setupDoor(&door_1_54, "door_1_54", onOpen_1_54, onClose_1_54);
					setupDoorSensor(Location(1, -22, 4, 8, 0, 14), "door_1_54",
									"door_1_54 sensor_0", 9, 0);
					setupDoorSensor(Location(1, -22, 4, 2, 0, 14), "door_1_54",
									"door_1_54 sensor_1", 9, 1);

					// World 1 Door 55
					auto onClose_1_55 = []() {
						closeDoor("door_1_55_1", 165, Location(1, -22, 4, 5, 0, 15));
						worldManager->getTileGroup(Location(1, -22, 5, 8, 0, 4))->addTile(205);
						worldManager->getTileGroup(Location(1, -22, 5, 8, 0, 4))->removeTile(
								206);
						worldManager->getTileGroup(Location(1, -22, 5, 2, 0, 4))->addTile(197);
						worldManager->getTileGroup(Location(1, -22, 5, 2, 0, 4))->removeTile(
								198);
					};

					auto onTimerCompletion_1_55 = [onClose_1_55]() {
						lockTimedDoorSensorIfNecessary(Location(1, -22, 5, 8, 0, 3), "door_1_55", 13, 0);
						lockTimedDoorSensorIfNecessary(Location(1, -22, 5, 2, 0, 3), "door_1_55", 9, 1);
						/*doorProvider->getDoor("door_1_55")->lock(0);
						doorProvider->getDoor("door_1_55")->lock(1);*/
					};

					auto onOpen_1_55 = [onTimerCompletion_1_55]() {
						openDoor("door_1_55_1", 165, Location(1, -22, 4, 5, 0, 15));
						worldManager->getTileGroup(Location(1, -22, 5, 8, 0, 4))->addTile(206);
						worldManager->getTileGroup(Location(1, -22, 5, 8, 0, 4))->removeTile(
								205);
						worldManager->getTileGroup(Location(1, -22, 5, 2, 0, 4))->addTile(198);
						worldManager->getTileGroup(Location(1, -22, 5, 2, 0, 4))->removeTile(
								197);
						LockTimer* lockTimer = new LockTimer(8200, TimerColour::BLUE, onTimerCompletion_1_55);
						lockTimer->start();
					};

					Door door_1_55(2);
					setupDoor(&door_1_55, "door_1_55", onOpen_1_55, onClose_1_55);
					setupTimedDoorSensor(Location(1, -22, 5, 8, 0, 3), "door_1_55",
									"door_1_55 sensor_0", 13, 0);
					setupTimedDoorSensor(Location(1, -22, 5, 2, 0, 3), "door_1_55",
									"door_1_55 sensor_1", 9, 1);
					break;
				}
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void Scripts::setupCheckPoints(int stage, int level) {
	TileDirection right = TileDirection::RIGHT;
	TileDirection forward = TileDirection::FORWARD;
	TileDirection left = TileDirection::LEFT;
	TileDirection backward = TileDirection::BACKWARD;

	switch (stage) {
		case 1:
			switch (level) {
				case 1:
					setupCheckPointWithBothSaveSigns("check_point_1_0", Location(1, -1, 0, 10, 0, 8),
													 Location(1, -1, 0, 11, 0, 8), Location(1, -1, 0, 9, 0, 8), forward);
					setupCheckPointWithLeftSaveSign("check_point_1_1", Location(1, -2, 0, 5, 0, 7),
													Location(1, -2, 0, 6, 0, 7), forward);
					setupCheckPointWithLeftSaveSign("check_point_1_2", Location(1, -3, 0, 9, 0, 7),
													Location(1, -3, 0, 10, 0, 7), forward);
					setupCheckPointWithLeftSaveSign("check_point_1_3", Location(1, -4, 0, 13, 0, 7),
													Location(1, -4, 0, 14, 0, 7), forward);
					setupCheckPointWithLeftSaveSign("check_point_1_4", Location(1, -2, 0, 5, 0, 7),
													Location(1, -2, 0, 6, 0, 7), forward);
					break;
				case 2:
					setupCheckPointWithBothSaveSigns("check_point_1_5", Location(1, -7, 1, 1, 0, 1),
													 Location(1, -7, 1, 2, 0, 1), Location(1, -7, 1, 0, 0, 1), forward);
					setupCheckPointWithBothSaveSigns("check_point_1_6", Location(1, -8, 2, 9, 0, 15),
													 Location(1, -8, 2, 10, 0, 15), Location(1, -8, 2, 8, 0, 15),
													 forward);
					setupCheckPointWithLeftSaveSign("check_point_1_7", Location(1, -11, 3, 13, 0, 9),
													Location(1, -11, 3, 14, 0, 9), forward);
					setupCheckPointWithBothSaveSigns("check_point_1_8", Location(1, -12, 3, 4, 0, 6),
													 Location(1, -12, 3, 5, 0, 6), Location(1, -12, 3, 3, 0, 6),
													 backward);
					setupCheckPointWithBothSaveSigns("check_point_1_8_1", Location(1, -11, 3, 0, 0, 0),
													 Location(1, -11, 3, 1, 0, 0), Location(1, -12, 3, 15, 0, 0),
													 backward);
					setupCheckPointWithBothSaveSigns("check_point_1_9", Location(1, -12, 2, 15, 0, 11),
													 Location(1, -11, 2, 0, 0, 11), Location(1, -12, 2, 14, 0, 11),
													 forward);
					setupCheckPointWithBothSaveSigns("check_point_1_10", Location(1, -15, 0, 1, 0, 14),
													 Location(1, -15, 0, 2, 0, 14), Location(1, -15, 0, 0, 0, 14),
													 forward);
					setupCheckPointWithLeftSaveSign("check_point_1_10_1", Location(1, -12, 3, 13, 0, 14),
													 Location(1, -12, 3, 14, 0, 14), forward);
					break;
				case 3:
					setupCheckPointWithBothSaveSigns("check_point_1_11", Location(1, -15, 0, 1, 0, 14),
													 Location(1, -15, 0, 2, 0, 14), Location(1, -15, 0, 0, 0, 14),
													 forward);
					setupCheckPointWithBothSaveSigns("check_point_1_12", Location(1, -16, 1, 5, 0, 0),
													 Location(1, -16, 1, 6, 0, 0), Location(1, -16, 1, 4, 0, 0),
													 forward);
					setupCheckPointWithBothSaveSigns("check_point_1_13", Location(1, -17, 0, 7, 0, 7),
													 Location(1, -17, 0, 8, 0, 7), Location(1, -17, 0, 6, 0, 7),
													 forward);
					// setupCheckPointWithLeftSaveSign("check_point_1_13", Location(1, -17, 3, 6, 0, 4),
					// 								Location(1, -17, 3, 7, 0, 4), forward);
					setupCheckPointWithBothSaveSigns("check_point_1_14", Location(1, -21, 0, 11, 0, 12),
													 Location(1, -21, 0, 12, 0, 12), Location(1, -21, 0, 10, 0, 12),
													 backward);
					setupCheckPointWithRightSaveSign("check_point_1_15", Location(1, -22, 1, 1, 0, 2),
													Location(1, -22, 1, 0, 0, 2), forward);
					setupCheckPointWithRightSaveSign("check_point_1_16", Location(1, -22, 3, 1, 0, 5),
													Location(1, -22, 3, 0, 0, 5), forward);
					setupCheckPointWithBothSaveSigns("check_point_1_17", Location(1, -22, 4, 1, 0, 2),
													 Location(1, -22, 4, 2, 0, 2), Location(1, -22, 4, 0, 0, 2),
													 forward);
					setupCheckPointWithBothSaveSigns("check_point_1_18", Location(1, -22, 5, 5, 0, 4),
													 Location(1, -22, 5, 6, 0, 4), Location(1, -22, 5, 4, 0, 4),
													 forward);
					setupCheckPointWithLeftSaveSign("check_point_1_19", Location(1, -17, 3, 6, 0, 4),
													Location(1, -17, 3, 7, 0, 4), forward);

					setupCheckPointWithLeftSaveSign("check_point_1_20", Location(1, -22, 1, 1, 0, 15),
													Location(1, -22, 1, 2, 0, 15), forward);
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void Scripts::setupGeneralEvents(int stage, int level) {
	switch (stage) {
		case 1:
			switch (level) {
				case 1: {
					auto kickstarter_1 = []() {
						dialogueDisplay->setTimeBetweenCharacterReveals(26);
						player->getCharacter()->setTexture("Robert");
						player->getCharacter()->addTeleportToQueue(Location(0, 1, -1, 4.5f, 0, 15.5f), TeleportationAnimation::NONE);
						player->getCharacter()->addTurnToQueue(TileDirection::BACKWARD);
						hintDisplay->clear();

						auto event = []() {
							showBlinkingDialogue("Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Hi! I'm Robert, the developer of Neo-Zero.", "kickstarter_2");
						};

						timer->scheduleEventInSeconds(10, event);
					};

					scriptManager->addScript("kickstarter_1", kickstarter_1);
					worldManager->addWalkedOnScript("kickstarter_1", Location(0, 1, -1, 3, 0, 15));

					scriptManager->addScript("kickstarter_2", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Neo-Zero is a role-playing game for the personal computer!", "kickstarter_3"));
					scriptManager->addScript("kickstarter_3", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "The game takes place inside of a simulation that runs on a 1990s super-computer.", "kickstarter_4"));
					scriptManager->addScript("kickstarter_4", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "One very clingy government takes over the simulated world.", "kickstarter_5"));
					scriptManager->addScript("kickstarter_5", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "You play as Apollo, a scientist that the government wants to capture VERY BADLY.", "kickstarter_6"));
					scriptManager->addScript("kickstarter_6", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Why don't we take a look at what you, Apollo, will be doing in this dystopia?", "kickstarter_7"));

					auto kickstarter_7 = []() {
						hideDialogue();
					};

					scriptManager->addScript("kickstarter_7", kickstarter_7);

					/*auto ferdinand_1 = []() {
						dialogueDisplay->resetTimeBetweenCharacterReveals();
						player->getCharacter()->addTurnToQueue(TileDirection::FORWARD);
						hintDisplay->clear();

						auto event = []() {
							showBlinkingDialogue("Ferdinand", "avatars_2/2/4.png", "avatars_2/2/5.png", "A dead end... whatever shall we do?", "ferdinand_2");
						};

						timer->scheduleEventInSeconds(10, event);
					};

					scriptManager->addScript("ferdinand_1", ferdinand_1);
					worldManager->addWalkedOnScript("ferdinand_1", Location(0, -2, 0, 14, 0, 4));

					scriptManager->addScript("ferdinand_2", std::bind(&showBlinkingDialogue, "Ferdinand", "avatars_2/2/4.png", "avatars_2/2/5.png", "Wait, I have an idea!", "hide_dialogue"));*/

	
					auto dialogue_0_1 = []() {
						player->lookAtWatch();
						showBlinkingDialogue("Aela", "avatars_1/2/0.png", "avatars_1/2/1.png", "Ah, there you are! You must have fallen asleep.", "dialogue_0_2");
						hintDisplay->clear();
					};

					scriptManager->addScript("dialogue_0_1", dialogue_0_1);
					scriptManager->addScript("dialogue_0_2", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Too tired from work, huh? I can't blame you.", "dialogue_0_3"));
					scriptManager->addScript("dialogue_0_3", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/2/2.png", "avatars_1/2/3.png", "Actually, you don't look tired... you look a little distressed.", "dialogue_0_4"));
					scriptManager->addScript("dialogue_0_4", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/2/0.png", "avatars_1/2/1.png", "Well, cheer up! You won't have to do too much work today, I promise.", "dialogue_0_5"));
					scriptManager->addScript("dialogue_0_5", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Anyways, welcome to the Oxylabs Testing Facility!", "dialogue_0_6"));
					scriptManager->addScript("dialogue_0_6", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "My name is Aela! I will be your designated assistant throughout your tests.", "dialogue_0_7"));
					scriptManager->addScript("dialogue_0_7", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "You will be testing Oxylabs' latest innovation: The Propulsion Gun.", "dialogue_0_8"));
					scriptManager->addScript("dialogue_0_8", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Unfortunately, according to new protocols, testing can no longer be outsourced.", "dialogue_0_9"));
					scriptManager->addScript("dialogue_0_9", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/4/0.png", "avatars_1/4/1.png", "This means that while our \"more intelligent\" higher ups look for testers to hire...", "dialogue_0_10"));
					scriptManager->addScript("dialogue_0_10", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "Us scientists are now responsible for testing each others' projects.", "dialogue_0_11"));
					scriptManager->addScript("dialogue_0_11", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/2/0.png", "avatars_1/2/1.png", "Only our management could be this moronic, right?", "dialogue_0_12"));
					scriptManager->addScript("dialogue_0_12", std::bind(&showDialogue, "Aela", "avatars_1/1/2.png", "It says here that you, " + PLAYER_NAME + ", are a computer engineer working on Project Foxtrot.", "dialogue_0_13"));
					scriptManager->addScript("dialogue_0_13", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "I am actually a mechanical engineer who was also working on a different project.", "dialogue_0_14"));
					scriptManager->addScript("dialogue_0_14", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/4/0.png", "avatars_1/4/1.png", "Nevertheless, we've been forced to waste our time on the testing of the Propulsion Gun!", "dialogue_0_15"));

					auto dialogue_0_15 = []() {
						player->getCharacter()->allowNewMovements(false);
						hideDialogue();
						player->stopLookingAtWatch();

						auto event = []() {
							stopAnimatingElevator();
						};

						timer->scheduleEventInMillis(2000, event);

						auto event2 = []() {
							showBlinkingDialogue("Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Looks like your elevator has arrived.", "dialogue_0_16");
							player->lookAtWatch();
						};

						timer->scheduleEventInMillis(3200, event2);
					};
					scriptManager->addScript("dialogue_0_15", dialogue_0_15);
					scriptManager->addScript("dialogue_0_16", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Once you leave the elevator, please walk to the kiosk marked with an \"i\".", "dialogue_0_17"));

					auto dialogue_0_17 = []() {
						dialogueDisplay->closeDialog();
						hintDisplay->displayHint("You can use W, A, S and D to walk!", HintDisplayDuration::FOREVER);
						hintDisplay->displayMostRecentHintUntilKeyPresses({SDLK_w, SDLK_a, SDLK_s, SDLK_d});
						player->stopLookingAtWatch();
						player->getCharacter()->allowNewMovements(true);
					};

					scriptManager->addScript("dialogue_0_17", dialogue_0_17);

					auto walked_on_0_0 = []() {
						fadeTeleportPlayer(Location(1, 0, 0, 14.5f, 0, 6.5f));
						hintDisplay->clear();
						worldManager->removeWalkedOnScript(Location(0, 0, -1, 1, 0, 15));
						gameAudioPlayer->loopStreamInfinitely("Eight Sixes", 125450, 4000);
					};

					scriptManager->addScript("walked_on_0_0", walked_on_0_0);
					worldManager->addWalkedOnScript("walked_on_0_0", Location(0, 0, -1, 1, 0, 15));

					auto walked_on_0_1 = []() {
						worldManager->removeWalkedOnScript(Location(0, 0, -1, 1, 0, 15));
						hintDisplay->displayHint("You can interact with objects, such as kiosks.", HintDisplayDuration::LONG);
						hintDisplay->displayHint("Press ENTER while facing an object to interact with it!", HintDisplayDuration::FOREVER);
					};

					scriptManager->addScript("walked_on_0_1", walked_on_0_1);
					worldManager->addWalkedOnScript("walked_on_0_1", Location(1, 0, 0, 14, 0, 6));

					auto dialogue_1_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, 0, 0, 10, 0, 6));
							hintDisplay->clear();
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "WELCOME TO LEVEL 1 OF THE PROPULSION GUN TESTING FACILITY.", "dialogue_1_1");
							// dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a ", "dialogue_1_1");
						}

						// game->startBattle("battle_1");
					};

					scriptManager->addScript("dialogue_1_0", dialogue_1_0);
					worldManager->addPromptedScript("dialogue_1_0", Location(1, 0, 0, 10, 0, 6));

					scriptManager->addScript("dialogue_1_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "THE PROPULSION GUN IS ALL ABOUT THE HIGH-SPEED TRANSPORTATION OF MATTER.", "dialogue_1_2"));
					scriptManager->addScript("dialogue_1_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "YOUR TESTING WILL REVOLVE AROUND SEVERAL MATTER-PROPULSION DEVICES.", "dialogue_1_3"));
					scriptManager->addScript("dialogue_1_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "HOWEVER, THE FOCUS OF YOUR TESTS WILL BE THE PROPULSION GUN.", "dialogue_1_4"));
					scriptManager->addScript("dialogue_1_4", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "YOUR PROPULSION GUN HAS BEEN DESIGNED TO PICK UP MATTER BELOW YOUR WAIST.", "dialogue_1_5"));
					scriptManager->addScript("dialogue_1_5", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "FOR EXAMPLE, YOU CAN USE YOUR PROPULSION GUN ON THE FLOOR IN FRONT OF YOU.", "dialogue_1_6"));

					auto dialogue_1_6 = []() {
						if (!aelaTalkedToPlayerInDialogue1) {
							game->getTileSwitchGun()->setActive(true);
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "HERE IS YOUR PROPULSION GUN.", "dialogue_1_7");
							tileInventoryDisplay->show();
							player->showAndEnableTileGun();
						} else {
							scriptManager->runScript("dialogue_1_7");
						}
					};

					scriptManager->addScript("dialogue_1_6", dialogue_1_6);

					auto dialogue_1_7 = []() {
						player->lookAtWatch();
						turnDisplayOff(Location(1, 0, 0, 10, 0, 6));
						if (!aelaTalkedToPlayerInDialogue1) {
							aelaTalkedToPlayerInDialogue1 = true;
							dialogueDisplay->showDialogue("Aela", "avatars_1/2/0.png", "avatars_1/2/1.png", PLAYER_NAME + "!", "dialogue_1_8");
						} else {
							dialogueDisplay->showDialogue("Aela", "avatars_1/1/0.png", "avatars_1/2/1.png", "Are you still wondering what to do, " + PLAYER_NAME + "?", "dialogue_1_12");
						}
					};

					scriptManager->addScript("dialogue_1_7", dialogue_1_7);

					scriptManager->addScript("dialogue_1_8", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Your propulsion gun has three slots, and each slot can hold a bit of matter.", "dialogue_1_9"));
					scriptManager->addScript("dialogue_1_9", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Currently, your three slots contain flooring.", "dialogue_1_10"));
					scriptManager->addScript("dialogue_1_10", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "When you use the gun, you replace matter in front of you with the matter in the slot.", "dialogue_1_11"));
					scriptManager->addScript("dialogue_1_11", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/2/0.png", "avatars_1/2/1.png", "It's actually really simple stuff, so don't panic!", "dialogue_1_12"));
					scriptManager->addScript("dialogue_1_12", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Try using your gun to make a bridge across the water to your right!", "dialogue_1_13"));

					auto dialogue_1_13 = []() {
						hideDialogue();
						player->stopLookingAtWatch();
						hintDisplay->displayHint("Press BACKSLASH to use the Propulsion Gun.", HintDisplayDuration::UNTIL_KEY);
						hintDisplay->displayMostRecentHintUntilKeyPress(SDLK_BACKSLASH);
						hintDisplay->displayHint("Use the LEFT and RIGHT arrow keys to select a slot.", HintDisplayDuration::FOREVER);
						hintDisplay->displayMostRecentHintUntilKeyPresses({SDLK_LEFT, SDLK_RIGHT});
					};

					scriptManager->addScript("dialogue_1_13", dialogue_1_13);

					auto hint_3_0 = []() {
						hintDisplay->displayHint("You can hold left shift to walk more slowly.", HintDisplayDuration::LONG);
						hintDisplay->displayMostRecentHintUntilKeyPress(SDLK_LSHIFT);
						worldManager->removeWalkedOnScript(Location(1, -2, 0, 15, 0, 9));
					};

					scriptManager->addScript("hint_3_0", hint_3_0);
					worldManager->addWalkedOnScript("hint_3_0", Location(1, -2, 0, 15, 0, 9));

					auto hint_6_0 = []() {
						hintDisplay->displayHint("Pressing 'R' will reset the game to the last save point.", HintDisplayDuration::LONG);
						hintDisplay->displayHint("If you make a mistake in a puzzle, pressing 'R' will reset it.", HintDisplayDuration::LONG);
						worldManager->removeWalkedOnScript(Location(1, -2, 0, 10, 0, 1));
					};

					scriptManager->addScript("hint_6_0", hint_6_0);
					worldManager->addWalkedOnScript("hint_6_0", Location(1, -2, 0, 10, 0, 1));

					auto hint_4_0 = []() {
						hintDisplay->displayHint("To open a door, place the required tile in front of the arrow.", HintDisplayDuration::FOREVER);
						worldManager->removeWalkedOnScript(Location(1, -1, 0, 11, 0, 1));
					};

					scriptManager->addScript("hint_4_0", hint_4_0);
					worldManager->addWalkedOnScript("hint_4_0", Location(1, -1, 0, 11, 0, 1));

					auto hint_1_0 = []() {
						hintDisplay->displayHint("Everytime you pass a checkpoint, your progress is saved.", HintDisplayDuration::LONG);
						worldManager->removeWalkedOnScript(Location(1, -1, 0, 10, 0, 10));
					};

					scriptManager->addScript("hint_1_0", hint_1_0);
					worldManager->addWalkedOnScript("hint_1_0", Location(1, -1, 0, 10, 0, 10));

					auto hint_7_0 = []() {
						hintDisplay->displayHint("Reminder: press R to restart a puzzle.", HintDisplayDuration::LONG);
						worldManager->removeWalkedOnScript(Location(1, -3, 0, 9, 0, 7));
					};

					scriptManager->addScript("hint_7_0", hint_7_0);
					worldManager->addWalkedOnScript("hint_7_0", Location(1, -3, 0, 9, 0, 7));


					auto dialogue_2_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, -3, 0, 15, 0, 5));
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "DID YOU KNOW?", "dialogue_2_1");
						}
					};

					scriptManager->addScript("dialogue_2_0", dialogue_2_0);
					worldManager->addPromptedScript("dialogue_2_0", Location(1, -3, 0, 15, 0, 5));

					scriptManager->addScript("dialogue_2_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "LAVA IS ACTUALLY HOT, MELTED ROCK!", "dialogue_2_2"));
					scriptManager->addScript("dialogue_2_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "WHEN LAVA INTERACTS WITH WATER, IT COOLS OFF AND BECOMES SOLID!", "dialogue_2_3"));

					auto dialogue_2_3 = []() {
						turnDisplayOff(Location(1, -3, 0, 15, 0, 5));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_2_3", dialogue_2_3);

					auto dialogue_3_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, -3, 0, 5, 0, 5));
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "DID YOU KNOW?", "dialogue_3_1");
						}
					};

					scriptManager->addScript("dialogue_3_0", dialogue_3_0);
					worldManager->addPromptedScript("dialogue_3_0", Location(1, -3, 0, 5, 0, 5));

					auto dialogue_3_1 = []() {
						// Insert that one animation here!
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "THE TWO MAIN POLLUTANTS OUTPUTTED BY OXYLABS ARE USUALLY TOXIC OR POISONOUS.", "dialogue_3_2");
					};

					scriptManager->addScript("dialogue_3_1", dialogue_3_1);

					scriptManager->addScript("dialogue_3_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "LUCKILY, OUR TOXIC WASTE AND POISONs ARE VERY UNREACTIVE.", "dialogue_3_3"));
					scriptManager->addScript("dialogue_3_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "OXYLABS TOXIC WASTE AND POISONS HAVE NO EFFECT WHEN MIXED WITH WATER OR LAVA.", "dialogue_3_4"));

					auto dialogue_3_4 = []() {
						turnDisplayOff(Location(1, -3, 0, 5, 0, 5));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_3_4", dialogue_3_4);

					// meme
					auto dialogue_41_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, -5, 0, 13, 0, 4));
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "DID YOU KNOW?", "dialogue_41_1");
						}
					};

					scriptManager->addScript("dialogue_41_0", dialogue_41_0);
					worldManager->addPromptedScript("dialogue_41_0", Location(1, -5, 0, 13, 0, 4));

					auto dialogue_41_1 = []() {
						// Insert that one animation here!
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "THE FOUNDER OF OXYLABS, DR ARTHUR WRIGHT, MIGHT HAVE BEEN AN ENTREPRENEUR.", "dialogue_41_2");
					};

					scriptManager->addScript("dialogue_41_1", dialogue_41_1);

					scriptManager->addScript("dialogue_41_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "HOWEVER, IN HIS HEART, HE WAS AN ENTHUSIASTIC SCIENTIST.", "dialogue_41_3"));
					scriptManager->addScript("dialogue_41_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "THE CURRENT PROJECT YOU ARE TESTING, THE PROPULSION GUN, WAS HIS LAST.", "dialogue_41_4"));
					scriptManager->addScript("dialogue_41_4", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "HIS DAUGHTER TOOK OVER AND HAS BEEN IN CHARGE FOR THE LAST 35 YEARS.", "dialogue_41_5"));

					auto dialogue_41_5 = []() {
						turnDisplayOff(Location(1, -5, 0, 13, 0, 4));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_41_5", dialogue_41_5);

					auto dialogue_4_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, -4, 0, 9, 0, 5));
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "DID YOU KNOW?", "dialogue_4_1");
						}
					};

					scriptManager->addScript("dialogue_4_0", dialogue_4_0);
					worldManager->addPromptedScript("dialogue_4_0", Location(1, -4, 0, 9, 0, 5));

					scriptManager->addScript("dialogue_4_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "THE PROPULSION GUN USES STATE OF THE ART TECHNOLOGY.", "dialogue_4_2"));
					scriptManager->addScript("dialogue_4_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "THIS TECHNOLOGY IS BASED ON MANY RECENT BREAKTHROUGHS IN PHYSICS,", "dialogue_4_3"));
					scriptManager->addScript("dialogue_4_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "ONE OF WHICH IS DR. READ'S EXPANSION OF SCHOPENHAUER'S THEORY OF THE POLYGON.", "dialogue_4_4"));

					auto dialogue_4_4 = []() {
						turnDisplayOff(Location(1, -4, 0, 9, 0, 5));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_4_4", dialogue_4_4);

					auto hint_1_1 = []() {
						hintDisplay->displayHint("You can press escape to pause the game.", HintDisplayDuration::LONG);
						hintDisplay->displayHint("Pressing escape also shows the controls.", HintDisplayDuration::LONG);
						worldManager->removeWalkedOnScript(Location(1, -2, 0, 5, 0, 7));
					};

					scriptManager->addScript("hint_1_1", hint_1_1);
					worldManager->addWalkedOnScript("hint_1_1", Location(1, -2, 0, 5, 0, 7));

					scriptManager->addScript("hint_2_0", std::bind(&showHint, (std::string) "Pressing ENTER will also interact with buttons.", HintDisplayDuration::LONG));
					worldManager->addWalkedOnScript("hint_2_0", Location(1, -5, 0, 13, 0, 10));

					auto hint_5_0 = []() {
						hintDisplay->displayHint("If you are in the middle of a closing door, you'll die.", HintDisplayDuration::LONG);
						worldManager->removeWalkedOnScript(Location(1, -4, 0, 3, 0, 8));
						worldManager->removeWalkedOnScript(Location(1, -4, 0, 4, 0, 8));
					};

					scriptManager->addScript("hint_5_0", hint_5_0);
					worldManager->addWalkedOnScript("hint_5_0", Location(1, -4, 0, 3, 0, 8));
					worldManager->addWalkedOnScript("hint_5_0", Location(1, -4, 0, 4, 0, 8));

					auto button_0_1 = []() {
						Character* character = player->getCharacter();
						character->allowNewMovements(false);
						pressButton(Location(1, -5, 0, 14, 0, 13));
						gameSaver->disableLoading();

						auto event2 = []() {
							openDoor("metal door", 345, Location(1, -5, 0, 13, 0, 13));
						};

						auto event3 = [character]() {
							gameAudioPlayer->stopAllLoops();
							character->allowNewMovements(true);
							character->addTurnToQueue(TileDirection::RIGHT);
							character->addTileMovementToQueue(TileDirection::RIGHT);
							character->addTurnToQueue(TileDirection::FORWARD);
							character->addTileMovementToQueue(TileDirection::FORWARD);
							character->addTeleportToQueue(Location(0, 0, 0, 1.5f, 0, 0.5f), TeleportationAnimation::FADE, "hide_and_disable_gun");
							character->addTileMovementToQueue(TileDirection::FORWARD);
							character->addTurnToQueue(TileDirection::BACKWARD);
							character->allowNewMovements(false);
							player->getTileInventory()->clear(3);
							setupLevel(1, 2);
						};

						timer->scheduleEventInMillis(1000, event2);
						timer->scheduleEventInMillis(1500, event3);
					};

					scriptManager->addScript("button_0_1", button_0_1);
					worldManager->addPromptedScript("button_0_1", Location(1, -5, 0, 14, 0, 13));
					break;
				}
				case 2: {
					/*auto annabelle_1 = []() {
						dialogueDisplay->resetTimeBetweenCharacterReveals();
						player->getCharacter()->addTurnToQueue(TileDirection::FORWARD);
						hintDisplay->clear();

						auto event = []() {
							showBlinkingDialogue("Annabelle", "avatars_2/3/4.png", "avatars_2/3/5.png", "I... really like you.", "annabelle_2");
						};

						timer->scheduleEventInSeconds(10, event);
					};

					scriptManager->addScript("annabelle_1", annabelle_1);
					worldManager->addWalkedOnScript("annabelle_1", Location(2, 0, 0, 8, 0, 15));

					scriptManager->addScript("annabelle_2", std::bind(&showBlinkingDialogue, "Annabelle", "avatars_2/3/4.png", "avatars_2/3/5.png", "Do you like me too?", "hide_dialogue"));*/


					auto telecom_1_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnTelecomOn(Location(0, -5, 0, 8, 1, 4));
							showDialogue("Telecom", "avatars_2/3/2.png", "YOU HAVE " + std::to_string(player->getCharacter()->getMoney()) + " JIMMIES.", "telecom_1_0_1");
						}
					};

					scriptManager->addScript("telecom_1_0", telecom_1_0);
					worldManager->addPromptedScript("telecom_1_0", Location(0, -5, 0, 8, 0, 4));

					scriptManager->addScript("telecom_1_0_1", std::bind(&showDialogue, "Telecom", "avatars_2/3/2.png", "YOU CAN CALL ANYONE FOR ONLY 2 JIMMIES.", "telecom_1_1"));
					scriptManager->addScript("telecom_1_1", std::bind(&showTwoOptions, true, DialogueOption("Call Brother", "telecom_1_2"), DialogueOption("Exit", "telecom_1_3")));

					auto telecom_1_2 = []() {
						showDialogue("Telecom", "avatars_2/3/2.png", "YOU LACK THE FUNDS NECESSARY TO MAKE THE CALL.", "telecom_1_1");
					};

					scriptManager->addScript("telecom_1_2", telecom_1_2);

					auto telecom_1_3 = []() {
						turnTelecomOff(Location(0, -5, 0, 8, 1, 4));
						hideDialogue();
					};

					scriptManager->addScript("telecom_1_3", telecom_1_3);

					auto post_battle_tutorial_0 = []() {
						characterProvider->getCharacterByName("tutorial_scientist")->turn(player->getCharacter()->getOppositeDirectionFacing());
						worldManager->addPromptedScript("post_battle_tutorial_1", Location(0, -4, 0, 0, 0, 0));
						hideDialogue();
					};

					scriptManager->addScript("post_battle_tutorial_0", post_battle_tutorial_0);

					auto post_battle_tutorial_1 = []() {
						characterProvider->getCharacterByName("tutorial_scientist")->turn(player->getCharacter()->getOppositeDirectionFacing());
						dialogueDisplay->showDialogue("Scientist", "avatars_2/0/2.png", "avatars_2/0/3.png", "Good luck on the next level. You might need it.", "hide_dialogue");
					};

					scriptManager->addScript("post_battle_tutorial_1", post_battle_tutorial_1);

					auto battle_tutorial_0_0 = []() {
						player->getCharacter()->allowNewMovements(false);
						characterProvider->getCharacterByName("tutorial_scientist")->addTurnToQueue(TileDirection::RIGHT);

						auto event = []() {
							dialogueDisplay->showDialogue("Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "Hey, " + PLAYER_NAME + "!", "battle_tutorial_1");
							characterProvider->getCharacterByName(PLAYER_NAME)->addTurnToQueue(TileDirection::LEFT);
						};

						timer->scheduleEventInSeconds(1, event);
					};

					auto battle_tutorial_0_1 = []() {
						dialogueDisplay->showDialogue("Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "Hey, " + PLAYER_NAME + "!", "battle_tutorial_1");
						characterProvider->getCharacterByName("tutorial_scientist")->addTurnToQueue(TileDirection::FORWARD);
					};

					scriptManager->addScript("battle_tutorial_0_0", battle_tutorial_0_0);
					worldManager->addWalkedOnScript("battle_tutorial_0_0", Location(0, -5, 0, 15, 0, 0));

					scriptManager->addScript("battle_tutorial_0_1", battle_tutorial_0_1);
					worldManager->addPromptedScript("battle_tutorial_0_1", Location(0, -4, 0, 0, 0, 0));

					scriptManager->addScript("battle_tutorial_1", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "Ya know, I'm actually one ova lead designers of your gun.", "battle_tutorial_2"));
					scriptManager->addScript("battle_tutorial_2", std::bind(&showDialogue, "Drunk Italics", "avatars_2/0/3.png", "... HICK! ...", "battle_tutorial_3"));
					scriptManager->addScript("battle_tutorial_3", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "That's right, I was the lead designer!", "battle_tutorial_4"));
					scriptManager->addScript("battle_tutorial_4", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "I am the alpha!", "battle_tutorial_5"));
					scriptManager->addScript("battle_tutorial_5", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "And you wanna know something about alphas? Those with brains?", "battle_tutorial_6"));
					scriptManager->addScript("battle_tutorial_6", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "Girls love 'em!", "battle_tutorial_7"));
					scriptManager->addScript("battle_tutorial_7", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "I'm not even kidding!", "battle_tutorial_8"));
					scriptManager->addScript("battle_tutorial_8", std::bind(&showDialogue, "Drunk Italics", "avatars_2/0/3.png", "... HICK! ...", "battle_tutorial_9"));
					scriptManager->addScript("battle_tutorial_9", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "Owww, that one hurt.", "battle_tutorial_10"));
					scriptManager->addScript("battle_tutorial_10", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "Well anyways, as the lead designer, I learned a party trick.", "battle_tutorial_11"));
					scriptManager->addScript("battle_tutorial_11", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "It involves the gun you hold in your hands!", "battle_tutorial_12"));
					scriptManager->addScript("battle_tutorial_12", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/0/2.png", "avatars_2/0/3.png", "The trick gets alotta girls, let me show you how it's done!", "battle_tutorial_13"));

					auto battle_tutorial_13 = []() {
						game->startBattle("battle_1");
						hideDialogue();
					};

					scriptManager->addScript("battle_tutorial_13", battle_tutorial_13);

					auto battle_tutorial_14 = []() {
						dialogueDisplay->showDialogue("Scientist", "avatars_2/0/2.png", "avatars_2/0/3.png", "See? That wasn't so hard!", "post_battle_tutorial_0");
						player->getCharacter()->allowNewMovements(true);
						worldManager->removeWalkedOnScript(Location(0, -5, 0, 15, 0, 0));
					};

					scriptManager->addScript("battle_tutorial_14", battle_tutorial_14);

					auto battle_0_0 = []() {
						gameAudioPlayer->stopAllLoops();
						dialogueDisplay->showDialogue("Soldier", "avatars_2/2/1.png", "What's that? You want to get through?", "battle_0_1");
						characterProvider->getCharacterByName("soldier_10")->turn(player->getCharacter()->getOppositeDirectionFacing());
						worldManager->removePromptedScript(Location(1, -7, 1, 1, 0, 0));
					};

					scriptManager->addScript("battle_0_0", battle_0_0);
					worldManager->addPromptedScript("battle_0_0", Location(1, -7, 1, 1, 0, 0));

					scriptManager->addScript("battle_0_1", std::bind(&showDialogue, "Soldier", "avatars_2/2/1.png", "You know, I only need to arrest one more evil scientist...", "battle_0_2"));
					scriptManager->addScript("battle_0_2", std::bind(&showDialogue, "Soldier", "avatars_2/2/1.png", "Then I'll get a raise!", "battle_0_3"));

					auto battle_0_3 = []() {
						game->startBattle("battle_2");
						hideDialogue();
					};

					scriptManager->addScript("battle_0_3", battle_0_3);

					auto battle_0_4 = []() {
						characterProvider->getCharacterByName("soldier_10")->kill();
					};

					scriptManager->addScript("battle_0_4", battle_0_4);

					auto battle_0_5 = []() {
						player->lookAtWatch();
						showBlinkingDialogue("Aela", "avatars_1/5/0.png", "avatars_1/5/1.png", PLAYER_NAME + "! Are you okay?!", "battle_0_5_1");
					};

					scriptManager->addScript("battle_0_5", battle_0_5);

					scriptManager->addScript("battle_0_5_1", std::bind(&showTwoOptions, true, DialogueOption("W-what just happened?", "battle_0_5_2"), DialogueOption("I didn't mean to kill him!", "battle_0_5_2")));
					scriptManager->addScript("battle_0_5_2", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/5/0.png", "avatars_1/5/1.png", "It's okay, it's all over now.", "battle_0_6"));
					scriptManager->addScript("battle_0_6", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/5/0.png", "avatars_1/5/1.png", "I tried calling for security but received no response!", "battle_0_7"));
					scriptManager->addScript("battle_0_7", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/5/0.png", "avatars_1/5/1.png", "Thank God you're okay!", "battle_0_8"));
					scriptManager->addScript("battle_0_8", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/5/0.png", "avatars_1/5/1.png", "The soldier may be dead, but... you were just defending yourself, right?", "battle_0_9"));
					scriptManager->addScript("battle_0_9", std::bind(&showDialogue, "Aela", "avatars_1/0/1.png", "I'll just... ignore this whole incident.", "battle_0_10"));
					scriptManager->addScript("battle_0_10", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "Nothing happened. No incident occured.", "battle_0_11"));
					scriptManager->addScript("battle_0_11", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "Carry on with your testing.", "battle_0_12"));

					auto battle_0_12 = []() {
						hideDialogue();
						player->stopLookingAtWatch();
						gameAudioPlayer->loopStreamInfinitely("Another Day, Another Pay", 163200, 3000);
					};

					scriptManager->addScript("battle_0_12", battle_0_12);


					auto battle_1_0 = []() {
						gameAudioPlayer->stopAllLoops();
						characterProvider->getCharacterByName("soldier_11")->turn(player->getCharacter()->getOppositeDirectionFacing());
						dialogueDisplay->showDialogue("Soldier", "avatars_2/2/0.png", "Who do you think you're looking at, you little punk?", "battle_1_1");
					};

					scriptManager->addScript("battle_1_0", battle_1_0);
					worldManager->addPromptedScript("battle_1_0", Location(1, -11, 3, 13.5f, 0, 8.5f));

					scriptManager->addScript("battle_1_1", std::bind(std::bind(&showTwoOptions, true, DialogueOption("Oh no, please. Not again.", "battle_1_2"), DialogueOption("Someone sounds insecure...", "battle_1_3"))));
					scriptManager->addScript("battle_1_2", std::bind(&showDialogue, "Soldier", "avatars_2/2/0.png", "Excuse me?! You're under arrest!", "battle_1_5"));
					scriptManager->addScript("battle_1_3", std::bind(&showDialogue, "Soldier", "avatars_2/2/0.png", "...", "battle_1_4"));
					scriptManager->addScript("battle_1_4", std::bind(&showDialogue, "Soldier", "avatars_2/2/0.png", "Shut up!", "battle_1_5"));

					auto battle_1_5 = []() {
						game->startBattle("battle_3");
						worldManager->removePromptedScript(Location(1, -11, 3, 13.5f, 0, 8.5f));
					};

					scriptManager->addScript("battle_1_5", battle_1_5);

					auto battle_1_6 = []() {
						characterProvider->getCharacterByName("soldier_11")->kill();
					};

					scriptManager->addScript("battle_1_6", battle_1_6);

					auto battle_1_7 = []() {
						player->lookAtWatch();
						showBlinkingDialogue("Aela", "avatars_1/5/0.png", "avatars_1/5/1.png", "Oh my...", "battle_1_8");
					};

					scriptManager->addScript("battle_1_7", battle_1_7);
					scriptManager->addScript("battle_1_8", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", PLAYER_NAME + ". Make it through this level as fast as you can.", "battle_1_9"));
					scriptManager->addScript("battle_1_9", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "Currently, there are no soldiers in the next level.", "battle_1_10"));
					scriptManager->addScript("battle_1_10", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "You should be safe there.", "battle_1_11"));
					scriptManager->addScript("battle_1_11", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "All of Oxylabs knows about this weird breach of our facilities.", "battle_1_12"));
					scriptManager->addScript("battle_1_12", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "Our security says everything is under control.", "battle_1_13"));

					auto battle_1_13 = []() {
						hideDialogue();
						player->stopLookingAtWatch();
						gameAudioPlayer->loopStreamInfinitely("Death of a Laboratory", 260330, 2000);
					};

					scriptManager->addScript("battle_1_13", battle_1_13);

					auto coffee_dialogue_0_0 = []() {
						dialogueDisplay->showDialogue("Scientist", "avatars_2/0/0.png", "avatars_2/0/1.png", "This news anchor just keeps on talking and talking...", "coffee_dialogue_0_1");
						characterProvider->getCharacterByName("scientist_4")->addTurnToQueue(player->getCharacter()->getOppositeDirectionFacing());
					};

					scriptManager->addScript("coffee_dialogue_0_0", coffee_dialogue_0_0);
					worldManager->addPromptedScript("coffee_dialogue_0_0", Location(0, -5, 0, 13, 0, 1));

					scriptManager->addScript("coffee_dialogue_0_1", std::bind(&showBlinkingDialogue, "Scientist", "avatars_2/0/0.png", "avatars_2/0/1.png", "I can barely stay awake!", "coffee_dialogue_0_2"));

					auto coffee_dialogue_0_2 = []() {
						hideDialogue();
						characterProvider->getCharacterByName("scientist_4")->addTurnToQueue(TileDirection::FORWARD);
					};

					scriptManager->addScript("coffee_dialogue_0_2", coffee_dialogue_0_2);

					auto coffee_dialogue_1_0 = []() {
						dialogueDisplay->showDialogue("Scientist", "avatars_2/1/0.png", "avatars_2/1/1.png", "Hey, " + PLAYER_NAME + "!", "coffee_dialogue_1_1");
						characterProvider->getCharacterByName("scientist_5")->addTurnToQueue(player->getCharacter()->getOppositeDirectionFacing());
					};

					scriptManager->addScript("coffee_dialogue_1_0", coffee_dialogue_1_0);
					worldManager->addPromptedScript("coffee_dialogue_1_0", Location(0, -5, 0, 10, 0, 2));

					scriptManager->addScript("coffee_dialogue_1_1", std::bind(&showBlinkingDialogue, "Scientist", "avatars_2/1/0.png",
						"avatars_2/1/1.png", "While you've been testing that gun, I've been waiting for my food to heat up!", "coffee_dialogue_1_2"));
					scriptManager->addScript("coffee_dialogue_1_2", std::bind(&showBlinkingDialogue, "Scientist", "avatars_2/1/0.png",
						"avatars_2/1/1.png", "Yes, this microwave is that bad. I swear our management finds new ways to cheap out on us!",  "coffee_dialogue_1_3"));
					scriptManager->addScript("coffee_dialogue_1_3", std::bind(&showBlinkingDialogue, "Scientist", "avatars_2/1/0.png",
						"avatars_2/1/1.png", "The cheap monitors, the awful coffee, and now the kitchen applicances! They all suck!",  "coffee_dialogue_1_4"));
					scriptManager->addScript("coffee_dialogue_1_4", std::bind(&showBlinkingDialogue, "Scientist", "avatars_2/1/0.png",
						"avatars_2/1/1.png", "...", "coffee_dialogue_1_5"));
					scriptManager->addScript("coffee_dialogue_1_5", std::bind(&showBlinkingDialogue, "Scientist", "avatars_2/1/0.png",
						"avatars_2/1/1.png", "Sorry, today has not been a good day for me.", "coffee_dialogue_1_6"));
					scriptManager->addScript("coffee_dialogue_1_6", std::bind(&showBlinkingDialogue, "Scientist", "avatars_2/1/0.png",
						"avatars_2/1/1.png", "Also, just a heads up: the exit to the next level is NOT the red door.", "coffee_dialogue_1_7"));
					scriptManager->addScript("coffee_dialogue_1_7", std::bind(&showBlinkingDialogue, "Scientist", "avatars_2/1/0.png",
						"avatars_2/1/1.png", "Honestly, don't even think about that red door. It will soon be deactivated.", "hide_dialogue"));

					auto button_3_0 = []() {
						Character* character = player->getCharacter();
						pressButton(Location(0, -5, 0, 6, 0, 4));
						gameSaver->disableLoading();
						character->allowNewMovements(false);

						auto event2 = []() {
							openDoor("metal door", 185, Location(0, -5, 0, 5, 0, 4));
						};

						auto event3 = [character]() {
							character->allowNewMovements(true);
							character->addTurnToQueue(TileDirection::RIGHT);
							character->addTileMovementToQueue(TileDirection::RIGHT);
							character->addTurnToQueue(TileDirection::FORWARD);
							character->addTileMovementToQueue(TileDirection::FORWARD);
							character->addTeleportToQueue(Location(2, 0, 0, 1.5f, 0, 0.5f), TeleportationAnimation::FADE);
							gameAudioPlayer->loopStreamInfinitely("Shadowed Secret", 469700, 3000);
						};

						timer->scheduleEventInMillis(1000, event2);
						timer->scheduleEventInMillis(1500, event3);
					};

					scriptManager->addScript("button_3_0", button_3_0);
					worldManager->addPromptedScript("button_3_0", Location(0, -5, 0, 6, 0, 4));

					auto television_0_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Info", "", "The news anchor is trying to report on new technology, but is clearly unprepared.", "television_0_1");
						}
					};

					scriptManager->addScript("television_0_0", television_0_0);
					worldManager->addPromptedScript("television_0_0", Location(0, -5, 0, 13, 0, 3));
					scriptManager->addScript("television_0_1", std::bind(&showThreeOptions, false, DialogueOption("Watch the news", "television_0_2"), DialogueOption("Exit", "hide_dialogue"),
						DialogueOption("Read the scrolling text", "television_0_7")));
					scriptManager->addScript("television_0_2", std::bind(&showDialogue, "Television", "avatars_2/4/0.png",
						"\"Government aviation company Voss has produced the world's fastest rocket, the V-9.\"", "television_0_3"));
					scriptManager->addScript("television_0_3", std::bind(&showDialogue, "Television", "avatars_2/4/0.png",
						"\"Voss is the largest rocket manufacturer in the world.\"", "television_0_3_1"));
					scriptManager->addScript("television_0_3_1", std::bind(&showDialogue, "Television", "avatars_2/4/0.png",
						"\"However, the V-9 was built as a collaboration project.\"", "television_0_4"));
					scriptManager->addScript("television_0_4", std::bind(&showDialogue, "Television", "avatars_2/4/0.png",
						"\"The most notable collaborator is the local technology company Oxylabs,\"", "television_0_4_1"));
					scriptManager->addScript("television_0_4_1", std::bind(&showDialogue, "Television", "avatars_2/4/0.png",
						"\"which helped build the rocket's propulsion system.\"", "television_0_5"));
					scriptManager->addScript("television_0_5", std::bind(&showDialogue, "Television", "avatars_2/4/0.png",
						"\"The rocket features a... mega-ionic... launch-assisted...\"", "television_0_5_1"));
					scriptManager->addScript("television_0_5_1", std::bind(&showDialogue, "Television", "avatars_2/4/0.png",
						"\"Magnetic induction... thruster system?\"", "television_0_6"));
					scriptManager->addScript("television_0_6", std::bind(&showDialogue, "Television", "avatars_2/4/0.png",
						"\"Whatever it is, it's big, it's powerful, and it's beautiful.\"", "television_0_1"));
					scriptManager->addScript("television_0_7", std::bind(&showDialogue, "Scrolling Text", "avatars_2/4/0.png",
						"MILITARY MARCHING SOUTH ON ELLESMERE STREET TOWARDS CHEMICAL DISTRICT.", "television_0_8"));
					scriptManager->addScript("television_0_8", std::bind(&showDialogue, "Scrolling Text", "avatars_2/4/0.png",
						"ANGRY MANAGER OF FIREWORKS STORE ARRESTED AFTER SETTING OFF ENTIRE SUPPLY.", "television_0_9"));
					scriptManager->addScript("television_0_9", std::bind(&showDialogue, "Scrolling Text", "avatars_2/4/0.png",
						"HUNGRY DEEP SEA EXPLORERS SUFFER CRITICAL INJURIES AFTER EATING JELLYFISH.", "television_0_10"));
					scriptManager->addScript("television_0_10", std::bind(&showDialogue, "Scrolling Text", "avatars_2/4/0.png",
						"EXPLORERS DISCOVER NEW SPECIES OF BACON-FLAVOURED JELLYFISH.", "television_0_1"));

					auto computer_0_0 = []() {
						TileDirection direction = player->getCharacter()->getDirectionFacing();
						if (direction == TileDirection::RIGHT) {
							showDialogue("Info", "", "Turn on the computer?", "computer_0_1");
						} else if (direction == TileDirection::LEFT) {
							showDialogue("Info", "", "The previous user of this computer left an unfinished word document open.", "computer_0_4");
						}
					};
					scriptManager->addScript("computer_0_0", computer_0_0);
					worldManager->addPromptedScript("computer_0_0", Location(0, -5, 0, 10, 0, 0));

					scriptManager->addScript("computer_0_1", std::bind(&showTwoOptions, true, DialogueOption("Yes", "computer_0_2"), DialogueOption("No", "hide_dialogue")));
					scriptManager->addScript("computer_0_2", std::bind(&showDialogue, "Info", "avatars_2/4/2.png", "...", "computer_0_3"));
					scriptManager->addScript("computer_0_3", std::bind(&showDialogue, "Info", "", "Other than a faint hiss from the monitor, nothing happens.", "hide_dialogue"));
					scriptManager->addScript("computer_0_4", std::bind(&showThreeOptions, true, DialogueOption("Read the document", "computer_0_5"), DialogueOption("Exit", "hide_dialogue"),
						DialogueOption("Close without saving", "computer_0_11")));
					scriptManager->addScript("computer_0_5", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "[CLASSIFIED] Project Lima - Progress and Next Steps", "computer_0_6"));
					scriptManager->addScript("computer_0_6", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "After two years of development, Project Lima reached a new milestone.", "computer_0_7"));
					scriptManager->addScript("computer_0_7", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "We have successfully transplanted an Ebony Wolf brain into a robotic body.", "computer_0_8"));
					scriptManager->addScript("computer_0_8", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "Our robotic wolf has retained most of its motor skills.", "computer_0_9"));
					scriptManager->addScript("computer_0_9", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "Figure 1 shows the results of our motor skill tests before and after the operation.", "computer_0_10"));
					scriptManager->addScript("computer_0_10", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "(Note to self: don't mention the wolf's death???).", "computer_0_4"));


					scriptManager->addScript("computer_0_11", std::bind(&showDialogue, "Info", "", "You close the document.", "computer_0_12"));

					auto computer_0_12 = []() {
						hideDialogue();
						player->getCharacter()->allowNewMovements(false);
						worldManager->removePromptedScript(Location(0, -5, 0, 13, 0, 1));
						Character* scientist_4_2 = characterProvider->getCharacterByName("scientist_4_2");
						scientist_4_2->changeLocation(Location(1, -7, 0, 8.5f, 0, 2.5f));
						scientist_4_2->turn(TileDirection::LEFT);

						auto event = []() {
							Character* scientist = characterProvider->getCharacterByName("scientist_4");
							scientist->addTurnToQueue(TileDirection::RIGHT);
							scientist->addTileMovementToQueue(TileDirection::RIGHT);
							scientist->addTileMovementToQueue(TileDirection::RIGHT);
							scientist->addTileMovementToQueue(TileDirection::RIGHT);
							scientist->addTileMovementToQueue(TileDirection::RIGHT);
							scientist->addTurnToQueue(TileDirection::BACKWARD);
						};

						timer->scheduleEventInMillis(1500, event);

						auto event2 = []() {
								showBlinkingDialogue("Scientist", "avatars_2/0/0.png", "avatars_2/0/1.png", "Excuse me. I was working here.", "computer_0_13");
							};
						timer->scheduleEventInMillis(3500, event2);
					};

					scriptManager->addScript("computer_0_12", computer_0_12);

					auto computer_0_13 = []() {
						Character* playerCharacter = player->getCharacter();
						playerCharacter->addTurnToQueue(TileDirection::RIGHT);
						playerCharacter->addTileMovementToQueue(TileDirection::RIGHT);
						playerCharacter->addTurnToQueue(TileDirection::LEFT);

						auto event = []() {
							Character* scientist = characterProvider->getCharacterByName("scientist_4");
							scientist->addTileMovementToQueue(TileDirection::BACKWARD);
							scientist->addTurnToQueue(TileDirection::LEFT);

							auto computer_0_15 = []() {
								showBlinkingDialogue("Scientist", "avatars_2/0/0.png", "avatars_2/0/1.png", "Please leave.", "computer_0_17");
								characterProvider->getCharacterByName("scientist_4")->turn(player->getCharacter()->getOppositeDirectionFacing());
							};

							scriptManager->addScript("computer_0_15", computer_0_15);
							worldManager->addPromptedScript("computer_0_15", Location(0, -5, 0, 9, 0, 0));

							auto computer_0_17 = []() {
								characterProvider->getCharacterByName("scientist_4")->turn(TileDirection::LEFT);
								hideDialogue();
							};

							scriptManager->addScript("computer_0_17", computer_0_17);

							auto turnLeft = [scientist]() {
								scientist->turn(TileDirection::LEFT);
							};

							auto turnRight = [scientist]() {
								scientist->turn(TileDirection::RIGHT);
							};

							timer->scheduleEventInMillis(3000, turnRight);
							timer->scheduleEventInMillis(4000, turnLeft);
							timer->scheduleEventInMillis(4600, turnRight);
							timer->scheduleEventInMillis(5200, turnLeft);
							timer->scheduleEventInMillis(5700, turnRight);
							timer->scheduleEventInMillis(8250, std::bind(&showBlinkingDialogue, "Scientist", "avatars_2/0/0.png", "avatars_2/0/1.png", "You messed with the wrong person.", "computer_0_14"));
						};
						timer->scheduleEventInMillis(1000, event);

						hideDialogue();
					};
					scriptManager->addScript("computer_0_13", computer_0_13);

					scriptManager->addScript("computer_0_14", std::bind(&showBlinkingDialogue, "Scientist", "avatars_2/0/0.png", "avatars_2/0/1.png", "I know people. I will make sure you lose your job.", "computer_0_16"));

					auto computer_0_16 = []() {
						player->getCharacter()->allowNewMovements(true);
						hideDialogue();
						characterProvider->getCharacterByName("scientist_4")->turn(TileDirection::LEFT);
					};
					scriptManager->addScript("computer_0_16", computer_0_16);

					auto computer_1_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Info", "", "The computer has an unsent email open.", "computer_1_1");
						}
					};

					scriptManager->addScript("computer_1_0", computer_1_0);
					worldManager->addPromptedScript("computer_1_0", Location(2, -1, 0, 14, 0, 6));

					scriptManager->addScript("computer_1_1", std::bind(&showTwoOptions, true, DialogueOption("Read the email", "computer_1_2"), DialogueOption("Exit", "hide_dialogue")));
					scriptManager->addScript("computer_1_2", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "Dear Ashley,", "computer_1_3"));
					scriptManager->addScript("computer_1_3", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "I would like to give you my apologies for not responding sooner.", "computer_1_4"));
					scriptManager->addScript("computer_1_4", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "As you know, I have been working as a security guard for an Oxylabs warehouse.", "computer_1_4_1"));
					scriptManager->addScript("computer_1_4_1", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "I started my work about a month ago.", "computer_1_5"));
					scriptManager->addScript("computer_1_5", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "The warehouse is attached to their main facility.", "computer_1_5_1"));
					scriptManager->addScript("computer_1_5_1", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "Lots of stuff gets moved in through this place.", "computer_1_6"));
					scriptManager->addScript("computer_1_6", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "This job has been a wild rollercoaster of oddities.", "computer_1_7"));
					scriptManager->addScript("computer_1_7", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "It rains every time I arrive at work.", "computer_1_7_1"));
					scriptManager->addScript("computer_1_7_1", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "Apparently, this is due the fumes from one of the chemical plants.", "computer_1_8"));
					scriptManager->addScript("computer_1_8", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "I used to enjoy rainy days as a kid, but this rain stings me when it touches my skin.", "computer_1_9"));
					scriptManager->addScript("computer_1_9", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "The colour has almost been completely washed off of my rain coat.", "computer_1_10"));
					scriptManager->addScript("computer_1_10", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "At first, all they kept here were salvaged exhibits.", "computer_1_10_1"));
					scriptManager->addScript("computer_1_10_1", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "The exhibits were retreived from the big toxic waste leak at the Oxylabs Museum.", "computer_1_11"));
					scriptManager->addScript("computer_1_11", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "However, everything changed a couple of weeks ago.", "computer_1_11_1"));
					scriptManager->addScript("computer_1_11_1", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "You see, a group of employees brought in several crates with ventilation holes.", "computer_1_12"));
					scriptManager->addScript("computer_1_12", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "Based on the sounds I hear, I could tell that these crates hold several animals,", "computer_1_12_1"));
					scriptManager->addScript("computer_1_12_1", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "including wild cats, mice, various birds, and other poor creatures.", "computer_1_13"));
					scriptManager->addScript("computer_1_13", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "One of the employees opened a crate once. I saw some type of a dog inside.", "computer_1_13_1"));
					scriptManager->addScript("computer_1_13_1", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "It had the saddest eyes I've ever seen.", "computer_1_14"));
					scriptManager->addScript("computer_1_14", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "It was as if the dog knew what was about to happen to it. I wish I could save it.", "computer_1_15"));
					scriptManager->addScript("computer_1_15", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "Recently, one of Oxylabs' top secret projects had been completed.", "computer_1_16"));
					scriptManager->addScript("computer_1_16", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "I could tell from the \"extremly hazardous chemicals\" crates that arrived.", "computer_1_17"));
					scriptManager->addScript("computer_1_17", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "We live in a weird world, don't we?", "computer_1_17_1"));
					scriptManager->addScript("computer_1_17_1", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "A world where nerds and corrupt politicians do as they please.", "computer_1_18"));
					scriptManager->addScript("computer_1_18", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "And we work for them because they're the ones who offer the jobs.", "computer_1_19"));
					scriptManager->addScript("computer_1_19", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "Anyways, I hope you feel better soon.", "computer_1_20"));
					scriptManager->addScript("computer_1_20", std::bind(&showDialogue, "Computer", "avatars_2/4/1.png", "Love, Elijah.", "hide_dialogue"));

					scriptManager->addScript("door_text_0_0", std::bind(&showDialogue, "Info", "", "The door seems to be bolted shut.", "door_text_0_1"));
					worldManager->addPromptedScript("door_text_0_0", Location(2, 0, 1, 11, 0, 1));
					scriptManager->addScript("door_text_0_1", std::bind(&showDialogue, "Info", "", "All you can do is stare outside at the pouring rain.", "hide_dialogue"));

					scriptManager->addScript("crate_0", std::bind(&showDialogue, "Info", "avatars_2/4/5.png", "This create gives off a foul smell.", "hide_dialogue"));
					worldManager->addPromptedScript("crate_0", Location(2, 0, 0, 3, 0, 4));
					worldManager->addPromptedScript("crate_0", Location(2, 0, 0, 3, 0, 5));
					worldManager->addPromptedScript("crate_0", Location(2, 0, 0, 3, 0, 6));
					worldManager->addPromptedScript("crate_0", Location(2, -1, 0, 15, 0, 13));

					scriptManager->addScript("crate_1", std::bind(&showDialogue, "Info", "avatars_2/5/5.png", "This crate gives off a slight warmth.", "hide_dialogue"));
					worldManager->addPromptedScript("crate_1", Location(2, 0, 0, 4, 0, 5));
					worldManager->addPromptedScript("crate_1", Location(2, 0, 0, 5, 0, 6));

					scriptManager->addScript("crate_2", std::bind(&showDialogue, "Info", "avatars_2/4/4.png", "Touching this crate gives you anxiety.", "hide_dialogue"));
					worldManager->addPromptedScript("crate_2", Location(2, 0, 0, 5, 0, 5));
					worldManager->addPromptedScript("crate_2", Location(2, 0, 0, 6, 0, 4));
					worldManager->addPromptedScript("crate_2", Location(2, 0, 0, 6, 0, 5));
					worldManager->addPromptedScript("crate_2", Location(2, 0, 0, 6, 0, 6));

					scriptManager->addScript("crate_3", std::bind(&showDialogue, "Info", "avatars_1/3/3.png", "This crate gives off the sound of breathing.", "hide_dialogue"));
					worldManager->addPromptedScript("crate_3", Location(2, 0, 0, 4, 0, 8));
					worldManager->addPromptedScript("crate_3", Location(2, 0, 0, 5, 0, 8));
					worldManager->addPromptedScript("crate_3", Location(2, 0, 0, 6, 0, 8));

					scriptManager->addScript("crate_4", std::bind(&showDialogue, "Info", "avatars_1/4/2.png", "You knock on the crate but get no response.", "hide_dialogue"));
					worldManager->addPromptedScript("crate_4", Location(2, 0, 0, 3, 0, 13));
					worldManager->addPromptedScript("crate_4", Location(2, 0, 0, 4, 0, 13));
					worldManager->addPromptedScript("crate_4", Location(2, 0, 0, 6, 0, 13));

					scriptManager->addScript("crate_5", std::bind(&showDialogue, "Info", "avatars_1/5/2.png", "You knock on the container but get no response.", "hide_dialogue"));
					worldManager->addPromptedScript("crate_5", Location(2, -1, 0, 13, 0, 10));
					worldManager->addPromptedScript("crate_5", Location(2, -1, 0, 12, 0, 10));
					worldManager->addPromptedScript("crate_5", Location(2, -1, 0, 12, 0, 11));
					worldManager->addPromptedScript("crate_5", Location(2, -1, 0, 12, 0, 12));
					worldManager->addPromptedScript("crate_5", Location(2, -1, 0, 13, 0, 12));
					worldManager->addPromptedScript("crate_5", Location(2, -1, 0, 12, 0, 13));
					worldManager->addPromptedScript("crate_5", Location(2, -1, 0, 13, 0, 13));
					
					worldManager->addPromptedScript("crate_5", Location(2, 0, 1, 5, 0, 1));
					worldManager->addPromptedScript("crate_5", Location(2, 0, 1, 6, 0, 1));
					worldManager->addPromptedScript("crate_5", Location(2, 0, 1, 6, 0, 2));
					worldManager->addPromptedScript("crate_5", Location(2, 0, 1, 6, 0, 3));
					worldManager->addPromptedScript("crate_5", Location(2, 0, 1, 6, 0, 4));
					worldManager->addPromptedScript("crate_5", Location(2, 0, 1, 5, 0, 4));

					scriptManager->addScript("crate_6", std::bind(&showDialogue, "Info", "avatars_2/4/2.png", "This television is unplugged... what did you expect?", "hide_dialogue"));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 12, 0, 1));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 13, 0, 1));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 14, 0, 1));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 15, 0, 1));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 15, 0, 2));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 15, 0, 3));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 15, 0, 4));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 14, 0, 4));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 13, 0, 4));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 12, 0, 4));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 12, 0, 3));
					worldManager->addPromptedScript("crate_6", Location(2, -1, 1, 12, 0, 2));

					scriptManager->addScript("crate_7", std::bind(&showDialogue, "Info", "avatars_1/4/2.png", "You knock on the container but get no response.", "hide_dialogue"));
					worldManager->addPromptedScript("crate_7", Location(2, -1, 0, 15, 0, 11));
					worldManager->addPromptedScript("crate_7", Location(2, -1, 0, 14, 0, 11));
					worldManager->addPromptedScript("crate_7", Location(2, 0, 1, 3, 0, 1));
					worldManager->addPromptedScript("crate_7", Location(2, 0, 1, 4, 0, 1));
					worldManager->addPromptedScript("crate_7", Location(2, 0, 1, 4, 0, 4));
					worldManager->addPromptedScript("crate_7", Location(2, 0, 1, 3, 0, 4));
					worldManager->addPromptedScript("crate_7", Location(2, 0, 1, 3, 0, 3));
					worldManager->addPromptedScript("crate_7", Location(2, 0, 1, 3, 0, 2));

					auto dialogue_31_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Plaque", "avatars_1/1/4.png", "\"Past the Yellow Hills\" - Anonymous.", "hide_dialogue");
						}
					};

					scriptManager->addScript("dialogue_31_0", dialogue_31_0);
					worldManager->addPromptedScript("dialogue_31_0", Location(2, 0, 0, 3, 0, 11));

					auto dialogue_32_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Plaque", "avatars_1/1/4.png", "This picture was taken from one of Oxylab's first rockets.", "hide_dialogue");
						}
					};

					scriptManager->addScript("dialogue_32_0", dialogue_32_0);
					worldManager->addPromptedScript("dialogue_32_0", Location(2, 0, 0, 5, 0, 11));

					auto walked_on_4_0 = []() {
						worldManager->getTileGroup(Location(0, -5, 0, 5.5f, 0, 4.5f))->addTile(185);
						fadeTeleportPlayer(Location(0, -5, 0, 5.5f, 0, 3.5f));
						gameAudioPlayer->stopAllLoops();

						auto event = []() {
							std::cout << "YEAH BOI!\n";
							worldManager->getWorld(2)->getChunks()->clear();
						};

						timer->scheduleEventInMillis(5000, event);
					};

					scriptManager->addScript("walked_on_4_0", walked_on_4_0);
					worldManager->addWalkedOnScript("walked_on_4_0", Location(2, 0, -1, 1, 0, 15));

					auto dialogue_8_0 = []() {
						// Some weird glitch happened that requires this.
						dialogueDisplay->resetTimeBetweenCharacterReveals();

						worldManager->removeWalkedOnScript(Location(0, 0, 0, 1, 0, 1));
						player->lookAtWatch();
						player->getCharacter()->allowNewMovements(true);
						animateElevator();
						dialogueDisplay->showDialogue("Aela", "avatars_1/2/0.png", "avatars_1/2/1.png", "Congratulations on making it through the first level!", "dialogue_8_1");
					};
					scriptManager->addScript("dialogue_8_0", dialogue_8_0);
					worldManager->addWalkedOnScript("dialogue_8_0", Location(0, 0, 0, 1, 0, 1));

					scriptManager->addScript("dialogue_8_1", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Was it easy?", "dialogue_8_2"));

					auto dialogue_8_2 = []() {
						auto yesOption = DialogueOption("Yes", "dialogue_8_3_1");
						auto noOption = DialogueOption("No", "dialogue_8_3_2");
						dialogueDisplay->showOptions(&yesOption, &noOption);
						hintDisplay->displayHint("You can use W/S or A/D to select a response.", HintDisplayDuration::UNTIL_KEY);
						hintDisplay->displayMostRecentHintUntilKeyPress(SDLK_RETURN);
					};

					scriptManager->addScript("dialogue_8_2", dialogue_8_2);

					scriptManager->addScript("dialogue_8_3_1", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/2/0.png", "avatars_1/2/1.png", "Well, I'm sure it was for you!", "dialogue_8_4"));
					scriptManager->addScript("dialogue_8_3_2", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Really? Well you made it look easy.", "dialogue_8_4"));
					scriptManager->addScript("dialogue_8_4", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/2.png", "avatars_1/1/3.png", "Hmm... according to this paper, I'm supposed to ask you a set of questions.", "dialogue_8_5"));
					scriptManager->addScript("dialogue_8_5", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/2.png", "avatars_1/1/3.png", "\"You are to ask the tester the following questions in order to get to know them.\"", "dialogue_8_6"));
					scriptManager->addScript("dialogue_8_6", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/2.png", "avatars_1/1/3.png", "\"Studies show that this raises their chances of making a purchase at our gift shop.\"", "dialogue_8_7"));
					scriptManager->addScript("dialogue_8_7", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Alright. First question.", "dialogue_8_8"));
					scriptManager->addScript("dialogue_8_8", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/2.png", "avatars_1/1/3.png", "\"Do you gain more enjoyment from group or one-on-one engagements?\"", "dialogue_8_9"));
					scriptManager->addScript("dialogue_8_9", std::bind(&showThreeOptions, true, DialogueOption("Group.", "dialogue_8_10_2"), DialogueOption("One-on-one.", "dialogue_8_10_1"), DialogueOption("I do not speak to people.", "dialogue_8_10_1")));

					auto dialogue_8_10_1 = []() {
						player->getPersonality()->setTrait(PersonalityTrait::INTROVERTED);
						showBlinkingDialogue("Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "Hmmm.... next question.", "dialogue_8_11");
					};
					scriptManager->addScript("dialogue_8_10_1", dialogue_8_10_1);

					auto dialogue_8_10_2 = []() {
						player->getPersonality()->setTrait(PersonalityTrait::EXTROVERTED);
						showBlinkingDialogue("Aela", "avatars_1/2/0.png", "avatars_1/2/1.png", "I actually like both... next question!", "dialogue_8_11");
					};
					scriptManager->addScript("dialogue_8_10_2", dialogue_8_10_2);

					scriptManager->addScript("dialogue_8_11", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/2.png", "avatars_1/1/3.png", "\"Do you recall past events based on what they were like or based on what happened?\"", "dialogue_8_12"));
					scriptManager->addScript("dialogue_8_12", std::bind(&showTwoOptions, true, DialogueOption("I recall them based on what they were like.", "dialogue_8_13_1"), DialogueOption("I recall them based on what happened.", "dialogue_8_13_2")));

					auto dialogue_8_13_1 = []() {
						player->getPersonality()->setTrait(PersonalityTrait::INTUITION);
						showBlinkingDialogue("Aela", "avatars_1/1/0.png", "avatars_1/1/2.png", "Me too.", "dialogue_8_14");
					};
					scriptManager->addScript("dialogue_8_13_1", dialogue_8_13_1);

					auto dialogue_8_13_2 = []() {
						player->getPersonality()->setTrait(PersonalityTrait::SENSING);
						showBlinkingDialogue("Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Then you must be very aware. That's a useful trait for a Propulsion Gun tester.", "dialogue_8_14");
					};
					scriptManager->addScript("dialogue_8_13_2", dialogue_8_13_2);

					scriptManager->addScript("dialogue_8_14", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/2.png", "avatars_1/1/3.png", "\"When dealing with social conflicts, do you rely more on your head or your heart?\"", "dialogue_8_15"));
					scriptManager->addScript("dialogue_8_15", std::bind(&showTwoOptions, true, DialogueOption("I rely on my head.", "dialogue_8_16_1"), DialogueOption("I rely on my heart.", "dialogue_8_16_2")));

					auto dialogue_8_16_1 = []() {
						player->getPersonality()->setTrait(PersonalityTrait::THINKING);
						showBlinkingDialogue("Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Hmm... Interesting...", "dialogue_8_17");
					};
					scriptManager->addScript("dialogue_8_16_1", dialogue_8_16_1);

					auto dialogue_8_16_2 = []() {
						player->getPersonality()->setTrait(PersonalityTrait::FEELING);
						showBlinkingDialogue("Aela", "avatars_1/2/0.png", "avatars_1/2/1.png", "I can relate to that!", "dialogue_8_17");
					};
					scriptManager->addScript("dialogue_8_16_2", dialogue_8_16_2);

					scriptManager->addScript("dialogue_8_17", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/2.png", "avatars_1/1/3.png", "\"Would you walk an extra five minutes just to cross a busy street using a crosswalk?\"", "dialogue_8_18"));
					scriptManager->addScript("dialogue_8_18", std::bind(&showTwoOptions, true, DialogueOption("Yes, I would.", "dialogue_8_19_1"), DialogueOption("No, I would not.", "dialogue_8_19_2")));

					auto dialogue_8_19_1 = []() {
						player->getPersonality()->setTrait(PersonalityTrait::JUDGING);
						showBlinkingDialogue("Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Ha, don't worry. I'm crazy enough to do so too.", "dialogue_8_20");
					};
					scriptManager->addScript("dialogue_8_19_1", dialogue_8_19_1);

					auto dialogue_8_19_2 = []() {
						player->getPersonality()->setTrait(PersonalityTrait::PROSPECTING);
						showBlinkingDialogue("Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "So you would just cross the street wherever you were?", "dialogue_8_19_3");
					};
					scriptManager->addScript("dialogue_8_19_2", dialogue_8_19_2);

					scriptManager->addScript("dialogue_8_19_3", std::bind(&showTwoOptions, true, DialogueOption("Yes, I would.", "dialogue_8_19_4"), DialogueOption("No, I would not.", "dialogue_8_19_6")));
					scriptManager->addScript("dialogue_8_19_4", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/4/0.png", "avatars_1/4/1.png", "...", "dialogue_8_19_5"));
					scriptManager->addScript("dialogue_8_19_5", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/4/0.png", "avatars_1/4/1.png", "Okay then.", "dialogue_8_20"));
					scriptManager->addScript("dialogue_8_19_6", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "So how would you cross the mildly busy street?", "dialogue_8_19_7"));
					scriptManager->addScript("dialogue_8_19_7", std::bind(&showFourOptions, true, DialogueOption("I'd let it clear.", "dialogue_8_19_8"), DialogueOption("I'd fly over it.", "dialogue_8_19_9"), DialogueOption("I'd jump over it.", "dialogue_8_19_9"), DialogueOption("I would not cross it.", "dialogue_8_19_4")));
					scriptManager->addScript("dialogue_8_19_8", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Good answer.", "dialogue_8_20"));
					scriptManager->addScript("dialogue_8_19_9", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "I like that answer.", "dialogue_8_20"));
					scriptManager->addScript("dialogue_8_20", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Apparently, the next level ups the difficulty. We'll see if that's true.", "dialogue_8_21"));

					auto dialogue_8_21 = []() {
						// End elevator animation here
						player->stopLookingAtWatch();
						hideDialogue();
						worldManager->removeWalkedOnScript(Location(0, 0, 0, 1, 0, 1));
						stopAnimatingElevator();
					};

					scriptManager->addScript("dialogue_8_21", dialogue_8_21);

					scriptManager->addScript("walked_on_1_0", std::bind(&fadeTeleportPlayer, Location(0, -5, 0, 15.5f, 0, 3.5f)), true);
					worldManager->addWalkedOnScript("walked_on_1_0", Location(0, 0, -1, 1, 0, 15));

					auto walked_on_1_1 = []() {
						player->getCharacter()->addTeleportToQueue(Location(1, -7, 0, 14.5f, 0, 1.5f), TeleportationAnimation::FADE, "show_and_enable_gun");
						gameAudioPlayer->loopStreamInfinitely("Another Day, Another Pay", 163200, 4000);
					};

					scriptManager->addScript("walked_on_1_1", walked_on_1_1, true);
					worldManager->addWalkedOnScript("walked_on_1_1", Location(0, -5, -1, 15, 0, 15));

					auto dialogue_5_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, -6, 0, 2, 0, 6));
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "WELCOME TO LEVEL 2 OF THE PROPULSION GUN TESTING FACILITY.", "dialogue_5_1");
						}
					};

					scriptManager->addScript("dialogue_5_0", dialogue_5_0);
					worldManager->addPromptedScript("dialogue_5_0", Location(1, -6, 0, 2, 0, 6));

					scriptManager->addScript("dialogue_5_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "THIS LEVEL HAS BEEN DESIGNED SO THAT YOUR TESTING IS MORE ENJOYABLE.", "dialogue_5_2"));
					scriptManager->addScript("dialogue_5_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "YOU WILL FIND PUZZLES THAT FORCE YOU TO THINK BEFORE YOU ACT.", "dialogue_5_3"));
					scriptManager->addScript("dialogue_5_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "GOOD LUCK!", "dialogue_5_4"));

					auto dialogue_5_4 = []() {
						turnDisplayOff(Location(1, -6, 0, 2, 0, 6));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_5_4", dialogue_5_4);

					auto dialogue_33_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, -7, 2, 4, 0, 1));
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "WHAT WILL HAPPEN WHEN YOU STEP ONTO THE BLUE DEVICE? THERE'S ONLY ONE WAY TO FIND OUT!", "dialogue_33_1");
						}
					};

					scriptManager->addScript("dialogue_33_0", dialogue_33_0);
					worldManager->addPromptedScript("dialogue_33_0", Location(1, -7, 2, 4, 0, 1));

					auto dialogue_33_1 = []() {
						turnDisplayOff(Location(1, -7, 2, 4, 0, 1));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_33_1", dialogue_33_1);

					scriptManager->addScript("keypad_0_0", std::bind(&showDialogue, "Info", "avatars_1/4/6.png", "Put your finger onto the fingerprint sensor?", "keypad_0_1"));
					scriptManager->addScript("keypad_0_1", std::bind(&showTwoOptions, true, DialogueOption("Yes", "keypad_0_2"), DialogueOption("No", "hide_dialogue")));

					auto keypad_0_2 = []() {
						turnScannerOn(Location(1, -9, 4, 0, 0, 9));
						showDialogue("Info", "avatars_1/4/6.png", "...", "keypad_0_3");
					};

					scriptManager->addScript("keypad_0_2", keypad_0_2);

					auto keypad_0_3 = []() {
						if (player->getPersonality()->hasTrait(PersonalityTrait::INTROVERTED)) {
							doorProvider->getDoor("door_1_56")->unlock(0);
							turnScannerOff(Location(1, -9, 4, 0, 0, 9));
							hideDialogue();
							worldManager->removePromptedScript(Location(1, -9, 4, 0, 0, 9));
						} else {
							turnScannerOff(Location(1, -9, 4, 0, 0, 9));
							showDialogue("Info", "avatars_1/4/6.png", "You place your fingerprint on the scanner but nothing happens.", "keypad_0_4");
						}
					};

					scriptManager->addScript("keypad_0_3", keypad_0_3);

					scriptManager->addScript("keypad_0_4", std::bind(&showDialogue, "Info", "avatars_1/4/6.png", "What a waste of wall space.", "hide_dialogue"));
					worldManager->addPromptedScript("keypad_0_0", Location(1, -9, 4, 0, 0, 9));

					scriptManager->addScript("keypad_1_0", std::bind(&showDialogue, "Info", "avatars_1/4/6.png", "Put your finger onto the fingerprint sensor?", "keypad_1_1"));
					scriptManager->addScript("keypad_1_1", std::bind(&showTwoOptions, true, DialogueOption("Yes", "keypad_1_2"), DialogueOption("No", "hide_dialogue")));

					auto keypad_1_2 = []() {
						turnScannerOn(Location(1, -8, 4, 5, 0, 9));
						showDialogue("Info", "avatars_1/4/6.png", "...", "keypad_1_3");
					};

					scriptManager->addScript("keypad_1_2", keypad_1_2);

					auto keypad_1_3 = []() {
						if (player->getPersonality()->hasTrait(PersonalityTrait::EXTROVERTED)) {
							doorProvider->getDoor("door_1_57")->unlock(0);
							turnScannerOff(Location(1, -8, 4, 5, 0, 9));
							hideDialogue();
							worldManager->removePromptedScript(Location(1, -8, 4, 5, 0, 9));
						} else {
							turnScannerOff(Location(1, -8, 4, 5, 0, 9));
							showDialogue("Info", "avatars_1/4/6.png", "You place your fingerprint on the scanner but nothing happens.", "keypad_1_4");
						}
					};

					scriptManager->addScript("keypad_1_3", keypad_1_3);

					scriptManager->addScript("keypad_1_4", std::bind(&showDialogue, "Info", "avatars_1/4/6.png", "What a waste of wall space.", "hide_dialogue"));
					worldManager->addPromptedScript("keypad_1_0", Location(1, -8, 4, 5, 0, 9));

					auto dialogue_6_0 = []() {
						turnDisplayOn(Location(1, -10, 4, 3, 0, 2));
						dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "DID YOU KNOW?", "dialogue_6_1");
					};

					scriptManager->addScript("dialogue_6_0", dialogue_6_0);
					worldManager->addPromptedScript("dialogue_6_0", Location(1, -10, 4, 3, 0, 2));

					scriptManager->addScript("dialogue_6_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "THE PADS YOU HAVE USED TO REACH THIS KIOSK ARE SIMPLY CALLED \"TELEPORTERS\".", "dialogue_6_2"));
					scriptManager->addScript("dialogue_6_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "YOUR PROPULSION GUN USES A MINIMIZED VERSION OF THE TECHNOLOGY IN TELEPORTERS.", "dialogue_6_3"));
					scriptManager->addScript("dialogue_6_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "CURRENTLY, OXYLABS ENGINEERS ARE TRYING TO ADAPT THIS TECHNOLOGY FOR MAINSTREAM USE.", "dialogue_6_4"));

					auto dialogue_6_4 = []() {
						turnDisplayOff(Location(1, -10, 4, 3, 0, 2));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_6_4", dialogue_6_4);

					auto walked_on_3_0 = []() {
						// gameAudioPlayer->setStreamVolume("Another Day, Another Pay", 0.6f);
						worldManager->removeWalkedOnScript(Location(1, -9, 3, 11, 0, 2));
						worldManager->removeWalkedOnScript(Location(1, -9, 3, 11, 0, 1));
						Character* character = player->getCharacter();
						character->allowNewMovements(false);
						character->stopMoving();

						std::queue<TileDirection> movements;

						for (size_t i = 0; i < 30; i++) {
							movements.push(TileDirection::BACKWARD);
						}

						// Move other characters
						Character* soldier = characterProvider->getCharacterByName("soldier_1");
						soldier->addTileMovementsToQueue(movements);
						soldier->setCollidable(false);

						soldier = characterProvider->getCharacterByName("soldier_2");
						soldier->addTileMovementsToQueue(movements);
						soldier->setCollidable(false);

						soldier = characterProvider->getCharacterByName("soldier_3");
						soldier->addTileMovementsToQueue(movements);
						soldier->setCollidable(false);

						soldier = characterProvider->getCharacterByName("soldier_4");
						soldier->addTileMovementsToQueue(movements);
						soldier->setCollidable(false);

						Character* scientist = characterProvider->getCharacterByName("scientist_1");
						scientist->setCollidable(false);
						scientist->addTileMovementsToQueue(movements);

						scientist = characterProvider->getCharacterByName("scientist_2");
						scientist->setCollidable(false);
						scientist->addTileMovementsToQueue(movements);
						
						scientist = characterProvider->getCharacterByName("scientist_3");
						scientist->setCollidable(false);
						scientist->addTileMovementsToQueue(movements);

						auto turnEvent = [character]() {
							character->addTurnToQueue(TileDirection::FORWARD);
						};

						auto walked_on_3_1 = []() {
							player->lookAtWatch();
							player->getCharacter()->allowNewMovements(true);
							showBlinkingDialogue("Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "What on earth?", "walked_on_3_2");
						};

						timer->scheduleEventInSeconds(2, turnEvent);
						timer->scheduleEventInSeconds(4, walked_on_3_1);
					};

					scriptManager->addScript("walked_on_3_0", walked_on_3_0);
					worldManager->addWalkedOnScript("walked_on_3_0", Location(1, -9, 3, 11, 0, 2));
					worldManager->addWalkedOnScript("walked_on_3_0", Location(1, -9, 3, 11, 0, 1));

					scriptManager->addScript("walked_on_3_2", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "What are those soldiers doing with those scientists?", "walked_on_3_3"));
					scriptManager->addScript("walked_on_3_3", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "I'm going to be back soon, I need to alert the management about this.", "walked_on_3_3_1"));
					scriptManager->addScript("walked_on_3_3_1", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/0/3.png", "Don't worry - I won't mention our little incident.", "walked_on_3_4"));

					auto walked_on_3_4 = []() {
						player->stopLookingAtWatch();
						hideDialogue();
						// gameAudioPlayer->setStreamVolume("Another Day, Another Pay", 1);
					};

					scriptManager->addScript("walked_on_3_4", walked_on_3_4);

					auto button_1_1 = []() {
						Character* character = player->getCharacter();
						character->allowNewMovements(false);
						pressButton(Location(1, -10, 3, 6, 0, 3));
						gameSaver->disableLoading();

						auto event2 = []() {
							openDoor("metal door", 345, Location(1, -10, 3, 5, 0, 3));
						};

						auto event3 = [character]() {
							gameAudioPlayer->stopAllLoops();
							character->allowNewMovements(true);
							character->addTurnToQueue(TileDirection::RIGHT);
							character->addTileMovementToQueue(TileDirection::RIGHT);
							character->addTurnToQueue(TileDirection::FORWARD);
							character->addTileMovementToQueue(TileDirection::FORWARD);
							character->addTeleportToQueue(Location(0, 0, 0, 1.5f, 0, 0.5f), TeleportationAnimation::FADE, "hide_and_disable_gun");
							character->addTileMovementToQueue(TileDirection::FORWARD);
							character->addTurnToQueue(TileDirection::BACKWARD);
							character->allowNewMovements(false);
							animateElevator();
							player->getTileInventory()->clear(3);
							setupLevel(1, 3);
						};

						timer->scheduleEventInMillis(1000, event2);
						timer->scheduleEventInMillis(1500, event3);
					};

					scriptManager->addScript("button_1_1", button_1_1);
					worldManager->addPromptedScript("button_1_1", Location(1, -10, 3, 6, 0, 3));


					auto dialogue_9_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, -7, 1, 2, 0, 10));
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "DO YOU SEE THE CLOCK TO THE RIGHT OF THIS KIOSK?", "dialogue_9_1");
						}
					};

					scriptManager->addScript("dialogue_9_0", dialogue_9_0);
					worldManager->addPromptedScript("dialogue_9_0", Location(1, -7, 1, 2, 0, 10));

					scriptManager->addScript("dialogue_9_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "IT MEANS THAT PLACING A SLIME WILL OPEN THE TEAL DOOR FOR A LIMITED AMOUNT OF TIME!", "dialogue_9_2"));
					scriptManager->addScript("dialogue_9_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "MAKE SURE YOU CAN WALK THROUGH THE DOOR BEFORE IT CLOSES!", "dialogue_9_3"));
					scriptManager->addScript("dialogue_9_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "IF YOU TAKE THE SLIME BACK BEFORE THE DOOR CLOSES...", "dialogue_9_3_1"));
					scriptManager->addScript("dialogue_9_3_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "THE DOOR WILL STAY OPEN UNTIL TIME RUNS OUT!", "dialogue_9_4"));

					auto dialogue_9_4 = []() {
						turnDisplayOff(Location(1, -7, 1, 2, 0, 10));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_9_4", dialogue_9_4);

					auto dialogue_10_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, -12, 4, 13, 0, 1));
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "TIP: PAY ATTENTION TO WHERE A TELEPORTER WILL TAKE YOU!", "dialogue_10_1");
						}
					};

					scriptManager->addScript("dialogue_10_0", dialogue_10_0);
					worldManager->addPromptedScript("dialogue_10_0", Location(1, -12, 4, 13, 0, 1));

					auto dialogue_10_1 = []() {
						turnDisplayOff(Location(1, -12, 4, 13, 0, 1));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_10_1", dialogue_10_1);



					auto skip_dialogue_0_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							if (doorProvider->getDoor("door_skip_1_0")->isOpen()) {
								dialogueDisplay->showDialogue("Kiosk", "avatars_2/4/3.png", "THE DOOR TO YOUR LEFT IS OPEN. ENJOY.", "skip_dialogue_0_4");
							} else {
								turnDisplayOn(Location(1, -12, 3, 6, 0, 6));
								dialogueDisplay->showDialogue("Kiosk", "avatars_2/4/3.png", "IF THIS PUZZLE IS TOO HARD, THE COST OF SKIPPING IT IS 2 JIMMIES.", "skip_dialogue_0_1");
							}
						}
					};

					scriptManager->addScript("skip_dialogue_0_0", skip_dialogue_0_0);
					worldManager->addPromptedScript("skip_dialogue_0_0", Location(1, -12, 3, 6, 0, 6));

					auto skip_dialogue_0_1 = []() {
						showDialogue("Kiosk", "avatars_2/4/3.png", "YOU CURRENTLY HAVE " + std::to_string(player->getCharacter()->getMoney()) + " JIMMIES.", "skip_dialogue_0_2");
					};

					scriptManager->addScript("skip_dialogue_0_1", skip_dialogue_0_1);
					scriptManager->addScript("skip_dialogue_0_2", std::bind(&showTwoOptions, true, DialogueOption("Skip puzzle (2 Jimmies)", "skip_dialogue_0_3"), DialogueOption("Exit", "skip_dialogue_0_4")));

					auto skip_dialogue_0_3 = []() {
						if (player->getCharacter()->makePurchase(2)) {
							doorProvider->getDoor("door_skip_1_0")->unlock(0);
							doorProvider->getDoor("door_skip_1_1")->unlock(0);
							showDialogue("Kiosk", "avatars_2/4/3.png", "YOUR PURCHASE WAS SUCCESSFUL.", "skip_dialogue_0_4");
						} else {
							showDialogue("Kiosk", "avatars_2/4/3.png", "YOU LACK THE FUNS NECESSARY TO MAKE THE PURCHASE.", "skip_dialogue_0_2");
						}
					};

					scriptManager->addScript("skip_dialogue_0_3", skip_dialogue_0_3);

					auto skip_dialogue_0_4 = []() {
						turnDisplayOff(Location(1, -12, 3, 6, 0, 6));
						hideDialogue();
					};

					scriptManager->addScript("skip_dialogue_0_4", skip_dialogue_0_4);
					break;
				}
				case 3: {
					scriptManager->addScript("coffee_dialogue_2_0", std::bind(&showDialogue, "Plaque", "avatars_1/2/4.png", "The above framed newspaper highlights Oxylabs' first accomplishment:", "coffee_dialogue_2_1"));
					worldManager->addPromptedScript("coffee_dialogue_2_0", Location(0, -5, 1, 6, 0, 0));

					scriptManager->addScript("coffee_dialogue_2_1", std::bind(&showDialogue, "Plaque", "avatars_1/2/4.png", "breakthrough rocket technology.", "hide_dialogue"));

					auto coffee_dialogue_3_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Info", "", "Someone left their food in the microwave. Turn it on?", "coffee_dialogue_3_1");
						}
					};

					auto coffee_dialogue_3_2 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Info", "", "The food in the microwave looks appetizing.", "hide_dialogue");
						}
					};

					auto coffee_dialogue_3_3 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Info", "", "The food in the microwave does not look very appetizing anymore...", "hide_dialogue");
						}
					};

					scriptManager->addScript("coffee_dialogue_3_0", coffee_dialogue_3_0);
					scriptManager->addScript("coffee_dialogue_3_2", coffee_dialogue_3_2);
					scriptManager->addScript("coffee_dialogue_3_3", coffee_dialogue_3_3);
					worldManager->addPromptedScript("coffee_dialogue_3_0", Location(0, -5, 0, 10, 0, 15));

					scriptManager->addScript("coffee_dialogue_3_1", std::bind(&showTwoOptions, true, DialogueOption("Yes", "coffee_dialogue_4_0"), DialogueOption("No", "hide_dialogue")));

					auto coffee_dialogue_4_0 = []() {
						TileGroup* tileGroup = worldManager->getTileGroup(Location(0, -5, 0, 10, 1, 15));
						tileGroup->removeTile(384);
						tileGroup->addTile(393);
						worldManager->addPromptedScript("coffee_dialogue_3_2", Location(0, -5, 0, 10, 0, 15));

						auto splatter = [tileGroup]() {
							tileGroup->removeTile(393);
							tileGroup->addTile(394);
							worldManager->addPromptedScript("coffee_dialogue_3_3", Location(0, -5, 0, 10, 0, 15));

							// Add sound effect here?
							AudioClip* clip;
							if (resourceManager->obtain<AudioClip>("res/audio/clips/splurt.wav", clip)) {
								GameObjectProvider::getAudioPlayer()->playClip(clip);
							}
						};
						timer->scheduleEventInSeconds(10, splatter);

						hideDialogue();
					};
					scriptManager->addScript("coffee_dialogue_4_0", coffee_dialogue_4_0);

					auto television_1_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Info", "", "The news anchor is spouting propaganda.", "television_1_1");
						}
					};

					scriptManager->addScript("television_1_0", television_1_0);
					worldManager->addPromptedScript("television_1_0", Location(0, -5, 0, 13, 0, 15));
					scriptManager->addScript("television_1_1", std::bind(&showThreeOptions, false, DialogueOption("Watch the news", "television_1_2"), DialogueOption("Exit", "hide_dialogue"),
						DialogueOption("Read the scrolling text", "television_1_6")));
					scriptManager->addScript("television_1_2", std::bind(&showDialogue, "Television", "avatars_2/4/0.png", "\"Our president has done it once again.\"", "television_1_3"));
					scriptManager->addScript("television_1_3", std::bind(&showDialogue, "Television", "avatars_2/4/0.png", "\"Latest reports show a 50 percent crime reduction.\"", "television_1_4"));
					scriptManager->addScript("television_1_4", std::bind(&showDialogue, "Television", "avatars_2/4/0.png", "\"In addition, the supply of uranium has increased by 200 percent.\"", "television_1_5"));
					scriptManager->addScript("television_1_5", std::bind(&showDialogue, "Television", "avatars_2/4/0.png", "\"These are just some of the joys President Dragowska has brought us.\"", "television_1_1"));
					scriptManager->addScript("television_1_6", std::bind(&showDialogue, "Scrolling Text", "avatars_2/4/0.png", "ELECTION STATIONS PREPARE FOR 2 WEEK ELECTION PERIOD.", "television_1_7"));
					scriptManager->addScript("television_1_7", std::bind(&showDialogue, "Scrolling Text", "avatars_2/4/0.png", "MILITARY HERO JAMES PEPPERS MAKES SPEECH AT RONDO SQUARE.", "television_1_8"));
					scriptManager->addScript("television_1_8", std::bind(&showDialogue, "Scrolling Text", "avatars_2/4/0.png", "NEW MILITARY MUSUEM OPENS IN RADESLAVA.", "television_1_1"));

					auto alfie_0 = []() {
						srand((unsigned int) time->getCurrentTimeInNanos());
						int random = rand() % 2;
						if (random == 0) {
							showBlinkingDialogue("Drunk", "avatars_2/1/2.png", "avatars_2/1/3.png", "Don't touch me, bro.", "alfie_1");
						}
						if (random == 1) {
							showBlinkingDialogue("Drunk", "avatars_2/1/2.png", "avatars_2/1/3.png", "I'm...", "alfie_2");
						}
					};

					scriptManager->addScript("alfie_0", alfie_0);
					worldManager->addPromptedScript("alfie_0", Location(0, -5, 0, 12, 0, 15));

					scriptManager->addScript("alfie_1", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/1/2.png", "avatars_2/1/3.png", "I don't swing that way, Tommy.", "hide_dialogue"));
					scriptManager->addScript("alfie_2", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/1/2.png", "avatars_2/1/3.png", "I'm not...", "alfie_3"));
					scriptManager->addScript("alfie_3", std::bind(&showDialogue, "Drunk Italics", "avatars_2/1/3.png", "HICK!", "alfie_4"));
					scriptManager->addScript("alfie_4", std::bind(&showBlinkingDialogue, "Drunk", "avatars_2/1/2.png", "avatars_2/1/3.png", "I'm not drunk.", "hide_dialogue"));

					auto dialogue_11_0 = []() {
						player->lookAtWatch();
						worldManager->removeWalkedOnScript(Location(0, 0, 0, 1, 0, 1));
						player->getCharacter()->allowNewMovements(true);
						dialogueDisplay->showDialogue("Aela", "avatars_1/1/2.png", "avatars_1/1/3.png", "Everything is okay... Everything is okay...", "dialogue_11_0_1");
					};
					scriptManager->addScript("dialogue_11_0", dialogue_11_0);
					worldManager->addWalkedOnScript("dialogue_11_0", Location(0, 0, 0, 1, 0, 1));

					scriptManager->addScript("dialogue_11_0_1", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/2.png", "avatars_1/1/3.png", "We're going to move on to the next level.", "dialogue_11_0_2"));

					auto dialogue_11_0_2 = []() {
						dialogueDisplay->setTimeBetweenCharacterReveals(100);
						showBlinkingDialogue("Aela", "avatars_1/0/0.png", "avatars_1/0/1.png", "And NOBODY DIED.", "dialogue_11_0_3");
					};

					scriptManager->addScript("dialogue_11_0_2", dialogue_11_0_2);
					scriptManager->addScript("dialogue_11_0_3", std::bind(&showDialogue, "Aela", "avatars_1/0/1.png", "...", "dialogue_11_1"));

					auto dialogue_11_1 = []() {
						dialogueDisplay->resetTimeBetweenCharacterReveals();
						showBlinkingDialogue("Aela", "avatars_1/0/0.png", "avatars_1/0/1.png", "The next level was built in a space previously taken up by a multi-floor museum.", "dialogue_11_2");
					};

					scriptManager->addScript("dialogue_11_1", dialogue_11_1);
					scriptManager->addScript("dialogue_11_2", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/1/3.png", "The museum was abruptly shut down after a horrible toxic leak flooded it.", "dialogue_11_3"));
					scriptManager->addScript("dialogue_11_3", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/0/2.png", "avatars_1/1/3.png", "Certain areas of the museum are still intact, but you're not supposed to touch them.", "dialogue_11_4"));
					scriptManager->addScript("dialogue_11_4", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Well, our management doesn't want you to touch them, but I do not care.", "dialogue_11_5"));
					scriptManager->addScript("dialogue_11_5", std::bind(&showBlinkingDialogue, "Aela", "avatars_1/1/0.png", "avatars_1/1/1.png", "Enjoy the level.", "dialogue_11_6"));

					auto dialogue_11_6 = []() {
						player->stopLookingAtWatch();
						hideDialogue();
						stopAnimatingElevator();
					};

					scriptManager->addScript("dialogue_11_6", dialogue_11_6);

					auto walked_on_2_0 = []() {
						fadeTeleportPlayer(Location(0, -5, 0, 15.5f, 0, 15.5f));

					};

					scriptManager->addScript("walked_on_2_0", walked_on_2_0, true);
					worldManager->addWalkedOnScript("walked_on_2_0", Location(0, 0, -1, 1, 0, 15));

					auto walked_on_2_1 = []() {
						player->getCharacter()->addTeleportToQueue(Location(1, -15, 0, 13.5f, 0, 3.5f), TeleportationAnimation::FADE, "show_and_enable_gun");
						gameAudioPlayer->loopStreamInfinitely("Synths at an Exhibition", 130000, 3000);
					};

					scriptManager->addScript("walked_on_2_1", walked_on_2_1, true);
					worldManager->addWalkedOnScript("walked_on_2_1", Location(0, -5, 0, 6, 0, 11));

					auto dialogue_12_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnDisplayOn(Location(1, -15, 0, 10, 0, 3));
							dialogueDisplay->showDialogue("Kiosk", "avatars_1/0/4.png", "WELCOME TO LEVEL 3 OF THE PROPULSION GUN TESTING FACILITY.", "dialogue_12_1");
						}
					};

					scriptManager->addScript("dialogue_12_0", dialogue_12_0);
					worldManager->addPromptedScript("dialogue_12_0", Location(1, -15, 0, 10, 0, 3));

					scriptManager->addScript("dialogue_12_1", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "REMEMBER THE ADVICE \"THINK BEFORE YOUR ACT\"?", "dialogue_12_2"));
					scriptManager->addScript("dialogue_12_2", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "WELL NOW YOU MAY NOT EVEN HAVE THE TIME TO THINK!", "dialogue_12_3"));
					scriptManager->addScript("dialogue_12_3", std::bind(&showDialogue, "Kiosk", "avatars_1/0/4.png", "ANY MISTAKE OF YOURS COULD BE LETHAL, SO TRY YOUR BEST!", "dialogue_12_4"));

					auto dialogue_12_4 = []() {
						turnDisplayOff(Location(1, -15, 0, 10, 0, 3));
						hideDialogue();
					};

					scriptManager->addScript("dialogue_12_4", dialogue_12_4);

					auto dialogue_34_0 = []() {
						characterProvider->getCharacterByName("robot_1")->turn(player->getCharacter()->getOppositeDirectionFacing());
						dialogueDisplay->showDialogue("Brobot", "avatars_2/5/0.png", "Bleep, bloop!", "dialogue_34_1");
					};

					scriptManager->addScript("dialogue_34_0", dialogue_34_0);
					worldManager->addPromptedScript("dialogue_34_0", Location(1, -16, 0, 15, 0, 11));

					scriptManager->addScript("dialogue_34_1", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "Hi there, visitor!", "dialogue_34_2"));
					scriptManager->addScript("dialogue_34_2", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "My name is Brobot!", "dialogue_34_3"));
					scriptManager->addScript("dialogue_34_3", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "Welcome to the Oxylabs museum!", "dialogue_34_4"));
					scriptManager->addScript("dialogue_34_4", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "This month's exhibit is: the teleporter!", "dialogue_34_5"));
					scriptManager->addScript("dialogue_34_5", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "If you need help, you may ask a question to a Brobot.", "dialogue_34_6"));
					scriptManager->addScript("dialogue_34_6", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "There are several of us in this museum!", "hide_dialogue"));

					auto dialogue_35_0 = []() {
						characterProvider->getCharacterByName("robot_2")->turn(player->getCharacter()->getOppositeDirectionFacing());
						dialogueDisplay->showDialogue("Brobot", "avatars_2/5/0.png", "Bleep, bloop!", "dialogue_35_1");
					};

					scriptManager->addScript("dialogue_35_0", dialogue_35_0);
					worldManager->addPromptedScript("dialogue_35_0", Location(1, -17, 3, 7, 0, 7));

					scriptManager->addScript("dialogue_35_1", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "Hi there!", "dialogue_35_2"));
					scriptManager->addScript("dialogue_35_2", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "My name is Brobot!", "dialogue_35_3"));
					scriptManager->addScript("dialogue_35_3", std::bind(&showFourOptions, true, DialogueOption("Where am I?", "dialogue_35_4"), DialogueOption("Humour me.", "dialogue_35_7"), DialogueOption("Where is the exit?", "dialogue_35_6"), DialogueOption("Goodbye.", "hide_dialogue")));
					scriptManager->addScript("dialogue_35_4", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "You are in the teleporter exhibit!", "dialogue_35_5"));
					scriptManager->addScript("dialogue_35_5", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "In this exhibit, you can learn about Oxylabs' latest invention!", "dialogue_35_3"));
					scriptManager->addScript("dialogue_35_6", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "The exit to the exhibit is up and to the left!", "dialogue_35_3"));
					scriptManager->addScript("dialogue_35_7", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "What is a robot's favourite type of music?", "dialogue_35_8"));
					scriptManager->addScript("dialogue_35_8", std::bind(&showDialogue, "Brobot", "avatars_2/5/2.png", "Heavy metal!", "dialogue_35_9"));
					scriptManager->addScript("dialogue_35_9", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "I know, it makes me cringe too.", "dialogue_35_10"));
					scriptManager->addScript("dialogue_35_10", std::bind(&showDialogue, "Brobot", "avatars_2/5/0.png", "Unfortunately, my programmers forced me to use that joke.", "dialogue_35_3"));

					auto dialogue_36_0 = []() {
						characterProvider->getCharacterByName("robot_3")->turn(player->getCharacter()->getOppositeDirectionFacing());
						dialogueDisplay->showDialogue("Brobot", "avatars_2/5/1.png", "Unhandled exception at 0x000000000FDC10D0!", "dialogue_36_1");
					};

					scriptManager->addScript("dialogue_36_0", dialogue_36_0);
					worldManager->addPromptedScript("dialogue_36_0", Location(1, -23, 1, 13, 0, 1));

					scriptManager->addScript("dialogue_36_1", std::bind(&showDialogue, "Brobot", "avatars_2/5/1.png", "Access violation reading location 0x000000008169DB38.", "hide_dialogue"));

					scriptManager->addScript("dialogue_37_0", std::bind(&showDialogue, "Soldier", "avatars_2/2/0.png", "Just hang on there!", "dialogue_37_1"));
					worldManager->addPromptedScript("dialogue_37_0", Location(1, -22, 1, 5, 0, 13));

					scriptManager->addScript("dialogue_37_1", std::bind(&showDialogue, "Soldier", "avatars_2/2/0.png", "Keep on moving your legs in the bicycle motion!", "dialogue_37_2"));
					scriptManager->addScript("dialogue_37_2", std::bind(&showDialogue, "Soldier", "avatars_2/2/0.png", "Trevor will come back with help any moment now!", "dialogue_37_3"));
					scriptManager->addScript("dialogue_37_3", std::bind(&showDialogue, "Soldier", "avatars_2/2/0.png", "This stuff may be like quicksand, but its not deep enough to drown in!", "dialogue_37_4"));

					auto dialogue_37_4 = []() {
						characterProvider->getCharacterByName("soldier_12")->turn(player->getCharacter()->getOppositeDirectionFacing());
						dialogueDisplay->showDialogue("Soldier", "avatars_2/2/0.png", "...", "dialogue_37_5");
					};

					scriptManager->addScript("dialogue_37_4", dialogue_37_4);

					scriptManager->addScript("dialogue_37_5", std::bind(&showDialogue, "Soldier", "avatars_2/2/0.png", "I don't have the time for you.", "dialogue_37_6"));
					scriptManager->addScript("dialogue_37_6", std::bind(&showDialogue, "Soldier", "avatars_2/2/0.png", "What's the advantage of capturing one more if your best friend is drowning?", "dialogue_37_7"));

					auto dialogue_37_7 = []() {
						characterProvider->getCharacterByName("soldier_12")->turn(TileDirection::LEFT);
						hideDialogue();
					};

					scriptManager->addScript("dialogue_37_7", dialogue_37_7);

					auto dialogue_38_0 = []() {
						showDialogue("Soldier", "avatars_2/2/3.png", "Hmmm...", "dialogue_38_1");
						gameAudioPlayer->stopAllLoops();
					};

					scriptManager->addScript("dialogue_38_0", dialogue_38_0);
					worldManager->addPromptedScript("dialogue_38_0", Location(1, -17, 0, 7, 0, 13));
					scriptManager->addScript("dialogue_38_1", std::bind(&showDialogue, "Soldier", "avatars_2/2/3.png", "That toxic leak really ruined this place...", "dialogue_38_2"));
					scriptManager->addScript("dialogue_38_2", std::bind(&showDialogue, "Soldier", "avatars_2/2/3.png", "And they repurposed it for some senseless tests?", "dialogue_38_3"));

					auto dialogue_38_3 = []() {
						characterProvider->getCharacterByName("soldier_14")->turn(TileDirection::BACKWARD);
						dialogueDisplay->showDialogue("Soldier", "avatars_2/2/1.png", "...", "dialogue_38_4");
					};

					scriptManager->addScript("dialogue_38_3", dialogue_38_3);

					scriptManager->addScript("dialogue_38_4", std::bind(&showDialogue, "Soldier", "avatars_2/2/1.png", "What'cha got in your hands, punk?", "dialogue_38_5"));
					scriptManager->addScript("dialogue_38_5", std::bind(&showTwoOptions, true, DialogueOption("Your fate.", "dialogue_38_6"), DialogueOption("Death's scythe.", "dialogue_38_7")));
					scriptManager->addScript("dialogue_38_6", std::bind(&showDialogue, "Soldier", "avatars_2/2/1.png", "We'll see about that.", "dialogue_38_9"));
					scriptManager->addScript("dialogue_38_7", std::bind(&showDialogue, "Soldier", "avatars_2/2/1.png", "...", "dialogue_38_8"));
					scriptManager->addScript("dialogue_38_8", std::bind(&showDialogue, "Soldier", "avatars_2/2/1.png", "What?", "dialogue_38_9"));

					auto dialogue_38_9 = []() {
						game->startBattle("battle_4");
					};

					scriptManager->addScript("dialogue_38_9", dialogue_38_9);

					auto dialogue_38_10 = []() {
						characterProvider->getCharacterByName("soldier_14")->kill();
						worldManager->removePromptedScript(Location(1, -17, 0, 7, 0, 13));
					};

					scriptManager->addScript("dialogue_38_10", dialogue_38_10);

					auto dialogue_38_11 = []() {
						player->lookAtWatch();
						showDialogue("Watch", "avatars_1/3/4.png", "...", "dialogue_38_12");
					};

					scriptManager->addScript("dialogue_38_11", dialogue_38_11);

					scriptManager->addScript("dialogue_38_12", std::bind(&showDialogue, "Watch", "avatars_1/3/4.png", "You seem to have lost your connection with Aela.", "dialogue_38_13"));

					auto dialogue_38_13 = []() {
						hideDialogue();
						player->stopLookingAtWatch();
						gameAudioPlayer->loopStreamInfinitely("Synths at an Exhibition", 130000, 3000);
					};

					scriptManager->addScript("dialogue_38_13", dialogue_38_13);

					scriptManager->addScript("dialogue_36_1", std::bind(&showDialogue, "Brobot", "avatars_2/5/1.png", "Access violation reading location 0x000000008169DB38.", "hide_dialogue"));

					auto dialogue_13_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Plaque", "avatars_1/1/4.png", "\"Hills outside of the first Oxylabs facility - 1905\"", "hide_dialogue");
						}
					};

					scriptManager->addScript("dialogue_13_0", dialogue_13_0);
					worldManager->addPromptedScript("dialogue_13_0", Location(1, -16, 1, 14, 0, 1));
					
					scriptManager->addScript("dialogue_14_0", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "\"The Beauty of Memester Bay - 1908\"", "dialogue_14_1"));
					worldManager->addPromptedScript("dialogue_14_0", Location(1, -19, 1, 15, 0, 6));
					scriptManager->addScript("dialogue_14_1", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "The following depicts Memester Bay - a rocket launch site of Oxylabs rocketry.", "hide_dialogue"));
					
					scriptManager->addScript("dialogue_15_0", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "\"Firstname Lastname - 1907\"", "dialogue_15_1"));
					worldManager->addPromptedScript("dialogue_15_0", Location(1, -21, 1, 14, 0, 5));
					scriptManager->addScript("dialogue_15_1", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "This is a portrait of Arthur Wright's fiancé.", "dialogue_15_2"));
					scriptManager->addScript("dialogue_15_2", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "She was supportive of Mr Wright's hard-working spirit.", "dialogue_15_3"));
					scriptManager->addScript("dialogue_15_3", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "Unfortunately, she died alongside Mr Wright due to an automative accident in 1909.", "hide_dialogue"));

					auto dialogue_16_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Plaque", "avatars_1/1/4.png", "\"A Picture of Arthur Wright on Kozack's first colour camera - 1909\"", "dialogue_16_1");
						}
					};

					scriptManager->addScript("dialogue_16_0", dialogue_16_0);
					worldManager->addPromptedScript("dialogue_16_0", Location(1, -17, 3, 9, 0, 11));
					scriptManager->addScript("dialogue_16_1", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "During testing, a scientist set two teleporters to teleport to each other.", "dialogue_16_2"));
					scriptManager->addScript("dialogue_16_2", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "One of his unsuspecting coworkers walked into one of those teleporters.", "dialogue_16_3"));
					scriptManager->addScript("dialogue_16_3", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "The coworker ended up teleporting in an endless loop.", "dialogue_16_4"));
					scriptManager->addScript("dialogue_16_4", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "The following photo depicts Mr Wright's reaction to the mess.", "hide_dialogue"));

					auto dialogue_17_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Plaque", "avatars_1/1/4.png", "Mr Wright got the idea for the Propulsion Gun after seeing the film this poster depicts.", "hide_dialogue");
						}
					};

					scriptManager->addScript("dialogue_17_0", dialogue_17_0);
					worldManager->addPromptedScript("dialogue_17_0", Location(1, -17, 3, 4, 0, 11));
					
					scriptManager->addScript("dialogue_18_0", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "The following is an early prototype of a teleporter.", "dialogue_18_1"));
					worldManager->addPromptedScript("dialogue_18_0", Location(1, -17, 3, 9, 0, 14));
					scriptManager->addScript("dialogue_18_1", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "The first few prototypes had to be tethered to a large computer.", "hide_dialogue"));
					
					scriptManager->addScript("dialogue_19_0", std::bind(&showDialogue, "Plaque", "avatars_1/1/4.png", "This is what a teleporter looks like before its paint is applied.", "hide_dialogue"));
					worldManager->addPromptedScript("dialogue_19_0", Location(1, -17, 3, 4, 0, 14));
					
					scriptManager->addScript("dialogue_20_0", std::bind(&showDialogue, "Plaque", "avatars_1/2/4.png", "Exhibit of the teleporter, right this way! Includes free cake!", "hide_dialogue"));
					worldManager->addPromptedScript("dialogue_20_0", Location(1, -17, 0, 8, 0, 13));
					
					scriptManager->addScript("dialogue_21_0", std::bind(&showDialogue, "Plaque", "avatars_1/2/4.png", "Exhibit of the teleporter, right this way! Fun for the whole extended family!", "hide_dialogue"));
					worldManager->addPromptedScript("dialogue_21_0", Location(1, -17, 0, 6, 0, 13));
					
					scriptManager->addScript("dialogue_22_0", std::bind(showDialogue, "Plaque", "avatars_1/2/4.png", "Three coloured robots, two stole your dye.", "dialogue_22_1"));
					worldManager->addPromptedScript("dialogue_22_0", Location(1, -16, 3, 4, 0, 13));
					scriptManager->addScript("dialogue_22_1", std::bind(showDialogue, "Plaque", "avatars_1/2/4.png", "They say those two tend to always lie.", "dialogue_22_2"));
					scriptManager->addScript("dialogue_22_2", std::bind(showDialogue, "Plaque", "avatars_1/2/4.png", "The one in blue begs \"Green is innocent. Don't make him cry!\"", "dialogue_22_3"));
					scriptManager->addScript("dialogue_22_3", std::bind(showDialogue, "Plaque", "avatars_1/2/4.png", "The one in green claims \"Red is guilty and deserves to die!\"", "dialogue_22_4"));
					scriptManager->addScript("dialogue_22_4", std::bind(showDialogue, "Plaque", "avatars_1/2/4.png", "You ask Red for her alibi.", "dialogue_22_5"));
					scriptManager->addScript("dialogue_22_5", std::bind(showDialogue, "Plaque", "avatars_1/2/4.png", "But Red refuses to reply.", "dialogue_22_6"));
					scriptManager->addScript("dialogue_22_6", std::bind(showDialogue, "Plaque", "avatars_1/2/4.png", "Three coloured robots, two stole your die.", "dialogue_22_7"));
					scriptManager->addScript("dialogue_22_7", std::bind(showDialogue, "Plaque", "avatars_1/2/4.png", "Which is the one that does not lie?", "hide_dialogue"));

					auto dialogue_23_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Plaque", "avatars_1/1/4.png", "This is a portrait of Mindy Cooper, who was engaged to Oxylabs founder Arthur Wright.", "dialogue_23_1");
						}
					};

					scriptManager->addScript("dialogue_23_0", dialogue_23_0);
					worldManager->addPromptedScript("dialogue_23_0", Location(1, -17, 0, 5, 0, 10));
					scriptManager->addScript("dialogue_23_1", std::bind(showDialogue, "Plaque", "avatars_1/1/4.png", "She was known for her bright spirit.", "dialogue_23_2"));
					scriptManager->addScript("dialogue_23_2", std::bind(showDialogue, "Plaque", "avatars_1/1/4.png", "Mindy was killed alongside Mr Wright in a car crash before the launch of Project Bravo.", "hide_dialogue"));

					auto dialogue_24_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Plaque", "avatars_1/1/4.png", "The following is a painting of a beach that Project Alpha rockets are launched from.", "hide_dialogue");
						}
					};

					scriptManager->addScript("dialogue_24", dialogue_24_0);
					worldManager->addPromptedScript("dialogue_24", Location(1, -17, 0, 3, 0, 10));

					scriptManager->addScript("dialogue_25_0", std::bind(showDialogue, "Sign", "avatars_1/2/4.png", "Right: Oxylabs Gift Shop", "dialogue_25_1"));
					worldManager->addPromptedScript("dialogue_25_0", Location(1, -22, 1, 2, 0, 2));
					scriptManager->addScript("dialogue_25_1", std::bind(showDialogue, "Sign", "avatars_1/2/4.png", "Ahead: Exit", "hide_dialogue"));

					auto dialogue_26_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("", "", "The news anchor seems to be reporting on politics.", "dialogue_26_1");
						}
					};

					scriptManager->addScript("dialogue_26_0", dialogue_26_0);
					worldManager->addPromptedScript("dialogue_26_0", Location(1, -23, 1, 1, 0, 3));
					scriptManager->addScript("dialogue_26_1", std::bind(&showThreeOptions, false, DialogueOption("Watch the news", "dialogue_26_2"), DialogueOption("Exit", "hide_dialogue"), DialogueOption("Read the scolling text", "dialogue_26_9")));
					scriptManager->addScript("dialogue_26_2", std::bind(showDialogue, "Television", "avatars_2/4/0.png", "\"Today, our ruling party, PARADOX, held a rally near Hamoa Beach.\"", "dialogue_26_3"));
					scriptManager->addScript("dialogue_26_3", std::bind(showDialogue, "Television", "avatars_2/4/0.png", "\"Their leader, President Dragowska, made some remarkable quotes amid her supporters:\"", "dialogue_26_4"));
					scriptManager->addScript("dialogue_26_4", std::bind(showDialogue, "Television", "avatars_2/4/0.png", "\"Together, we have done the impossible.\"", "dialogue_26_5"));
					scriptManager->addScript("dialogue_26_5", std::bind(showDialogue, "Television", "avatars_2/4/0.png", "\"Our last twenty eight years have seen remarkable growth.\"", "dialogue_26_6"));
					scriptManager->addScript("dialogue_26_6", std::bind(showDialogue, "Television", "avatars_2/4/0.png", "\"Such a decrease in crime and unemployment will never come from little Neroy.\"", "dialogue_26_7"));
					scriptManager->addScript("dialogue_26_7", std::bind(showDialogue, "Television", "avatars_2/4/0.png", "\"The leader of the opposition, Neroy Ladaby, made several accusations today.\"", "dialogue_26_8"));
					scriptManager->addScript("dialogue_26_8", std::bind(showDialogue, "Television", "avatars_2/4/0.png", "\"According to a poll of Checkmark Corp., there is a 97.3% chance of Dragowska winning.\"", "dialogue_26_1"));
					scriptManager->addScript("dialogue_26_9", std::bind(showDialogue, "Scrolling Text", "avatars_2/4/0.png", "NEW STUDY REVEALS 40% INCREASE IN JOBS IN THE LAST TEN YEARS.", "dialogue_26_10"));
					scriptManager->addScript("dialogue_26_10", std::bind(showDialogue, "Scrolling Text", "avatars_2/4/0.png", "TORNADO DESTROYS 10000 HOMES IN THE SOUTH WEST.", "dialogue_26_11"));
					scriptManager->addScript("dialogue_26_11", std::bind(showDialogue, "Scrolling Text", "avatars_2/4/0.png", "MILITARY SHUTS DOWN OXYLABS FOR CONSPIRING AGAINST THE GOVERNMENT.", "dialogue_26_12"));
					scriptManager->addScript("dialogue_26_12", std::bind(showDialogue, "Scrolling Text", "avatars_2/4/0.png", "VIDEO GAME GRAPE ESCAPE TAKEN OFF OF MARKET DUE TO HIDDEN OFFENSIVE CONTENT.", "dialogue_26_1"));

					auto dialogue_27_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("", "", "...", "dialogue_27_1");
						}
					};

					scriptManager->addScript("dialogue_27_0", dialogue_27_0);
					worldManager->addPromptedScript("dialogue_27_0", Location(1, -23, 1, 10, 0, 4));
					scriptManager->addScript("dialogue_27_1", std::bind(showDialogue, "", "", "The fridge door seems to be locked.", "hide_dialogue"));

					auto dialogue_28_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("", "", "These snacks have some sort of fungus growing out from them.", "hide_dialogue");
						}
					};

					scriptManager->addScript("dialogue_28_0", dialogue_28_0);
					worldManager->addPromptedScript("dialogue_28_0", Location(1, -23, 1, 8, 0, 4));
					worldManager->addPromptedScript("dialogue_28_0", Location(1, -23, 1, 6, 0, 4));

					auto button_2_0 = []() {
						Character* character = player->getCharacter();
						character->allowNewMovements(false);
						pressButton(Location(1, -22, 5, 2, 0, 14));

						auto event2 = []() {
							openDoor("metal door", 345, Location(1, -22, 5, 1, 0, 14));
						};

						auto event3 = [character]() {
							gameAudioPlayer->stopAllLoops();
							character->allowNewMovements(true);
							character->addTurnToQueue(TileDirection::RIGHT);
							character->addTileMovementToQueue(TileDirection::RIGHT);
							character->addTurnToQueue(TileDirection::FORWARD);
							character->addTileMovementToQueue(TileDirection::FORWARD);
							character->addTeleportToQueue(Location(0, 0, 0, 1.5f, 0, 0.5f), TeleportationAnimation::FADE);
							character->addTileMovementToQueue(TileDirection::FORWARD);
							character->addTurnToQueue(TileDirection::BACKWARD);
							character->allowNewMovements(false);
							animateElevator();
							setupLevel(2, 1);
						};

						auto event4 = [character]() {
							character->allowNewMovements(true);
							stopAnimatingElevator();
						};

						timer->scheduleEventInMillis(1000, event2);
						timer->scheduleEventInMillis(1500, event3);
						timer->scheduleEventInMillis(6000, event4);
					};

					scriptManager->addScript("button_2_0", button_2_0);
					// worldManager->addPromptedScript("button_2_0", Location(1, -22, 5, 2, 0, 14));

					scriptManager->addScript("television_2_0", std::bind(&showDialogue, "Info", "",
						"It seems as if the news anchor accidentally said a swear word.", "television_2_1"));
					worldManager->addPromptedScript("television_2_0", Location(0, -5, 1, 8, 0, 9));
					scriptManager->addScript("television_2_1", std::bind(&showThreeOptions, false, DialogueOption("Watch the news", "television_2_2"), DialogueOption("Exit", "hide_dialogue"),
						DialogueOption("Read the scrolling text", "television_2_6")));
					scriptManager->addScript("television_2_2", std::bind(&showDialogue, "Television", "", "...", "television_2_3"));
					scriptManager->addScript("television_2_3", std::bind(&showDialogue, "Television", "", "The news anchor is not saying anything.", "television_2_4"));
					scriptManager->addScript("television_2_4", std::bind(&showDialogue, "Television", "", "Instead, they're just... sitting still.", "television_2_5"));
					scriptManager->addScript("television_2_5", std::bind(&showDialogue, "Television", "", "A blunder like that could be punishable by death.", "television_2_1"));
					scriptManager->addScript("television_2_6", std::bind(&showDialogue, "Scrolling Text", "avatars_2/4/0.png", "MASSIVE RECALL ON ALL LAB-MADE FELINES MADE BY COMPANY QUADRUPED.", "television_2_7"));
					scriptManager->addScript("television_2_7", std::bind(&showDialogue, "Scrolling Text", "avatars_2/4/0.png", "MUSICIAN AND SERGEANT AL GALORE DIES AT THE AGE OF 91.", "television_2_8"));
					scriptManager->addScript("television_2_8", std::bind(&showDialogue, "Scrolling Text", "avatars_2/4/0.png", "REPORTER FIRED AFTER MISPRONOUNCING WORD \"FIRE TRUCK\".", "television_2_1"));

					auto soldier_boss_0_0 = []() {
						dialogueDisplay->showDialogue("Soldier", "avatars_2/2/2.png", "Hello, " + PLAYER_NAME + ".", "soldier_boss_0_1");
						worldManager->removePromptedScript(Location(1, -22, 5, 1.5f, 0, 13.5f));
						gameAudioPlayer->stopAllLoops();
						characterProvider->getCharacterByName("soldier_15")->turn(player->getCharacter()->getOppositeDirectionFacing());
					};

					scriptManager->addScript("soldier_boss_0_0", soldier_boss_0_0);
					worldManager->addPromptedScript("soldier_boss_0_0", Location(1, -22, 5, 1.5f, 0, 13.5f));

					scriptManager->addScript("soldier_boss_0_1", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "The party is over.", "soldier_boss_0_2"));
					scriptManager->addScript("soldier_boss_0_2", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "The government, and the world, know of your sins.", "soldier_boss_0_3"));
					scriptManager->addScript("soldier_boss_0_3", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "At first, we were trying to arrest you.", "soldier_boss_0_4"));
					scriptManager->addScript("soldier_boss_0_4", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "However, you refused to give in.", "soldier_boss_0_5"));
					scriptManager->addScript("soldier_boss_0_5", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "You ruthlessly murdered my men.", "soldier_boss_0_6"));
					scriptManager->addScript("soldier_boss_0_6", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "You've become an infamous legend amongst soldiers, and that is not good in any way.", "soldier_boss_0_7"));
					scriptManager->addScript("soldier_boss_0_7", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "Oxylabs may have been unethical for the sake of science, but you had no motive.", "soldier_boss_0_8"));
					scriptManager->addScript("soldier_boss_0_8", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "It was not self defense - we were merely trying to arrest you.", "soldier_boss_0_9"));
					scriptManager->addScript("soldier_boss_0_9", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "Instead, you killed without reason.", "soldier_boss_0_10"));
					scriptManager->addScript("soldier_boss_0_10", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "Based on your records, we would have set you free.", "soldier_boss_0_11"));
					scriptManager->addScript("soldier_boss_0_11", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "But now I have the following orders from my lieutenant:", "soldier_boss_0_12"));
					scriptManager->addScript("soldier_boss_0_12", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "\"Rid him off the face of this planet, it will do the world a bit of good.\"", "soldier_boss_0_13"));
					scriptManager->addScript("soldier_boss_0_13", std::bind(&showTwoOptions, true, DialogueOption("You better watch what you say next.", "soldier_boss_0_14"), DialogueOption("Bro, chill.", "soldier_boss_0_24")));
					scriptManager->addScript("soldier_boss_0_14", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "Don't try and sweet talk me.", "soldier_boss_0_15"));
					scriptManager->addScript("soldier_boss_0_15", std::bind(&showTwoOptions, true, DialogueOption("You're quite arrogant.", "soldier_boss_0_16"), DialogueOption("I'm just trying to be peaceful.", "soldier_boss_0_19")));
					scriptManager->addScript("soldier_boss_0_16", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "That's it.", "soldier_boss_0_17"));
					scriptManager->addScript("soldier_boss_0_17", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "You're done.", "soldier_boss_0_18"));
					scriptManager->addScript("soldier_boss_0_18", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "It's over.", "soldier_boss_0_29"));
					scriptManager->addScript("soldier_boss_0_19", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "Peaceful?!", "soldier_boss_0_20"));
					scriptManager->addScript("soldier_boss_0_20", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "Don't make me laugh!", "soldier_boss_0_21"));
					scriptManager->addScript("soldier_boss_0_21", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "Do you want to know the real hilarity of this situation?", "soldier_boss_0_22"));
					scriptManager->addScript("soldier_boss_0_22", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "I was told to find and kill you.", "soldier_boss_0_23"));

					auto soldier_boss_0_23 = []() {
						dialogueDisplay->setTimeBetweenCharacterReveals(100);
						showDialogue("Soldier", "avatars_2/2/2.png", "Yet you're the one who found me.", "soldier_boss_0_29");
					};

					scriptManager->addScript("soldier_boss_0_23", soldier_boss_0_23);

					auto soldier_boss_0_24 = []() {
						// dialogueDisplay->setTimeBetweenCharacterReveals(15);
						showDialogue("Soldier", "avatars_2/2/2.png", "OH, YOU THINK THIS IS FUNNY, HUH?", "soldier_boss_0_25");
					};

					scriptManager->addScript("soldier_boss_0_24", soldier_boss_0_24);
					scriptManager->addScript("soldier_boss_0_25", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "YOU ARROGANT PUNK!", "soldier_boss_0_26"));
					scriptManager->addScript("soldier_boss_0_26", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "BRO, CHILL?!", "soldier_boss_0_27"));
					scriptManager->addScript("soldier_boss_0_27", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "YOU WANT TO KNOW WHAT REALLY IS FUNNY?", "soldier_boss_0_28"));
					scriptManager->addScript("soldier_boss_0_28", std::bind(&showDialogue, "Soldier", "avatars_2/2/2.png", "I WAS TOLD TO FIND AND KILL YOU!", "soldier_boss_0_23"));

					auto soldier_boss_0_29 = []() {
						dialogueDisplay->resetTimeBetweenCharacterReveals();
						game->startBattle("battle_5");
					};

					scriptManager->addScript("soldier_boss_0_29", soldier_boss_0_29);

					auto soldier_boss_0_30 = []() {
						// Idk why, but the transition to the end screen seems to show elements of the regular gameplay world for a frame or two.
						// That is, unless I do the stuff directly below this comment:
						characterProvider->getCharacterByName("soldier_15")->changeLocation(Location(1, 0, 0, 0, 0, 0));
						tileInventoryDisplay->hide();
						hideDialogue();
						game->showBlackness();

						// Regular kill still seems to contribute to the bug mentioned above. killImmediately() may work, but I'm too lazy to check again.
						// characterProvider->getCharacterByName("soldier_15")->kill();

						player->getCharacter()->setTexture("shadow");
						player->getCharacter()->changeLocation(Location(0, 3, 0, 0, 0, 0));
						player->getCharacter()->addTurnToQueue(TileDirection::FORWARD);
					};

					scriptManager->addScript("soldier_boss_0_30", soldier_boss_0_30);

					auto soldier_boss_0_31 = []() {
						animator->delete2DTracksByTag("game_fade");
						game->showBlackness();
						// player->getCharacter()->allowNewMovements(true);
						dialogueDisplay->resetTimeBetweenCharacterReveals();
						// dialogueDisplay->setTimeBetweenCharacterReveals(100);
						tileInventoryDisplay->hide();
						player->getCharacter()->allowNewMovements(false);

						auto event = []() {
							dialogueDisplay->showDialogue("???", "", "All I can say is: I'm surprised.", "soldier_boss_0_32");
						};
						timer->scheduleEventInMillis(2500, event);
					};

					scriptManager->addScript("soldier_boss_0_31", soldier_boss_0_31);

					auto soldier_boss_0_32 = []() {
						game->fadeIn(3000);
						showDialogue("???", "", "With such little time, you brought a small terror into this world.", "soldier_boss_0_33");
					};

					scriptManager->addScript("soldier_boss_0_32", soldier_boss_0_32);
					scriptManager->addScript("soldier_boss_0_33", std::bind(&showDialogue, "???", "", "How far will you go once you reach full funding?", "soldier_boss_0_34"));
					scriptManager->addScript("soldier_boss_0_34", std::bind(&showDialogue, "???", "", "Currently, all we can do now is speculate.", "soldier_boss_0_35"));
					scriptManager->addScript("soldier_boss_0_35", std::bind(&showDialogue, "???", "", "But for your sake, I hope we get to find out.", "credits"));

					auto credits = []() {
						game->fadeOut(3000);
						hideDialogue();
						/*auto event = []() {
							GameObjectProvider::getWindow()->quit();
						};

						timer->scheduleEventInSeconds(4, event);*/
						auto event2 = []() {
							Font* pressStart2P;

							if (!resourceManager->obtain<Font>("res/fonts/pressStart2P.ttf", pressStart2P)) {
								AelaErrorHandling::windowError("A critical font (pressStart2P.ttf) could not be loaded, aborting!");
								return;
							}

							ColourRGBA almostWhite(0.9f, 0.9f, 0.9f, 1.0f);
							auto endText = std::make_shared<Label>("Thank you for playing the Neo-Zero demo!", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, &almostWhite);
							endText->setPositioningMode(PositioningMode2D::CENTER);
							endText->getDimensions()->setXY(512, 372);
							endText->show();
							gameplayScene->getMenu()->add(endText);

							auto endText2 = std::make_shared<Label>("www.kickstarter.com/projects/robert-ciborowski/neo-zero", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, &almostWhite);
							endText2->setPositioningMode(PositioningMode2D::CENTER);
							endText2->getDimensions()->setXY(512, 396);
							endText2->show();
							gameplayScene->getMenu()->add(endText2);
						};
						timer->scheduleEventInSeconds(5, event2);

						auto event3 = []() {
							game->setFullscreen(false);
						};
						timer->scheduleEventInSeconds(13, event3);
					};

					scriptManager->addScript("credits", credits);

					auto dialogue_39_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							showDialogue("Sign", "avatars_1/2/4.png", "Museum Warehouse - Staff Only!", "hide_dialogue");
						}
					};

					scriptManager->addScript("dialogue_39_0", dialogue_39_0);
					worldManager->addPromptedScript("dialogue_39_0", Location(1, -22, 1, 0, 0, 15));

					auto telecom_0_0 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							turnTelecomOn(Location(1, -23, 1, 12, 1, 2));
							showDialogue("Telecom", "avatars_2/3/2.png", "YOU HAVE " + std::to_string(player->getCharacter()->getMoney()) + " JIMMIES.", "telecom_0_0_1");
						}
					};

					scriptManager->addScript("telecom_0_0", telecom_0_0);
					worldManager->addPromptedScript("telecom_0_0", Location(1, -23, 1, 12, 0, 2));

					scriptManager->addScript("telecom_0_0_1", std::bind(&showDialogue, "Telecom", "avatars_2/3/2.png", "YOU CAN CALL ANYONE FOR ONLY 2 JIMMIES.", "telecom_0_1"));

					scriptManager->addScript("telecom_0_1", std::bind(&showTwoOptions, true, DialogueOption("Call Brother", "telecom_0_2"), DialogueOption("Exit", "telecom_0_3")));

					auto telecom_0_2 = []() {
						if (player->getCharacter()->makePurchase(2)) {
							showDialogue("Telecom", "avatars_2/3/2.png", "...", "telecom_0_4");
						} else {
							showDialogue("Telecom", "avatars_2/3/2.png", "YOU LACK THE FUNDS NECESSARY TO MAKE THE CALL.", "telecom_0_1");
						}
					};

					scriptManager->addScript("telecom_0_2", telecom_0_2);

					auto telecom_0_3 = []() {
						turnTelecomOff(Location(1, -23, 1, 12, 1, 2));
						hideDialogue();
					};

					scriptManager->addScript("telecom_0_3", telecom_0_3);

					auto telecom_0_4 = []() {
						if (playerCalledBrotherInTelecom0) {
							showDialogue("Telecom", "avatars_2/3/2.png", "There was no response.", "telecom_0_1");
						} else {
							worldManager->getTileGroup(Location(1, -23, 1, 12, 1, 2))->removeTile(449);
							worldManager->getTileGroup(Location(1, -23, 1, 12, 1, 2))->addTile(445);
							showDialogue("Telecom", "avatars_2/3/3.png", "Hello?", "telecom_0_5");
							playerCalledBrotherInTelecom0 = true;
						}
					};

					scriptManager->addScript("telecom_0_4", telecom_0_4);
					scriptManager->addScript("telecom_0_5", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "Hi, " + PLAYER_NAME + "...", "telecom_0_6"));
					scriptManager->addScript("telecom_0_6", std::bind(&showTwoOptions, true, DialogueOption("How is life at home, Vlad?", "telecom_0_7"), DialogueOption("How is school?", "telecom_0_27")));
					scriptManager->addScript("telecom_0_7", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "At first, it was quiet and peaceful.", "telecom_0_8"));
					scriptManager->addScript("telecom_0_8", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "But now? It's been lonely without Mom and Dad.", "telecom_0_9"));
					scriptManager->addScript("telecom_0_9", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "Even when we went to sleep with the sound of them yelling...", "telecom_0_10"));
					scriptManager->addScript("telecom_0_10", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "That was better than living alone.", "telecom_0_11"));
					scriptManager->addScript("telecom_0_11", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "How has work been, big brother?", "telecom_0_12"));
					scriptManager->addScript("telecom_0_12", std::bind(&showTwoOptions, true, DialogueOption("It's been the usual grind...", "telecom_0_13"), DialogueOption("It's been as unescapable nightmare.", "telecom_0_22")));
					scriptManager->addScript("telecom_0_13", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "It's okay, you don't have to hide it from me.", "telecom_0_14"));
					scriptManager->addScript("telecom_0_14", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "The state of Oxylabs has been all over the news.", "telecom_0_15"));
					scriptManager->addScript("telecom_0_15", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "I see you're in some shop... ?", "telecom_0_16"));
					scriptManager->addScript("telecom_0_16", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "Ah, you must be on your way back home. Thank God.", "telecom_0_17"));
					scriptManager->addScript("telecom_0_17", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "Maybe you'll come back and we can live together again!", "telecom_0_18"));
					scriptManager->addScript("telecom_0_18", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "Just the two of us. Just like before you started working.", "telecom_0_19"));
					scriptManager->addScript("telecom_0_19", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "It would mean plenty to a fourteen year old like me.", "telecom_0_20"));
					scriptManager->addScript("telecom_0_20", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "LOW BATTERY?! Son of a gun!", "telecom_0_21"));

					auto telecom_0_21 = []() {
						worldManager->getTileGroup(Location(1, -23, 1, 12, 1, 2))->addTile(449);
						worldManager->getTileGroup(Location(1, -23, 1, 12, 1, 2))->removeTile(445);
						showDialogue("Italics", "avatars_2/3/2.png", "CLICK!", "telecom_0_1");
					};

					scriptManager->addScript("telecom_0_21", telecom_0_21);
					scriptManager->addScript("telecom_0_22", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "The news stories regarding Oxylabs have been ridiculous.", "telecom_0_23"));
					scriptManager->addScript("telecom_0_23", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "Are you safe where you currently are?", "telecom_0_24"));
					scriptManager->addScript("telecom_0_24", std::bind(&showTwoOptions, true, DialogueOption("Yeah... I'm in a candy store right now.", "telecom_0_25"), DialogueOption("Almost...", "telecom_0_26")));
					scriptManager->addScript("telecom_0_25", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "A candy shop with a telecom? This is not a joke, " + PLAYER_NAME + ".", "telecom_0_26"));
					scriptManager->addScript("telecom_0_26", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "Please be safe. I'll pray for you.", "telecom_0_17"));
					scriptManager->addScript("telecom_0_27", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "Terrible. Mainly because of Mr K's dreadful classes.", "telecom_0_28"));
					scriptManager->addScript("telecom_0_28", std::bind(&showDialogue, "Telecom", "avatars_2/3/3.png", "That biased creep...", "telecom_0_11"));

					scriptManager->addScript("sign_0_0", std::bind(&showDialogue, "Sign", "avatars_2/5/3.png", "Welcome to Oxylabs Gift Shop #9!", "sign_0_1"));
					worldManager->addPromptedScript("sign_0_0", Location(1, -23, 1, 14, 0, 2));

					scriptManager->addScript("sign_0_1", std::bind(&showDialogue, "Sign", "avatars_2/5/3.png", "In this shop, you may find several products that match your tastes!", "sign_0_2"));
					scriptManager->addScript("sign_0_2", std::bind(&showDialogue, "Sign", "avatars_2/5/3.png", "If you are hungry, you may purchase some exquisite snacks!", "sign_0_3"));
					scriptManager->addScript("sign_0_3", std::bind(&showDialogue, "Sign", "avatars_2/5/3.png", "The exit to the museum can be found to your left.", "sign_0_4"));
					scriptManager->addScript("sign_0_4", std::bind(&showDialogue, "Sign", "avatars_2/5/3.png", "EDIT: This shop has been repurposed to serve Propulsion Gun Testers.", "hide_dialogue"));

					auto shop_0_0_0 = []() {
						dialogueDisplay->showDialogue("Robot", "avatars_2/5/0.png", "Welcome to the Oxylabs Gift Shop, Sir or Ma'am!", "shop_0_1");
						characterProvider->getCharacterByName("robot_4")->turn(player->getCharacter()->getOppositeDirectionFacing());
					};

					scriptManager->addScript("shop_0_0_0", shop_0_0_0);
					worldManager->addPromptedScript("shop_0_0_0", Location(1, -23, 1, 4, 0, 4));

					auto shop_0_0_1 = []() {
						if (player->getCharacter()->getDirectionFacing() == TileDirection::FORWARD) {
							dialogueDisplay->showDialogue("Robot", "avatars_2/5/0.png", "Welcome to the Oxylabs Gift Shop, Sir or Ma'am!", "shop_0_1");
							characterProvider->getCharacterByName("robot_4")->turn(player->getCharacter()->getOppositeDirectionFacing());
						}
					};

					scriptManager->addScript("shop_0_0_1", shop_0_0_1);
					worldManager->addPromptedScript("shop_0_0_1", Location(1, -23, 1, 4, 0, 3));

					scriptManager->addScript("shop_0_1", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "What product would you like to take a look at?", "shop_0_2"));
					scriptManager->addScript("shop_0_2", std::bind(&showFourOptions, true, DialogueOption("Gun Capo", "shop_0_3"), DialogueOption("Snik Snak", "shop_0_11"), DialogueOption("Gun Slot", "shop_0_14"), DialogueOption("Exit", "hide_dialogue")));
					scriptManager->addScript("shop_0_3", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "Ah, the Gun Capo. This product will automatically hold down \"long\" notes for you.", "shop_0_4"));
					scriptManager->addScript("shop_0_4", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "You must press the note once yourself for the automatic hold to initiate.", "shop_0_5"));
					scriptManager->addScript("shop_0_5", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "Would you like to purchase this item for 50 Jimmies?", "shop_0_6"));
					scriptManager->addScript("shop_0_6", std::bind(&showTwoOptions, true, DialogueOption("Yes", "shop_0_7"), DialogueOption("No", "shop_0_2")));
					scriptManager->addScript("shop_0_7", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "...", "shop_0_8"));
					scriptManager->addScript("shop_0_8", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "What's that? You don't have enough Jimmies?", "shop_0_9"));
					scriptManager->addScript("shop_0_9", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "Sorry, Sir or Ma'am, but we do not give out credit.", "shop_0_10"));
					scriptManager->addScript("shop_0_10", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "Come back when you're a little... richer.", "shop_0_2"));
					scriptManager->addScript("shop_0_11", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "Yum! The Snik Snak! Use this product to restore half of your health.", "shop_0_12"));
					scriptManager->addScript("shop_0_12", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "You can use the Snik Snak once per battle.", "shop_0_13"));
					scriptManager->addScript("shop_0_13", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "Would you like to purchase this item for 80 Jimmies?", "shop_0_6"));
					scriptManager->addScript("shop_0_14", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "The Gun Slot. This product gives your Propulsion Gun one extra slot.", "shop_0_15"));
					scriptManager->addScript("shop_0_15", std::bind(&showDialogue, "Robot", "avatars_2/5/0.png", "Would you like to purchase this item for 110 Jimmies?", "shop_0_6"));

					scriptManager->addScript("dialogue_40_0", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Hello, " + PLAYER_NAME + "!", "dialogue_40_1"));
					worldManager->addPromptedScript("dialogue_40_0", Location(1, -22, 1, 1.5f, 0, 7.5f));
					scriptManager->addScript("dialogue_40_1", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Let me introduce myself.", "dialogue_40_2"));
					scriptManager->addScript("dialogue_40_2", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "My name is Robert.", "dialogue_40_3"));
					scriptManager->addScript("dialogue_40_3", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "I am the creator of this game.", "dialogue_40_4"));
					scriptManager->addScript("dialogue_40_4", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Well... I almost am.", "dialogue_40_5"));
					scriptManager->addScript("dialogue_40_5", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Let me explain.", "dialogue_40_6"));
					scriptManager->addScript("dialogue_40_6", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Robert personally injected a clone of himself into every copy of this game.", "dialogue_40_7"));
					scriptManager->addScript("dialogue_40_7", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "I am one of these clones, made specifically to interact with you.", "dialogue_40_8"));
					scriptManager->addScript("dialogue_40_8", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Yup! The purpose of my existence is to speak with you!", "dialogue_40_9"));
					scriptManager->addScript("dialogue_40_9", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "It's why I've been given so much charisma and charm!", "dialogue_40_10"));
					scriptManager->addScript("dialogue_40_10", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "I hope it isn't too much for you to handle.", "dialogue_40_11"));
					scriptManager->addScript("dialogue_40_11", std::bind(&showTwoOptions, true, DialogueOption("It is, but I'll manage.", "dialogue_40_12"), DialogueOption("It is, but I can handle it!", "dialogue_40_12")));
					scriptManager->addScript("dialogue_40_12", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Wow, it is... ?", "dialogue_40_13"));
					scriptManager->addScript("dialogue_40_13", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "I'm sure Robert gave you the option to say \"it isn't\"...", "dialogue_40_14"));
					scriptManager->addScript("dialogue_40_14", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "But you still said \"it is\"!", "dialogue_40_15"));
					scriptManager->addScript("dialogue_40_15", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Wow, I feel so much better about myself!", "dialogue_40_16"));
					scriptManager->addScript("dialogue_40_16", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Thank you!", "dialogue_40_17"));
					scriptManager->addScript("dialogue_40_17", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "You know, just like you, Robert is one nice guy.", "dialogue_40_18"));
					scriptManager->addScript("dialogue_40_18", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "I haven't actually met him, but I created a formal proof to back up my claim.", "dialogue_40_19"));
					scriptManager->addScript("dialogue_40_19", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "It goes something like this...", "dialogue_40_20"));
					scriptManager->addScript("dialogue_40_20", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Theorem: The human Robert contains the attribute \"nice\".", "dialogue_40_21"));
					scriptManager->addScript("dialogue_40_21", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Proof: We know that I am a direct clone of Robert.", "dialogue_40_22"));
					scriptManager->addScript("dialogue_40_22", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Therefore, for every attribute I contain, this attribute is contained by Robert.", "dialogue_40_23"));
					scriptManager->addScript("dialogue_40_23", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "I clearly contain the attribute \"nice\", so Robert must contain it too.", "dialogue_40_24"));
					scriptManager->addScript("dialogue_40_24", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Therefore, Robert contains the attribute \"nice\"!", "dialogue_40_25"));
					scriptManager->addScript("dialogue_40_25", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Quod erat demonstrandum.", "dialogue_40_26"));
					scriptManager->addScript("dialogue_40_26", std::bind(&showTwoOptions, true, DialogueOption("You're a genius!", "dialogue_40_27"), DialogueOption("You must also contain the attribute \"smart\"!", "dialogue_40_27")));
					scriptManager->addScript("dialogue_40_27", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Aww, thanks!", "dialogue_40_28"));
					scriptManager->addScript("dialogue_40_28", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "A corollary of this theorem is: The human Robert contains the attribute \"funny\".", "dialogue_40_29"));
					scriptManager->addScript("dialogue_40_29", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Anyways, I genuinely hope you enjoyed this demo.", "dialogue_40_30"));
					scriptManager->addScript("dialogue_40_30", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Several litres of blood, sweat and tears went into the game and its engine.", "dialogue_40_31"));
					scriptManager->addScript("dialogue_40_31", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "After all, it isn't easy to make a game and engine while being a student.", "dialogue_40_32"));
					scriptManager->addScript("dialogue_40_32", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Feel free to support the game on Kickstarter!", "dialogue_40_33"));
					scriptManager->addScript("dialogue_40_33", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "And tell your friends about it!", "dialogue_40_34"));
					scriptManager->addScript("dialogue_40_34", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "Well... it looks like I've completed my goal here.", "dialogue_40_35"));
					scriptManager->addScript("dialogue_40_35", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "The rest of the demo is up ahead.", "dialogue_40_36"));
					scriptManager->addScript("dialogue_40_36", std::bind(&showBlinkingDialogue, "Robert", "avatars_1/0/5.png", "avatars_1/1/5.png", "I wish you the best of luck!", "dialogue_40_37"));

					auto dialogue_40_37 = []() {
						characterProvider->getCharacterByName("Robert")->kill();
						worldManager->removePromptedScript(Location(1, -22, 1, 1.5f, 0, 7.5f));
						hideDialogue();
					};
					scriptManager->addScript("dialogue_40_37", dialogue_40_37);

					break;
				}
				default:
					break;
			}
			break;
		case 2:
			switch (level) {
				case 1:
					scriptManager->addScript("walked_on_2_0", std::bind(&fadeTeleportPlayer, Location(0, -3, 0, 6.5f, 0, 7.5f)), true);
					worldManager->addWalkedOnScript("walked_on_2_0", Location(0, 0, -1, 1, 0, 15));

					auto end_dialogue_0_0 = []() {
						worldManager->removeWalkedOnScript(Location(0, -2, 0, 13, 0, 10));
						if (*player->getCharacter()->getLocation() == Location(0, -2, 0, 13, 0, 10)) {
							dialogueDisplay->showDialogue("??? (Robert_2)", "avatars_1/0/5.png", "Hi!",
							                              "end_dialogue_1");

							Character *robert = characterProvider->getCharacterByName("Robert");
							robert->addTileMovementToQueue(TileDirection::BACKWARD);
							robert->addTurnToQueue(TileDirection::RIGHT);
						}
					};

					scriptManager->addScript("end_dialogue_0_0", end_dialogue_0_0);
					worldManager->addWalkedOnScript("end_dialogue_0_0", Location(0, -2, 0, 13, 0, 10));

					auto end_dialogue_0_1 = []() {
						worldManager->removeWalkedOnScript(Location(0, -2, 0, 14, 0, 10));
						if (*player->getCharacter()->getLocation() == Location(0, -2, 0, 14, 0, 10)) {
							dialogueDisplay->showDialogue("??? (Robert_2)", "avatars_1/0/5.png", "Hi!",
							                              "end_dialogue_1");

							Character *Robert_2 = characterProvider->getCharacterByName("Robert_2");
							Robert_2->addTileMovementToQueue(TileDirection::BACKWARD);
							Robert_2->addTurnToQueue(TileDirection::LEFT);
						}
					};

					scriptManager->addScript("end_dialogue_0_1", end_dialogue_0_1);
					worldManager->addWalkedOnScript("end_dialogue_0_1", Location(0, -2, 0, 14, 0, 10));

					scriptManager->addScript("end_dialogue_1", std::bind(&showDialogue, "??? (Robert)", "avatars_1/1/5.png", "I'm Robert and this is Robert_2.", "end_dialogue_2"));
					scriptManager->addScript("end_dialogue_2", std::bind(&showDialogue, "Robert", "avatars_1/1/5.png", "We're the developers of this game.", "end_dialogue_3"));
					scriptManager->addScript("end_dialogue_3", std::bind(&showDialogue, "Robert_2", "avatars_1/0/5.png", "How'd you like the third level?", "end_dialogue_4"));
					scriptManager->addScript("end_dialogue_4", std::bind(&showDialogue, "Robert_2", "avatars_1/0/5.png", "Was it difficult?", "end_dialogue_5"));
					scriptManager->addScript("end_dialogue_5", std::bind(&showDialogue, "Robert", "avatars_1/1/5.png", "Wait-- didn't we remove the third level from this demo?", "end_dialogue_6"));
					scriptManager->addScript("end_dialogue_6", std::bind(&showDialogue, "Robert", "avatars_1/1/5.png", "Because we wanted to change it?", "end_dialogue_7"));
					scriptManager->addScript("end_dialogue_7", std::bind(&showDialogue, "Robert_2", "avatars_1/0/5.png", "Right, we did...", "end_dialogue_8"));
					scriptManager->addScript("end_dialogue_8", std::bind(&showDialogue, "Robert_2", "avatars_1/0/5.png", "Well that kind of sucks. Only two levels in a demo?", "end_dialogue_9"));
					scriptManager->addScript("end_dialogue_9", std::bind(&showDialogue, "Robert_2", "avatars_1/0/5.png", "Don't worry, Player, we plan on making more demos with more levels!", "end_dialogue_10"));
					scriptManager->addScript("end_dialogue_10", std::bind(&showDialogue, "Robert", "avatars_1/1/5.png", "And we'll hopefully not stumble into development hell along the way.", "end_dialogue_11"));
					scriptManager->addScript("end_dialogue_11", std::bind(&showDialogue, "Robert_2", "avatars_1/0/5.png", "If you found any bugs, feel free to tell us about them! We appreciate it!", "end_dialogue_12"));
					scriptManager->addScript("end_dialogue_12", std::bind(&showDialogue, "Robert", "avatars_1/1/5.png", "And we hope you enjoyed the game!", "end_dialogue_13"));
					scriptManager->addScript("end_dialogue_13", std::bind(&showDialogue, "Robert_2 and Robert", "avatars_1/2/5.png", "See you in the next version of the game!", "end_dialogue_14"));

					auto end_dialogue_14 = []() {
						game->fadeOut();

						auto event = []() {
							GameObjectProvider::getWindow()->quit();
						};

						timer->scheduleEventInSeconds(2, event);
					};
					scriptManager->addScript("end_dialogue_14", end_dialogue_14);

					auto dialogue_29_0 = []() {
						worldManager->removeWalkedOnScript(Location(0, -3, 0, 6, 0, 7));
						player->getCharacter()->allowNewMovements(false);
						Character* aela = characterProvider->getCharacterByName("Aela");

						auto event1 = [aela]() {
							aela->addTurnToQueue(TileDirection::LEFT);
						};

						auto event2 = []() {
							showBlinkingDialogue("Aela", "avatars_2/3/0.png", "avatars_2/3/1.png", PLAYER_NAME + "!", "dialogue_29_1");
						};

						timer->scheduleEventInMillis(500, event1);
						timer->scheduleEventInMillis(1000, event2);
					};

					scriptManager->addScript("dialogue_29_0", dialogue_29_0);
					worldManager->addWalkedOnScript("dialogue_29_0", Location(0, -3, 0, 6, 0, 7));

					auto dialogue_29_1 = []() {
						Character* aela = characterProvider->getCharacterByName("Aela");
						for (int i = 0; i < 3; i++) {
							aela->addTileMovementToQueue(TileDirection::LEFT);
						}

						player->getCharacter()->addTurnToQueue(TileDirection::RIGHT);

						auto event1 = []() {
							player->getCharacter()->allowNewMovements(true);
							showBlinkingDialogue("Aela", "avatars_2/3/0.png", "avatars_2/3/1.png", "I'm so glad you made it out alive!", "dialogue_29_2");
						};

						timer->scheduleEventInMillis(300, event1);
					};

					scriptManager->addScript("dialogue_29_1", dialogue_29_1);

					// Eventually, once its implemented, some of these dialogue options will modify the relationship with Aela.
					// If the player tries to make an advance on her and the relationship is not strong, she will reject the offer.
					// A rejection counter will go up and the relationsip value will decrease. But if the player manages to make
					// a successful advance later and the rejection counter is not zero, Aela might say "I know I declined before
					// but sure" or whatever.
					scriptManager->addScript("dialogue_29_2", std::bind(&showFourOptions, true, DialogueOption("What's going on?", "dialogue_29_3"), DialogueOption("Am I in trouble?", "dialogue_29_4"), DialogueOption("Do you know how hard that level was?!", "dialogue_29_5"), DialogueOption("I'm glad to finally see you!", "dialogue_29_7")));
					scriptManager->addScript("dialogue_29_3", std::bind(&showDialogue, "Aela", "avatars_1/2/4.png", "The Project Echo leader is here to speak to us. Come join us!", "dialogue_29_10"));
					scriptManager->addScript("dialogue_29_4", std::bind(&showDialogue, "Aela", "avatars_1/2/4.png", "Haha, don't worry, you're fine. Follow me!", "dialogue_29_10"));
					scriptManager->addScript("dialogue_29_5", std::bind(&showDialogue, "Aela", "avatars_1/2/4.png", "Uhh...", "dialogue_29_6"));
					scriptManager->addScript("dialogue_29_6", std::bind(&showDialogue, "Aela", "avatars_1/2/4.png", "The Project Echo leader is here to talk to us. You might want to listen.", "dialogue_29_10"));
					scriptManager->addScript("dialogue_29_7", std::bind(&showDialogue, "Aela", "avatars_1/2/4.png", "And I'm glad to see you too!", "dialogue_29_8"));
					scriptManager->addScript("dialogue_29_8", std::bind(&showDialogue, "Aela", "avatars_1/2/4.png", "I know we're supposed to have lunch right now. I'm hungry too.", "dialogue_29_9"));
					scriptManager->addScript("dialogue_29_9", std::bind(&showDialogue, "Aela", "avatars_1/2/4.png", "However, the Project Echo leader is here to talk with us. Come!", "dialogue_29_10"));
					
					auto dialogue_29_10 = []() {
						hideDialogue();
						player->getCharacter()->allowNewMovements(true);
						Character* aela = characterProvider->getCharacterByName("Aela");
						aela->addTurnToQueue(TileDirection::RIGHT);

						for (int i = 0; i < 3; i++) {
							aela->addTileMovementToQueue(TileDirection::RIGHT);
						}

						aela->addTurnToQueue(TileDirection::BACKWARD);
					};

					scriptManager->addScript("dialogue_29_10", dialogue_29_10);

					auto dialogue_30_0 = []() {
						if (*player->getCharacter()->getLocation() == Location(0, -3, 0, 3, 0, 7)) {
							worldManager->removeWalkedOnScript(Location(0, -3, 0, 3, 0, 7));
							player->getCharacter()->addTurnToQueue(TileDirection::BACKWARD);

							auto event1 = []() {
								showDialogue("Ferdinand", "avatars_1/2/4.png", "My friends, I have been working at Oxylabs for 25 years.", "dialogue_30_1");
							};

							timer->scheduleEventInMillis(500, event1);
						}
					};

					scriptManager->addScript("dialogue_30_0", dialogue_30_0);
					worldManager->addWalkedOnScript("dialogue_30_0", Location(0, -3, 0, 3, 0, 7));
					scriptManager->addScript("dialogue_30_1", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "Although I have worked on several projects and with several people, Project Echo has been my greatest achievement.", "dialogue_30_2"));
					scriptManager->addScript("dialogue_30_2", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "Unfortunately, most of you were ripped from your own projects to work on mine.", "dialogue_30_3"));
					scriptManager->addScript("dialogue_30_3", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "I understand the frustration and I appreciate the work you all put towards Echo.", "dialogue_30_4"));
					scriptManager->addScript("dialogue_30_4", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "However, we find ourselves in an awful situation.", "dialogue_30_5"));
					scriptManager->addScript("dialogue_30_5", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "Our own military has broken into our facilities and taken all of our property.", "dialogue_30_6"));
					scriptManager->addScript("dialogue_30_6", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "But what angers me the most is that they've imprisoned our coworkers.", "dialogue_30_7"));
					scriptManager->addScript("dialogue_30_7", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "Our facility is so large that they have not reached us yet. But they will.", "dialogue_30_8"));
					scriptManager->addScript("dialogue_30_8", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "By now we all know that our management was plotting against our corrupt government.", "dialogue_30_9"));
					scriptManager->addScript("dialogue_30_9", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "Although that's a stupid idea, I can't blame them for trying it.", "dialogue_30_10"));
					scriptManager->addScript("dialogue_30_10", std::bind(&showDialogue, "Scientist", "avatars_1/2/4.png", "Sorry to interrupt you, Ferdinand, but does anyone else sense an awful smell?", "dialogue_30_11"));
					scriptManager->addScript("dialogue_30_11", std::bind(&showDialogue, "Scientist", "avatars_1/2/4.png", "I smell it too. It smells like rotten eggs, right?", "dialogue_30_12"));
					scriptManager->addScript("dialogue_30_12", std::bind(&showDialogue, "Scientist", "avatars_1/2/4.png", "Yuck, it smells awful!", "dialogue_30_13"));
					scriptManager->addScript("dialogue_30_13", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "Hah, that's ironic! They got a hold of my old neurotoxin side project and are using it against us!", "dialogue_30_14"));
					scriptManager->addScript("dialogue_30_14", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "Good night everyone!", "dialogue_30_15"));
					scriptManager->addScript("dialogue_30_15", std::bind(&showDialogue, "Ferdinand", "avatars_1/2/4.png", "Let's hope this is the toxin which puts us in a temporary sleep, not a permanent one!", "hide_dialogue"));



					break;
			}
		default:
			break;
	}
}

void Scripts::setupCharacterFontAssociations() {
	// This sets up the special fonts used by certain characters in their dialogue.
	/*Font* consolas;
	if (!resourceManager->obtain<Font>("res/fonts/consolas.ttf", consolas)) {
		AelaErrorHandling::windowError("A critical font (consolas.ttf) could not be loaded, aborting!");
		return;
	}
	consolas->setDefaultSize(FontSizes::MEDIUM_FONT_SIZE + 10);
	consolas->setAntialiasing(false);*/

	Font* pressStart2P;
	if (!resourceManager->obtain<Font>("res/fonts/pressStart2P.ttf", pressStart2P)) {
		AelaErrorHandling::windowError("A critical font (pressStart2P.ttf) could not be loaded, aborting!");
		return;
	}

	pressStart2P->setDefaultSize(FontSizes::MEDIUM_FONT_SIZE);
	pressStart2P->setAntialiasing(false);

	Font* pressStart2PItalics;
	if (!resourceManager->obtain<Font>("res/fonts/pressStart2PItalics.ttf", pressStart2PItalics)) {
		AelaErrorHandling::windowError("A critical font (pressStart2PItalics.ttf) could not be loaded, aborting!");
		return;
	}

	pressStart2PItalics->setDefaultSize(FontSizes::MEDIUM_FONT_SIZE);
	pressStart2PItalics->setAntialiasing(false);

	/*Font* upheaval;
	if (!resourceManager->obtain<Font>("res/fonts/UpheavalPro.ttf", upheaval)) {
		AelaErrorHandling::windowError("A critical font (acknowtt.ttf) could not be loaded, aborting!");
		return;
	}

	upheaval->setDefaultSize(FontSizes::MEDIUM_FONT_SIZE);
	upheaval->setAntialiasing(false);

	Font* brobot;
	if (!resourceManager->obtain<Font>("res/fonts/Brobot.ttf", brobot)) {
		AelaErrorHandling::windowError("A critical font (Brobot.ttf) could not be loaded, aborting!");
		return;
	}

	brobot->setAntialiasing(false);
	brobot->setDefaultSize(28);

	Font* acknowledge;
	if (!resourceManager->obtain<Font>("res/fonts/acknowtt.ttf", acknowledge)) {
		AelaErrorHandling::windowError("A critical font (acknowtt.ttf) could not be loaded, aborting!");
		return;
	}

	acknowledge->setAntialiasing(false);
	acknowledge->setDefaultSize(16);*/

	// I need to find more fonts. For now, I'm just using pressStart2P.
	dialogueDisplay->associateCharacterWithFont("Aela", pressStart2P);
	// dialogueDisplay->associateCharacterWithFont("Soldier", upheaval);
	dialogueDisplay->associateCharacterWithFont("Soldier", pressStart2P);
	dialogueDisplay->associateCharacterWithFont("Brobot", /*brobot*/ pressStart2P);
	dialogueDisplay->associateCharacterWithFont("Scrolling Text", /*acknowledge*/ pressStart2P);
	dialogueDisplay->associateCharacterWithFont("Drunk Italics", pressStart2PItalics);
	dialogueDisplay->associateCharacterWithFont("Italics", pressStart2PItalics);
	// dialogueDisplay->associateCharacterWithFont("Kiosk", consolas);
}

void Scripts::startNewGame() {
	// player->getCharacter()->addTeleportToQueue(Location(0, 0, 0, 1.5f, 0, 2.5f), TeleportationAnimation::NONE);
	animateElevator();
	setupMainAnimations();
	setupCharacterFontAssociations();

	// This sets up the player's tile inventory for a new game.
	for (int i = 0; i < 3; i++) {
		auto tile = Tile(3);
		player->getTileInventory()->addTile(&tile);
	}

	// The tile switch gun is inactive by default.
	// game->getTileSwitchGun()->setActive(false);

	setupLevel(1, 1);
	tileInventoryDisplay->hide();

	auto starting_event = []() {
		auto event1 = []() {
			dialogueDisplay->showDialogue("???", "", "Well, well, well...", "");
			dialogueDisplay->setTimeBetweenCharacterReveals(100);
		};

		timer->scheduleEventInSeconds(4, event1);

		auto event2 = []() {
			game->fadeIn(3000);
			dialogueDisplay->setTimeBetweenCharacterReveals(60);
			dialogueDisplay->showDialogue("???", "", "Your presence is a surprise...", "");
		};

		timer->scheduleEventInSeconds(11, event2);

		auto event3 = []() {
			dialogueDisplay->showDialogue("???", "", "We've been waiting for you to reboot this experiment one last time.", "");
		};

		timer->scheduleEventInSeconds(16, event3);

		auto event4 = []() {
			dialogueDisplay->showDialogue("???", "", "I hope it does not end in dissapointment.", "");
		};

		timer->scheduleEventInSeconds(23, event4);

		auto event5 = []() {
			dialogueDisplay->setTimeBetweenCharacterReveals(100);
			dialogueDisplay->showDialogue("???", "", "It would be shameful if it did...", "");
		};

		timer->scheduleEventInSeconds(28, event5);

		auto event6 = []() {
			game->showBlackness();
			player->getCharacter()->setTexture("player_6");
			player->getCharacter()->allowNewMovements(true);
			player->getCharacter()->changeLocation(Location(0, 0, 0, 1.5f, 0, 2.5f));
			dialogueDisplay->closeDialog();
			player->getCharacter()->allowNewMovements(false);
		};

		timer->scheduleEventInSeconds(38, event6);

		auto event7 = []() {
			game->fadeIn(7000);
		};

		timer->scheduleEventInSeconds(42, event7);

		auto event8 = []() {
			player->getCharacter()->setTexture("player_1");
			dialogueDisplay->setContinueOnReturnPress(true);
			dialogueDisplay->resetTimeBetweenCharacterReveals();
			dialogueDisplay->showDialogue("Aela", "avatars_1/2/2.png", "avatars_1/2/3.png", "Hello? Are you awake?", "dialogue_0_1");
		};

		timer->scheduleEventInSeconds(51, event8);

		auto event9 = []() {
			if (dialogueDisplay->getLineOfText(0) == "Hello? Are you awake?") {
				hintDisplay->displayHint("Press ENTER to continue.", HintDisplayDuration::FOREVER);
			}
		};

		timer->scheduleEventInSeconds(55, event9);
	};

	scriptManager->addScript("starting_event", starting_event);
	worldManager->addWalkedOnScript("starting_event", Location(0, 3, 0, 0, 0, 0));

	player->getCharacter()->allowNewMovements(false);
	player->getCharacter()->setTexture("shadow");
	player->getCharacter()->addTeleportToQueue(Location(0, 3, 0, 0, 0, 0), TeleportationAnimation::NONE);

	game->showBlackness();
	dialogueDisplay->setContinueOnReturnPress(false);
}

void Scripts::continueGame() {
	if (!gameSaver->loadFromFile("save_0")) {
		// AelaErrorHandling::windowError("Neo-Zero", "Could not find or load save file save_0. Starting a new game.");
		hintDisplay->displayHint("Could not load save, starting a new game.", HintDisplayDuration::LONG);
		startNewGame();
		return;
	}

	size_t worldID = player->getCharacter()->getLocation()->getWorld();
	if (!worldManager->doesWorldExist(worldID)) {
		loadTiledMap("stage_" + std::to_string(worldID), worldID);
	}

	animateElevator();
	setupMainAnimations();
	setupCharacterFontAssociations();
	setupLevel(player->getCurrentStage(), player->getCurrentLevel());

	// This gives the player a cleared tile inventory.
	for (int i = 0; i < 3; i++) {
		auto tile = Tile(3);
		player->getTileInventory()->addTile(&tile);
	}
	
	tileInventoryDisplay->refreshSubMenu();
	tileInventoryDisplay->show();
	game->getTileSwitchGun()->setActive(true);

	// Since we added a bunch of events to the world, we want to save again so that those events save
	// (in case the player tries to go back to the checkpoint they are currently at).
	gameSaver->save("save_0");
}

void Scripts::editMap() {
	game->getTileSwitchGun()->setActive(true);
	player->getCharacter()->addTeleportToQueue(Location(MAP_BEING_EDITED_WORLD_ID, 0, 0, 0.5f, 0, 0.5f), TeleportationAnimation::NONE);
}

void Scripts::setupLevel(int stage, int level) {
	if (!worldManager->doesWorldExist(stage)) {
		loadTiledMap("stage_" + std::to_string(stage), stageMapAssociations[stage - 1]);
	}

	if (game->getGameMode() == GameMode::GAMEPLAY) {
		scriptManager->clearNonEssentialScripts();
		setupTeleporters(stage, level);
		setupDoors(stage, level);
		setupCheckPoints(stage, level);
		setupCharacters(stage, level);
		setupGeneralEvents(stage, level);
		setupMusicForLevel(stage, level);
		player->setCurrentStage(stage);
		player->setCurrentLevel(level);

		// This does the rest of the setting up.
		switch (stage) {
			case 1:
				if (level < 4) {
					loadLogoMaterialAnimation(level);
				}
				if (level == 3) {
					
				}
				break;
			default:
				break;
		}
	}
}

void onWaterPlace(const Location& location) {
	std::vector<glm::ivec2> chunkCoords;
	std::vector<glm::ivec3> tileCoords;

	for (int i = 0; i < 4; i++) {
		chunkCoords.push_back(location.getChunk());
		tileCoords.push_back(location.getTileGroup());
	}

	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[0], chunkCoords[0], TileDirection::RIGHT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[1], chunkCoords[1], TileDirection::FORWARD);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[2], chunkCoords[2], TileDirection::LEFT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[3], chunkCoords[3], TileDirection::BACKWARD);

	for (int i = 0; i < 4; i++) {
		TileGroup* group = worldManager->getTileGroup(Location(location.getWorld(), chunkCoords[i], tileCoords[i]));
		if (group != nullptr) {
			Tile* tile = group->getTile(10);
			if (tile == nullptr) {
				continue;
			}
			group->removeTile(tile->getType());
			group->addTile(13);
			worldManager->tileWasPlaced(Location(location.getWorld(), chunkCoords[i], tileCoords[i]), 13);
			gameAudioPlayer->playAudioIfNotPlaying("lava");
		}
	}
}

void onLavaPlace(const Location& location) {
	std::vector<glm::ivec2> chunkCoords;
	std::vector<glm::ivec3> tileCoords;

	for (int i = 0; i < 4; i++) {
		chunkCoords.push_back(location.getChunk());
		tileCoords.push_back(location.getTileGroup());
	}

	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[0], chunkCoords[0], TileDirection::RIGHT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[1], chunkCoords[1], TileDirection::FORWARD);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[2], chunkCoords[2], TileDirection::LEFT);
	Scripts::worldManager->getCoordinateOfNeighbouringTile(tileCoords[3], chunkCoords[3], TileDirection::BACKWARD);

	for (int i = 0; i < 4; i++) {
		TileGroup* waterTileGroup = Scripts::worldManager->getTileGroup(Location(location.getWorld(), chunkCoords[i], tileCoords[i]));
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
			gameAudioPlayer->playAudioIfNotPlaying("lava");
			break;
		}
	}
}

void Scripts::addTileBehaviours() {
	std::function<void(const Location&)> water = &onWaterPlace;
	tileBehaviourExecuter->addBehaviour(9, &water);
	std::function<void(const Location&)> lava = &onLavaPlace;
	tileBehaviourExecuter->addBehaviour(10, &lava);
}

void Scripts::fadeTeleportPlayer(Location& location) {
	player->getCharacter()->addTeleportToQueue(location, TeleportationAnimation::FADE);
}

void Scripts::riseTeleportPlayer(Location& location) {
	player->getCharacter()->addTeleportToQueue(location, TeleportationAnimation::RISE);
}

void Scripts::unlockDoorLock(std::string tag, size_t lockID) {
	doorProvider->getDoor(std::move(tag))->unlock(lockID);
}

void Scripts::openDoor(std::string doorID, size_t tileType, Location location) {
	clearDoorAnimations(doorID);

	TileGroup* tileGroup = worldManager->getTileGroup(location);
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

	TileGroup* tileGroup = worldManager->getTileGroup(location);
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

	Character* character = characterProvider->getCharacterByLocation(location);
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
	TileGroup* tileGroup = worldManager->getTileGroup(location);
	tileGroup->removeTile(73);
	tileGroup->addTile(74);
}

void Scripts::turnDisplayOff(Location location) {
	TileGroup* tileGroup = worldManager->getTileGroup(location);
	tileGroup->addTile(73);
	tileGroup->removeTile(74);
}

void Scripts::turnScannerOn(Location location) {
	TileGroup* tileGroup = worldManager->getTileGroup(location);
	tileGroup->removeTile(355);
	tileGroup->addTile(456);
}

void Scripts::turnScannerOff(Location location) {
	TileGroup* tileGroup = worldManager->getTileGroup(location);
	tileGroup->addTile(355);
	tileGroup->removeTile(456);
}

void Scripts::turnTelecomOn(Location location) {
	TileGroup* tileGroup = worldManager->getTileGroup(location);
	tileGroup->removeTile(444);
	tileGroup->addTile(449);
}

void Scripts::turnTelecomOff(Location location) {
	TileGroup* tileGroup = worldManager->getTileGroup(location);
	tileGroup->addTile(444);
	tileGroup->removeTile(449);
}

void Scripts::pressButton(Location location) {
	TileGroup* tileGroup = worldManager->getTileGroup(location);
	tileGroup->removeTile(343);
	tileGroup->addTile(344);

	auto event = [tileGroup]() {
		tileGroup->addTile(343);
		tileGroup->removeTile(344);
	};

	timer->scheduleEventInMillis(BUTTON_PRESSING_TIME, event);
}

void Scripts::showAndEnableGun() {
	player->showAndEnableTileGun();
}

void Scripts::hideAndDisableGun() {
	player->hideAndDisableTileGun();
}

void Scripts::setupDoor(Door* door, std::string name, std::function<void()> onOpen, std::function<void()> onClose) {
	door->setOnOpen(&onOpen);
	door->setOnClose(&onClose);
	Door* door2 = doorProvider->getDoor(name);
	if (door2 == nullptr) {
		doorProvider->addDoor(std::move(name), door);
	} else {
		door2->setOnOpen(&onOpen);
		door2->setOnClose(&onClose);
	}
}

void Scripts::setupDoorSensor(const Location& location, std::string doorName, std::string sensorScriptName, size_t tileType, size_t lockID) {
	scriptManager->addScript(sensorScriptName, [=] {
		return triggerDoorSensor(location, tileType, doorName, lockID);
	});
	worldManager->addTileSwitchScript(sensorScriptName, location);

	if (worldManager->getTileGroup(location)->getTile(tileType) != nullptr) {
		triggerDoorSensor(location, tileType, doorName, lockID);
	}
}

void Scripts::setupTimedDoorSensor(const Location& location, std::string doorName, std::string sensorScriptName,
	size_t tileType, size_t lockID) {
	scriptManager->addScript(sensorScriptName, [=] {
		return triggerTimedDoorSensor(location, tileType, doorName, lockID);
	});
	worldManager->addTileSwitchScript(sensorScriptName, location);

	// if (worldManager->getTileGroup(location)->getTile(tileType) != nullptr) {
	// 	triggerTimedDoorSensor(location, tileType, doorName, lockID);
	// }
}

void Scripts::lockTimedDoorSensorIfNecessary(const Location& location, std::string doorName, size_t tileType, size_t lockID) {
	if (worldManager->getTileGroup(location)->getTile(tileType) == nullptr) {
		doorProvider->getDoor(doorName)->lock(lockID);
	} else {
		// This might be broken?!?!?!?!
		doorProvider->getDoor(doorName)->lockWithoutLockingLocks();
	}
}

void Scripts::setupMultiTypeDoorSensor(const Location& location, std::string doorName, std::string sensorScriptName, std::vector<size_t> correctTileTypes, size_t lockID) {
	scriptManager->addScript(sensorScriptName, [=] {
		return triggerMultiTypeDoorSensor(location, correctTileTypes, doorName, lockID);
	});
	worldManager->addTileSwitchScript(sensorScriptName, location);

	for (auto t : correctTileTypes) {
		if (worldManager->getTileGroup(location)->getTile(t) != nullptr) {
			triggerMultiTypeDoorSensor(location, correctTileTypes, doorName, lockID);
		}
	}
}

void Scripts::setupDoorTileAmountSensors(TileAmountLock* lock, std::string sensorScriptName) {
	auto script = [lock]() {
		lock->checkIfUnlocked();
	};
	scriptManager->addScript(sensorScriptName, script);

	for (auto& location : *lock->getRegion()) {
		GameObjectProvider::getWorldManager()->getTileGroup(location)->setSwitchScript(sensorScriptName);
	}
}

void Scripts::setupCheckPointWithRightSaveSign(std::string name, Location checkPointLocation, Location rightSaveSignLocation,
	TileDirection directionToMovePlayer) {
	auto onClose_1_0 = [name, checkPointLocation]() {
		closeDoor("save_door_" + name, 307, checkPointLocation);
	};

	Door door({false});
	setupDoor(&door, "save_door_" + name, nullptr, onClose_1_0);
	
	auto event = [name, rightSaveSignLocation, checkPointLocation, directionToMovePlayer]() mutable {
		TileGroup* tileGroup = worldManager->getTileGroup(rightSaveSignLocation);
		tileGroup->removeTile(161);
		tileGroup->addTile(162);

		player->getTileInventory()->clear(3);
		// worldManager->getTileGroup(checkPointLocation)->addTile(307);
		doorProvider->getDoor("save_door_" + name)->lock(0);
		timer->scheduleEventInNanos((size_t) (TIME_BETWEEN_DOOR_ANIMATION_FRAMES * 3), std::bind(&GameSaver::save, gameSaver, "save_0"));
	};

	Character* character = player->getCharacter();
	Location location = checkPointLocation;
	worldManager->getCoordinateOfNeighbouringTile(location, directionToMovePlayer);
	scriptManager->addScript(name, event);
	worldManager->addWalkedOnScript(name, location);
}

void Scripts::setupCheckPointWithLeftSaveSign(std::string name, Location checkPointLocation, Location leftSaveSignLocation,
	TileDirection directionToMovePlayer) {
	auto onClose_1_0 = [name, checkPointLocation]() {
		closeDoor("save_door_" + name, 307, checkPointLocation);
	};

	Door door({false});
	setupDoor(&door, "save_door_" + name, nullptr, onClose_1_0);
	
	auto event = [name, leftSaveSignLocation, checkPointLocation, directionToMovePlayer]() mutable {
		TileGroup* tileGroup = worldManager->getTileGroup(leftSaveSignLocation);
		tileGroup->removeTile(163);
		tileGroup->addTile(164);

		player->getTileInventory()->clear(3);
		// worldManager->getTileGroup(checkPointLocation)->addTile(307);
		doorProvider->getDoor("save_door_" + name)->lock(0);
		timer->scheduleEventInNanos((size_t) (TIME_BETWEEN_DOOR_ANIMATION_FRAMES * 3), std::bind(&GameSaver::save, gameSaver, "save_0"));
	};

	Character* character = player->getCharacter();
	Location location = checkPointLocation;
	worldManager->getCoordinateOfNeighbouringTile(location, directionToMovePlayer);
	scriptManager->addScript(name, event);
	worldManager->addWalkedOnScript(name, location);
}

void Scripts::setupCheckPointWithBothSaveSigns(std::string name, Location checkPointLocation, Location leftSaveSignLocation,
	Location rightSaveSignLocation, TileDirection directionToMovePlayer) {
	auto onClose_1_0 = [name, checkPointLocation]() {
		closeDoor("save_door_" + name, 307, checkPointLocation);
	};

	Door door({false});
	setupDoor(&door, "save_door_" + name, nullptr, onClose_1_0);
	
	auto event = [name, leftSaveSignLocation, rightSaveSignLocation, checkPointLocation, directionToMovePlayer]() mutable {
		// If loading was disabled earlier, it should be re-enabled now.
		gameSaver->enableLoading();

		std::cout << "FUCK YOU!!!!!!!!!!\n";

		TileGroup* leftTileGroup = worldManager->getTileGroup(leftSaveSignLocation);
		leftTileGroup->removeTile(163);
		leftTileGroup->addTile(164);

		TileGroup* rightTileGroup = worldManager->getTileGroup(rightSaveSignLocation);
		rightTileGroup->removeTile(161);
		rightTileGroup->addTile(162);

		player->getTileInventory()->clear(3);
		// worldManager->getTileGroup(checkPointLocation)->addTile(307);
		doorProvider->getDoor("save_door_" + name)->lock(0);
		timer->scheduleEventInNanos((size_t) (TIME_BETWEEN_DOOR_ANIMATION_FRAMES * 3), std::bind(&GameSaver::save, gameSaver, "save_0"));
	};

	Character* character = player->getCharacter();
	Location location = checkPointLocation;
	worldManager->getCoordinateOfNeighbouringTile(location, directionToMovePlayer);
	scriptManager->addScript(name, event);
	worldManager->addWalkedOnScript(name, location);
}

void Scripts::triggerDoorSensor(Location location, size_t correctTileType, std::string doorTag, size_t lockID) {
	if (worldManager->getTileGroup(location)->getTile(correctTileType) != nullptr) {
		doorProvider->getDoor(doorTag)->unlock(lockID);
	} else {
		doorProvider->getDoor(doorTag)->lock(lockID);
	}
}

void Scripts::triggerTimedDoorSensor(Location location, size_t correctTileType, std::string doorTag, size_t lockID) {
	if (worldManager->getTileGroup(location)->getTile(correctTileType) != nullptr) {
		doorProvider->getDoor(doorTag)->unlock(lockID);
	} else if (!doorProvider->getDoor(doorTag)->isOpen()) {
		doorProvider->getDoor(doorTag)->lock(lockID);
	}
}

void Scripts::triggerMultiTypeDoorSensor(Location location, std::vector<size_t> correctTileTypes, std::string doorTag, size_t lockID) {
	for (size_t type : correctTileTypes) {
		if (worldManager->getTileGroup(location)->getTile(type) != nullptr) {
			doorProvider->getDoor(doorTag)->unlock(lockID);
			return;
		}
	}
	doorProvider->getDoor(doorTag)->lock(lockID);
}

void Scripts::addImportantScripts() {
	scriptManager->addEssentialScript("hide_dialogue", &hideDialogue);
	scriptManager->addEssentialScript("start_new_game", &startNewGame);
	scriptManager->addEssentialScript("continue_game", &continueGame);
	scriptManager->addEssentialScript("edit_map", &editMap);
	scriptManager->addEssentialScript("add_tile_behaviours", &addTileBehaviours);
	scriptManager->addEssentialScript("show_and_enable_gun", &showAndEnableGun);
	scriptManager->addEssentialScript("hide_and_disable_gun", &hideAndDisableGun);
}

void Scripts::setupScripts() {
	addImportantScripts();
}

void Scripts::runGameStartingScripts() {
	scriptManager->runScript("add_tile_behaviours");
}