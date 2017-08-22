// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once

#include "Aela_Engine.h"
#include "Scenes\SceneManager.h"
#include "Menus\Label.h"
#include "Menus\ImageComponent.h"
#include "Menus\ButtonComponent.h"
#include "Menus\RectComponent.h"

using namespace Aela;

void setupScenes(Engine* engine, AelaGame* game) {
	// This creates some objects for later.
	TextManager* textManager = engine->getTextManager();
	int arialLarge = textManager->createNewTextFont("res/fonts/xerox.ttf");
	textManager->adjustFontSize(arialLarge, 35);

	if (arialLarge == -1) {
		return;
	}

	int arial = textManager->createNewTextFont("res/fonts/xerox.ttf");
	textManager->adjustFontSize(arial, 18);
	ColourRGBA VSBlue(0.8392f, 0.8588f, 0.9137f, 1.0f);
	Rect<int> windowDimensions = *((Rect<signed int>*) engine->getWindow()->getWindowDimensions());

	// This sets up an image.
	ImageComponent* mainMenuImage = new ImageComponent();
	Texture* mainMenuTexture;
	bool success = engine->getResourceManager()->obtain<Texture>("res/textures/map_editor_main_background.dds", mainMenuTexture);
	mainMenuImage->setDimensions(&windowDimensions);
	mainMenuImage->setTexture(mainMenuTexture);

	// This sets up text.
	Label* titleText = new Label("Project Aela Map Editor", arialLarge, &VSBlue, textManager);
	titleText->getDimensions()->setX((int) (windowDimensions.getWidth() / 20));
	titleText->getDimensions()->setY((int) (windowDimensions.getHeight() / 1.3f));

	Label* ekkonGamesText = new Label("Ekkon Games", arial, &VSBlue, textManager);
	ekkonGamesText->getDimensions()->setX((int) (windowDimensions.getWidth() * 0.8));
	ekkonGamesText->getDimensions()->setY((int) (windowDimensions.getHeight() * 0.95));

	Label* newMapButtonText = new Label("Create New Map", arial, &VSBlue, textManager);
	int spacing = newMapButtonText->getDimensions()->getHeight() + windowDimensions.getHeight() / 25;
	Label* loadMapButtonText = new Label("Load Map", arial, &VSBlue, textManager);
	Label* exitButtonText = new Label("Exit", arial, &VSBlue, textManager);

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

	Label* entityTypeText = new Label("Entity: Model", arial, &VSBlue, textManager);
	entityTypeText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.02f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setEntityTypeText(entityTypeText);

	Label* positionText = new Label("Position: 0, 0, 0", arial, &VSBlue, textManager);
	positionText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.15f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setPositionText(positionText);

	Label* rotationText = new Label("Rotation: 0, 0, 0", arial, &VSBlue, textManager);
	rotationText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.45f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setRotationText(rotationText);

	Label* scalingText = new Label("Scaling: 1, 1, 1", arial, &VSBlue, textManager);
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
	tintRect->setColour(&ColourRGBA(0.3f, 0.3f, 0.3f, 0.6f));

	Scene* optionsScene = new Scene();
	optionsScene->setId(3);
	optionsScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	optionsScene->getMenu()->add(tintRect);

	Map3D* map;
	success = engine->getResourceManager()->obtain<Map3D>("res/maps/sample_map.txt", map);
	if (success) {
		mapCreationScene->setMap(map);
		optionsScene->setMap(map);
		game->setMapBeingEdited(map);
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