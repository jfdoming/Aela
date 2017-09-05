// The contents of this file will be moved to LUA once we can get LUA to work!
#pragma once

#include "Aela_Engine.h"
#include "Scenes\SceneManager.h"
#include "Menus\Label.h"
#include "Menus\ImageComponent.h"
#include "Menus\Button.h"
#include "Menus\RectComponent.h"
#include "3D\Maps\Map3DExporter.h"

#include <memory>

using namespace Aela;

#define MAIN_MENU_SCENE 1
#define EDITOR_SCENE 2
#define PAUSE_ENTITY_TOOL_SCENE 3
#define PAUSE_SKYBOX_SCENE 4
#define PAUSE_EXPORT_SCENE 5
#define PAUSE_OPTIONS_SCENE 6

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

	// The following blocks of code setup the main menu scene.
	// This sets up an image.
	auto mainMenuImage = std::make_shared<ImageComponent>();
	Texture* mainMenuTexture;
	bool success = engine->getResourceManager()->obtain<Texture>("res/textures/map_editor_main_background.dds", mainMenuTexture);
	mainMenuImage->setDimensions(&windowDimensions);
	mainMenuImage->setCropping(mainMenuTexture->getDimensions());
	mainMenuImage->setTexture(mainMenuTexture);

	// This sets up text.
	auto titleText = std::make_shared<Label>("Project Aela Map Editor", xeroxLarge, &VSBlue, fontManager);
	titleText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.05), (int) (windowDimensions.getHeight() / 1.3f));
	auto ekkonGamesText = std::make_shared<Label>("Ekkon Games", xerox, &VSBlue, fontManager);
	ekkonGamesText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.8), ((int) (windowDimensions.getHeight() * 0.95)));
	auto editMapButtonText = std::make_shared<Label>("Edit Map", xerox, &VSBlue, fontManager);
	int spacing = editMapButtonText->getDimensions()->getHeight() + windowDimensions.getHeight() / 25;
	auto loadMapButtonText = std::make_shared<Label>("Help", xerox, &VSBlue, fontManager);
	auto exitButtonText = std::make_shared<Label>("Exit", xerox, &VSBlue, fontManager);

	// This sets up actions for the main menu buttons.
	auto editMapAction = [game](Engine* engine) {
		engine->getSceneManager()->setCurrentScene(EDITOR_SCENE);
		game->performActionOnSceneSwitch(EDITOR_SCENE);
	};
	auto helpMapAction = [](Engine* engine) {
		// Lol, this is temporary until I feel like creating a seperate scene for this garbage.
		std::string controls = (std::string) "WASD - move camera\n"
			+ "\\ - change entity type being placed\n"
			+ "Esc - open pause menu\n"
			+ "Left/Right Arrows - change resource of entity\n"
			+ "1/2/3 - modify rotation\n"
			+ "4/5/6 - modify scaling\n"
			+ "Up/Down Arrows - increase/decrease rotation/scaling"
			+ "7/8 - decrease/increase distance of object from camera";
		AelaErrorHandling::windowWarning("Aela Map Editor: Controls", controls);
		AelaErrorHandling::windowWarning("Aela Map Editor: Contact", "For more help, ask a lead developer on the Ekkon Discord!");
	};
	auto exitAction = [](Engine* engine) {engine->getWindow()->quit(); };

	// This sets up some textureless buttons. Note: setText() uses information about the button's dimensions. In order to setup text for
	// a button, make sure that you set the button's position before hand.
	auto editMapButton = std::make_shared<Button>();
	editMapButton->setDimensions(editMapButtonText->getDimensions());
	editMapButton->setupOnClick(std::bind(editMapAction, engine), engine->getEventHandler());
	editMapButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.06), (int) (windowDimensions.getHeight() / 1.24f));
	editMapButton->setText(editMapButtonText, engine->getFontManager());
	std::cout << editMapButtonText << " is a pointer.\n";

	auto helpButton = std::make_shared<Button>();
	helpButton->setDimensions(loadMapButtonText->getDimensions());
	helpButton->setupOnClick(std::bind(helpMapAction, engine), engine->getEventHandler());
	helpButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.06), (int) (windowDimensions.getHeight() / 1.24f + spacing));
	helpButton->setText(loadMapButtonText, engine->getFontManager());

	auto exitButton = std::make_shared<Button>();
	exitButton->setDimensions(exitButtonText->getDimensions());
	exitButton->setupOnClick(std::bind(exitAction, engine), engine->getEventHandler());
	exitButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.06), (int) (windowDimensions.getHeight() / 1.24f + spacing * 2));
	exitButton->setText(exitButtonText, engine->getFontManager());

	// This sets up the title screen scene.
	auto mainMenuScene = new Scene();
	mainMenuScene->setId(MAIN_MENU_SCENE);
	mainMenuScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	mainMenuScene->getMenu()->add(mainMenuImage);
	mainMenuScene->getMenu()->add(titleText);
	mainMenuScene->getMenu()->add(ekkonGamesText);
	mainMenuScene->getMenu()->add(editMapButton);
	mainMenuScene->getMenu()->add(helpButton);
	mainMenuScene->getMenu()->add(exitButton);

	// The following blocks of code setup the map editor scene.
	// This sets up the top bar image.
	auto topBarImage = std::make_shared<ImageComponent>();
	Texture* topBarTexture;
	success = engine->getResourceManager()->obtain<Texture>("res/textures/map_editor_top_bar.dds", topBarTexture);
	topBarImage->setDimensions(&Rect<int>(0, 0, windowDimensions.getWidth(), windowDimensions.getHeight() / 18));
	topBarImage->setCropping(topBarTexture->getDimensions());
	topBarImage->setTexture(topBarTexture);

	// This sets sup text.
	auto entityTypeText = std::make_shared<Label>("Entity: Model", xerox, &VSBlue, fontManager);
	entityTypeText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.015f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setEntityTypeText(entityTypeText);
	auto positionText = std::make_shared<Label>("Position: 0, 0, 0", xerox, &VSBlue, fontManager);
	positionText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.15f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setPositionText(positionText);
	auto rotationText = std::make_shared<Label>("Rotation: 0, 0, 0", xerox, &VSBlue, fontManager);
	rotationText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.45f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setRotationText(rotationText);
	auto scalingText = std::make_shared<Label>("Scaling: 1, 1, 1", xerox, &VSBlue, fontManager);
	scalingText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.75f), (int) (windowDimensions.getHeight() * 0.04f));
	game->setScalingText(scalingText);

	// This creates the map creation scene.
	auto mapCreationScene = new Scene();
	mapCreationScene->setId(EDITOR_SCENE);
	mapCreationScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	mapCreationScene->getMenu()->add(topBarImage);
	mapCreationScene->getMenu()->add(entityTypeText);
	mapCreationScene->getMenu()->add(positionText);
	mapCreationScene->getMenu()->add(rotationText);
	mapCreationScene->getMenu()->add(scalingText);

	// The following blocks of code setup the pause menu scenes.
	auto tintRect = std::make_shared<RectComponent>();
	tintRect->setDimensions(&windowDimensions);
	tintRect->setColour(&ColourRGBA(0.4f, 0.4f, 0.4f, 0.35f));

	// This sets up text for buttons in the pause menu's sidebar.
	auto entityToolText = std::make_shared<Label>("Entity Tool", xerox, &almostWhite, fontManager);
	auto skyboxesText = std::make_shared<Label>("Skyboxes", xerox, &almostWhite, fontManager);
	auto exportText = std::make_shared<Label>("Export", xerox, &almostWhite, fontManager);
	auto optionsText = std::make_shared<Label>("Options", xerox, &almostWhite, fontManager);

	// This sets up text for the entity tool pause menu scene.
	auto entityToolTitleText = std::make_shared<Label>("Entity Tool", xeroxLarge, &almostWhite, fontManager);
	entityToolTitleText->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.375), (int)(windowDimensions.getHeight() * 0.2222));

	// This sets up text for the skybox pause menu scene.
	auto skyboxTitleText = std::make_shared<Label>("Skybox", xeroxLarge, &almostWhite, fontManager);
	skyboxTitleText->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.375), (int)(windowDimensions.getHeight() * 0.2222));

	// This sets up text for the export pause menu scene.
	auto exportToRegularMapText = std::make_shared<Label>("Export to Regular Map File (recommended)", xerox, &almostWhite, fontManager);
	auto exportToReadableMapText = std::make_shared<Label>("Export to Readable Map File", xerox, &almostWhite, fontManager);
	auto exportTitleText = std::make_shared<Label>("Export", xeroxLarge, &almostWhite, fontManager);
	exportTitleText->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.375), (int)(windowDimensions.getHeight() * 0.2222));

	// This sets up text for the options pause menu scene.
	auto optionsTitleText = std::make_shared<Label>("Options", xeroxLarge, &almostWhite, fontManager);
	optionsTitleText->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.375), (int)(windowDimensions.getHeight() * 0.2222));

	// These are tints for buttons.
	ColourRGBA settingsHoverTint(0.8f, 0.8f, 0.8f, 1.0f);
	ColourRGBA settingsClickTint(0.6f, 0.6f, 0.6f, 1.0f);

	// This creates sidebar buttons for the pause menu.
	auto entityToolButton = std::make_shared<Button>(&settingsHoverTint, &settingsClickTint);
	auto skyboxesButton = std::make_shared<Button>(&settingsHoverTint, &settingsClickTint);
	auto exportButton = std::make_shared<Button>(&settingsHoverTint, &settingsClickTint);
	auto optionsButton = std::make_shared<Button>(&settingsHoverTint, &settingsClickTint);

	// This creates export scene buttons.
	auto exportRegularMapButton = std::make_shared<Button>(&settingsHoverTint, &settingsClickTint);
	auto exportReadableMapButton = std::make_shared<Button>(&settingsHoverTint, &settingsClickTint);

	// This gets textures for the pause menu buttons.
	Texture* simpleButtonTexture, *simpleButtonTextureLight;
	success = engine->getResourceManager()->obtain<Texture>("res/textures/simple_button.dds", simpleButtonTexture);
	success = engine->getResourceManager()->obtain<Texture>("res/textures/simple_button_light.dds", simpleButtonTextureLight);

	// This sets up actions for buttons.
	auto goToEntityToolSceneAction = [game, entityToolButton, skyboxesButton, exportButton, optionsButton, simpleButtonTexture, simpleButtonTextureLight](Engine* engine) {
		entityToolButton->setTexture(simpleButtonTextureLight);
		skyboxesButton->setTexture(simpleButtonTexture);
		exportButton->setTexture(simpleButtonTexture);
		optionsButton->setTexture(simpleButtonTexture);
		engine->getSceneManager()->setCurrentScene(PAUSE_ENTITY_TOOL_SCENE);
	};

	auto goToSkyboxSceneAction = [game, entityToolButton, skyboxesButton, exportButton, optionsButton, simpleButtonTexture, simpleButtonTextureLight](Engine* engine) {
		entityToolButton->setTexture(simpleButtonTexture);
		skyboxesButton->setTexture(simpleButtonTextureLight);
		exportButton->setTexture(simpleButtonTexture);
		optionsButton->setTexture(simpleButtonTexture);
		engine->getSceneManager()->setCurrentScene(PAUSE_SKYBOX_SCENE);
	};

	auto goToExportSceneAction = [game, entityToolButton, skyboxesButton, exportButton, optionsButton, simpleButtonTexture, simpleButtonTextureLight](Engine* engine) {
		entityToolButton->setTexture(simpleButtonTexture);
		skyboxesButton->setTexture(simpleButtonTexture);
		exportButton->setTexture(simpleButtonTextureLight);
		optionsButton->setTexture(simpleButtonTexture);
		engine->getSceneManager()->setCurrentScene(PAUSE_EXPORT_SCENE);
	};

	auto exportRegularMapAction = [game](Engine* engine) {
		game->exportMap("res/maps/map.txt", false);
	};

	auto exportReadableMapAction = [game](Engine* engine) {
		game->exportMap("res/maps/map.txt", true);
	};

	auto goToOptionsAction = [game, entityToolButton, skyboxesButton, exportButton, optionsButton, simpleButtonTexture, simpleButtonTextureLight](Engine* engine) {
		entityToolButton->setTexture(simpleButtonTexture);
		skyboxesButton->setTexture(simpleButtonTexture);
		exportButton->setTexture(simpleButtonTexture);
		optionsButton->setTexture(simpleButtonTextureLight);
		engine->getSceneManager()->setCurrentScene(PAUSE_OPTIONS_SCENE);
	};

	// This sets up buttons.
	entityToolButton->setTexture(simpleButtonTextureLight);
	entityToolButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.1111),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	entityToolButton->setupOnClick(std::bind(goToEntityToolSceneAction, engine), engine->getEventHandler());
	entityToolButton->setText(entityToolText, engine->getFontManager());

	skyboxesButton->setTexture(simpleButtonTexture);
	skyboxesButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.2222),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	skyboxesButton->setupOnClick(std::bind(goToSkyboxSceneAction, engine), engine->getEventHandler());
	skyboxesButton->setText(skyboxesText, engine->getFontManager());

	exportButton->setTexture(simpleButtonTexture);
	exportButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.3333),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	exportButton->setupOnClick(std::bind(goToExportSceneAction, engine), engine->getEventHandler());
	exportButton->setText(exportText, engine->getFontManager());

	optionsButton->setTexture(simpleButtonTexture);
	optionsButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.4444),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	optionsButton->setupOnClick(std::bind(goToOptionsAction, engine), engine->getEventHandler());
	optionsButton->setText(optionsText, engine->getFontManager());

	// This sets up buttons from the export scene.
	exportRegularMapButton->setTexture(simpleButtonTexture);
	exportRegularMapButton->setDimensions(exportToRegularMapText->getDimensions());
	exportRegularMapButton->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.375), (int) (windowDimensions.getHeight() * 0.3333));
	exportRegularMapButton->setupOnClick(std::bind(exportRegularMapAction, engine), engine->getEventHandler());
	exportRegularMapButton->setText(exportToRegularMapText, engine->getFontManager());

	exportReadableMapButton->setTexture(simpleButtonTexture);
	exportReadableMapButton->setDimensions(exportToReadableMapText->getDimensions());
	exportReadableMapButton->getDimensions()->setXY((int)(windowDimensions.getWidth() * 0.375), (int)(windowDimensions.getHeight() * 0.4444));
	exportReadableMapButton->setupOnClick(std::bind(exportReadableMapAction, engine), engine->getEventHandler());
	exportReadableMapButton->setText(exportToReadableMapText, engine->getFontManager());

	// This sets up a rectangle that the pause menu scenes will use.
	auto rightRect = std::make_shared<RectComponent>();
	rightRect->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.3125), (int) (windowDimensions.getHeight() * 0.1111),
		(int) (windowDimensions.getWidth() * 0.5625), (int) (windowDimensions.getHeight() * 0.7777)));
	rightRect->setColour(&ColourRGBA(0.2f, 0.2f, 0.2f, 0.95f));

	// This sets up the scenes for the pause menu.
	Scene* pauseEntityToolScene = new Scene();
	pauseEntityToolScene->setId(PAUSE_ENTITY_TOOL_SCENE);
	pauseEntityToolScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	pauseEntityToolScene->getMenu()->add(tintRect);
	pauseEntityToolScene->getMenu()->add(entityToolButton);
	pauseEntityToolScene->getMenu()->add(skyboxesButton);
	pauseEntityToolScene->getMenu()->add(exportButton);
	pauseEntityToolScene->getMenu()->add(optionsButton);
	pauseEntityToolScene->getMenu()->add(rightRect);
	pauseEntityToolScene->getMenu()->add(entityToolTitleText);

	Scene* pauseSkyboxScene = new Scene();
	pauseSkyboxScene->setId(PAUSE_SKYBOX_SCENE);
	pauseSkyboxScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	pauseSkyboxScene->getMenu()->add(tintRect);
	pauseSkyboxScene->getMenu()->add(entityToolButton);
	pauseSkyboxScene->getMenu()->add(skyboxesButton);
	pauseSkyboxScene->getMenu()->add(exportButton);
	pauseSkyboxScene->getMenu()->add(optionsButton);
	pauseSkyboxScene->getMenu()->add(rightRect);
	pauseSkyboxScene->getMenu()->add(skyboxTitleText);

	Scene* pauseExportScene = new Scene();
	pauseExportScene->setId(PAUSE_EXPORT_SCENE);
	pauseExportScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	pauseExportScene->getMenu()->add(tintRect);
	pauseExportScene->getMenu()->add(entityToolButton);
	pauseExportScene->getMenu()->add(skyboxesButton);
	pauseExportScene->getMenu()->add(exportButton);
	pauseExportScene->getMenu()->add(optionsButton);
	pauseExportScene->getMenu()->add(rightRect);
	pauseExportScene->getMenu()->add(exportTitleText);
	pauseExportScene->getMenu()->add(exportRegularMapButton);
	pauseExportScene->getMenu()->add(exportReadableMapButton);

	Scene* pauseOptionsScene = new Scene();
	pauseOptionsScene->setId(PAUSE_OPTIONS_SCENE);
	pauseOptionsScene->enableMenu(engine->getWindow()->getWindowDimensions(), engine->getRenderer());
	pauseOptionsScene->getMenu()->add(tintRect);
	pauseOptionsScene->getMenu()->add(entityToolButton);
	pauseOptionsScene->getMenu()->add(skyboxesButton);
	pauseOptionsScene->getMenu()->add(exportButton);
	pauseOptionsScene->getMenu()->add(optionsButton);
	pauseOptionsScene->getMenu()->add(rightRect);
	pauseOptionsScene->getMenu()->add(optionsTitleText);

	// This loads a map.
	Map3D* map;
	success = engine->getResourceManager()->obtain<Map3D>("res/maps/map.txt", map);
	if (success) {
		mapCreationScene->setMap(map);
		pauseEntityToolScene->setMap(map);
		pauseSkyboxScene->setMap(map);
		pauseExportScene->setMap(map);
		pauseOptionsScene->setMap(map);
		game->setMapBeingEdited(map);
	} else {
		AelaErrorHandling::consoleWindowError("Scene Script", "res/maps/map.txt wasn't loaded properly or something.");
	}

	// This registers all scenes with the scene manager.
	engine->getSceneManager()->registerScene(mainMenuScene, MAIN_MENU_SCENE);
	engine->getSceneManager()->registerScene(mapCreationScene, EDITOR_SCENE);
	engine->getSceneManager()->registerScene(pauseEntityToolScene, PAUSE_ENTITY_TOOL_SCENE);
	engine->getSceneManager()->registerScene(pauseSkyboxScene, PAUSE_SKYBOX_SCENE);
	engine->getSceneManager()->registerScene(pauseExportScene, PAUSE_EXPORT_SCENE);
	engine->getSceneManager()->registerScene(pauseOptionsScene, PAUSE_OPTIONS_SCENE);
	engine->getSceneManager()->setCurrentScene(MAIN_MENU_SCENE);

	// engine->getWindow()->hideCursor();
	// engine->getRenderer()->getCamera()->setUseControls(false);

	// The renderer's camera must be bound to the KeyedAnimator for movement.
	engine->getRenderer()->getCamera()->setPosition(0, 10, -10);
	engine->getRenderer()->getCamera()->setRotation(0, glm::pi<float>() / -4, 0);
}