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
	CharacterManager* characterManager;
	ScriptManager* scriptManager;
	DialogueHandler* dialogueHandler;
	Engine* engine;

	void testGameScript() {
		dialogueHandler->showDialogue("Yo!", "");
		std::vector<TileDirection> movements(30, TileDirection::RIGHT);
		/*worldManager->moveCharacterIfPossible("Meowth", movements);
		worldManager->moveCharacterIfPossible("Meowth Clone", movements);*/
		for (int i = 0; i < 30; i++) {
			worldManager->moveCharacterIfPossible("Meowth", TileDirection::RIGHT);
		}
		for (int i = 0; i < 30; i++) {
			worldManager->moveCharacterIfPossible("Meowth Clone", TileDirection::RIGHT);
		}
	}

	void testGameScript2() {
		dialogueHandler->showDialogue("Hello! This is some dialogue that should continue onto the second line!", "");
	}

	void testGameScript3() {
		std::vector<TileDirection> movements(4, TileDirection::LEFT);
		worldManager->moveCharacterIfPossible(PLAYER_NAME, movements);
	}

	void hideDialogue() {
		dialogueHandler->closeDialog();
	}

	// This sets up the characters if the world.
	void setupCharacters() {
		Character testCharacter;
		testCharacter.setup(&Location(0, glm::ivec2(0, 0), glm::ivec3(15, 0, 7)));
		testCharacter.setTextureNames("character_right", "character_forward", "character_left", "character_backward");
		testCharacter.setName("Meowth");
		size_t playerID;
		if (!characterManager->addCharacter(&testCharacter, &playerID)) {
			AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up Meowth.");
		}

		Character testCharacter2;
		testCharacter2.setup(&Location(0, glm::ivec2(0, 0), glm::ivec3(15, 0, 8)));
		testCharacter2.setTextureNames("character_right", "character_forward", "character_left", "character_backward");
		testCharacter2.setName("Meowth Clone");
		size_t playerID2;
		if (!characterManager->addCharacter(&testCharacter2, &playerID2)) {
			AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up Meowth.");
		}
	}

	// This adds all gameplay scripts to the ScriptManager and also binds tile-related scripts to their tiles.
	void addScriptsToGameWorld() {
		scriptManager->addScript("test_game_script", std::bind(&testGameScript));
		worldManager->addWalkedOnScript("test_game_script", &Location(0, glm::ivec2(0, 0), glm::ivec3(15, 0, 1)));
		scriptManager->addScript("test_game_script_2", std::bind(&testGameScript2));
		worldManager->addWalkedOnScript("test_game_script_2", &Location(0, glm::ivec2(0, 0), glm::ivec3(2, 0, 1)));
		scriptManager->addScript("test_game_script_3", std::bind(&testGameScript3));
		worldManager->addWalkedOnScript("test_game_script_3", &Location(0, glm::ivec2(0, 0), glm::ivec3(3, 0, 1)));
		scriptManager->addScript("setup_characters", std::bind(&setupCharacters));
		scriptManager->addScript("hide_dialogue", std::bind(&hideDialogue));
	}

	// This sets up necessary items for gameplay scripts.
	void setupGameScripts(AelaGame* gameToUse, Engine* engineToUse) {
		game = gameToUse;
		worldManager = game->getWorldManager();
		characterManager = worldManager->getCharacterManager();
		scriptManager = game->getScriptManager();
		dialogueHandler = game->getDialogueHandler();
		engine = engineToUse;
		addScriptsToGameWorld();
	}

	// This runs all setup scripts.
	void runStartingScripts() {
		scriptManager->runScript("setup_characters");
	}
}