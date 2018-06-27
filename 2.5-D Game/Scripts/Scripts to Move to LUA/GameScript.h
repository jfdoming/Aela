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

	void testGameScript() {
		dialogueHandler->showDialogue("Yo! This dialogue will automatically continue onto the second line!", "test_game_script_2");
	}

	void testGameScript2() {
		dialogueHandler->showDialogue("Well, it's now time to leave.",
			"test_game_script_3");
	}

	void testGameScript3() {
		// You can do one of the following. Like this:
		std::vector<TileDirection> movements(30, TileDirection::RIGHT);
		/*worldManager->moveCharacterIfPossible("Meowth", movements);
		worldManager->moveCharacterIfPossible("Meowth Clone", movements);*/

		// Or this:
		for (int i = 0; i < 30; i++) {
			worldManager->moveCharacterIfPossible("Meowth", TileDirection::RIGHT);
		}

		hideDialogue();
	}

	void testGameScript4() {
		dialogueHandler->showDialogue("Get away from me! You filthy monster, you!", "test_game_script_5");
	}

	void testGameScript5() {
		dialogueHandler->showOptions(&DialogueOption("No!", "hide_dialogue"), &DialogueOption("You go away!", "test_game_script_6"),
			&DialogueOption("Fine.", "test_game_script_7"));
	}

	void testGameScript6() {
		for (int i = 0; i < 30; i++) {
			worldManager->moveCharacterIfPossible("Meowth Clone", TileDirection::RIGHT);
		}
		hideDialogue();
	}

	void testGameScript7() {
		std::vector<TileDirection> movements(4, TileDirection::LEFT);
		worldManager->moveCharacterIfPossible(PLAYER_NAME, movements);
		hideDialogue();
	}

	void testGameScript8() {
		dialogueHandler->showDialogue("You found the secret tile!", "hide_dialogue");
	}

	void testGameScript9() {
		dialogueHandler->showDialogue("Shoo!", "hide_dialogue");
	}

	void testGameScript10() {
		dialogueHandler->showDialogue("Danny Shoo!", "hide_dialogue");
	}


	// This sets up the characters if the world.
	void setupCharacters() {
		Character* testCharacter = new Character();
		testCharacter->setup(&Location(0, glm::ivec2(0, 0), glm::ivec3(15, 0, 7)));
		testCharacter->setTextureName("character");
		testCharacter->setName("Meowth");
		size_t charID;
		if (!characterTracker->trackCharacter(testCharacter, &charID)) {
			AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up Meowth.");
		}

		Character* testCharacter2 = new Character();
		testCharacter2->setup(&Location(0, glm::ivec2(0, 0), glm::ivec3(15, 0, 8)));
		testCharacter2->setTextureName("character");
		testCharacter2->setName("Meowth Clone");
		size_t charID2;
		if (!characterTracker->trackCharacter(testCharacter2, &charID2)) {
			AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up a Meowth Clone.");
		}

		Character* testCharacter3 = new Character();
		testCharacter3->setup(&Location(0, glm::ivec2(1, 0), glm::ivec3(0, 0, 6)));
		testCharacter3->setTextureName("character");
		testCharacter3->setName("Meowth Clone 2");
		size_t charID3;
		if (!characterTracker->trackCharacter(testCharacter3, &charID3)) {
			AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up a Meowth Clone.");
		}

		Character* testCharacter4 = new Character();
		testCharacter4->setup(&Location(0, glm::ivec2(1, 0), glm::ivec3(0, 0, 7)));
		testCharacter4->setTextureName("character");
		testCharacter4->setName("Meowth Clone 3");
		size_t charID4;
		if (!characterTracker->trackCharacter(testCharacter4, &charID4)) {
			AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up a Meowth Clone.");
		}

		// This will add an enemy for testing purposes.
		Turret* turret = new Turret(2, 1024, 50000000);
		turret->setup(&Location(0, glm::ivec2(1, 0), glm::ivec3(1, 0, 4)));
		turret->setTextureName("turret");
		turret->setName("Test Turret");
		size_t turretID = 0;
		if (!characterTracker->trackCharacter(turret, &turretID)) {
			AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up the test turret.");
		}
		enemyRegistrar->registerTurret(turret);
	}

	void setupTeleporters() {
		Teleporter teleporter(&Location(0, glm::ivec2(0, 0), glm::ivec3(2, 0, 1)));
		worldManager->addTeleporter(&teleporter, &Location(0, glm::ivec2(0, 0), glm::ivec3(4, 1, 4)));
		teleporter = Teleporter(&Location(0, glm::ivec2(0, 0), glm::ivec3(4, 1, 4)));
		worldManager->addTeleporter(&teleporter, &Location(0, glm::ivec2(0, 0), glm::ivec3(2, 0, 1)));
		teleporter = Teleporter(&Location(0, glm::ivec2(1, 0), glm::ivec3(10, 1, 7)));
		worldManager->addTeleporter(&teleporter, &Location(0, glm::ivec2(0, 0), glm::ivec3(7, 0, 9)));
		teleporter = Teleporter(&Location(0, glm::ivec2(0, 0), glm::ivec3(7, 0, 9)));
		worldManager->addTeleporter(&teleporter, &Location(0, glm::ivec2(1, 0), glm::ivec3(10, 1, 7)));
	}

	// This adds all gameplay scripts to the ScriptManager and also binds tile-related scripts to their tiles.
	void addScriptsToGameWorld() {
		scriptManager->addScript("test_game_script", std::bind(&testGameScript));
		worldManager->addPromptedScript("test_game_script", &Location(0, glm::ivec2(0, 0), glm::ivec3(15, 0, 7)));
		worldManager->addTileSwitchScript("test_game_script", &Location(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 4)));
		scriptManager->addScript("test_game_script_2", std::bind(&testGameScript2));
		scriptManager->addScript("test_game_script_3", std::bind(&testGameScript3));
		scriptManager->addScript("test_game_script_4", std::bind(&testGameScript4));
		worldManager->addPromptedScript("test_game_script_4", &Location(0, glm::ivec2(0, 0), glm::ivec3(15, 0, 8)));
		scriptManager->addScript("test_game_script_5", std::bind(&testGameScript5));
		scriptManager->addScript("test_game_script_6", std::bind(&testGameScript6));
		scriptManager->addScript("test_game_script_7", std::bind(&testGameScript7));
		scriptManager->addScript("test_game_script_8", std::bind(&testGameScript8));
		worldManager->addWalkedOnScript("test_game_script_8", &Location(0, glm::ivec2(0, 0), glm::ivec3(2, 0, 2)));
		scriptManager->addScript("test_game_script_9", std::bind(&testGameScript9));
		worldManager->addPromptedScript("test_game_script_9", &Location(0, glm::ivec2(1, 0), glm::ivec3(0, 0, 6)));
		scriptManager->addScript("test_game_script_10", std::bind(&testGameScript10));
		worldManager->addPromptedScript("test_game_script_10", &Location(0, glm::ivec2(1, 0), glm::ivec3(0, 0, 7)));
		scriptManager->addScript("setup_characters", std::bind(&setupCharacters));
		scriptManager->addScript("setup_teleporters", std::bind(&setupTeleporters));
		scriptManager->addScript("hide_dialogue", std::bind(&hideDialogue));
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