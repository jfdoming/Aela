// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once

#include "Aela_Engine.h"
#include "Scenes\SceneManager.h"
#include "Menus\TextComponent.h"
#include "Menus\ImageComponent.h"
#include "Menus\ButtonComponent.h"
#include "Menus\RectComponent.h"

using namespace Aela;

void setupScenes(Engine* engine, AelaGame* game) {
	// This creates some objects for later.
	FontManager* textManager = engine->getTextManager();
	TextFont xeroxLarge = *textManager->obtainTextFont("res/fonts/xerox.ttf", 35);
	TextFont xerox = *textManager->obtainTextFont("res/fonts/xerox.ttf", 18);
	ColourRGBA VSBlue(0.8392f, 0.8588f, 0.9137f, 1.0f);
	ColourRGBA almostWhite(0.9f, 0.9f, 0.9f, 1.0f);
	Rect<int> windowDimensions = *((Rect<signed int>*) engine->getWindow()->getWindowDimensions());

	// This sets up an image.
	ImageComponent* mainMenuImage = new ImageComponent();
	Texture* mainMenuTexture;
	bool success = engine->getResourceManager()->obtain<Texture>("res/textures/map_editor_main_background.dds", mainMenuTexture);
	mainMenuImage->setDimensions(&windowDimensions);
	mainMenuImage->setTexture(mainMenuTexture);

	// This sets up text.
	TextComponent* titleText = new TextComponent("Project Aela Map Editor", &xeroxLarge, &VSBlue, textManager);
	titleText->getDimensions()->setX((int) (windowDimensions.getWidth() / 20));
	titleText->getDimensions()->setY((int) (windowDimensions.getHeight() / 1.3f));

	TextComponent* ekkonGamesText = new TextComponent("Ekkon Games", &xerox, &VSBlue, textManager);
	ekkonGamesText->getDimensions()->setX((int) (windowDimensions.getWidth() * 0.8));
	ekkonGamesText->getDimensions()->setY((int) (windowDimensions.getHeight() * 0.95));

	TextComponent* newMapButtonText = new TextComponent("Create New Map", &xerox, &VSBlue, textManager);
	int spacing = newMapButtonText->getDimensions()->getHeight() + windowDimensions.getHeight() / 25;
	TextComponent* loadMapButtonText = new TextComponent("Load Map", &xerox, &VSBlue, textManager);
	TextComponent* exitButtonText = new TextComponent("Exit", &xerox, &VSBlue, textManager);

	auto newMap = [](Engine* engine) {
		engine->getSceneManager()->setCurrentScene(2);
		engine->getWindow()->hideCursor();
		engine->getRenderer()->getCamera()->setInUse(true);
	};
	auto loadMap = [](Engine* engine) {engine->getSceneManager()->setCurrentScene(2);};
	auto exit = [](Engine* engine) {engine->getWindow()->quit(); };

	// This sets up some buttons. Note: setText() uses information about the button's dimensions. In order to setup text for
	// a button, make sure that you set the button's position before hand.
	ButtonComponent* newMapButton = new ButtonComponent();
	newMapButton->setDimensions(newMapButtonText->getDimensions());
	newMapButton->setupOnClick(&AelaEngineFunctor(engine, newMap), engine->getEventHandler());
	newMapButton->getDimensions()->setX((int) (windowDimensions.getWidth() / 20));
	newMapButton->getDimensions()->setY((int) (windowDimensions.getHeight() / 1.24f));
	newMapButton->setText(newMapButtonText, engine->getTextManager());

	ButtonComponent* loadMapButton = new ButtonComponent();
	loadMapButton->setDimensions(loadMapButtonText->getDimensions());
	loadMapButton->setupOnClick(&AelaEngineFunctor(engine, loadMap), engine->getEventHandler());
	loadMapButton->getDimensions()->setX((int) (windowDimensions.getWidth() / 20));
	loadMapButton->getDimensions()->setY((int) (windowDimensions.getHeight() / 1.24f + spacing));
	loadMapButton->setText(loadMapButtonText, engine->getTextManager());

	ButtonComponent* exitButton = new ButtonComponent();
	exitButton->setDimensions(exitButtonText->getDimensions());
	exitButton->setupOnClick(&AelaEngineFunctor(engine, exit), engine->getEventHandler());
	exitButton->getDimensions()->setX((int) (windowDimensions.getWidth() / 20));
	exitButton->getDimensions()->setY((int) (windowDimensions.getHeight() / 1.24f + spacing * 2));
	exitButton->setText(exitButtonText, engine->getTextManager());

	// This sets up the title screen scene.
	Scene* mainMenuScene = new Scene();
	mainMenuScene->setId(1);
	mainMenuScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	mainMenuScene->getMenu()->add(mainMenuImage);
	mainMenuScene->getMenu()->add(titleText);
	mainMenuScene->getMenu()->add(ekkonGamesText);
	mainMenuScene->getMenu()->add(newMapButton);
	mainMenuScene->getMenu()->add(loadMapButton);
	mainMenuScene->getMenu()->add(exitButton);

	ImageComponent* topBarImage = new ImageComponent();
	Texture* topBarTexture;
	success = engine->getResourceManager()->obtain<Texture>("res/textures/map_editor_top_bar.dds", topBarTexture);
	topBarImage->setDimensions(&Rect<int>(0, 0, windowDimensions.getWidth(), windowDimensions.getHeight() / 18));
	topBarImage->setTexture(topBarTexture);

	TextComponent* entityTypeText = new TextComponent("Entity: Model", &xerox, &VSBlue, textManager);
	entityTypeText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.02f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setEntityTypeText(entityTypeText);

	TextComponent* positionText = new TextComponent("Position: 0, 0, 0", &xerox, &VSBlue, textManager);
	positionText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.15f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setPositionText(positionText);

	TextComponent* rotationText = new TextComponent("Rotation: 0, 0, 0", &xerox, &VSBlue, textManager);
	rotationText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.45f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setRotationText(rotationText);

	TextComponent* scalingText = new TextComponent("Scaling: 1, 1, 1", &xerox, &VSBlue, textManager);
	scalingText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.75f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setScalingText(scalingText);

	Scene* mapCreationScene = new Scene();
	mapCreationScene->setId(2);
	mapCreationScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	mapCreationScene->getMenu()->add(topBarImage);
	mapCreationScene->getMenu()->add(entityTypeText);
	mapCreationScene->getMenu()->add(positionText);
	mapCreationScene->getMenu()->add(rotationText);
	mapCreationScene->getMenu()->add(scalingText);

	RectComponent* tintRect = new RectComponent();
	tintRect->setDimensions(&windowDimensions);
	tintRect->setColour(&ColourRGBA(0.4f, 0.4f, 0.4f, 0.35f));

	TextComponent* entityToolText = new TextComponent("Entity Tool", &xerox, &almostWhite, textManager);

	ButtonComponent* entityToolButton = new ButtonComponent();
	Texture* simpleButtonTexture;
	success = engine->getResourceManager()->obtain<Texture>("res/textures/simple_button.dds", simpleButtonTexture);
	entityToolButton->setTexture(simpleButtonTexture);
	entityToolButton->setDimensions(&Rect<int>(windowDimensions.getWidth() * 0.125, windowDimensions.getHeight() * 0.1111,
		windowDimensions.getWidth() * 0.1875, windowDimensions.getHeight() * 0.1111));
	entityToolButton->setupOnClick(&AelaEngineFunctor(engine, exit), engine->getEventHandler());
	entityToolButton->setText(entityToolText, engine->getTextManager());

	RectComponent* rightRect = new RectComponent();
	rightRect->setDimensions(&Rect<int>(windowDimensions.getWidth() * 0.3125, windowDimensions.getHeight() * 0.1111,
		windowDimensions.getWidth() * 0.5625, windowDimensions.getHeight() * 0.7777));
	rightRect->setColour(&ColourRGBA(0.2f, 0.2f, 0.2f, 0.95f));

	Scene* optionsScene = new Scene();
	optionsScene->setId(3);
	optionsScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	optionsScene->getMenu()->add(tintRect);
	optionsScene->getMenu()->add(entityToolButton);
	optionsScene->getMenu()->add(rightRect);

	Map3D* map;
	success = engine->getResourceManager()->obtain<Map3D>("res/maps/sample_map.txt", map);
	if (success) {
		mapCreationScene->setMap(map);
		optionsScene->setMap(map);
	} else {
		AelaErrorHandling::consoleWindowError("Scene Script", "res/maps/sample_map.txt wasn't loaded properly or something.");
	}

	engine->getSceneManager()->registerScene(mainMenuScene, 1);
	engine->getSceneManager()->registerScene(mapCreationScene, 2);
	engine->getSceneManager()->registerScene(optionsScene, 3);
	engine->getSceneManager()->setCurrentScene(1);


	// engine->getWindow()->hideCursor();
	engine->getRenderer()->getCamera()->setUseControls(false);

	// The renderer's camera must be bound to the KeyedAnimator3D for movement.
	engine->getKeyedAnimator3D()->addTransformable(engine->getRenderer()->getCamera());
	engine->getRenderer()->getCamera()->setPosition(0, 10, -10);
	engine->getRenderer()->getCamera()->setRotation(0, glm::pi<float>() / -4, 0);
}