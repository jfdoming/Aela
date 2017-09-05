// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once

#include "Aela_Engine.h"
#include "Scenes\SceneManager.h"
#include "Menus\Label.h"
#include "Menus\ImageComponent.h"
#include "Menus\Button.h"

using namespace Aela;

#define EKKON_INTRO_SCENE 1
#define MAIN_MENU_SCENE 2
#define GAMEPLAY_SCENE 3

void setupScenes(Engine* engine, AelaGame* game) {
	// This creates some objects for later.
	FontManager* fontManager = engine->getFontManager();
	TextFont* xeroxLarge = fontManager->obtainTextFont("res/fonts/xerox.ttf", 35);
	TextFont* xerox = fontManager->obtainTextFont("res/fonts/xerox.ttf", 18);
	if (xeroxLarge == nullptr || xerox == nullptr) {
		AelaErrorHandling::windowError("A critical font (xerox.ttf) could not be loaded, aborting!");
		return;
	}
	ColourRGBA VSBlue(0.8392f, 0.8588f, 0.9137f, 1.0f);
	ColourRGBA almostWhite(0.9f, 0.9f, 0.9f, 1.0f);
	Rect<int> windowDimensions = *((Rect<signed int>*) engine->getWindow()->getWindowDimensions());

	// The following blocks of code set up the Ekkon intro scene.
	auto ekkonImage = std::make_shared<ImageComponent>();
	Texture* ekkonTexture;
	bool success = engine->getResourceManager()->obtain<Texture>("res/textures/ekkon.dds", ekkonTexture);
	ekkonImage->setDimensions(&windowDimensions);
	ekkonImage->setTexture(ekkonTexture);
	ekkonImage->setTint(&ColourRGBA(1, 1, 1, 0));

	// This sets up the ekkon scene.
	auto ekkonScene = new Scene();
	ekkonScene->setId(EKKON_INTRO_SCENE);
	ekkonScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	ekkonScene->getMenu()->add(ekkonImage);

	// The following blocks of code set up the main menu scene.
	// This sets up text. You know what's annoying though? When Visual Studio turns "(int) (...)" into "(int)(...)" when I copy
	// and paste the former!
	auto titleText = std::make_shared<Label>("Aela Program", xeroxLarge, &VSBlue, fontManager);
	titleText->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.05), (int)(windowDimensions.getHeight() / 1.3f));
	auto ekkonGamesText = std::make_shared<Label>("Ekkon Games", xerox, &VSBlue, fontManager);
	ekkonGamesText->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.8), ((int)(windowDimensions.getHeight() * 0.95)));
	auto continueGameButtonText = std::make_shared<Label>("Edit Map", xerox, &VSBlue, fontManager);
	int spacing = continueGameButtonText->getDimensions()->getHeight() + windowDimensions.getHeight() / 25;
	auto optionsButtonText = std::make_shared<Label>("Help", xerox, &VSBlue, fontManager);
	auto exitButtonText = std::make_shared<Label>("Exit", xerox, &VSBlue, fontManager);

	// This sets up actions for the main menu buttons.
	auto continueGameAction = [game](Engine* engine) {
		Rect<unsigned int>* dimensions = engine->getWindow()->getWindowDimensions();
		engine->getWindow()->setCursorPositionInWindow(dimensions->getWidth() / 2, dimensions->getHeight() / 2);
		engine->getSceneManager()->setCurrentScene(GAMEPLAY_SCENE);
		game->performActionOnSceneSwitch(GAMEPLAY_SCENE);
	};
	auto optionsAction = [](Engine* engine) {
		AelaErrorHandling::windowWarning("There's nothing to see here.");
	};
	auto exitAction = [](Engine* engine) {engine->getWindow()->quit(); };

	// This sets up some textureless buttons. Note: setText() uses information about the button's dimensions. In order to setup text for
	// a button, make sure that you set the button's position before hand.
	auto continueGameButton = std::make_shared<Button>();
	continueGameButton->setDimensions(continueGameButtonText->getDimensions());
	continueGameButton->setupOnClick(std::bind(continueGameAction, engine), engine->getEventHandler());
	continueGameButton->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.06), (int)(windowDimensions.getHeight() / 1.24f));
	continueGameButton->setText(continueGameButtonText.get(), engine->getFontManager());

	auto optionsButton = std::make_shared<Button>();
	optionsButton->setDimensions(optionsButtonText->getDimensions());
	optionsButton->setupOnClick(std::bind(optionsAction, engine), engine->getEventHandler());
	optionsButton->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.06), (int)(windowDimensions.getHeight() / 1.24f + spacing));
	optionsButton->setText(optionsButtonText.get(), engine->getFontManager());

	auto exitButton = std::make_shared<Button>();
	exitButton->setDimensions(exitButtonText->getDimensions());
	exitButton->setupOnClick(std::bind(exitAction, engine), engine->getEventHandler());
	exitButton->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.06), (int)(windowDimensions.getHeight() / 1.24f + spacing * 2));
	exitButton->setText(exitButtonText.get(), engine->getFontManager());

	// This sets up particles.
	PlanarParticleEmitter* particleEmitter = new PlanarParticleEmitter();
	Rect<GLfloat> emitterDimensions(0, 0, 10, 10);
	particleEmitter->setupDimensions(&emitterDimensions);
	particleEmitter->setCamera(engine->getRenderer()->getCamera());
	particleEmitter->setStats(0.000000001f, 20, 0.000000001f, 2, 2);
	particleEmitter->setPosition(5, 0, 5);
	particleEmitter->setRotation(0, 3.4f, 0.7f);
	particleEmitter->setTimeManager(engine->getTimeManager());

	// This sets up the title screen scene.
	Scene* mainMenuScene = new Scene();
	mainMenuScene->setId(MAIN_MENU_SCENE);
	mainMenuScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	mainMenuScene->getMenu()->add(titleText);
	mainMenuScene->getMenu()->add(ekkonGamesText);
	mainMenuScene->getMenu()->add(continueGameButton);
	mainMenuScene->getMenu()->add(optionsButton);
	mainMenuScene->getMenu()->add(exitButton);
	mainMenuScene->putParticleEmitter(particleEmitter);

	std::vector<Texture*> particleTextures;
	Texture* tResult;
	success = engine->getResourceManager()->obtain<Texture>("res/particles/particle_1.dds", tResult);
	particleTextures.push_back(tResult);
	success = engine->getResourceManager()->obtain<Texture>("res/particles/particle_2.dds", tResult);
	particleTextures.push_back(tResult);
	particleEmitter->setupParticles(&particleTextures, 0.6f, 0.6f, 25);

	Scene* gameplayScene = new Scene();
	gameplayScene->setId(GAMEPLAY_SCENE);
	gameplayScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	gameplayScene->putParticleEmitter(particleEmitter);

	Map3D* map;
	success = engine->getResourceManager()->obtain<Map3D>("res/maps/map.txt", map);
	if (success) {
		mainMenuScene->setMap(map);
		gameplayScene->setMap(map);
	} else {
		AelaErrorHandling::consoleWindowError("Scene Script", "res/maps/map.txt wasn't loaded properly or something.");
	}

	engine->getSceneManager()->registerScene(ekkonScene, EKKON_INTRO_SCENE);
	engine->getSceneManager()->registerScene(mainMenuScene, MAIN_MENU_SCENE);
	engine->getSceneManager()->registerScene(gameplayScene, GAMEPLAY_SCENE);
	engine->getSceneManager()->setCurrentScene(EKKON_INTRO_SCENE);
	engine->getSceneManager()->setDisposingScenesOnDestroy(true);


	// Experimentation with Animators.
	Animator* animator = engine->getAnimator();
	for (int i = 0; i < 4; i++) {
		KeyFrame2DList list;
		KeyFrame2D frame;
		frame.setObject(ekkonImage);
		if (i == 3) {
			auto action = [](Engine* engine) {engine->getSceneManager()->setCurrentScene(MAIN_MENU_SCENE); };
			frame.setEndingAction(std::bind(action, engine));
		}
		switch (i) {
			case 0:
				frame.setTint(&ColourRGBA(1, 1, 1, 0));
				list.setTimeAfterPreviousKeyFrameInMillis(2500);
				break;
			case 1:
				frame.setTint(&ColourRGBA(1, 1, 1, 1));
				list.setTimeAfterPreviousKeyFrameInMillis(2800);
				break;
			case 2:
				frame.setTint(&ColourRGBA(1, 1, 1, 1));
				list.setTimeAfterPreviousKeyFrameInMillis(2500);
				break;
			case 3:
				frame.setTint(&ColourRGBA(1, 1, 1, -0.3f));
				list.setTimeAfterPreviousKeyFrameInMillis(2300);
				break;
		}
		list.addKeyFrame(&frame);
		animator->addKeyFrame2DList(&list);
	}
}