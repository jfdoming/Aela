#include "SceneScript.h"
#include "../../Game Object Provider/GameObjectProvider.h"
#include "../../Worlds/WorldManager.h"
#include "ScriptObjects.h"
#include "../../../Project Aela/Scenes/SceneManager.h"
#include "../../../Project Aela/Menus/Label.h"
#include "../../../Project Aela/Menus/ImageComponent.h"
#include "../../../Project Aela/Menus/Button.h"
#include "../../../Project Aela/Menus/RectComponent.h"
#include "../../../Project Aela/Menus/SubMenu.h"
#include "../Aela Game/AelaGame.h"
#include "../../Displays/Dialogue/DialogueDisplay.h"
#include "../../Displays/Hints/HintDisplay.h"
#include "../../Displays/Tiles/TileInventoryDisplay.h"
#include "../../Menus/FontSizes.h"
#include "../../Displays/Timer/TimerDisplay.h"
#include "SceneIDs.h"
#include "../../Displays/Battle/BattleDisplay.h"
#include "../../Displays/Main Menu/MainMenuDisplay.h"
#include "../../Displays/Battle Dialogue/BattleDialogueDisplay.h"
#include "../../Constants/WindowConstants.h"

void Scripts::setupScenes() {
	using namespace Game;
	Font* pressStart2P;

	if (!resourceManager->obtain<Font>("res/fonts/pressStart2P.ttf", pressStart2P)) {
		AelaErrorHandling::windowError("A critical font (pressStart2P.ttf) could not be loaded, aborting!");
		return;
	}

	pressStart2P->setDefaultSize(FontSizes::MEDIUM_FONT_SIZE);
	pressStart2P->setAntialiasing(false);
	dialogueDisplay->setDefaultFont(pressStart2P);
	battleDialogueDisplay->setDefaultFont(pressStart2P);

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

	Rect<int> renderingDimensions(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

	// Note: the image below seems to take up 30MB of memory.
	// The following blocks of code set up the intro scene.
	auto introImage = std::make_shared<ImageComponent>();
	GLTexture* introTexture;
	bool success = resourceManager->obtain<GLTexture>("res/textures/start_screen.png", introTexture);
	if (success) {
		introImage->setDimensions(&renderingDimensions);
		introImage->setTexture(introTexture);
		introImage->hide();
	} else {
		AelaErrorHandling::consoleWindowError("Missing texture: res/textures/start_screen.png");
	}

	// This sets up the intro scene.
	auto introScene = new Scene();
	introScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
	introScene->getMenu()->add(introImage);

	mainMenuDisplay->disable();

	// This is the main background image, which starts off as the main background for the loading screen.
	auto mainMenuBackground = std::make_shared<ImageComponent>();
	GLTexture* mainMenuTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/main menu.png", mainMenuTexture);
	if (success) {
		mainMenuBackground->setDimensions(&renderingDimensions);
		mainMenuBackground->setTexture(mainMenuTexture);
	} else {
		AelaErrorHandling::consoleWindowError("Missing texture: ../../res/textures/main menu.png");
	}

	/*GLTexture* mainMenuTexture2;
	success = resourceManager->obtain<GLTexture>("res/textures/main menu.png", mainMenuTexture2);
	if (!success) {
		AelaErrorHandling::consoleWindowError("Missing texture: ../../res/textures/main menu.png");
	}*/

	// This is the texture that shows the loading window.
	/*auto loadingWindow = std::make_shared<ImageComponent>();
	loadingWindow->setPositioningMode(PositioningMode2D::CENTER);
	loadingWindow->hide();
	GLTexture* loadingWindowTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/loading screen 2.png", loadingWindowTexture);
	if (success) {
		loadingWindow->setDimensions(&Rect<int>(510, 384, 0, 103)); // The width starts at zero and goes up to 240.
		loadingWindow->setTexture(loadingWindowTexture);
	} else {
		AelaErrorHandling::consoleWindowError("Missing texture: ../../res/textures/res/textures/loading screen 2.png");
	}*/

	/*auto loadingRect = std::make_shared<RectComponent>();
	loadingRect->setColour(&ColourRGBA(0.259f, 0, 0.518f, 1));
	loadingRect->setDimensions(&Rect<int>(353, 401, 0, 20)); // Width starts at 0 and goes up to 318.
	loadingRect->hide();*/

	/*auto foxtrotWindowBackground = std::make_shared<ImageComponent>();
	foxtrotWindowBackground->setPositioningMode(PositioningMode2D::CENTER);
	foxtrotWindowBackground->hide();
	GLTexture* foxtrotWindowTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/main menu 2.png", foxtrotWindowTexture);
	if (success) {
		foxtrotWindowBackground->setDimensions(&Rect<int>(396, 381, 0, 270)); // Width starts at 0 and goes to 360.
		foxtrotWindowBackground->setTexture(foxtrotWindowTexture);
	} else {
		AelaErrorHandling::consoleWindowError("Missing texture: ../../res/textures/main menu 2.png");
	}*/

	/*auto foxtrotIcon = std::make_shared<ImageComponent>();
	GLTexture* foxtrotIconTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/main menu 3.png", foxtrotIconTexture);
	foxtrotIcon->hide();
	if (success) {
		foxtrotIcon->setDimensions(&Rect<int>(25, 3, 208, 19));
		foxtrotIcon->setTexture(foxtrotIconTexture);
	} else {
		AelaErrorHandling::consoleWindowError("Missing texture: ../../res/textures/main menu 3.png");
	}*/

	// This sets up text.
	auto titleText = std::make_shared<Label>("Neo-Zero Demo", pressStart2P, FontSizes::LARGE_FONT_SIZE, almostWhitePtr);
	titleText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.05), (int) (renderingDimensions.getHeight() * 0.61));
	// auto companyText = std::make_shared<Label>("Robert Ciborowski", pressStart2P, 15, almostWhitePtr);
	// companyText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.8), ((int) (renderingDimensions.getHeight() * 0.95)));
	auto arrowKeysText = std::make_shared<Label>("Use ARROW KEYS + ENTER to start, F4 to toggle", pressStart2P, 14, almostWhitePtr);
	arrowKeysText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.5), ((int) (renderingDimensions.getHeight() * 0.925)));
	arrowKeysText->setPositioningMode(PositioningMode2D::CENTER);
	auto volumeText = std::make_shared<Label>("fullscreen. Make sure to unmute your volume.", pressStart2P, 14, almostWhitePtr);
	volumeText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.5), ((int) (renderingDimensions.getHeight() * 0.96)));
	volumeText->setPositioningMode(PositioningMode2D::CENTER);

	/*auto loadingText = std::make_shared<Label>("Loading simulator...", pressStart2P, 12, almostWhitePtr);
	loadingText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.4), ((int) (renderingDimensions.getHeight() * 0.80)));
	loadingText->setPositioningMode(PositioningMode2D::CENTER);
	loadingText->hide();*/

	// This sets up actions for the main menu buttons.
	auto startNewGameAction = []() {
		game->startNewGame();
	};

	auto continueGameAction = []() {
		game->continueGame();
	};

	auto editMapAction = []() {
		game->editMap();
	};

	auto optionsAction = []() {AelaErrorHandling::windowWarning("There are no options!"); };
	auto exitAction = []() {engine->getWindow()->quit(); };

	auto startGameLabel = std::make_shared<Label>("Start New Game", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	startGameLabel->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.06), (int) (renderingDimensions.getHeight() / 1.5f));
	startGameLabel->setFont(pressStart2P);
	mainMenuDisplay->addOption(startGameLabel, startNewGameAction);

	int spacing = startGameLabel->getDimensions()->getHeight() + renderingDimensions.getHeight() / 25;

	auto continueGameLabel = std::make_shared<Label>("Continue Game", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	continueGameLabel->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.06), (int) (renderingDimensions.getHeight() / 1.5f + spacing));
	continueGameLabel->setFont(pressStart2P);
	mainMenuDisplay->addOption(continueGameLabel, continueGameAction);

	auto editMapLabel = std::make_shared<Label>("Edit Map", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	editMapLabel->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.06), (int) (renderingDimensions.getHeight() / 1.5f + spacing * 2));
	editMapLabel->setFont(pressStart2P);
	mainMenuDisplay->addOption(editMapLabel, editMapAction);

	auto optionsLabel = std::make_shared<Label>("Options", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	optionsLabel->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.06), (int) (renderingDimensions.getHeight() / 1.5f + spacing * 3));
	optionsLabel->setFont(pressStart2P);
	mainMenuDisplay->addOption(optionsLabel, optionsAction);

	auto exitLabel = std::make_shared<Label>("Exit", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	exitLabel->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.06), (int) (renderingDimensions.getHeight() / 1.5f + spacing * 4));
	exitLabel->setFont(pressStart2P);
	mainMenuDisplay->addOption(exitLabel, exitAction);

	auto selector = std::make_shared<ImageComponent>();
	GLTexture* selectorTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/selector.png", selectorTexture);
	if (success) {
		selector->setTexture(selectorTexture);
		selector->setDimensions(selectorTexture->getDimensions());
	} else {
		AelaErrorHandling::consoleWindowError("Missing texture: ../../res/textures/selector.png");
	}

	mainMenuDisplay->setSelectorImage(selector);
	dialogueDisplay->setSelectorImage(selector);

	/*auto foxtrotWindowElements = std::make_shared<SubMenu>();
	foxtrotWindowElements->init(&Rect<int>(36, 111, 720, 540), *renderer);
	foxtrotWindowElements->setTint(&ColourRGBA(1, 1, 1, 0));
	foxtrotWindowElements->hide();*/

	// This sets up the main menu scene.
	auto mainMenuScene = new Scene();
	mainMenuScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
	mainMenuScene->getMenu()->add(mainMenuBackground);
	mainMenuScene->getMenu()->add(titleText);
	// mainMenuScene->getMenu()->add(companyText);
	mainMenuScene->getMenu()->add(arrowKeysText);
	mainMenuScene->getMenu()->add(volumeText);
	mainMenuScene->getMenu()->add(startGameLabel);
	mainMenuScene->getMenu()->add(continueGameLabel);
	mainMenuScene->getMenu()->add(editMapLabel);
	mainMenuScene->getMenu()->add(optionsLabel);
	mainMenuScene->getMenu()->add(exitLabel);
	mainMenuScene->getMenu()->add(selector);
	/*mainMenuScene->getMenu()->add(loadingWindow);
	mainMenuScene->getMenu()->add(loadingRect);
	mainMenuScene->getMenu()->add(foxtrotWindowBackground);
	mainMenuScene->getMenu()->add(foxtrotIcon);
	mainMenuScene->getMenu()->add(loadingText);*/
	/*foxtrotWindowElements->add(titleText);
	// foxtrotWindowElements->add(companyText);
	foxtrotWindowElements->add(arrowKeysText);
	foxtrotWindowElements->add(volumeText);
	foxtrotWindowElements->add(startGameLabel);
	foxtrotWindowElements->add(continueGameLabel);
	foxtrotWindowElements->add(editMapLabel);
	foxtrotWindowElements->add(optionsLabel);
	// foxtrotWindowElements->add(exitLabel);
	foxtrotWindowElements->add(selector);
	mainMenuScene->getMenu()->add(foxtrotWindowElements);*/
	

	// This is the submenu for dialogue.
	auto dialogueRect = Rect<int>(0, renderingDimensions.getHeight() * 3 / 4, renderingDimensions.getWidth() / 2,
									renderingDimensions.getHeight() / 8);

	auto dialogueBoxSubMenu = std::make_shared<SubMenu>();
	dialogueBoxSubMenu->init(&dialogueRect, *renderer);

	auto dialgoueBoxImage = std::make_shared<ImageComponent>();
	dialgoueBoxImage->setPositioningMode(PositioningMode2D::CENTER);
	GLTexture* dialgueBoxTexture;

	// This is our special spot.
	success = resourceManager->obtain<GLTexture>("res/textures/dialogue_box.png", dialgueBoxTexture);

	dialogueRect.setX(renderingDimensions.getWidth() / 2);
	dialogueRect.setY(renderingDimensions.getHeight() * 7 / 8);
	dialgoueBoxImage->setDimensions(&dialogueRect);
	dialgoueBoxImage->setTexture(dialgueBoxTexture);

	auto avatarImage = std::make_shared<ImageComponent>();
	avatarImage->setPositioningMode(PositioningMode2D::CENTER);
	GLTexture* avatarTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/avatars_1/0/0.png", avatarTexture);
	// Note: the width and height of avatarDimensions used to be height / 8.
	Rect<int> avatarDimensions(renderingDimensions.getWidth() / 8, renderingDimensions.getHeight() * 7 / 8, (int) (renderingDimensions.getHeight() * 0.11f),
	                           (int) (renderingDimensions.getHeight() * 0.11f));
	avatarImage->setDimensions(&avatarDimensions);
	avatarImage->setTexture(avatarTexture);
	avatarImage->hide();

	auto dialogueTextOne = std::make_shared<Label>("Dialogue!", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	auto dialogueTextTwo = std::make_shared<Label>("Dialogue 2!", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	auto dialogueTextThree = std::make_shared<Label>("Dialogue 3!", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	auto dialogueTextFour = std::make_shared<Label>("Dialogue 4!", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	auto dialogueTextName = std::make_shared<Label>("", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	dialogueTextName->setPositioningMode(PositioningMode2D::CENTER);
	float dialogueTextStartX = dialogueDisplay->getDialogueTextStartX();
	dialogueTextOne->getDimensions()->setXY((int) (renderingDimensions.getWidth() * dialogueTextStartX), (int) (renderingDimensions.getHeight() * 0.86));
	dialogueTextTwo->getDimensions()->setXY((int) (renderingDimensions.getWidth() * dialogueTextStartX), (int) (renderingDimensions.getHeight() * 0.92));
	dialogueTextThree->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.55), (int) (renderingDimensions.getHeight() * 0.86));
	dialogueTextFour->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.55), (int) (renderingDimensions.getHeight() * 0.92));
	dialogueTextName->getDimensions()->setXY((int)(renderingDimensions.getWidth() / 8), (int)(renderingDimensions.getHeight() * 0.98));

	dialogueBoxSubMenu->add(dialgoueBoxImage);
	dialogueBoxSubMenu->add(avatarImage);
	dialogueBoxSubMenu->add(dialogueTextOne);
	dialogueBoxSubMenu->add(dialogueTextTwo);
	dialogueBoxSubMenu->add(dialogueTextThree);
	dialogueBoxSubMenu->add(dialogueTextFour);
	dialogueBoxSubMenu->add(dialogueTextName);
	dialogueBoxSubMenu->add(selector);

	dialogueDisplay->setSubMenu(dialogueBoxSubMenu);
	dialogueDisplay->setBackdrop(dialgoueBoxImage);
	dialogueDisplay->setAvatarImage(avatarImage);
	dialogueDisplay->setDialogueLabels({dialogueTextOne, dialogueTextTwo, dialogueTextThree, dialogueTextFour});
	dialogueDisplay->setNameLabel(dialogueTextName);

	// This sets up the submenu that shows the player's tile inventory. The second submenu only contains the
	// box that signifies which tile is selected, which is done so that the selection box is always above
	// the tiles.
	auto tileInventorySubMenu = std::make_shared<SubMenu>(), tileInventorySubMenu2 = std::make_shared<SubMenu>();
	auto invMenuRect = Rect<int>(renderingDimensions.getWidth() / 2, 0, renderingDimensions.getWidth() / 2, renderingDimensions.getHeight() / 3);
	tileInventorySubMenu->init(&invMenuRect, *renderer);
	tileInventorySubMenu2->init(&invMenuRect, *renderer);

	auto tileInventoryBackgroundImage = std::make_shared<ImageComponent>();
	GLTexture* tileInventoryBackgroundTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/inventory_background.png", tileInventoryBackgroundTexture);
	success = resourceManager->obtain<GLTexture>("res/textures/inventory_background.png", tileInventoryBackgroundTexture);
	Rect<int> tileInventoryBackgroundDimensions(renderingDimensions.getWidth() / 2, renderingDimensions.getHeight() / 15,
			  renderingDimensions.getWidth() / 2, renderingDimensions.getHeight() / 18);
	tileInventoryBackgroundImage->setDimensions(&tileInventoryBackgroundDimensions);
	tileInventoryBackgroundImage->setTexture(tileInventoryBackgroundTexture);

	auto tileInventoryText = std::make_shared<Label>("<current tile>", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	tileInventoryText->setPositioningMode(PositioningMode2D::CENTER);
	tileInventoryText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.845f), (int) (renderingDimensions.getHeight() * 0.2));

	auto tileSelectorBox = std::make_shared<ImageComponent>();
	GLTexture* tileInventoryBoxTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/selector_box.png", tileInventoryBoxTexture);
	int tileSelectorWidthAndHeight = renderingDimensions.getHeight() / 8;

	auto tileSelectorRect = Rect<int>((int) (renderingDimensions.getWidth() * 0.98 - tileSelectorWidthAndHeight - tileSelectorWidthAndHeight * 0.1),
									  (int) (renderingDimensions.getHeight() * 0.15 - tileSelectorWidthAndHeight - tileSelectorWidthAndHeight * 0.1),
									  (int) (tileSelectorWidthAndHeight * 1.2), (int) (tileSelectorWidthAndHeight * 1.2));
	tileSelectorBox->setDimensions(&tileSelectorRect);
	tileSelectorBox->setTexture(tileInventoryBoxTexture);

	tileInventorySubMenu->add(tileInventoryBackgroundImage);
	tileInventorySubMenu->add(tileInventoryText);
	tileInventorySubMenu2->add(tileSelectorBox);
	tileInventorySubMenu->show();
	tileInventorySubMenu2->show();
	tileInventoryDisplay->setMenuItems(tileInventorySubMenu, tileInventoryText, tileInventoryBackgroundImage, tileSelectorBox);

	auto deathBackgroundRect = std::make_shared<RectComponent>();
	deathBackgroundRect->setDimensions(&renderingDimensions);
	ColourRGBA deathBackgroundColour(0.225f, 0.15f, 0.15f, 0.95f);
	deathBackgroundRect->setColour(&deathBackgroundColour);
	deathBackgroundRect->hide();

	auto deathText = std::make_shared<Label>("You died.", pressStart2P, FontSizes::LARGE_FONT_SIZE, almostWhitePtr);
	deathText->setPositioningMode(PositioningMode2D::CENTER);
	deathText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.5), (int) (renderingDimensions.getHeight() * 0.4));
	deathText->hide();

	auto deathText2 = std::make_shared<Label>("Press 'R' to go back to your last checkpoint.", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	deathText2->setPositioningMode(PositioningMode2D::CENTER);
	deathText2->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.5), (int) (renderingDimensions.getHeight() * 0.6));
	deathText2->hide();

	game->setDeathMenuComponents(deathBackgroundRect, deathText, deathText2);

	auto fadeTeleportRect = std::make_shared<RectComponent>();
	fadeTeleportRect->setDimensions(&renderingDimensions);
	ColourRGBA fadeTeleportColour(0.00f, 0.00f, 0.00f, 1.0f);
	fadeTeleportRect->setColour(&fadeTeleportColour);
	fadeTeleportRect->hide();
	game->setFadeTeleportRect(fadeTeleportRect);

	auto hintImage = std::make_shared<ImageComponent>();
	GLTexture* hintTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/hint_box.png", hintTexture);
	Rect<int> hintDimensions(renderingDimensions.getWidth() / 12, renderingDimensions.getHeight() / 15,
			  renderingDimensions.getWidth() / 2, renderingDimensions.getHeight() / 18);
	hintImage->setDimensions(&hintDimensions);
	hintImage->setTexture(hintTexture);
	hintImage->hide();
	hintDisplay->setHintBackdrop(hintImage);

	// This sets up the hint text.
	auto hintText = std::make_shared<Label>("", pressStart2P, FontSizes::SMALL_FONT_SIZE, almostWhitePtr);
	hintText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.105), (int) (renderingDimensions.getHeight() * 0.105));
	hintDisplay->setHintLabel(hintText);
	hintDisplay->setSpacing((int) (renderingDimensions.getWidth() / 11 - renderingDimensions.getWidth() / 12));

	// This sets up the world gameplay scene, in which the player is given a top-down view of the world.
	auto worldGameplayScene = new Scene();
	worldGameplayScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
	worldGameplayScene->getMenu()->add(tileInventorySubMenu);
	worldGameplayScene->getMenu()->add(tileInventorySubMenu2);
	worldGameplayScene->getMenu()->add(deathBackgroundRect);
	worldGameplayScene->getMenu()->add(deathText);
	worldGameplayScene->getMenu()->add(deathText2);
	worldGameplayScene->getMenu()->add(fadeTeleportRect);
	worldGameplayScene->getMenu()->add(dialogueBoxSubMenu);
	worldGameplayScene->getMenu()->add(hintImage);
	worldGameplayScene->getMenu()->add(hintText);

	// This sets up the scenes for the pause menu.
	auto* pauseScene = new Scene();
	pauseScene->enableMenu(engine->getWindow()->getDimensions(), *engine->getRenderer());

	// The following blocks of code setup the pause menu scene.
	auto tintRect = std::make_shared<RectComponent>();
	tintRect->setDimensions(&renderingDimensions);
	auto tintRectColour = ColourRGBA(0.4f, 0.4f, 0.4f, 0.35f);
	tintRect->setColour(&tintRectColour);

	// This sets up a rectangle that the pause menu scene will use.
	auto pauseBackgroundRect = std::make_shared<RectComponent>();
	// auto pauseBackgroundRectn = Rect<int>((int) (windowDimensions.getWidth() * 0.3125), (int) (windowDimensions.getHeight() * 0.1111),
	// 									  (int) (windowDimensions.getWidth() * 0.5625), (int) (windowDimensions.getHeight() * 0.7777));
	auto pauseBackgroundRectn = Rect<int>((int)(renderingDimensions.getWidth() * 0.1111), (int)(renderingDimensions.getHeight() * 0.1111),
		(int)(renderingDimensions.getWidth() * 0.7777), (int)(renderingDimensions.getHeight() * 0.7777));
	pauseBackgroundRect->setDimensions(&pauseBackgroundRectn);
	// auto pauseBackgroundColour = ColourRGBA(0.2f, 0.2f, 0.2f, 0.95f);
	auto pauseBackgroundColour = ColourRGBA(0.063f, 0.063f, 0.2f, 1.0f);
	pauseBackgroundRect->setColour(&pauseBackgroundColour);

	// This is the box that says "Pause Menu".
	// auto pauseMenuTextRect = std::make_shared<RectComponent>();
	// auto pauseMenuTextRectm = Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.1111),
	// 									(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111));
	// pauseMenuTextRect->setDimensions(&pauseMenuTextRectm);
	// auto pauseMenuTextColour = ColourRGBA(0.2f, 0.2f, 0.2f, 0.95f);
	// pauseMenuTextRect->setColour(&pauseMenuTextColour);

	spacing = (int) (renderingDimensions.getHeight() * 0.075f);
	auto pauseMenuTitleText = std::make_shared<Label>("The game has been paused.", pressStart2P, 24, almostWhitePtr);
	pauseMenuTitleText->getDimensions()->setXY((int)(renderingDimensions.getWidth() / 2), (int)(renderingDimensions.getHeight() * 0.25));
	pauseMenuTitleText->setPositioningMode(PositioningMode2D::CENTER);

	auto movementControlsText = std::make_shared<Label>("Movement: WASD + LEFT SHIFT", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	movementControlsText->getDimensions()->setXY((int)(renderingDimensions.getWidth() / 2), (int)(renderingDimensions.getHeight() * 0.25 + spacing * 2));
	movementControlsText->setPositioningMode(PositioningMode2D::CENTER);

	auto gunControlsText = std::make_shared<Label>("Gun: BACKSLASH + ARROW KEYS", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	gunControlsText->getDimensions()->setXY((int)(renderingDimensions.getWidth() / 2), (int)(renderingDimensions.getHeight() * 0.25 + spacing * 3));
	gunControlsText->setPositioningMode(PositioningMode2D::CENTER);

	auto interactControlsText = std::make_shared<Label>("Interact: ENTER   Go to checkpoint: R", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	interactControlsText->getDimensions()->setXY((int)(renderingDimensions.getWidth() / 2), (int)(renderingDimensions.getHeight() * 0.25 + spacing * 4));
	interactControlsText->setPositioningMode(PositioningMode2D::CENTER);

	auto battleControlsText = std::make_shared<Label>("Battle: WASD  Fullscreen: F4  Exit: Alt + F4", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	battleControlsText->getDimensions()->setXY((int)(renderingDimensions.getWidth() / 2), (int)(renderingDimensions.getHeight() * 0.25 + spacing * 5));
	battleControlsText->setPositioningMode(PositioningMode2D::CENTER);

	auto foundABugText = std::make_shared<Label>("Found a bug? Let me know at bugs@neozero.com!", pressStart2P, FontSizes::SMALL_FONT_SIZE, almostWhitePtr);
	foundABugText->getDimensions()->setXY((int)(renderingDimensions.getWidth() / 2), (int)(renderingDimensions.getHeight() * 0.25 + spacing * 7));
	foundABugText->setPositioningMode(PositioningMode2D::CENTER);

	auto kickstarterText = std::make_shared<Label>("Support Neo-Zero on Kickstarter!", pressStart2P, FontSizes::SMALL_FONT_SIZE, almostWhitePtr);
	kickstarterText->getDimensions()->setXY((int)(renderingDimensions.getWidth() / 2), (int)(renderingDimensions.getHeight() * 0.25 + spacing * 7.3));
	kickstarterText->setPositioningMode(PositioningMode2D::CENTER);

	// // This creates sidebar buttons for the pause menu.
	// auto pauseMenuGameButton = std::make_shared<Button>(&hoverTint, &clickTint);
	// auto pauseMenuOptionsButton = std::make_shared<Button>(&hoverTint, &clickTint);
	// auto pauseMenuMapEditorButton = std::make_shared<Button>(&hoverTint, &clickTint);
	// auto pauseMenuExportButton = std::make_shared<Button>(hoverTintPtr, clickTintPtr);
	//
	// // This gets textures for the pause menu buttons.
	// GLTexture* simpleButtonTexture;
	// GLTexture* simpleButtonTextureLight;
	// success = resourceManager->obtain<GLTexture>((std::string) DEFAULT_TEXTURE_PATH + "simple_button.dds", simpleButtonTexture);
	// success = resourceManager->obtain<GLTexture>((std::string) DEFAULT_TEXTURE_PATH + "simple_button_light.dds", simpleButtonTextureLight);
	//
	// auto pauseMenuGameSubMenu = std::make_shared<SubMenu>();
	// pauseMenuGameSubMenu->init(&windowDimensions, *renderer);
	// auto pauseMenuOptionsSubMenu = std::make_shared<SubMenu>();
	// pauseMenuOptionsSubMenu->init(&windowDimensions, *renderer);
	// auto pauseMenuMapEditorSubMenu = std::make_shared<SubMenu>();
	// pauseMenuMapEditorSubMenu->init(&windowDimensions, *renderer);
	//
	// auto goToGameSubMenu = [pauseMenuGameSubMenu, pauseMenuOptionsSubMenu, pauseMenuMapEditorSubMenu](Engine* engine) {
	// 	pauseMenuGameSubMenu->show();
	// 	pauseMenuOptionsSubMenu->hide();
	// 	pauseMenuMapEditorSubMenu->hide();
	// };
	//
	// auto goToOptionsSubMenu = [pauseMenuGameSubMenu, pauseMenuOptionsSubMenu, pauseMenuMapEditorSubMenu](Engine* engine) {
	// 	pauseMenuGameSubMenu->hide();
	// 	pauseMenuOptionsSubMenu->show();
	// 	pauseMenuMapEditorSubMenu->hide();
	// };
	//
	// auto goToMapEditorSubMenu = [pauseMenuGameSubMenu, pauseMenuOptionsSubMenu, pauseMenuMapEditorSubMenu](Engine* engine) {
	// 	pauseMenuGameSubMenu->hide();
	// 	pauseMenuOptionsSubMenu->hide();
	// 	pauseMenuMapEditorSubMenu->show();
	// };
	//
	// auto exportMap = [](Engine* engine) {
	// 	WorldManager* worldManager = GameObjectProvider::getWorldManager();
	// 	if (worldManager->exportCurrentWorld()) {
	// 		AelaErrorHandling::windowWarning("Map was successfully exported.");
	// 	} else {
	// 		AelaErrorHandling::windowWarning("The map could not be exported.");
	// 	}
	// };
	//
	// // std::this_thread::sleep_for(std::chrono::seconds(1));
	//
	// pauseMenuGameButton->setTexture(simpleButtonTextureLight);
	// auto pauseMenuGameButtonRect = Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.2222),
	// 										 (int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111));
	// pauseMenuGameButton->setDimensions(&pauseMenuGameButtonRect);
	// pauseMenuGameButton->setupOnClick(std::bind(goToGameSubMenu, engine));
	// pauseMenuGameButton->setText("Game");
	// pauseMenuGameButton->setFont(pressStart2P);
	// pauseMenuGameButton->setFontSize(FontSizes::MEDIUM_FONT_SIZE);
	// pauseMenuGameButton->setTextColour(almostWhitePtr);
	//
	// pauseMenuOptionsButton->setTexture(simpleButtonTextureLight);
	// auto pauseMenuOptionsButtonRect = Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.3333),
	// 														(int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111));
	// pauseMenuOptionsButton->setDimensions(&pauseMenuOptionsButtonRect);
	// pauseMenuOptionsButton->setupOnClick(std::bind(goToOptionsSubMenu, engine));
	// pauseMenuOptionsButton->setText("Options");
	// pauseMenuOptionsButton->setFont(pressStart2P);
	// pauseMenuOptionsButton->setFontSize(FontSizes::MEDIUM_FONT_SIZE);
	// pauseMenuOptionsButton->setTextColour(almostWhitePtr);
	//
	// pauseMenuMapEditorButton->setTexture(simpleButtonTextureLight);
	// auto pauseMenuMapEditorButtonRect = Rect<int>((int) (windowDimensions.getWidth() * 0.125), (int) (windowDimensions.getHeight() * 0.4444),
	// 														  (int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111));
	// pauseMenuMapEditorButton->setDimensions(&pauseMenuMapEditorButtonRect);
	// pauseMenuMapEditorButton->setupOnClick(std::bind(goToOptionsSubMenu, engine));
	// pauseMenuMapEditorButton->setText("Map Editor");
	// pauseMenuMapEditorButton->setFont(pressStart2P);
	// pauseMenuMapEditorButton->setFontSize(FontSizes::MEDIUM_FONT_SIZE);
	// pauseMenuMapEditorButton->setTextColour(almostWhitePtr);
	//
	// auto pauseMenuExportButtonRect = Rect<int>((int) (windowDimensions.getWidth() * 0.4), (int) (windowDimensions.getHeight() * 0.3333),
	// 													   (int) (windowDimensions.getWidth() * 0.1875), (int) (windowDimensions.getHeight() * 0.1111));
	// pauseMenuExportButton->setDimensions(&pauseMenuExportButtonRect);
	// pauseMenuExportButton->setupOnClick(std::bind(exportMap, engine));
	// pauseMenuExportButton->setText("Export");
	// pauseMenuExportButton->setFont(pressStart2P);
	// pauseMenuExportButton->setFontSize(FontSizes::MEDIUM_FONT_SIZE);
	// pauseMenuExportButton->setTextColour(almostWhitePtr);
	// pauseMenuExportButton->wrapAroundText();
	// pauseMenuMapEditorSubMenu->add(pauseMenuExportButton);

	// pauseMenuGameSubMenu->show();

	pauseScene->getMenu()->add(tintRect);
	pauseScene->getMenu()->add(pauseBackgroundRect);
	/*pauseScene->getMenu()->add(pauseMenuGameButton);
	pauseScene->getMenu()->add(pauseMenuGameSubMenu);
	pauseScene->getMenu()->add(pauseMenuOptionsButton);
	pauseScene->getMenu()->add(pauseMenuOptionsSubMenu);
	pauseScene->getMenu()->add(pauseMenuMapEditorButton);
	pauseScene->getMenu()->add(pauseMenuMapEditorSubMenu);*/
	// pauseScene->getMenu()->add(pauseMenuTextRect);
	pauseScene->getMenu()->add(pauseMenuTitleText);
	pauseScene->getMenu()->add(movementControlsText);
	pauseScene->getMenu()->add(gunControlsText);
	pauseScene->getMenu()->add(interactControlsText);
	pauseScene->getMenu()->add(battleControlsText);
	pauseScene->getMenu()->add(foundABugText);
	pauseScene->getMenu()->add(kickstarterText);

	// This sets up the inventory scene.
	/*auto* inventoryScene = new Scene();
	inventoryScene->enableMenu(engine->getWindow()->getDimensions(), *engine->getRenderer());

	// This sets up a rectangle that the inventory scene will use.
	auto inventoryBackgroundRect = std::make_shared<RectComponent>();
	auto inventoryBackgroundRectm = Rect<int>((int) (windowDimensions.getWidth() * 0.3125), (int) (windowDimensions.getHeight() * 0.33333),
														  (int) (windowDimensions.getWidth() * 0.375), (int) (windowDimensions.getHeight() * 0.22222));
	inventoryBackgroundRect->setDimensions(&inventoryBackgroundRectm);
	auto inventoryBackgroundColour = ColourRGBA(0.15f, 0.15f, 0.15f, 0.95f);
	inventoryBackgroundRect->setColour(&inventoryBackgroundColour);

	// This sets up text for the inventory scene.
	auto inventoryTitleText = std::make_shared<Label>("Inventory", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	inventoryTitleText->getDimensions()->setXY((int) (windowDimensions.getWidth() * 0.33), (int) (windowDimensions.getHeight() * 0.38));

	auto inventorySlotA = std::make_shared<ImageComponent>();
	GLTexture* inventorySlotTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/inventory_slot.png", inventorySlotTexture);
	auto inventorySlotARect = Rect<int>((int) (windowDimensions.getWidth() * 0.37), (int) (windowDimensions.getHeight() * 0.4),
													(int) (windowDimensions.getWidth() * 0.1), (int) (windowDimensions.getWidth() * 0.1));
	inventorySlotA->setDimensions(&inventorySlotARect);
	inventorySlotA->setTexture(inventorySlotTexture);

	auto inventorySlotB = std::make_shared<ImageComponent>();
	auto inventorySlotBRect = Rect<int>((int) (windowDimensions.getWidth() * 0.53), (int) (windowDimensions.getHeight() * 0.4),
													(int) (windowDimensions.getWidth() * 0.1), (int) (windowDimensions.getWidth() * 0.1));
	inventorySlotB->setDimensions(&inventorySlotBRect);
	inventorySlotB->setTexture(inventorySlotTexture);

	inventoryScene->getMenu()->add(tintRect);
	inventoryScene->getMenu()->add(inventoryBackgroundRect);
	inventoryScene->getMenu()->add(inventoryTitleText);
	inventoryScene->getMenu()->add(inventorySlotA);
	inventoryScene->getMenu()->add(inventorySlotB);*/

	auto battleSubMenu = std::make_shared<SubMenu>();
	battleSubMenu->init(&renderingDimensions, *renderer);
	battleSubMenu->show();

	auto battleDialogueSubMenu = std::make_shared<SubMenu>();
	battleDialogueSubMenu->init(&Rect<int>(0, 0, 1024, 768), *renderer);
	battleDialogueSubMenu->show();

	// This sets up text.
	auto scoreText = std::make_shared<Label>("0", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	scoreText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.05), (int) (renderingDimensions.getHeight() * 0.97));

	auto timingText = std::make_shared<Label>("", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	timingText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.85), (int) (renderingDimensions.getHeight() * 0.97));
	battleDisplay->setTimingLabel(timingText);

	auto speechBubbleText = std::make_shared<Label>("blah blah blah", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostBlackPtr);
	speechBubbleText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.65), (int) (renderingDimensions.getHeight() * 0.125));
	auto speechBubbleText2 = std::make_shared<Label>("blah blah blah", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostBlackPtr);
	speechBubbleText2->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.65), (int) (renderingDimensions.getHeight() * 0.175));
	auto speechBubbleText3 = std::make_shared<Label>("blah blah blah", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostBlackPtr);
	speechBubbleText3->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.65), (int) (renderingDimensions.getHeight() * 0.225));
	battleDialogueDisplay->setDialogueLabels({speechBubbleText, speechBubbleText2, speechBubbleText3});

	// This sets up images.
	auto battleBackground = std::make_shared<ImageComponent>();
	auto battleBackground2 = std::make_shared<ImageComponent>();
	GLTexture* battleBackgroundTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/battle_1_1.png", battleBackgroundTexture);
	if (success) {
		battleBackground->setDimensions(&renderingDimensions);
		battleBackground->setTexture(battleBackgroundTexture);
		battleBackground2->setDimensions(&renderingDimensions);
		battleBackground2->setTexture(battleBackgroundTexture);
	} else {
		AelaErrorHandling::consoleWindowError("Missing texture: ../../res/textures/battle_1_1.png");
	}
	battleDisplay->setBattleBackgrounds(battleBackground, battleBackground2);

	auto speechBubble = std::make_shared<ImageComponent>();
	GLTexture* speechBubbleTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/speech_bubble.png", speechBubbleTexture);
	if (success) {
		speechBubble->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.61328), (int) (renderingDimensions.getHeight() / 12),
			 (int) (renderingDimensions.getHeight() * 0.41666), renderingDimensions.getHeight() / 6));
		speechBubble->setTexture(speechBubbleTexture);
	} else {
		AelaErrorHandling::consoleWindowError("Missing texture: ../../res/textures/speech_bubble.png");
	}

	auto rightKeyImage = std::make_shared<ImageComponent>();
	auto upKeyImage = std::make_shared<ImageComponent>();
	auto leftKeyImage = std::make_shared<ImageComponent>();
	auto downKeyImage = std::make_shared<ImageComponent>();
	auto healthBar = std::make_shared<ImageComponent>();
	auto emptyHealthBar = std::make_shared<ImageComponent>();
	auto battleAvatarImage = std::make_shared<ImageComponent>();
	battleDialogueDisplay->setAvatarImage(battleAvatarImage);
	GLTexture* battleTexture;
	success = resourceManager->obtain<GLTexture>("res/textures/battle/0/0.png", battleTexture);
	if (success) {
		int keyWidthAndHeight = (int) (renderingDimensions.getHeight() / 12);
		rightKeyImage->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.53125), (int) (renderingDimensions.getHeight() * 0.625),
			 keyWidthAndHeight, keyWidthAndHeight));
		rightKeyImage->setTexture(battleTexture);

		resourceManager->obtain<GLTexture>("res/textures/battle/2/0.png", battleTexture);
		upKeyImage->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.46875), (int) (renderingDimensions.getHeight() * 0.541666),
			 keyWidthAndHeight, keyWidthAndHeight));
		upKeyImage->setTexture(battleTexture);

		resourceManager->obtain<GLTexture>("res/textures/battle/4/0.png", battleTexture);
		leftKeyImage->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.40625), (int) (renderingDimensions.getHeight() * 0.625),
			 keyWidthAndHeight, keyWidthAndHeight));
		leftKeyImage->setTexture(battleTexture);

		resourceManager->obtain<GLTexture>("res/textures/battle/6/0.png", battleTexture);
		downKeyImage->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.46875), (int) (renderingDimensions.getHeight() * 0.625),
			 keyWidthAndHeight, keyWidthAndHeight));
		downKeyImage->setTexture(battleTexture);

		resourceManager->obtain<GLTexture>("res/textures/battle/7/1.png", battleTexture);
		healthBar->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.375), (int) (renderingDimensions.getHeight() * 0.9375),
			(int) (renderingDimensions.getWidth() / 4), (int) (renderingDimensions.getHeight() / 24)));
		healthBar->setTexture(battleTexture);

		resourceManager->obtain<GLTexture>("res/textures/battle/4/2.png", battleTexture);
		emptyHealthBar->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.375), (int) (renderingDimensions.getHeight() * 0.9375),
			(int) (renderingDimensions.getWidth() / 4), (int) (renderingDimensions.getHeight() / 24)));
		emptyHealthBar->setTexture(battleTexture);

		resourceManager->obtain<GLTexture>("res/textures/battle/5/2.png", battleTexture);
		battleAvatarImage->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.4296875), (int) (renderingDimensions.getHeight() * 0.0625),
			(int) (renderingDimensions.getHeight() * 0.1875), (int) (renderingDimensions.getHeight() * 0.1875)));
		battleAvatarImage->setTexture(battleTexture);
	} else {
		AelaErrorHandling::consoleWindowError("Missing all textures from: ../../res/textures/battle.png");
	}

	auto battleEndRect = std::make_shared<RectComponent>();
	battleEndRect->setDimensions(&renderingDimensions);
	ColourRGBA battleEndColour(0, 0, 0, 1);
	battleEndRect->setColour(&battleEndColour);
	battleEndRect->hide();

	auto battleEndText = std::make_shared<Label>("You died.", pressStart2P, FontSizes::LARGE_FONT_SIZE, almostWhitePtr);
	battleEndText->setPositioningMode(PositioningMode2D::CENTER);
	battleEndText->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.5), (int) (renderingDimensions.getHeight() * 0.4));
	battleEndText->hide();

	auto battleEndText2 = std::make_shared<Label>("Press 'R' to go back to your last checkpoint.", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	battleEndText2->setPositioningMode(PositioningMode2D::CENTER);
	battleEndText2->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.5), (int) (renderingDimensions.getHeight() * 0.65));
	battleEndText2->hide();

	auto battleEndText3 = std::make_shared<Label>("Score: 0", pressStart2P, FontSizes::MEDIUM_FONT_SIZE, almostWhitePtr);
	battleEndText3->setPositioningMode(PositioningMode2D::CENTER);
	battleEndText3->getDimensions()->setXY((int) (renderingDimensions.getWidth() * 0.5), (int) (renderingDimensions.getHeight() * 0.5));
	battleDisplay->setEndMenuItems(battleEndRect, battleEndText, battleEndText2, battleEndText3);

	// This sets up the main menu scene.
	auto battleScene = new Scene();
	battleScene->enableMenu(engine->getWindow()->getDimensions(), engine->getRendererReference());
	battleScene->getMenu()->add(battleBackground);
	battleScene->getMenu()->add(battleBackground2);
	battleSubMenu->add(rightKeyImage);
	battleSubMenu->add(upKeyImage);
	battleSubMenu->add(leftKeyImage);
	battleSubMenu->add(downKeyImage);
	battleSubMenu->add(emptyHealthBar);
	battleSubMenu->add(healthBar);
	battleSubMenu->add(scoreText);
	battleSubMenu->add(timingText);
	battleDialogueSubMenu->add(speechBubble);
	battleSubMenu->add(battleAvatarImage);
	battleDialogueSubMenu->add(speechBubbleText);
	battleDialogueSubMenu->add(speechBubbleText2);
	battleDialogueSubMenu->add(speechBubbleText3);
	battleSubMenu->add(battleDialogueSubMenu);
	battleSubMenu->add(battleEndRect);
	battleSubMenu->add(battleEndText);
	battleSubMenu->add(battleEndText2);
	battleSubMenu->add(battleEndText3);
	battleScene->getMenu()->add(battleSubMenu);

	battleDisplay->setBattleSubMenu(battleSubMenu);
	battleDisplay->setArrowKeyImages(rightKeyImage, upKeyImage, leftKeyImage, downKeyImage);
	battleDisplay->setScoreLabel(scoreText);
	battleDisplay->setHealthBarImage(healthBar);
	battleDisplay->setBattleScene(battleScene);
	battleDialogueDisplay->setSubMenu(battleDialogueSubMenu);

	Map3D* map;
	success = engine->getResourceManager()->obtain<Map3D>("res/maps/title_screen.txt", map);
	if (success) {
		// mainMenuScene->setMap(map);
		worldGameplayScene->setMap(map);
		pauseScene->setMap(map);
		// inventoryScene->setMap(map);
	} else {
		AelaErrorHandling::windowError("There was a problem loading title_screen.txt!");
	}

	engine->getSceneManager()->registerScene(introScene, INTRO_SCENE);
	engine->getSceneManager()->registerScene(mainMenuScene, MAIN_MENU_SCENE);
	engine->getSceneManager()->registerScene(worldGameplayScene, WORLD_GAMEPLAY_SCENE);
	engine->getSceneManager()->registerScene(pauseScene, PAUSE_SCENE);
	// engine->getSceneManager()->registerScene(inventoryScene, INVENTORY_SCENE);
	engine->getSceneManager()->registerScene(battleScene, BATTLE_SCENE);
	engine->getSceneManager()->setDisposingScenesOnDestroy(true);

	engine->getSceneManager()->setCurrentScene(INTRO_SCENE);
	GameObjectProvider::setGameplayScene(worldGameplayScene);
	GameObjectProvider::setPauseScene(pauseScene);
	Scripts::gameplayScene = worldGameplayScene;
	Scripts::pauseScene = pauseScene;
	game->sceneWasSwitched(INTRO_SCENE);

	introImage->show();

	// I set this animation to go by really quickly in order to save debugging time.
	Animator* animator = engine->getAnimator();
	AnimationTrack2D track;
	for (int i = 0; i < 4; i++) {
		KeyFrame2D frame;
		switch (i) {
			case 0: {
				auto onEnd = [introImage]() {
					introImage->show();
				};

				frame.setEndingAction(onEnd);
				frame.setObject(introImage);
				auto frameTint = ColourRGBA(1, 1, 1, 0);
				frame.setTint(&frameTint);
				track.addKeyFrameUsingMillis(1000, &frame);
				break;
			}
			case 1: {
				frame.setObject(introImage);
				auto frameTint = ColourRGBA(1, 1, 1, 1);
				frame.setTint(&frameTint);
				track.addKeyFrameUsingMillis(1500, &frame);
				break;
			}
			case 2: {
				frame.setObject(introImage);
				auto frameTint = ColourRGBA(1, 1, 1, 1);
				frame.setTint(&frameTint);
				track.addKeyFrameUsingMillis(1750, &frame);
				break;
			}
			case 3: {
				auto action = [](Engine* engine, AelaGame* game) {
					/*engine->getSceneManager()->setCurrentScene(WORLD_GAMEPLAY_SCENE);
					game->switchScene(WORLD_GAMEPLAY_SCENE);*/
					auto event = [engine, game]() {
						engine->getSceneManager()->setCurrentScene(MAIN_MENU_SCENE);
						game->sceneWasSwitched(MAIN_MENU_SCENE);
						mainMenuDisplay->enable();
					};

					timer->scheduleEventInMillis(500, event);
					/*loadingWindow->show();
					loadingRect->show();*/
				};

				frame.setEndingAction(std::bind(action, engine, game));
				frame.setObject(introImage);
				auto frameTint = ColourRGBA(1, 1, 1, 0);
				frame.setTint(&frameTint);
				track.addKeyFrameUsingMillis(1500, &frame);
				break;
			}

			/*case 4: {
				KeyFrame2D frame2;
				track.addKeyFrameUsingMillis(3500, &frame2);
				loadingWindow->show();
				frame.setObject(loadingWindow);
				frame.setDimensions(&Rect<int>(510, 384, 240, 103));
				track.addKeyFrameUsingMillis(125, &frame);
				break;
			}
			case 5: {
				KeyFrame2D frame2;
				track.addKeyFrameUsingMillis(3000, &frame2);
				frame.setObject(loadingRect);
				frame.setDimensions(&Rect<int>(353, 401, 150, 20));
				track.addKeyFrameUsingMillis(1400, &frame);
				break;
			}
			case 6: {
				KeyFrame2D frame2;
				track.addKeyFrameUsingMillis(600, &frame2);
				frame.setObject(loadingRect);
				frame.setDimensions(&Rect<int>(353, 401, 200, 20));
				track.addKeyFrameUsingMillis(600, &frame);
				break;
			}
			case 7: {
				auto onEnd = [loadingWindow, loadingRect, mainMenuBackground, mainMenuTexture2]() {
					auto event = [loadingWindow, loadingRect, mainMenuBackground, mainMenuTexture2]() {
						loadingWindow->hide();
						loadingRect->hide();
						mainMenuBackground->setTexture(mainMenuTexture2);
					};
					timer->scheduleEventInMillis(1000, event);
				};

				KeyFrame2D frame2;
				track.addKeyFrameUsingMillis(600, &frame2);
				frame.setObject(loadingRect);
				frame.setDimensions(&Rect<int>(353, 401, 318, 20));
				frame.setEndingAction(onEnd);
				track.addKeyFrameUsingMillis(1200, &frame);
				break;
			}
			case 8: {
				auto onEnd = [foxtrotIcon, foxtrotWindowBackground]() {
					foxtrotIcon->show();
					foxtrotWindowBackground->show();
				};

				auto onEnd2 = [loadingText]() {
					loadingText->show();
				};

				KeyFrame2D frame2;
				frame2.setEndingAction(onEnd);
				track.addKeyFrameUsingMillis(5500, &frame2);
				frame.setEndingAction(onEnd2);
				frame.setObject(foxtrotWindowBackground);
				frame.setDimensions(&Rect<int>(396, 381, 360, 270));
				track.addKeyFrameUsingMillis(125, &frame);
				break;
			}
			case 9: {
				auto onEnd = [foxtrotWindowElements, loadingText]() {
					foxtrotWindowElements->show();
					mainMenuDisplay->enable();
					loadingText->hide();
				};

				frame.setEndingAction(onEnd);
				track.addKeyFrameUsingMillis(3000, &frame);
				break;
			}*/
			default:
				break;
		}
	}
	animator->addAnimationTrack2D(&track);

	// std::this_thread::sleep_for(std::chrono::seconds(1));
	// Add memory point here!
	// std::this_thread::sleep_for(std::chrono::seconds(1));
}
