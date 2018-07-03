/*
* Name: Game Scripts
* Author: Robert Ciborowski
* Date: 03/01/2018
* Description: A file which contains gameplay-related scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once
#include "../../Aela Game/AelaGame.h"


namespace GameScripts {
	using namespace Game;

	AelaGame* game;
	WorldManager* worldManager;
	CharacterTracker* characterTracker;
	EnemyRegistrar* enemyRegistrar;
	ScriptManager* scriptManager;
	DialogueHandler* dialogueHandler;
	Engine* engine;

	void hideDialogue() {
		dialogueHandler->closeDialog();
	}

	// This sets up the characters if the world.
	void setupCharacters() {
		
	}

	void setupTeleporters() {
		
	}

	void openDoor1_1_1() {
		// worldManager->getWorld(worldManager->getCurrentWorld())->getChunks()->at()
	}

	// This adds all gameplay scripts to the ScriptManager and also binds tile-related scripts to their tiles.
	void addScriptsToGameWorld() {
		scriptManager->addScript("setup_characters", std::bind(&setupCharacters));
		scriptManager->addScript("setup_teleporters", std::bind(&setupTeleporters));
		scriptManager->addScript("hide_dialogue", std::bind(&hideDialogue));
		// scriptManager->addScript("")
	}

	// This sets up necessary items for gameplay scripts.
	void setupGameScripts(AelaGame* gameToUse, Engine* engineToUse) {
		game = gameToUse;
		worldManager = game->getWorldManager();
		characterTracker = worldManager->getCharacterTracker();
		enemyRegistrar = game->getEnemyRegistrar();
		scriptManager = game->getScriptManager();
		dialogueHandler = game->getDialogueHandler();
		engine = engineToUse;
		addScriptsToGameWorld();
	}

	// This runs all setup scripts.
	void runStartingScripts() {
		scriptManager->runScript("setup_characters");
		scriptManager->runScript("setup_teleporters");
	}
}