/*
* Name: Scene Script
* Author: Robert Ciborowski
* Date: 03/01/2018
* Description: A file which contains scene-related scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once

#include "Aela_Engine.h"
#include "Scenes/SceneManager.h"
#include "Menus/Label.h"
#include "Menus/ImageComponent.h"
#include "Menus/Button.h"
#include "Menus/RectComponent.h"
#include "Menus/SubMenu.h"
#include "../Aela Game/AelaGame.h"
#include "../../Resources/ResourceInfo.h"

#define EKKON_INTRO_SCENE 1
#define MAIN_MENU_SCENE 2
#define WORLD_GAMEPLAY_SCENE 3
#define PAUSE_SCENE 4
#define INVENTORY_SCENE 5

using namespace Aela;

namespace Game {
	// This script is currently incomplete! The other menus still need to be added!
	static void setupScenes(Engine* engine, AelaGame* game) {
		// This creates some objects for later.
		ResourceManager* resourceManager = engine->getResourceManager();
		FontManager* fontManager = engine->getFontManager();
		GLRenderer* renderer = engine->getRenderer();
		DialogueHandler* dialogueHandler = game->getDialogueHandler();
		TextFont* xeroxLarge = fontManager->obtainTextFont((std::string) RESOURCE_ROOT, "res/fonts/xerox.ttf", 35);
		TextFont* xerox = fontManager->obtainTextFont((std::string) RESOURCE_ROOT, "res/fonts/xerox.ttf", 22);

		if (xeroxLarge == nullptr || xerox == nullptr) {
			AelaErrorHandling::windowError("A critical font (xerox.ttf) could not be loaded, aborting!");
			return;
		}

		// These are some fun colours.
		ColourRGBA darkerBlue(0.8392f, 0.8588f, 0.9137f, 1.0f);
		ColourRGBA almostWhite(0.9f, 0.9f, 0.9f, 1.0f);
		ColourRGBA almostBlack(0.1f, 0.1f, 0.1f, 1.0f);

		// These are tints for buttons.
		ColourRGBA hoverTint(0.8f, 0.8f, 0.8f, 1.0f);
		ColourRGBA clickTint(0.6f, 0.6f, 0.6f, 1.0f);

		// These are some pointers that we won't have to recreate over and over.
		ColourRGBA* darkerBluePtr = &darkerBlue;
		ColourRGBA* almostWhitePtr = &almostWhite;
		ColourRGBA* almostBlackPtr = &almostBlack;
		ColourRGBA* hoverTintPtr = &hoverTint;
		ColourRGBA* clickTintPtr = &clickTint;

		Rect<int> windowDimensions = *((Rect<signed int>*) engine->getWindow()->getDimensions());

		// The following blocks of code set up the Ekkon intro scene.
		auto ekkonImage = std::make_shared<ImageComponent>();
		GLTexture* ekkonTexture;
		bool success = resourceManager->obtain<GLTexture>("res/textures/ekkon.dds", ekkonTexture);
		if (success) {
			ekkonImage->setDimensions(&windowDimensions);
			ekkonImage->setTexture(ekkonTexture);
			ekkonImage->setTint(&ColourRGBA(1, 1, 1, 0));
		} else {
			AelaErrorHandling::consoleWindowError("Missing texture: ../../res/textures/ekkon.dds");
		}

		// This sets up the ekkon scene.
		auto ekkonScene = new Scene();
		ekkonScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
		ekkonScene->getMenu()->add(ekkonImage);

		// This sets up text.
		auto titleText = std::make_shared<Label>("Pokemon Meitnerium", xeroxLarge, &almostWhite);
		titleText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.05), (int) (windowDimensions.getHeight() / 1.5f));
		auto ekkonGamesText = std::make_shared<Label>("Ekkon Games", xerox, &almostWhite);
		ekkonGamesText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.8), ((int) (windowDimensions.getHeight() * 0.95)));

		// This sets up actions for the main menu buttons.
		auto startNewGameAction = [game](Engine* engine) {
			engine->getSceneManager()->setCurrentScene(WORLD_GAMEPLAY_SCENE);
			game->switchScene(WORLD_GAMEPLAY_SCENE);
		};

		auto continueGameAction = [game](Engine* engine) {
			engine->getSceneManager()->setCurrentScene(WORLD_GAMEPLAY_SCENE);
			game->switchScene(WORLD_GAMEPLAY_SCENE);
		};

		auto optionsAction = [game](Engine* engine) {AelaErrorHandling::windowWarning("There are no options!");};
		auto exitAction = [](Engine* engine) {engine->getWindow()->quit();};

		// This sets up some textureless buttons. Note: setText() uses information about the button's dimensions. In order to setup text for
		// a button, make sure that you set the button's position before hand.
		auto startGameButton = std::make_shared<Button>();
		startGameButton->setupOnClick(std::bind(startNewGameAction, engine));
		startGameButton->setPosition((int) (windowDimensions.getWidth() * 0.06),
			(int) (windowDimensions.getHeight() / 1.42f));
		startGameButton->setText("Start a New Game");
		startGameButton->setTextFont(xerox);
		startGameButton->setTextColour(almostWhitePtr);
		startGameButton->wrapAroundText();

		int spacing = startGameButton->getDimensions()->getHeight() + windowDimensions.getHeight() / 25;

		auto continueGameButton = std::make_shared<Button>();
		continueGameButton->setupOnClick(std::bind(continueGameAction, engine));
		continueGameButton->setPosition((int) (windowDimensions.getWidth() * 0.06),
			(int) (windowDimensions.getHeight() / 1.42f + spacing));
		continueGameButton->setText("Continue Game");
		continueGameButton->setTextFont(xerox);
		continueGameButton->setTextColour(almostWhitePtr);
		continueGameButton->wrapAroundText();

		auto optionsButton = std::make_shared<Button>();
		optionsButton->setupOnClick(std::bind(optionsAction, engine));
		optionsButton->setPosition((int) (windowDimensions.getWidth() * 0.06),
			(int) (windowDimensions.getHeight() / 1.42f + spacing * 2));
		optionsButton->setText("Options");
		optionsButton->setTextFont(xerox);
		optionsButton->setTextColour(almostWhitePtr);
		optionsButton->wrapAroundText();

		auto exitButton = std::make_shared<Button>();
		exitButton->setupOnClick(std::bind(exitAction, engine));
		exitButton->setPosition((int) (windowDimensions.getWidth() * 0.06),
			(int) (windowDimensions.getHeight() / 1.42f + spacing * 3));
		exitButton->setText("Exit");
		exitButton->setTextFont(xerox);
		exitButton->setTextColour(almostWhitePtr);
		exitButton->wrapAroundText();

		// This sets up the main menu scene.
		auto mainMenuScene = new Scene();
		mainMenuScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
		mainMenuScene->getMenu()->add(titleText);
		mainMenuScene->getMenu()->add(ekkonGamesText);
		mainMenuScene->getMenu()->add(startGameButton);
		mainMenuScene->getMenu()->add(continueGameButton);
		mainMenuScene->getMenu()->add(optionsButton);
		mainMenuScene->getMenu()->add(exitButton);

		// This is the submenu for dialogue.
		auto dialogueBoxSubMenu = std::make_shared<SubMenu>();
		dialogueBoxSubMenu->init(&windowDimensions, *renderer);

		auto dialgoueBoxImage = std::make_shared<ImageComponent>();
		GLTexture* dialgoueBoxTexture;
		success = resourceManager->obtain<GLTexture>("res/textures/dialogue_box.dds", dialgoueBoxTexture);
		dialgoueBoxImage->setDimensions(&Rect<int>(0, windowDimensions.getHeight() * 3 / 4, windowDimensions.getWidth(),
			windowDimensions.getHeight() / 4));
		dialgoueBoxImage->setTexture(dialgoueBoxTexture);

		auto dialogueTextOne = std::make_shared<Label>("Dialogue!", xeroxLarge, &almostBlack);
		auto dialogueTextTwo = std::make_shared<Label>("Dialogue 2!", xeroxLarge, &almostBlack);
		auto dialogueTextThree = std::make_shared<Label>("Dialogue 3!", xeroxLarge, &almostBlack);
		auto dialogueTextFour = std::make_shared<Label>("Dialogue 4!", xeroxLarge, &almostBlack);
		dialogueTextOne->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.05), (int) (windowDimensions.getHeight() * 0.85));
		dialogueTextTwo->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.05), (int) (windowDimensions.getHeight() * 0.95));
		dialogueTextThree->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.5), (int) (windowDimensions.getHeight() * 0.85));
		dialogueTextFour->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.5), (int) (windowDimensions.getHeight() * 0.95));

		dialogueBoxSubMenu->add(dialgoueBoxImage);
		dialogueBoxSubMenu->add(dialogueTextOne);
		dialogueBoxSubMenu->add(dialogueTextTwo);
		dialogueBoxSubMenu->add(dialogueTextThree);
		dialogueBoxSubMenu->add(dialogueTextFour);
		
		dialogueHandler->setDialogueSubMenu(dialogueBoxSubMenu);
		dialogueHandler->setDialogueLabels(dialogueTextOne, dialogueTextTwo, dialogueTextThree, dialogueTextFour);

		// This sets up the submenu that shows the player's tile inventory. The second submenu only contains the
		// box that signifies which tile is selected, which is done so that the selection box is always above
		// the tiles.
		auto tileInventorySubMenu = std::make_shared<SubMenu>(), tileInventorySubMenu2 = std::make_shared<SubMenu>();
		tileInventorySubMenu->init(&Rect<int>(0, 0, windowDimensions.getWidth(), windowDimensions.getHeight() / 3), *renderer);
		tileInventorySubMenu2->init(&Rect<int>(0, 0, windowDimensions.getWidth(), windowDimensions.getHeight() / 3), *renderer);

		auto tileInventoryTextOne = std::make_shared<Label>("<current tile>", xerox, &almostWhite);
		tileInventoryTextOne->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.8), (int) (windowDimensions.getHeight() / 5));

		auto tileSelectorBox = std::make_shared<ImageComponent>();
		GLTexture* tileInventoryBoxTexture;
		success = resourceManager->obtain<GLTexture>("res/textures/selector_box.png", tileInventoryBoxTexture);
		int tileSelectorWidthAndHeight = windowDimensions.getHeight() / 8;
		tileSelectorBox->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.98 - tileSelectorWidthAndHeight - tileSelectorWidthAndHeight * 0.1),
			(int) (windowDimensions.getHeight() * 0.15 - tileSelectorWidthAndHeight - tileSelectorWidthAndHeight * 0.1),
			(int) (tileSelectorWidthAndHeight * 1.2), (int) (tileSelectorWidthAndHeight * 1.2)));
		tileSelectorBox->setTexture(tileInventoryBoxTexture);

		tileInventorySubMenu->add(tileInventoryTextOne);
		tileInventorySubMenu2->add(tileSelectorBox);
		tileInventorySubMenu->show();
		tileInventorySubMenu2->show();
		game->setTileInventoryMenuItems(tileInventorySubMenu, tileInventoryTextOne, tileSelectorBox);

		auto deathBackgroundRect = std::make_shared<RectComponent>();
		deathBackgroundRect->setDimensions(&windowDimensions);
		deathBackgroundRect->setColour(&ColourRGBA(0.15f, 0.15f, 0.15f, 0.95f));
		deathBackgroundRect->setTint(&ColourRGBA(1, 1, 1, 0));

		auto deathText = std::make_shared<Label>("You died.", xerox, &almostWhite);
		deathText->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.30), (int)(windowDimensions.getHeight() * 0.30));
		deathText->setTint(&ColourRGBA(1, 1, 1, 0));

		game->getWorldManager()->getCharacterTracker()->setGameplayMenuItems(deathBackgroundRect, deathText);

		// This sets up the world gameplay scene, in which the player is given a top-down view of the world.
		auto worldGameplayScene = new Scene();
		worldGameplayScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
		worldGameplayScene->getMenu()->add(dialogueBoxSubMenu);
		worldGameplayScene->getMenu()->add(tileInventorySubMenu);
		worldGameplayScene->getMenu()->add(tileInventorySubMenu2);
		worldGameplayScene->getMenu()->add(deathBackgroundRect);
		worldGameplayScene->getMenu()->add(deathText);

		// This sets up the scenes for the pause menu.
		Scene* pauseScene = new Scene();
		pauseScene->enableMenu(engine->getWindow()->getDimensions(), *engine->getRenderer());

		// The following blocks of code setup the pause menu scene.
		auto tintRect = std::make_shared<RectComponent>();
		tintRect->setDimensions(&windowDimensions);
		tintRect->setColour(&ColourRGBA(0.4f, 0.4f, 0.4f, 0.35f));

		// This sets up a rectangle that the pause menu scene will use.
		auto pauseBackgroundRect = std::make_shared<RectComponent>();
		pauseBackgroundRect->setDimensions(&Rect<int>((int)(windowDimensions.getWidth() * 0.3125), (int)(windowDimensions.getHeight() * 0.1111),
			(int)(windowDimensions.getWidth() * 0.5625), (int)(windowDimensions.getHeight() * 0.7777)));
		pauseBackgroundRect->setColour(&ColourRGBA(0.2f, 0.2f, 0.2f, 0.95f));

		// This is the box that says "Pause Menu".
		auto pauseMenuTextRect = std::make_shared<RectComponent>();
		pauseMenuTextRect->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.1111),
			(int)(windowDimensions.getWidth() * 0.1875), (int)(windowDimensions.getHeight() * 0.1111)));
		pauseMenuTextRect->setColour(&ColourRGBA(0.2f, 0.2f, 0.2f, 0.95f));

		auto pauseMenuTitleText = std::make_shared<Label>("Pause Menu", xerox, &almostWhite);
		pauseMenuTitleText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.15), (int) (windowDimensions.getHeight() * 0.18));

		// This creates sidebar buttons for the pause menu.
		auto pauseMenuGameButton = std::make_shared<Button>(&hoverTint, &clickTint);
		auto pauseMenuOptionsButton = std::make_shared<Button>(&hoverTint, &clickTint);

		// This gets textures for the pause menu buttons.
		GLTexture* simpleButtonTexture;
		GLTexture* simpleButtonTextureLight;
		success = engine->getResourceManager()->obtain<GLTexture>((std::string) DEFAULT_TEXTURE_PATH + "simple_button.dds", simpleButtonTexture);
		success = engine->getResourceManager()->obtain<GLTexture>((std::string) DEFAULT_TEXTURE_PATH + "simple_button_light.dds", simpleButtonTextureLight);

		auto pauseMenuGameSubMenu = std::make_shared<SubMenu>();
		pauseMenuGameSubMenu->init(&windowDimensions, *renderer);
		auto pauseMenuOptionsSubMenu = std::make_shared<SubMenu>();
		pauseMenuOptionsSubMenu->init(&windowDimensions, *renderer);

		auto goToGameSubMenu = [game](Engine* engine) {

		};

		auto goToOptionsSubMenu = [game](Engine* engine) {
			
		};

		pauseMenuGameButton->setTexture(simpleButtonTextureLight);
		pauseMenuGameButton->setDimensions(&Rect<int>((int)(windowDimensions.getWidth() * 0.125), (int)(windowDimensions.getHeight() * 0.2222),
			(int)(windowDimensions.getWidth() * 0.1875), (int)(windowDimensions.getHeight() * 0.1111)));
		pauseMenuGameButton->setupOnClick(std::bind(goToGameSubMenu, engine));
		pauseMenuGameButton->setText("Game");
		pauseMenuGameButton->setTextFont(xerox);
		pauseMenuGameButton->setTextColour(almostWhitePtr);

		pauseMenuOptionsButton->setTexture(simpleButtonTextureLight);
		pauseMenuOptionsButton->setDimensions(&Rect<int>((int)(windowDimensions.getWidth() * 0.125), (int)(windowDimensions.getHeight() * 0.3333),
			(int)(windowDimensions.getWidth() * 0.1875), (int)(windowDimensions.getHeight() * 0.1111)));
		pauseMenuOptionsButton->setupOnClick(std::bind(goToOptionsSubMenu, engine));
		pauseMenuOptionsButton->setText("Options");
		pauseMenuOptionsButton->setTextFont(xerox);
		pauseMenuOptionsButton->setTextColour(almostWhitePtr);
		pauseMenuOptionsButton->wrapAroundText();

		pauseMenuGameSubMenu->show();

		pauseScene->getMenu()->add(tintRect);
		pauseScene->getMenu()->add(pauseBackgroundRect);
		pauseScene->getMenu()->add(pauseMenuGameButton);
		pauseScene->getMenu()->add(pauseMenuGameSubMenu);
		pauseScene->getMenu()->add(pauseMenuOptionsButton);
		pauseScene->getMenu()->add(pauseMenuOptionsSubMenu);
		pauseScene->getMenu()->add(pauseMenuTextRect);
		pauseScene->getMenu()->add(pauseMenuTitleText);

		// This sets up the inventory scene.
		Scene* inventoryScene = new Scene();
		inventoryScene->enableMenu(engine->getWindow()->getDimensions(), *engine->getRenderer());

		// This sets up a rectangle that the inventory scene will use.
		auto inventoryBackgroundRect = std::make_shared<RectComponent>();
		inventoryBackgroundRect->setDimensions(&Rect<int>((int)(windowDimensions.getWidth() * 0.3125), (int)(windowDimensions.getHeight() * 0.33333),
			(int)(windowDimensions.getWidth() * 0.375), (int)(windowDimensions.getHeight() * 0.22222)));
		inventoryBackgroundRect->setColour(&ColourRGBA(0.15f, 0.15f, 0.15f, 0.95f));

		// This sets up text for the inentory scene.
		auto inventoryTitleText = std::make_shared<Label>("Inventory", xerox, &almostWhite);
		inventoryTitleText->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.33), (int)(windowDimensions.getHeight() * 0.38));

		auto inventorySlotA = std::make_shared<ImageComponent>();
		GLTexture* inventorySlotTexture;
		success = resourceManager->obtain<GLTexture>("res/textures/inventory_slot.png", inventorySlotTexture);
		inventorySlotA->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.37), (int) (windowDimensions.getHeight() * 0.4),
			(int) (windowDimensions.getWidth() * 0.1), (int) (windowDimensions.getWidth() * 0.1)));
		inventorySlotA->setTexture(inventorySlotTexture);

		auto inventorySlotB = std::make_shared<ImageComponent>();
		inventorySlotB->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.53), (int) (windowDimensions.getHeight() * 0.4),
			(int) (windowDimensions.getWidth() * 0.1), (int) (windowDimensions.getWidth() * 0.1)));
		inventorySlotB->setTexture(inventorySlotTexture);

		inventoryScene->getMenu()->add(tintRect);
		inventoryScene->getMenu()->add(inventoryBackgroundRect);
		inventoryScene->getMenu()->add(inventoryTitleText);
		inventoryScene->getMenu()->add(inventorySlotA);
		inventoryScene->getMenu()->add(inventorySlotB);

		Map3D* map;
		success = engine->getResourceManager()->obtain<Map3D>("res/maps/map.txt", map);
		if (success) {
			mainMenuScene->setMap(map);
			worldGameplayScene->setMap(map);
			pauseScene->setMap(map);
			inventoryScene->setMap(map);
		} else {
			AelaErrorHandling::windowError("There was a problem loading map.txt!");
		}

		engine->getSceneManager()->registerScene(ekkonScene, EKKON_INTRO_SCENE);
		engine->getSceneManager()->registerScene(mainMenuScene, MAIN_MENU_SCENE);
		engine->getSceneManager()->registerScene(worldGameplayScene, WORLD_GAMEPLAY_SCENE);
		engine->getSceneManager()->registerScene(pauseScene, PAUSE_SCENE);
		engine->getSceneManager()->registerScene(inventoryScene, INVENTORY_SCENE);
		engine->getSceneManager()->setDisposingScenesOnDestroy(true);

		engine->getSceneManager()->setCurrentScene(EKKON_INTRO_SCENE);
		game->setGameplayScene(worldGameplayScene);
		game->switchScene(EKKON_INTRO_SCENE);
		// engine->getSceneManager()->setCurrentScene(WORLD_GAMEPLAY_SCENE);
		// game->switchScene(WORLD_GAMEPLAY_SCENE);

		// I set this animation to go by really quickly in order to save debugging time.
		Animator* animator = engine->getAnimator();
		AnimationTrack2D track;
		for (int i = 0; i < 4; i++) {
			KeyFrame2D frame;
			frame.setObject(ekkonImage);
			if (i == 3) {
				auto action = [](Engine* engine, AelaGame* game) {
					/*engine->getSceneManager()->setCurrentScene(WORLD_GAMEPLAY_SCENE);
					game->switchScene(WORLD_GAMEPLAY_SCENE);*/
					engine->getSceneManager()->setCurrentScene(MAIN_MENU_SCENE);
					game->switchScene(MAIN_MENU_SCENE);
				};
				frame.setEndingAction(std::bind(action, engine, game));
			}
			switch (i) {
				case 0:
					frame.setTint(&ColourRGBA(1, 1, 1, 0));
					track.addKeyFrameUsingMillis(50 /*500*/, &frame);
					break;
				case 1:
					frame.setTint(&ColourRGBA(1, 1, 1, 1));
					track.addKeyFrameUsingMillis(150 /*1500*/, &frame);
					break;
				case 2:
					frame.setTint(&ColourRGBA(1, 1, 1, 1));
					track.addKeyFrameUsingMillis(100 /*1000*/, &frame);
					break;
				case 3:
					frame.setTint(&ColourRGBA(1, 1, 1, 0));
					track.addKeyFrameUsingMillis(150 /*1500*/, &frame);
					break;
			}
		}
		animator->addAnimationTrack2D(&track);
	}
}