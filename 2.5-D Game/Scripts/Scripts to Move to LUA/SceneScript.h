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
#include "../Aela Game/AelaGame.h"
#include "Menus/SubMenu.h"

#define EKKON_INTRO_SCENE 1
#define MAIN_MENU_SCENE 2
#define WORLD_GAMEPLAY_SCENE 3
#define BATTLE_GAMEPLAY_SCENE 4

using namespace Aela;

namespace Game {
	// This script is currently incomplete! The other menus still need to be added!
	static void setupScenes(Engine* engine, AelaGame* game) {
		// This creates some objects for later.
		ResourceManager* resourceManager = engine->getResourceManager();
		FontManager* fontManager = engine->getFontManager();
		GLRenderer* renderer = engine->getRenderer();
		DialogueHandler* dialogueHandler = game->getDialogueHandler();
		TextFont* xeroxLarge = fontManager->obtainTextFont("../../res/fonts/xerox.ttf", 35);
		TextFont* xerox = fontManager->obtainTextFont("../../res/fonts/xerox.ttf", 22);
		if (xeroxLarge == nullptr || xerox == nullptr) {
			AelaErrorHandling::windowError("A critical font (xerox.ttf) could not be loaded, aborting!");
			return;
		}
		ColourRGBA VSBlue(0.8392f, 0.8588f, 0.9137f, 1.0f);
		ColourRGBA almostWhite(0.9f, 0.9f, 0.9f, 1.0f);
		ColourRGBA almostBlack(0.1f, 0.1f, 0.1f, 1.0f);
		Rect<int> windowDimensions = *((Rect<signed int>*) engine->getWindow()->getDimensions());

		// The following blocks of code set up the Ekkon intro scene.
		auto ekkonImage = std::make_shared<ImageComponent>();
		GLTexture* ekkonTexture;
		bool success = resourceManager->obtain<GLTexture>("../../res/textures/ekkon.dds", ekkonTexture);
		ekkonImage->setDimensions(&windowDimensions);
		ekkonImage->setTexture(ekkonTexture);
		ekkonImage->setTint(&ColourRGBA(1, 1, 1, 0));

		// This sets up the ekkon scene.
		auto ekkonScene = new Scene();
		ekkonScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
		ekkonScene->getMenu()->add(ekkonImage);

		// This sets up text.
		auto titleText = std::make_shared<Label>("Pokemon Meitnerium", xeroxLarge, &almostWhite);
		titleText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.05), (int) (windowDimensions.getHeight() / 1.5f));
		auto ekkonGamesText = std::make_shared<Label>("Ekkon Games", xerox, &almostWhite);
		ekkonGamesText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.8), ((int) (windowDimensions.getHeight() * 0.95)));
		auto startGameButtonText = std::make_shared<Label>("Start a New Game", xerox, &almostWhite);
		int spacing = startGameButtonText->getDimensions()->getHeight() + windowDimensions.getHeight() / 25;
		auto continueGameButtonText = std::make_shared<Label>("Continue Game", xerox, &almostWhite);
		auto optionsButtonText = std::make_shared<Label>("Options", xerox, &almostWhite);
		auto exitButtonText = std::make_shared<Label>("Exit", xerox, &almostWhite);

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
		startGameButton->setDimensions(startGameButtonText->getDimensions());
		startGameButton->setupOnClick(std::bind(startNewGameAction, engine));
		startGameButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.06),
			(int) (windowDimensions.getHeight() / 1.42f));
		startGameButton->setText(startGameButtonText);

		auto continueGameButton = std::make_shared<Button>();
		continueGameButton->setDimensions(continueGameButtonText->getDimensions());
		continueGameButton->setupOnClick(std::bind(continueGameAction, engine));
		continueGameButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.06),
			(int) (windowDimensions.getHeight() / 1.42f + spacing));
		continueGameButton->setText(continueGameButtonText);

		auto optionsButton = std::make_shared<Button>();
		optionsButton->setDimensions(optionsButtonText->getDimensions());
		optionsButton->setupOnClick(std::bind(optionsAction, engine));
		optionsButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.06),
			(int) (windowDimensions.getHeight() / 1.42f + spacing * 2));
		optionsButton->setText(optionsButtonText);

		auto exitButton = std::make_shared<Button>();
		exitButton->setDimensions(exitButtonText->getDimensions());
		exitButton->setupOnClick(std::bind(exitAction, engine));
		exitButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.06),
			(int) (windowDimensions.getHeight() / 1.42f + spacing * 3));
		exitButton->setText(exitButtonText);

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
		success = resourceManager->obtain<GLTexture>("../../res/textures/dialogue_box.dds", dialgoueBoxTexture);
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

		auto tileInventoryBox = std::make_shared<ImageComponent>();
		GLTexture* tileInventoryBoxTexture;
		success = resourceManager->obtain<GLTexture>("../../res/textures/selector_box.dds", tileInventoryBoxTexture);
		int imageWidthAndHeight = windowDimensions.getHeight() / 8;
		tileInventoryBox->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.98 - imageWidthAndHeight - imageWidthAndHeight * 0.1),
			(int) (windowDimensions.getHeight() * 0.15 - imageWidthAndHeight - imageWidthAndHeight * 0.1),
			(int) (imageWidthAndHeight * 1.2), (int) (imageWidthAndHeight * 1.2)));
		tileInventoryBox->setTexture(tileInventoryBoxTexture);

		tileInventorySubMenu->add(tileInventoryTextOne);
		tileInventorySubMenu2->add(tileInventoryBox);
		tileInventorySubMenu->show();
		tileInventorySubMenu2->show();
		game->setTileInventoryMenuItems(tileInventorySubMenu, tileInventoryTextOne, tileInventoryBox);

		// This sets up the world gameplay scene, in which the player is given a top-down view of the world.
		auto worldGameplayScene = new Scene();
		worldGameplayScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
		worldGameplayScene->getMenu()->add(dialogueBoxSubMenu);
		worldGameplayScene->getMenu()->add(tileInventorySubMenu);
		worldGameplayScene->getMenu()->add(tileInventorySubMenu2);

		Map3D* map;
		success = engine->getResourceManager()->obtain<Map3D>("../../res/maps/map.txt", map);
		if (success) {
			mainMenuScene->setMap(map);
			worldGameplayScene->setMap(map);
		} else {
			AelaErrorHandling::windowError("There was a problem loading map.txt!");
		}

		engine->getSceneManager()->registerScene(ekkonScene, EKKON_INTRO_SCENE);
		engine->getSceneManager()->registerScene(mainMenuScene, MAIN_MENU_SCENE);
		engine->getSceneManager()->registerScene(worldGameplayScene, WORLD_GAMEPLAY_SCENE);
		engine->getSceneManager()->setDisposingScenesOnDestroy(true);

		engine->getSceneManager()->setCurrentScene(EKKON_INTRO_SCENE);
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
					frame.setDimensions(&windowDimensions);
					track.addKeyFrameUsingMillis(500, &frame);
					break;
				case 1:
					frame.setTint(&ColourRGBA(1, 1, 1, 1));
					frame.setDimensions(&windowDimensions);
					track.addKeyFrameUsingMillis(1500, &frame);
					break;
				case 2:
					frame.setTint(&ColourRGBA(1, 1, 1, 1));
					frame.setDimensions(&windowDimensions);
					track.addKeyFrameUsingMillis(1000, &frame);
					break;
				case 3:
					frame.setTint(&ColourRGBA(1, 1, 1, 0));
					frame.setDimensions(&windowDimensions);
					track.addKeyFrameUsingMillis(1500, &frame);
					break;
			}
		}
		animator->addAnimationTrack2D(&track);
	}
}