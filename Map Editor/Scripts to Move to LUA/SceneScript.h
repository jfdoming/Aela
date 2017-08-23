// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once

#include "Aela_Engine.h"
#include "Scenes\SceneManager.h"
#include "Menus\Label.h"
#include "Menus\ImageComponent.h"
#include "Menus\Button.h"
#include "Menus\RectComponent.h"
#include "3D\Maps\Map3DExporter.h"

using namespace Aela;

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

	// This sets up an image.
	ImageComponent* mainMenuImage = new ImageComponent();
	Texture* mainMenuTexture;
	bool success = engine->getResourceManager()->obtain<Texture>("res/textures/map_editor_main_background.dds", mainMenuTexture);
	mainMenuImage->setDimensions(&windowDimensions);
	mainMenuImage->setTexture(mainMenuTexture);

	// This sets up text.
	Label* titleText = new Label("Project Aela Map Editor", xeroxLarge, &VSBlue, fontManager);
	titleText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.05), (int) (windowDimensions.getHeight() / 1.3f));

	Label* ekkonGamesText = new Label("Ekkon Games", xerox, &VSBlue, fontManager);
	ekkonGamesText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.8), ((int) (windowDimensions.getHeight() * 0.95)));

	Label* editMapButtonText = new Label("Edit Map", xerox, &VSBlue, fontManager);
	int spacing = editMapButtonText->getDimensions()->getHeight() + windowDimensions.getHeight() / 25;
	Label* loadMapButtonText = new Label("Help", xerox, &VSBlue, fontManager);
	Label* exitButtonText = new Label("Exit", xerox, &VSBlue, fontManager);

	auto editMapAction = [](Engine* engine) {
		engine->getSceneManager()->setCurrentScene(2);
		engine->getWindow()->hideCursor();
		engine->getRenderer()->getCamera()->setInUse(true);
	};
	auto helpMapAction = [](Engine* engine) {
		// Lol, this is temporary until I feel like creating a seperate scene for this garbage.
		std::string controls = (std::string) "WASD - move camera\n"
			+ "\\ - change entity type being placed\n"
			+ "Esc - open pause menu";
		AelaErrorHandling::windowWarning("Aela Map Editor: Controls", controls);
		AelaErrorHandling::windowWarning("Aela Map Editor: Contact", "For more help, ask a lead developer on the Ekkon Discord!");
		AelaErrorHandling::windowWarning("Aela Map Editor: Notes", "BTW, everything except export in the pause menu exits the program.");
	};
	auto exitAction = [](Engine* engine) {engine->getWindow()->quit(); };

	// This sets up some errorButtons. Note: setText() uses information about the button's dimensions. In order to setup text for
	// a button, make sure that you set the button's position before hand.
	Button* editMapButton = new Button();
	editMapButton->setDimensions(editMapButtonText->getDimensions());
	editMapButton->setupOnClick(&AelaEngineFunctor(engine, editMapAction), engine->getEventHandler());
	editMapButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.06), (int) (windowDimensions.getHeight() / 1.24f));
	editMapButton->setText(editMapButtonText, engine->getFontManager());

	Button* helpButton = new Button();
	helpButton->setDimensions(loadMapButtonText->getDimensions());
	helpButton->setupOnClick(&AelaEngineFunctor(engine, helpMapAction), engine->getEventHandler());
	helpButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.06), (int) (windowDimensions.getHeight() / 1.24f + spacing));
	helpButton->setText(loadMapButtonText, engine->getFontManager());

	Button* exitButton = new Button();
	exitButton->setDimensions(exitButtonText->getDimensions());
	exitButton->setupOnClick(&AelaEngineFunctor(engine, exitAction), engine->getEventHandler());
	exitButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.06), (int) (windowDimensions.getHeight() / 1.24f + spacing * 2));
	exitButton->setText(exitButtonText, engine->getFontManager());

	// This sets up the title screen scene.
	Scene* mainMenuScene = new Scene();
	mainMenuScene->setId(1);
	mainMenuScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	mainMenuScene->getMenu()->add(mainMenuImage);
	mainMenuScene->getMenu()->add(titleText);
	mainMenuScene->getMenu()->add(ekkonGamesText);
	mainMenuScene->getMenu()->add(editMapButton);
	mainMenuScene->getMenu()->add(helpButton);
	mainMenuScene->getMenu()->add(exitButton);

	ImageComponent* topBarImage = new ImageComponent();
	Texture* topBarTexture;
	success = engine->getResourceManager()->obtain<Texture>("res/textures/map_editor_top_bar.dds", topBarTexture);
	topBarImage->setDimensions(&Rect<int>(0, 0, windowDimensions.getWidth(), windowDimensions.getHeight() / 18));
	topBarImage->setTexture(topBarTexture);

	Label* entityTypeText = new Label("Entity: Model", xerox, &VSBlue, fontManager);
	entityTypeText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.015f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setEntityTypeText(entityTypeText);

	Label* positionText = new Label("Position: 0, 0, 0", xerox, &VSBlue, fontManager);
	positionText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.15f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setPositionText(positionText);

	Label* rotationText = new Label("Rotation: 0, 0, 0", xerox, &VSBlue, fontManager);
	rotationText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.45f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setRotationText(rotationText);

	Label* scalingText = new Label("Scaling: 1, 1, 1", xerox, &VSBlue, fontManager);
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

	Label* entityToolText = new Label("Entity Tool", xerox, &almostWhite, fontManager);
	Label* skyboxesText = new Label("Skyboxes", xerox, &almostWhite, fontManager);
	Label* exportText = new Label("Export", xerox, &almostWhite, fontManager);
	Label* optionsText = new Label("Options", xerox, &almostWhite, fontManager);

	ColourRGBA settingsHoverTint(0.8f, 0.8f, 0.8f, 1.0f);
	ColourRGBA settingsClickTint(0.6f, 0.6f, 0.6f, 1.0f);

	auto exportAction = [&](Engine* engine) {
		bool success = engine->getMapExporter()->exportMap("res/maps/map.txt",
			game->getMapBeingEdited(), true);
		if (success) {
			AelaErrorHandling::windowWarning("Aela Map Creator", "The map was successfully exported to res/maps/map.txt.");
		} else {
			AelaErrorHandling::windowError("Aela Map Creator", "Error! The map could not be exported to res/maps/map.txt!");
		}
	};

	Button* entityToolButton = new Button(&settingsHoverTint, &settingsClickTint);
	Texture* simpleButtonTexture;
	success = engine->getResourceManager()->obtain<Texture>("res/textures/simple_button.dds", simpleButtonTexture);
	entityToolButton->setTexture(simpleButtonTexture);
	entityToolButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.1111),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	entityToolButton->setupOnClick(&AelaEngineFunctor(engine, exitAction), engine->getEventHandler());
	entityToolButton->setText(entityToolText, engine->getFontManager());

	Button* skyboxesButton = new Button(&settingsHoverTint, &settingsClickTint);
	skyboxesButton->setTexture(simpleButtonTexture);
	skyboxesButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.2222),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	skyboxesButton->setupOnClick(&AelaEngineFunctor(engine, exitAction), engine->getEventHandler());
	skyboxesButton->setText(skyboxesText, engine->getFontManager());

	Button* exportButton = new Button(&settingsHoverTint, &settingsClickTint);
	exportButton->setTexture(simpleButtonTexture);
	exportButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.3333),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	exportButton->setupOnClick(&AelaEngineFunctor(engine, exportAction), engine->getEventHandler());
	exportButton->setText(exportText, engine->getFontManager());

	Button* optionsButton = new Button(&settingsHoverTint, &settingsClickTint);
	optionsButton->setTexture(simpleButtonTexture);
	optionsButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.4444),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	optionsButton->setupOnClick(&AelaEngineFunctor(engine, exitAction), engine->getEventHandler());
	optionsButton->setText(optionsText, engine->getFontManager());

	RectComponent* rightRect = new RectComponent();
	rightRect->setDimensions(&Rect<int>(windowDimensions.getWidth() * 0.3125, windowDimensions.getHeight() * 0.1111,
		windowDimensions.getWidth() * 0.5625, windowDimensions.getHeight() * 0.7777));
	rightRect->setColour(&ColourRGBA(0.2f, 0.2f, 0.2f, 0.95f));

	Scene* optionsScene = new Scene();
	optionsScene->setId(3);
	optionsScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	optionsScene->getMenu()->add(tintRect);
	optionsScene->getMenu()->add(entityToolButton);
	optionsScene->getMenu()->add(skyboxesButton);
	optionsScene->getMenu()->add(exportButton);
	optionsScene->getMenu()->add(optionsButton);
	optionsScene->getMenu()->add(rightRect);

	Map3D* map;
	success = engine->getResourceManager()->obtain<Map3D>("res/maps/map.txt", map);
	if (success) {
		mapCreationScene->setMap(map);
		optionsScene->setMap(map);
		game->setMapBeingEdited(map);
	} else {
		AelaErrorHandling::consoleWindowError("Scene Script", "res/maps/map.txt wasn't loaded properly or something.");
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