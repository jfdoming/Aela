#include "SceneScript.h"
#include "../../Game Object Provider/GameObjectProvider.h"
#include "../../Worlds/WorldManager.h"
#include "ScriptObjects.h"
#include "Scenes/SceneManager.h"
#include "Menus/Label.h"
#include "Menus/ImageComponent.h"
#include "Menus/Button.h"
#include "Menus/RectComponent.h"
#include "Menus/SubMenu.h"
#include "../Aela Game/AelaGame.h"
#include "../../Resources/ResourceInfo.h"
#include "../../Displays/Dialogue/DialogueDisplay.h"
#include "../../Displays/Hints/HintDisplay.h"
#include "../../Worlds/WorldManager.h"
#include "../../Displays/Tiles/TileInventoryDisplay.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"

void Scripts::setupScenes() {
	using namespace Game;

	// This creates some font objects for use later.
	TextFont* xeroxLarge = fontManager->obtainTextFont((std::string) RESOURCE_ROOT, "res/fonts/xerox.ttf", 31);
	TextFont* xeroxMedium = fontManager->obtainTextFont((std::string) RESOURCE_ROOT, "res/fonts/xerox.ttf", 24);
	TextFont* xeroxSmall = fontManager->obtainTextFont((std::string) RESOURCE_ROOT, "res/fonts/xerox.ttf", 18);

	if (xeroxLarge == nullptr || xeroxMedium == nullptr) {
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

	// These are some pointers that we won't have to recreate over and over if we declare them here.
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
		ekkonImage->hide();
	} else {
		AelaErrorHandling::consoleWindowError("Missing texture: ../../res/textures/ekkon.dds");
	}

	// This sets up the ekkon scene.
	auto ekkonScene = new Scene();
	ekkonScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
	ekkonScene->getMenu()->add(ekkonImage);

	// This sets up text.
	auto titleText = std::make_shared<Label>("Pokemon Meitnerium", xeroxLarge, &almostWhite);
	titleText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.05), (int) (windowDimensions.getHeight() / 1.6f));
	auto ekkonGamesText = std::make_shared<Label>("Ekkon Games", xeroxMedium, &almostWhite);
	ekkonGamesText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.8), ((int) (windowDimensions.getHeight() * 0.95)));

	// This sets up actions for the main menu buttons.
	auto startNewGameAction = [](Engine* engine) {
		game->startNewGame();
	};

	auto continueGameAction = [](Engine* engine) {
		game->continueGame();
	};

	auto editMapAction = [](Engine* engine) {
		game->editMap();
	};

	auto optionsAction = [](Engine* engine) {AelaErrorHandling::windowWarning("There are no options!"); };
	auto exitAction = [](Engine* engine) {engine->getWindow()->quit(); };

	// This sets up some textureless buttons. Note: setText() uses information about the button's dimensions. In order to setup text for
	// a button, make sure that you set the button's position before hand.
	auto startGameButton = std::make_shared<Button>();
	startGameButton->setupOnClick(std::bind(startNewGameAction, engine));
	startGameButton->setPosition((int) (windowDimensions.getWidth() * 0.06),
		(int) (windowDimensions.getHeight() / 1.5f));
	startGameButton->setText("Start a New Game");
	startGameButton->setTextFont(xeroxMedium);
	startGameButton->setTextColour(almostWhitePtr);
	startGameButton->wrapAroundText();

	int spacing = startGameButton->getDimensions()->getHeight() + windowDimensions.getHeight() / 25;

	auto continueGameButton = std::make_shared<Button>();
	continueGameButton->setupOnClick(std::bind(continueGameAction, engine));
	continueGameButton->setPosition((int) (windowDimensions.getWidth() * 0.06),
		(int) (windowDimensions.getHeight() / 1.5f + spacing));
	continueGameButton->setText("Continue Game");
	continueGameButton->setTextFont(xeroxMedium);
	continueGameButton->setTextColour(almostWhitePtr);
	continueGameButton->wrapAroundText();

	auto editMapButton = std::make_shared<Button>();
	editMapButton->setupOnClick(std::bind(editMapAction, engine));
	editMapButton->setPosition((int) (windowDimensions.getWidth() * 0.06),
		(int) (windowDimensions.getHeight() / 1.5f + spacing * 2));
	editMapButton->setText("Edit Map");
	editMapButton->setTextFont(xeroxMedium);
	editMapButton->setTextColour(almostWhitePtr);
	editMapButton->wrapAroundText();

	auto optionsButton = std::make_shared<Button>();
	optionsButton->setupOnClick(std::bind(optionsAction, engine));
	optionsButton->setPosition((int) (windowDimensions.getWidth() * 0.06),
		(int) (windowDimensions.getHeight() / 1.5f + spacing * 3));
	optionsButton->setText("Options");
	optionsButton->setTextFont(xeroxMedium);
	optionsButton->setTextColour(almostWhitePtr);
	optionsButton->wrapAroundText();

	auto exitButton = std::make_shared<Button>();
	exitButton->setupOnClick(std::bind(exitAction, engine));
	exitButton->setPosition((int) (windowDimensions.getWidth() * 0.06),
		(int) (windowDimensions.getHeight() / 1.5f + spacing * 4));
	exitButton->setText("Exit");
	exitButton->setTextFont(xeroxMedium);
	exitButton->setTextColour(almostWhitePtr);
	exitButton->wrapAroundText();

	// This sets up the main menu scene.
	auto mainMenuScene = new Scene();
	mainMenuScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
	mainMenuScene->getMenu()->add(titleText);
	mainMenuScene->getMenu()->add(ekkonGamesText);
	mainMenuScene->getMenu()->add(startGameButton);
	mainMenuScene->getMenu()->add(continueGameButton);
	mainMenuScene->getMenu()->add(editMapButton);
	mainMenuScene->getMenu()->add(optionsButton);
	mainMenuScene->getMenu()->add(exitButton);

	// This is the submenu for dialogue.
	auto dialogueBoxSubMenu = std::make_shared<SubMenu>();
	dialogueBoxSubMenu->init(&windowDimensions, *renderer);

	auto dialgoueBoxImage = std::make_shared<ImageComponent>();
	dialgoueBoxImage->setPositioningMode(PositioningMode2D::CENTER);
	GLTexture* dialgueBoxTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/dialogue_box.png", dialgueBoxTexture);
	dialgoueBoxImage->setDimensions(&Rect<int>(windowDimensions.getWidth() / 2, windowDimensions.getHeight() * 7 / 8, windowDimensions.getWidth() / 2,
		windowDimensions.getHeight() / 8));
	dialgoueBoxImage->setTexture(dialgueBoxTexture);

	auto avatarImage = std::make_shared<ImageComponent>();
	GLTexture* avatarTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/avatars_1/0/0.png", avatarTexture);
	avatarImage->setDimensions(&Rect<int>(0, windowDimensions.getHeight() * 21 / 32, windowDimensions.getWidth() / 3,
		windowDimensions.getHeight() / 3));
	avatarImage->setTexture(avatarTexture);
	avatarImage->hide();

	auto dialogueTextOne = std::make_shared<Label>("Dialogue!", xeroxMedium, &almostWhite);
	auto dialogueTextTwo = std::make_shared<Label>("Dialogue 2!", xeroxMedium, &almostWhite);
	auto dialogueTextThree = std::make_shared<Label>("Dialogue 3!", xeroxMedium, &almostWhite);
	auto dialogueTextFour = std::make_shared<Label>("Dialogue 4!", xeroxMedium, &almostWhite);
	auto dialogueTextName = std::make_shared<Label>("Name!", xeroxMedium, &almostWhite);
	dialogueTextName->setPositioningMode(PositioningMode2D::CENTER);
	float dialogueTextStartX = dialogueDisplay->getDialogueTextStartX();
	dialogueTextOne->getDimensions()->setXY((int) (windowDimensions.getWidth() * dialogueTextStartX), (int) (windowDimensions.getHeight() * 0.85));
	dialogueTextTwo->getDimensions()->setXY((int) (windowDimensions.getWidth() * dialogueTextStartX), (int) (windowDimensions.getHeight() * 0.95));
	dialogueTextThree->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.5), (int) (windowDimensions.getHeight() * 0.85));
	dialogueTextFour->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.5), (int) (windowDimensions.getHeight() * 0.95));
	dialogueTextName->getDimensions()->setXY((int)(windowDimensions.getWidth() / 8), (int)(windowDimensions.getHeight() * 0.98));

	dialogueBoxSubMenu->add(dialgoueBoxImage);
	dialogueBoxSubMenu->add(avatarImage);
	dialogueBoxSubMenu->add(dialogueTextOne);
	dialogueBoxSubMenu->add(dialogueTextTwo);
	dialogueBoxSubMenu->add(dialogueTextThree);
	dialogueBoxSubMenu->add(dialogueTextFour);
	dialogueBoxSubMenu->add(dialogueTextName);

	dialogueDisplay->setSubMenu(dialogueBoxSubMenu);
	dialogueDisplay->setBackdrop(dialgoueBoxImage);
	dialogueDisplay->setAvatar(avatarImage);
	dialogueDisplay->setDialogueLabels(dialogueTextOne, dialogueTextTwo, dialogueTextThree, dialogueTextFour);
	dialogueDisplay->setNameLabel(dialogueTextName);

	// This sets up the submenu that shows the player's tile inventory. The second submenu only contains the
	// box that signifies which tile is selected, which is done so that the selection box is always above
	// the tiles.
	auto tileInventorySubMenu = std::make_shared<SubMenu>(), tileInventorySubMenu2 = std::make_shared<SubMenu>();
	tileInventorySubMenu->init(&Rect<int>(0, 0, windowDimensions.getWidth(), windowDimensions.getHeight() / 3), *renderer);
	tileInventorySubMenu2->init(&Rect<int>(0, 0, windowDimensions.getWidth(), windowDimensions.getHeight() / 3), *renderer);

	auto tileInventoryBackgroundImage = std::make_shared<ImageComponent>();
	GLTexture* tileInventoryBackgroundTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/inventory_background.png", tileInventoryBackgroundTexture);
	tileInventoryBackgroundImage->setDimensions(&Rect<int>(windowDimensions.getWidth() / 2, windowDimensions.getHeight() / 15,
		windowDimensions.getWidth() / 2, windowDimensions.getHeight() / 18));
	tileInventoryBackgroundImage->setTexture(tileInventoryBackgroundTexture);

	auto tileInventoryText = std::make_shared<Label>("<current tile>", xeroxMedium, &almostWhite);
	tileInventoryText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.8), (int) (windowDimensions.getHeight() / 5));

	auto tileSelectorBox = std::make_shared<ImageComponent>();
	GLTexture* tileInventoryBoxTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/selector_box.png", tileInventoryBoxTexture);
	int tileSelectorWidthAndHeight = windowDimensions.getHeight() / 8;
	tileSelectorBox->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.98 - tileSelectorWidthAndHeight - tileSelectorWidthAndHeight * 0.1),
		(int) (windowDimensions.getHeight() * 0.15 - tileSelectorWidthAndHeight - tileSelectorWidthAndHeight * 0.1),
		(int) (tileSelectorWidthAndHeight * 1.2), (int) (tileSelectorWidthAndHeight * 1.2)));
	tileSelectorBox->setTexture(tileInventoryBoxTexture);

	tileInventorySubMenu->add(tileInventoryBackgroundImage);
	tileInventorySubMenu->add(tileInventoryText);
	tileInventorySubMenu2->add(tileSelectorBox);
	tileInventorySubMenu->show();
	tileInventorySubMenu2->show();
	tileInventoryDisplay->setMenuItems(tileInventorySubMenu, tileInventoryText, tileInventoryBackgroundImage, tileSelectorBox);

	auto deathBackgroundRect = std::make_shared<RectComponent>();
	deathBackgroundRect->setDimensions(&windowDimensions);
	deathBackgroundRect->setColour(&ColourRGBA(0.15f, 0.15f, 0.15f, 0.95f));
	deathBackgroundRect->hide();

	auto deathText = std::make_shared<Label>("You died. Press 'R' to go back to your last checkpoint.", xeroxMedium, &almostWhite);
	deathText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.30), (int) (windowDimensions.getHeight() * 0.30));
	deathText->hide();

	game->setDeathMenuComponents(deathBackgroundRect, deathText);

	auto fadeTeleportRect = std::make_shared<RectComponent>();
	fadeTeleportRect->setDimensions(&windowDimensions);
	fadeTeleportRect->setColour(&ColourRGBA(0.05f, 0.05f, 0.05f, 1.0f));
	fadeTeleportRect->hide();
	game->setFadeTeleportRect(fadeTeleportRect);

	auto hintImage = std::make_shared<ImageComponent>();
	GLTexture* hintTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/hint_box.png", hintTexture);
	hintImage->setDimensions(&Rect<int>(windowDimensions.getWidth() / 12, windowDimensions.getHeight() / 15,
		windowDimensions.getWidth() / 2, windowDimensions.getHeight() / 18));
	hintImage->setTexture(hintTexture);
	hintImage->hide();
	hintDisplay->setHintBackdrop(hintImage);

	// This sets up the coordinate text.
	auto hintText = std::make_shared<Label>("", xeroxSmall, almostWhitePtr);
	hintText->getDimensions()->setXY((int) (windowDimensions.getWidth() / 10), (int) (windowDimensions.getHeight() / 10));
	hintDisplay->setHintLabel(hintText);

	// This sets up the world gameplay scene, in which the player is given a top-down view of the world.
	auto worldGameplayScene = new Scene();
	worldGameplayScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
	worldGameplayScene->getMenu()->add(dialogueBoxSubMenu);
	worldGameplayScene->getMenu()->add(tileInventorySubMenu);
	worldGameplayScene->getMenu()->add(tileInventorySubMenu2);
	worldGameplayScene->getMenu()->add(deathBackgroundRect);
	worldGameplayScene->getMenu()->add(deathText);
	worldGameplayScene->getMenu()->add(fadeTeleportRect);
	worldGameplayScene->getMenu()->add(hintImage);
	worldGameplayScene->getMenu()->add(hintText);

	// This sets up the scenes for the pause menu.
	Scene* pauseScene = new Scene();
	pauseScene->enableMenu(engine->getWindow()->getDimensions(), *engine->getRenderer());

	// The following blocks of code setup the pause menu scene.
	auto tintRect = std::make_shared<RectComponent>();
	tintRect->setDimensions(&windowDimensions);
	tintRect->setColour(&ColourRGBA(0.4f, 0.4f, 0.4f, 0.35f));

	// This sets up a rectangle that the pause menu scene will use.
	auto pauseBackgroundRect = std::make_shared<RectComponent>();
	pauseBackgroundRect->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.3125), (int) (windowDimensions.getHeight() * 0.1111),
		(int) (windowDimensions.getWidth() * 0.5625), (int) (windowDimensions.getHeight() * 0.7777)));
	pauseBackgroundRect->setColour(&ColourRGBA(0.2f, 0.2f, 0.2f, 0.95f));

	// This is the box that says "Pause Menu".
	auto pauseMenuTextRect = std::make_shared<RectComponent>();
	pauseMenuTextRect->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.1111),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	pauseMenuTextRect->setColour(&ColourRGBA(0.2f, 0.2f, 0.2f, 0.95f));

	auto pauseMenuTitleText = std::make_shared<Label>("Pause Menu", xeroxMedium, &almostWhite);
	pauseMenuTitleText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.15), (int) (windowDimensions.getHeight() * 0.18));

	// This creates sidebar buttons for the pause menu.
	auto pauseMenuGameButton = std::make_shared<Button>(&hoverTint, &clickTint);
	auto pauseMenuOptionsButton = std::make_shared<Button>(&hoverTint, &clickTint);
	auto pauseMenuMapEditorButton = std::make_shared<Button>(&hoverTint, &clickTint);
	auto pauseMenuExportButton = std::make_shared<Button>(hoverTintPtr, clickTintPtr);

	// This gets textures for the pause menu buttons.
	GLTexture* simpleButtonTexture;
	GLTexture* simpleButtonTextureLight;
	success = resourceManager->obtain<GLTexture>((std::string) DEFAULT_TEXTURE_PATH + "simple_button.dds", simpleButtonTexture);
	success = resourceManager->obtain<GLTexture>((std::string) DEFAULT_TEXTURE_PATH + "simple_button_light.dds", simpleButtonTextureLight);

	auto pauseMenuGameSubMenu = std::make_shared<SubMenu>();
	pauseMenuGameSubMenu->init(&windowDimensions, *renderer);
	auto pauseMenuOptionsSubMenu = std::make_shared<SubMenu>();
	pauseMenuOptionsSubMenu->init(&windowDimensions, *renderer);
	auto pauseMenuMapEditorSubMenu = std::make_shared<SubMenu>();
	pauseMenuMapEditorSubMenu->init(&windowDimensions, *renderer);

	auto goToGameSubMenu = [pauseMenuGameSubMenu, pauseMenuOptionsSubMenu, pauseMenuMapEditorSubMenu](Engine* engine) {
		pauseMenuGameSubMenu->show();
		pauseMenuOptionsSubMenu->hide();
		pauseMenuMapEditorSubMenu->hide();
	};

	auto goToOptionsSubMenu = [pauseMenuGameSubMenu, pauseMenuOptionsSubMenu, pauseMenuMapEditorSubMenu](Engine* engine) {
		pauseMenuGameSubMenu->hide();
		pauseMenuOptionsSubMenu->show();
		pauseMenuMapEditorSubMenu->hide();
	};

	auto goToMapEditorSubMenu = [pauseMenuGameSubMenu, pauseMenuOptionsSubMenu, pauseMenuMapEditorSubMenu](Engine* engine) {
		pauseMenuGameSubMenu->hide();
		pauseMenuOptionsSubMenu->hide();
		pauseMenuMapEditorSubMenu->show();
	};

	auto exportMap = [](Engine* engine) {
		WorldManager* worldManager = GameObjectProvider::getWorldManager();
		if (worldManager->exportCurrentWorld()) {
			AelaErrorHandling::windowWarning("Map was successfully exported.");
		} else {
			AelaErrorHandling::windowWarning("The map could not be exported.");
		}
	};

	pauseMenuGameButton->setTexture(simpleButtonTextureLight);
	pauseMenuGameButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.2222),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	pauseMenuGameButton->setupOnClick(std::bind(goToGameSubMenu, engine));
	pauseMenuGameButton->setText("Game");
	pauseMenuGameButton->setTextFont(xeroxMedium);
	pauseMenuGameButton->setTextColour(almostWhitePtr);

	pauseMenuOptionsButton->setTexture(simpleButtonTextureLight);
	pauseMenuOptionsButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.3333),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	pauseMenuOptionsButton->setupOnClick(std::bind(goToOptionsSubMenu, engine));
	pauseMenuOptionsButton->setText("Options");
	pauseMenuOptionsButton->setTextFont(xeroxMedium);
	pauseMenuOptionsButton->setTextColour(almostWhitePtr);
	pauseMenuOptionsButton->wrapAroundText();

	pauseMenuMapEditorButton->setTexture(simpleButtonTextureLight);
	pauseMenuMapEditorButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.4444),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	pauseMenuMapEditorButton->setupOnClick(std::bind(goToOptionsSubMenu, engine));
	pauseMenuMapEditorButton->setText("Map Editor");
	pauseMenuMapEditorButton->setTextFont(xeroxMedium);
	pauseMenuMapEditorButton->setTextColour(almostWhitePtr);
	pauseMenuMapEditorButton->wrapAroundText();

	pauseMenuExportButton->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.4), (int) (windowDimensions.getHeight() * 0.3333),
		(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111)));
	pauseMenuExportButton->setupOnClick(std::bind(exportMap, engine));
	pauseMenuExportButton->setText("Export");
	pauseMenuExportButton->setTextFont(xeroxMedium);
	pauseMenuExportButton->setTextColour(almostWhitePtr);
	pauseMenuMapEditorSubMenu->add(pauseMenuExportButton);

	pauseMenuGameSubMenu->show();

	pauseScene->getMenu()->add(tintRect);
	pauseScene->getMenu()->add(pauseBackgroundRect);
	pauseScene->getMenu()->add(pauseMenuGameButton);
	pauseScene->getMenu()->add(pauseMenuGameSubMenu);
	pauseScene->getMenu()->add(pauseMenuOptionsButton);
	pauseScene->getMenu()->add(pauseMenuOptionsSubMenu);
	pauseScene->getMenu()->add(pauseMenuMapEditorButton);
	pauseScene->getMenu()->add(pauseMenuMapEditorSubMenu);
	pauseScene->getMenu()->add(pauseMenuTextRect);
	pauseScene->getMenu()->add(pauseMenuTitleText);

	// This sets up the inventory scene.
	Scene* inventoryScene = new Scene();
	inventoryScene->enableMenu(engine->getWindow()->getDimensions(), *engine->getRenderer());

	// This sets up a rectangle that the inventory scene will use.
	auto inventoryBackgroundRect = std::make_shared<RectComponent>();
	inventoryBackgroundRect->setDimensions(&Rect<int>((int) (windowDimensions.getWidth() * 0.3125), (int) (windowDimensions.getHeight() * 0.33333),
		(int) (windowDimensions.getWidth() * 0.375), (int) (windowDimensions.getHeight() * 0.22222)));
	inventoryBackgroundRect->setColour(&ColourRGBA(0.15f, 0.15f, 0.15f, 0.95f));

	// This sets up text for the inentory scene.
	auto inventoryTitleText = std::make_shared<Label>("Inventory", xeroxMedium, &almostWhite);
	inventoryTitleText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.33), (int) (windowDimensions.getHeight() * 0.38));

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
	success = engine->getResourceManager()->obtain<Map3D>("res/maps/title_screen.txt", map);
	if (success) {
		mainMenuScene->setMap(map);
		worldGameplayScene->setMap(map);
		pauseScene->setMap(map);
		inventoryScene->setMap(map);
	} else {
		AelaErrorHandling::windowError("There was a problem loading title_screen.txt!");
	}

	engine->getSceneManager()->registerScene(ekkonScene, EKKON_INTRO_SCENE);
	engine->getSceneManager()->registerScene(mainMenuScene, MAIN_MENU_SCENE);
	engine->getSceneManager()->registerScene(worldGameplayScene, WORLD_GAMEPLAY_SCENE);
	engine->getSceneManager()->registerScene(pauseScene, PAUSE_SCENE);
	engine->getSceneManager()->registerScene(inventoryScene, INVENTORY_SCENE);
	engine->getSceneManager()->setDisposingScenesOnDestroy(true);

	engine->getSceneManager()->setCurrentScene(EKKON_INTRO_SCENE);
	GameObjectProvider::setGameplayScene(worldGameplayScene);
	GameObjectProvider::setPauseScene(pauseScene);
	Scripts::gameplayScene = worldGameplayScene;
	Scripts::pauseScene = pauseScene;
	game->switchScene(EKKON_INTRO_SCENE);

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