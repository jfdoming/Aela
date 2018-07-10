#include "GameScript.h"
#include "ScriptObjects.h"
#include "../../Player/Player.h"
#include "../../Aela Game/AelaGame.h"
#include "../../Dialogue/DialogueHandler.h"
#include "../../Worlds/WorldManager.h"
#include "../ScriptManager.h"

using namespace Game;

void Scripts::hideDialogue() {
	dialogueHandler->closeDialog();
}

void Scripts::setupCharacters() {}

void Scripts::setupTeleporters() {}

void Scripts::startNewGame() {
	player->getCharacter()->teleport(&Location(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 2)), false);		
	// worldManager->setCurrentWorld(0);

	// This sets up the player's tile inventory for a new game.
	for (int i = 0; i < 3; i++) {
		player->getTileInventory()->addTile(&Tile(1));
	}
}

void Scripts::continueGame() {}

void Scripts::addScriptsToGameWorld() {
	scriptManager->addScript("setup_characters", std::bind(&setupCharacters));
	scriptManager->addScript("setup_teleporters", std::bind(&setupTeleporters));
	scriptManager->addScript("hide_dialogue", std::bind(&hideDialogue));
	scriptManager->addScript("start_new_game", std::bind(&startNewGame));
	scriptManager->addScript("continue_game", std::bind(&continueGame));
	// scriptManager->addScript("")
}

void Scripts::setupScripts() {
	addScriptsToGameWorld();
}

void Scripts::runStartingScripts() {
	scriptManager->runScript("setup_characters");
	scriptManager->runScript("setup_teleporters");
}