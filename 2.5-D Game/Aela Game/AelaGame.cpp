/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#include "AelaGame.h"
#include "../Worlds/WorldManager.h"
#include "../Character/CharacterProvider.h"
#include "../Enemies/EnemyProvider.h"
#include "../Scripts/ScriptManager.h"
#include "../Player/Player.h"
#include "../Displays/Dialogue/DialogueDisplay.h"
#include "../Displays/Tiles/TileInventoryDisplay.h"
#include "../Scripts/Scripts to Move to LUA/GameScript.h"
#include "../Scripts/Scripts to Move to LUA/ResourceScript.h"
#include "../Scripts/Scripts to Move to LUA/SceneScript.h"
#include "../Scripts/Scripts to Move to LUA/MapScript.h"
#include "../Scripts/Scripts to Move to LUA/AnimationScript.h"
#include "../Particles/TileSwitchParticleEmitter.h"
#include "../Constants/WindowConstants.h"
#include "../Constants/CameraConstants.h"
#include "../Resources/ResourceInfo.h"
#include "../Tiles/TileBehaviourExecutor.h"
#include "../Worlds/WorldExporter.h"
#include "../Scripts/Scripts to Move to LUA/ScriptObjects.h"
#include "../Camera/CameraController.h"
#include "../Displays/Hints/HintDisplay.h"
#include "../Doors/DoorProvider.h"
#include "../Save States/GameSaver.h"
#include "../../Project Aela/Events/EventListener.h"
#include "../Displays/Timer/TimerDisplay.h"
#include <utility>
#include "../Scripts/Scripts to Move to LUA/SceneIDs.h"
#include "../Displays/Battle/BattleDisplay.h"
#include "../Displays/Main Menu/MainMenuDisplay.h"
#include "../Battle/BattleLoader.h"
#include "../Displays/Battle Dialogue/BattleDialogueDisplay.h"
#include "../Audio/GameAudioPlayer.h"
#include "../Scripts/Scripts to Move to LUA/SoundScript.h"
using namespace Aela;

Game::AelaGame::AelaGame() {
	cameraMode = CameraMode::PLAYER_LOCKED;

	worldManager = new WorldManager();
	characterProvider = new CharacterProvider();
	enemyProvider = new EnemyProvider();
	player = new Player();
	scriptManager = new ScriptManager();
	dialogueDisplay = new DialogueDisplay();
	tileInventoryDisplay = new TileInventoryDisplay();
	auto* tileAtlas = new TileAtlas();
	auto* worldExporter = new WorldExporter();
	cameraController = new CameraController();
	tileBehaviourExecuter = new TileBehaviourExecutor();
	hintDisplay = new HintDisplay();
	timerDisplay = new TimerDisplay();
	auto* doorProvider = new DoorProvider();
	gameSaver = new GameSaver();
	mainMenuDisplay = new MainMenuDisplay();
	battleDisplay = new BattleDisplay();
	battleDialogueDisplay = new BattleDialogueDisplay();
	auto* gameAudioPlayer = new GameAudioPlayer();
	footstepAudioPlayer = new FootstepAudioPlayer();

	GameObjectProvider::setWorldManager(worldManager);
	GameObjectProvider::setCharacterProvider(characterProvider);
	GameObjectProvider::setEnemyProvider(enemyProvider);
	GameObjectProvider::setPlayer(player);
	GameObjectProvider::setScriptManager(scriptManager);
	GameObjectProvider::setDialogueDisplay(dialogueDisplay);
	GameObjectProvider::setTileInventoryDisplay(tileInventoryDisplay);
	GameObjectProvider::setTileAtlas(tileAtlas);
	GameObjectProvider::setWorldExporter(worldExporter);
	GameObjectProvider::setCameraController(cameraController);
	GameObjectProvider::setTileBehaviourExecuter(tileBehaviourExecuter);
	GameObjectProvider::setHintDisplay(hintDisplay);
	GameObjectProvider::setTimerDisplay(timerDisplay);
	GameObjectProvider::setDoorProvider(doorProvider);
	GameObjectProvider::setGameSaver(gameSaver);
	GameObjectProvider::setMainMenuDisplay(mainMenuDisplay);
	GameObjectProvider::setBattleDisplay(battleDisplay);
	GameObjectProvider::setBattleDialogueDisplay(battleDialogueDisplay);
	GameObjectProvider::setGameAudioPlayer(gameAudioPlayer);
	GameObjectProvider::setFootstepAudioPlayer(footstepAudioPlayer);
	GameObjectProvider::setGame(this);

	engine = GameObjectProvider::getEngine();
	window = GameObjectProvider::getWindow();
	resourceManager = GameObjectProvider::getResourceManager();
	renderer = GameObjectProvider::getRenderer();
	eventHandler = GameObjectProvider::getEventHandler();
	time = GameObjectProvider::getTime();
	sceneManager = GameObjectProvider::getSceneManager();
	animator = GameObjectProvider::getAnimator();
	camera = GameObjectProvider::getCamera();

	// This isn't actually necessary since RESOURCE_ROOT is the default for the ResourceManager.
	// However, if ResourceManager's RESOURCE_ROOT changes in Project Aela, then this will keep
	// the game from breaking.
	resourceManager->setResourceRoot(RESOURCE_ROOT);

	renderingDimensions = Rect<int>(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

Game::AelaGame::~AelaGame() {
	scriptManager->runScript("unload resources");
}

void Game::AelaGame::loadResources() {
	scriptManager->runScript("setup script objects");
	scriptManager->runScript("load materials");
	scriptManager->runScript("load models");
	scriptManager->runScript("load textures");
	scriptManager->runScript("load sprite sheets");
	scriptManager->runScript("load particles");
	scriptManager->runScript("load skyboxes");
	scriptManager->runScript("load maps");
	scriptManager->runScript("load tile atlas");
	// scriptManager->runScript("load all tiled maps");
	scriptManager->runScript("load starting tiled maps");
	scriptManager->runScript("load sound effects");
	scriptManager->runScript("setup sound");
	scriptManager->runScript("load fonts");
}

void Game::AelaGame::loadScenes() {
	scriptManager->runScript("load scenes");

	gameplayScene = GameObjectProvider::getGameplayScene();
	pauseScene = GameObjectProvider::getPauseScene();

	enemyProvider->scenesWereSetUp();
	worldManager->scenesWereSetUp();
}

void Game::AelaGame::loadAnimations() {
	scriptManager->runScript("load startup material animations");
}

void Game::AelaGame::setup() {
	resourceManager->setResourceRoot("../../../");
	fullscreen = window->isFullscreen();

	dialogueDisplay->setup();
	enemyProvider->setup();
	tileInventoryDisplay->setup();
	timerDisplay->setup();
	tileSwitchGun.setup();
	mainMenuDisplay->setup();
	battleDialogueDisplay->setup();
	footstepAudioPlayer->setup();
	
	// Setup the scripts of the game!
	setupScripts();

	// This runs the scripts that were set up.
	loadResources();
	loadScenes();
	loadAnimations();

	Scripts::setupScripts();

	// renderer->activateFeature(RendererFeature::MSAA_2D_X4);
	// renderer->activateFeature(RendererFeature::MSAA_3D_X4);

	using namespace Game;

	// Setup the player!
	playerCharacter = new Character(PLAYER_NAME, Location(0, 0, 0, 2.5, 0, 2.5));
	player->setCharacter(playerCharacter);
	playerCharacter->setTexture("player_1");
	playerCharacter->setMaxHealth(100);
	playerCharacter->setHealth(100);
	playerCharacter->setRunning(true);
	size_t playerID;
	if (!characterProvider->addCharacter(playerCharacter, &playerID)) {
		// Is this even possible to reach?!
		AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up the player. This error "
			+ "is supposed to be impossible to reach.");
	}

	tileInventoryDisplay->refreshSubMenu();

	// Tell the character manager that we've added all of our characters!
	// characterProvider->generateCharacterModelsForAllCharacters();
	
	player->setCharacterID(playerID);

	// Setup the world manager!
	worldManager->setup();
	map = worldManager->getMap3D();
	gameplayScene->setMap(map);
	pauseScene->setMap(map);
	cameraController->setup();
	tileBehaviourExecuter->setup();
	battleDisplay->setup();
	battlePlayer.setup();

	worldManager->rebuildMap();

	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(AelaGame::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(AelaGame::onEvent, this));

	Scripts::runGameStartingScripts();
}

void Game::AelaGame::update() {
	if (currentScene == WORLD_GAMEPLAY_SCENE) {
		if (player->isAlive()) {
			if (!movingRight && !movingForward && !movingLeft && !movingBackward && playerCharacter->isMovingFreely()) {
				playerCharacter->stopMoving();
			}

			/*float tint = playerCharacter->getHealth() / 100.0f;
			renderer->set3DTint(&ColourRGBA(1, tint, tint, 1));*/

			if (!dialogueDisplay->isShowingDialogue() && cameraMode == CameraMode::PLAYER_LOCKED
				&& playerCharacter->areNewMovementsAllowed()) {
				if (movingRight && (playerCharacter->getMovementDirection() != TileDirection::RIGHT || !playerCharacter->isMoving())) {
					if (playerCharacter->getDirectionFacing() != TileDirection::RIGHT) {
						playerCharacter->stopMoving();
						playerCharacter->turnFreely(TileDirection::RIGHT);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						if (gameMode == GameMode::GAMEPLAY) {
							playerCharacter->startMovingInDirection(TileDirection::RIGHT);
						} else {
							playerCharacter->addTileMovementToQueue(TileDirection::RIGHT, true);
						}
					}
					/*if (playerCharacter->getDirectionFacing() != TileDirection::RIGHT && !playerCharacter->hasMovementJustEnded()) {
						playerCharacter->turn(TileDirection::RIGHT);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						playerCharacter->addTileMovementToQueue(TileDirection::RIGHT);
					}*/
				}
				if (movingForward && (playerCharacter->getMovementDirection() != TileDirection::FORWARD || !playerCharacter->isMoving())) {
					if (playerCharacter->getDirectionFacing() != TileDirection::FORWARD) {
						playerCharacter->stopMoving();
						playerCharacter->turnFreely(TileDirection::FORWARD);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						if (gameMode == GameMode::GAMEPLAY) {
							playerCharacter->startMovingInDirection(TileDirection::FORWARD);
						} else {
							playerCharacter->addTileMovementToQueue(TileDirection::FORWARD, true);
						}
					}
					/*if (playerCharacter->getDirectionFacing() != TileDirection::FORWARD && !playerCharacter->hasMovementJustEnded()) {
						playerCharacter->turn(TileDirection::FORWARD);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						playerCharacter->addTileMovementToQueue(TileDirection::FORWARD);
					}*/
				}
				if (movingLeft && (playerCharacter->getMovementDirection() != TileDirection::LEFT || !playerCharacter->isMoving())) {
					if (playerCharacter->getDirectionFacing() != TileDirection::LEFT) {
						playerCharacter->stopMoving();
						playerCharacter->turnFreely(TileDirection::LEFT);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						if (gameMode == GameMode::GAMEPLAY) {
							playerCharacter->startMovingInDirection(TileDirection::LEFT);
						} else {
							playerCharacter->addTileMovementToQueue(TileDirection::LEFT, true);
						}
					}
					/*if (playerCharacter->getDirectionFacing() != TileDirection::LEFT && !playerCharacter->hasMovementJustEnded()) {
						playerCharacter->turn(TileDirection::LEFT);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						playerCharacter->addTileMovementToQueue(TileDirection::LEFT);
					}*/
				}
				if (movingBackward && (playerCharacter->getMovementDirection() != TileDirection::BACKWARD || !playerCharacter->isMoving())) {
					if (playerCharacter->getDirectionFacing() != TileDirection::BACKWARD) {
						playerCharacter->stopMoving();
						playerCharacter->turnFreely(TileDirection::BACKWARD);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						if (gameMode == GameMode::GAMEPLAY) {
							playerCharacter->startMovingInDirection(TileDirection::BACKWARD);
						} else {
							playerCharacter->addTileMovementToQueue(TileDirection::BACKWARD, true);
						}
					}
					/*if (playerCharacter->getDirectionFacing() != TileDirection::BACKWARD && !playerCharacter->hasMovementJustEnded()) {
						playerCharacter->turn(TileDirection::BACKWARD);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						// playerCharacter->addTileMovementToQueue(TileDirection::BACKWARD);
						Location location = *playerCharacter->getLocation();
						location.translateByAmount(glm::vec3(0, 0, -0.3));
						playerCharacter->addTileMovementToQueue(&Movement(&location, &glm::vec3(0, 0, -0.3), TileDirection::BACKWARD, false));
					}*/
				}

				if (pressedTileSwitch) {
					pressedTileSwitch = false;
					useTileSwitchGun();
				}

				if (gameMode == GameMode::MAP_EDITOR) {
					if (movingUp) {
						if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
							&& !playerCharacter->isMoving()) {
							playerCharacter->addTileMovementToQueue(TileDirection::UP, true);
						}
					}
					if (movingDown) {
						if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn
							&& !playerCharacter->isMoving()) {
							playerCharacter->addTileMovementToQueue(TileDirection::DOWN, true);
						}
					}
					if (pressingTileSwitch) {
						useTileSwitchGun();
					}
					if (pressingBackspace) {
						clearTilesAtPlayerLocation();
					}
				}

				if (pressingTileSelectLeft && time->getCurrentTimeInNanos() > timeAtLastTileSelect + timeBetweenTileSelects) {
					tileSelectLeftAction();
					timeBetweenTileSelects = TIME_BETWEEN_TILE_SELECTS_ON_HOLD;
				}

				if (pressingTileSelectRight && time->getCurrentTimeInNanos() > timeAtLastTileSelect + timeBetweenTileSelects) {
					tileSelectRightAction();
					timeBetweenTileSelects = TIME_BETWEEN_TILE_SELECTS_ON_HOLD;
				}

				if (pressedGoToTile1) {
					pressedGoToTile1 = false;
					TileMap* tiles = worldManager->getTileGroup(*playerCharacter->getLocation())->getTiles();
					if (tiles != nullptr && !tiles->empty()) {
						player->getTileInventory()->setCurrentTile(tiles->at(tiles->begin()->first).getType());
						tileInventoryDisplay->refreshSubMenu();
					}
				}

				if (pressedGoToTile2) {
					pressedGoToTile2 = false;
					TileMap* tiles = worldManager->getTileGroup(*playerCharacter->getLocation())->getTiles();
					if (tiles != nullptr && tiles->size() > 1) {
						auto iter = tiles->begin();
						iter.operator++();
						player->getTileInventory()->setCurrentTile(tiles->at(iter->first).getType());
						tileInventoryDisplay->refreshSubMenu();
					}
				}

				if (pressedGoToTile3) {
					pressedGoToTile3 = false;
					TileMap* tiles = worldManager->getTileGroup(*playerCharacter->getLocation())->getTiles();
					if (tiles != nullptr && tiles->size() > 2) {
						auto iter = tiles->begin();
						iter.operator++();
						iter.operator++();
						player->getTileInventory()->setCurrentTile(tiles->at(iter->first).getType());
						tileInventoryDisplay->refreshSubMenu();
					}
				}
			}

			if (pressedPauseButton) {
				sceneBeforePause = currentScene;
				sceneManager->setCurrentScene(PAUSE_SCENE);
				sceneWasSwitched(PAUSE_SCENE);
				animator->pause3DAnimations();
				engine->getTimer()->pause();
				pressedPauseButton = false;
			}
		}

		if (pressedReturn) {
			if (!battlePlayer.isInBattle()) {
				Location* playerLocation = playerCharacter->getLocation();
				glm::vec3 playerTile = playerLocation->getTileGroup();
				glm::ivec2 chunk = playerLocation->getChunk();
				worldManager->getCoordinateOfNeighbouringTile(playerTile, chunk, playerCharacter->getDirectionFacing());
				worldManager->runPromptedScriptOfTile(Location(playerLocation->getWorld(), chunk, playerTile));
				enemyProvider->returnWasPressed();
			}

			pressingReturn = true;
			pressedReturn = false;
		}

		if (pressedSaveReload) {
			pressedSaveReload = false;
			if (gameSaver->load("save_0")) {
				hintDisplay->clearAndDisplayHint("Loaded last savepoint!", HintDisplayDuration::LONG);
			} else if (!hintDisplay->isDisplayingHint()) {
				hintDisplay->displayHint("Failed to load savepoint!", HintDisplayDuration::LONG);
			}
		}

		worldManager->update();
		characterProvider->update();
		cameraController->update();
		enemyProvider->updateRegisteredEnemies();
		dialogueDisplay->update();

		if (gameMode == GameMode::MAP_EDITOR && time->getCurrentTimeInSecs() >= timeAtLastAutoWorldExport
			+ TIME_BETWEEN_AUTO_WORLD_EXPORTS) {
			worldManager->autoExportCurrentWorld();
			timeAtLastAutoWorldExport = time->getCurrentTimeInSecs();
		}
	} else if (currentScene == MAIN_MENU_SCENE) {
		mainMenuDisplay->update();
	} else if (currentScene == BATTLE_SCENE) {
		battlePlayer.update();
		battleDisplay->update();
		battleDialogueDisplay->update();
	} else if (currentScene == PAUSE_SCENE) {
		if (pressedPauseButton) {
			sceneManager->setCurrentScene(sceneBeforePause);
			sceneWasSwitched(sceneBeforePause);
			animator->unpause3DAnimations();
			engine->getTimer()->unpause();
			pressedPauseButton = false;
		}
	}

	if (window->isFullscreen() != fullscreen) {
		toggleFullscreen();
	}
}

void Game::AelaGame::onEvent(Event* event) {
	if (sceneManager->getCurrentSceneId() == WORLD_GAMEPLAY_SCENE) {
		// This processes events if the player is not in dialogue.
		if (!dialogueDisplay->isShowingDialogue()) {
			if (event->getType() == EventConstants::KEY_PRESSED) {
				auto* keyEvent = dynamic_cast<KeyEvent*>(event);
				switch (keyEvent->getKeycode()) {
					case SDLK_BACKSLASH:
						if (!pressingTileSwitch) {
							pressedTileSwitch = true;
							pressingTileSwitch = true;
						}
						break;
					case SDLK_RETURN:
						if (!pressingReturn && !dialogueDisplay->hadJustSkippedDialogue() && playerCharacter->areNewMovementsAllowed()) {
							pressedReturn = true;
						}
						break;
					case SDLK_i:
						if (!pressingInventoryButton) {
							sceneManager->setCurrentScene(INVENTORY_SCENE);
							sceneWasSwitched(INVENTORY_SCENE);
							pressingInventoryButton = true;
							// animator->pause3DAnimations();
						}
						break;
					case SDLK_BACKSPACE:
						pressingBackspace = true;
						if (gameMode == GameMode::MAP_EDITOR) {
							clearTilesAtPlayerLocation();
						}
						break;
					case SDLK_d:
					pressingRight = true;
					movingRight = true;
					movingForward = false;
					movingLeft = false;
					movingBackward = false;
					movingUp = false;
					movingDown = false;
					break;
				case SDLK_w:
					pressingForward = true;
					movingRight = false;
					movingForward = true;
					movingLeft = false;
					movingBackward = false;
					movingUp = false;
					movingDown = false;
					break;
				case SDLK_a:
					pressingLeft = true;
					movingRight = false;
					movingForward = false;
					movingLeft = true;
					movingBackward = false;
					movingUp = false;
					movingDown = false;
					break;
				case SDLK_s:
					pressingBackward = true;
					movingRight = false;
					movingForward = false;
					movingLeft = false;
					movingBackward = true;
					movingUp = false;
					movingDown = false;
					break;
				case SDLK_LEFT: {
					if (!player->isAlive() || pressingTileSelectLeft || pressingTileSelectRight) {
						break;
					}

					auto function = [this]() {
						tileSelectLeftAction();
						pressingTileSelectLeft = true;
						timeBetweenTileSelects = TIME_BETWEEN_TILE_SELECTS_ON_FIRST_PRESS;
					};

					engine->runNextUpdate(function);
					break;
				}
				case SDLK_RIGHT: {
					if (!player->isAlive() || pressingTileSelectLeft || pressingTileSelectRight) {
						break;
					}

					auto function = [this]() {
						tileSelectRightAction();
						pressingTileSelectRight = true;
						timeBetweenTileSelects = TIME_BETWEEN_TILE_SELECTS_ON_FIRST_PRESS;
					};

					engine->runNextUpdate(function);
					break;
				}
				case SDLK_LCTRL:
					if (gameMode == GameMode::MAP_EDITOR) {
						movingUp = false;
						movingDown = true;
						movingRight = false;
						movingForward = false;
						movingLeft = false;
						movingBackward = false;
					}
					break;
				case SDLK_SPACE: {
					if (gameMode == GameMode::MAP_EDITOR) {
						movingUp = true;
						movingDown = false;
						movingRight = false;
						movingForward = false;
						movingLeft = false;
						movingBackward = false;
					}
					break;
				}
					default:
						break;
				}
			}
		}
	} else if (currentScene == PAUSE_SCENE) {
		if (event->getType() == EventConstants::KEY_PRESSED) {
			auto* keyEvent = dynamic_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
				case SDLK_ESCAPE:
					if (!pressedPauseButton) {
						pressedPauseButton = true;
					}
					break;
				default:
					break;
			}
		}
	} else if (currentScene == INVENTORY_SCENE) {
		if (event->getType() == EventConstants::KEY_PRESSED) {
			auto* keyEvent = dynamic_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
				case SDLK_i:
					if (!pressingInventoryButton) {
						sceneManager->setCurrentScene(WORLD_GAMEPLAY_SCENE);
						sceneWasSwitched(WORLD_GAMEPLAY_SCENE);
						pressingInventoryButton = true;
						// animator->unpause3DAnimations();
					}
					break;
				default:
					break;
			}
		}
	}

	// This occurs for any scene.
	if (event->getType() == EventConstants::KEY_PRESSED) {
		auto* keyEvent = dynamic_cast<KeyEvent*>(event);
		if (!dialogueDisplay->isShowingDialogue()) {
			switch (keyEvent->getKeycode()) {
				case SDLK_ESCAPE:
					if (!pressedPauseButton && (currentScene == WORLD_GAMEPLAY_SCENE || currentScene == INVENTORY_SCENE)) {
						pressedPauseButton = true;
					}
					break;
				default:
					break;
			}
		}
		switch (keyEvent->getKeycode()) {
			case SDLK_LSHIFT:
				pressingLeftShift = true;
				if (playerCharacter->isRunning() && playerCharacter->areNewMovementsAllowed() && !playerCharacter->hasMovementsInQueue()) {
					playerCharacter->setRunning(false);
					changePlayerAnimationToRunning();
				}
				break;
			case SDLK_LALT:
				pressingLeftAlt = true;
				break;
			default:
				break;
		}
	}

	if (event->getType() == EventConstants::KEY_RELEASED) {
		auto* keyEvent = dynamic_cast<KeyEvent*>(event);
		switch (keyEvent->getKeycode()) {
			case SDLK_d:
				if (movingRight) {
					if (pressingForward) {
						movingForward = true;
					} else if (pressingBackward) {
						movingBackward = true;
					} else if (pressingLeft) {
						movingLeft = true;
					}
				}
				pressingRight = false;
				movingRight = false;
				break;
			case SDLK_w:
				if (movingForward) {
					if (pressingLeft) {
						movingLeft = true;
					} else if (pressingRight) {
						movingRight = true;
					} else if (pressingBackward) {
						movingBackward = true;
					}
				}
				pressingForward = false;
				movingForward = false;
				break;
			case SDLK_a:
				if (movingLeft) {
					playerCharacter->stopMoving();
					if (pressingBackward) {
						movingBackward = true;
					} else if (pressingForward) {
						movingForward = true;
					} else if (pressingRight) {
						movingRight = true;
					}
				}
				pressingLeft = false;
				movingLeft = false;
				break;
			case SDLK_s:
				if (movingBackward) {
					playerCharacter->stopMoving();
					if (pressingRight) {
						movingRight = true;
					} else if (pressingLeft) {
						movingLeft = true;
					} else if (pressingForward) {
						movingForward = true;
					}
				}
				pressingBackward = false;
				movingBackward = false;
				break;
			case SDLK_F4:
				fullscreen = !fullscreen;
				break;
			case SDLK_BACKSLASH:
				pressingTileSwitch = false;
				break;
			case SDLK_BACKSPACE:
				pressingBackspace = false;
				break;
			case SDLK_LCTRL:
				movingDown = false;
				break;
			case SDLK_SPACE:
				movingUp = false;
				break;
			case SDLK_LEFT: {
				auto function = [this]() {
					pressingTileSelectLeft = false;
				};

				engine->runNextUpdate(function);
				break;
			}
			case SDLK_RIGHT: {
				auto function = [this]() {
					pressingTileSelectRight = false;
				};

				engine->runNextUpdate(function);
				break;
			}
			case SDLK_RETURN:
				pressingReturn = false;
				break;
			case SDLK_LSHIFT:
				pressingLeftShift = false;
				playerCharacter->setRunning(true);
				break;
			case SDLK_LALT:
				pressingLeftAlt = false;
				break;
			case SDLK_i:
				pressingInventoryButton = false;
				break;
			case SDLK_r:
				if (currentScene == WORLD_GAMEPLAY_SCENE && !dialogueDisplay->isShowingDialogue()) {
					pressedSaveReload = true;
				}
				break;
			case SDLK_p:
				if (gameMode == GameMode::MAP_EDITOR) {
					pressedGoToTile1 = true;
				}
				break;
			case SDLK_LEFTBRACKET:
				if (gameMode == GameMode::MAP_EDITOR) {
					pressedGoToTile2 = true;
				}
				break;
			case SDLK_RIGHTBRACKET:
				if (gameMode == GameMode::MAP_EDITOR) {
					pressedGoToTile3 = true;
				}
				break;
			
			default:
				break;
		}

		if (pressingLeftShift && pressingLeftAlt) {
			switch (keyEvent->getKeycode()) {
				case SDLK_0:
					// This is here for debugging!
					if (cameraMode == CameraMode::PLAYER_LOCKED) {
						switchCameraMode(CameraMode::INDEPENDENT);
					} else {
						switchCameraMode(CameraMode::PLAYER_LOCKED);
					}
					break;
				case SDLK_1: {
					// This is here for debugging!
					if (!pressingLeftShift || !pressingLeftAlt) {
						break;
					}

					Scripts::setupLevel(1, 2);
					player->showAndEnableTileGun();
					tileInventoryDisplay->show();
					tileSwitchGun.setActive(true);
					playerCharacter->addTeleportToQueue(Location(1, -7, 0, 14.5f, 0, 1.5f), TeleportationAnimation::RISE);
					hintDisplay->clearAndDisplayHint("Cheat Activated: Rise Teleport", HintDisplayDuration::SHORT);
					break;
				}
				case SDLK_2: {
					// This is here for debugging!
					if (!pressingLeftShift || !pressingLeftAlt) {
						break;
					}

					Scripts::setupLevel(1, 3);
					player->showAndEnableTileGun();
					tileInventoryDisplay->show();
					tileSwitchGun.setActive(true);
					playerCharacter->addTeleportToQueue(Location(1, -21, 0, 11.5f, 0, 15.5f), TeleportationAnimation::RISE);
					hintDisplay->clearAndDisplayHint("Cheat Activated: Rise Teleport", HintDisplayDuration::SHORT);
					break;
				}
				case SDLK_3:
					// This is here for debugging!
					// player->kill();
					// hintDisplay->clearAndDisplayHint("Cheat Activated: Kill Player", HintDisplayDuration::SHORT);

					Scripts::setupLevel(2, 1);
					player->showAndEnableTileGun();
					tileInventoryDisplay->show();
					tileSwitchGun.setActive(true);
					playerCharacter->addTeleportToQueue(Location(0, -3, 0, 7.5f, 0, 7.5f), TeleportationAnimation::RISE);
					hintDisplay->clearAndDisplayHint("Cheat Activated: Rise Teleport", HintDisplayDuration::SHORT);
					break;
				case SDLK_4:
					// This is here for debugging!
					player->getCharacter()->toggleCollidability();
					hintDisplay->clearAndDisplayHint("Cheat Toggled: Player Collidability", HintDisplayDuration::SHORT);
					break;
				case SDLK_5:
					// This is here for debugging!
					/*
					renderer->toggleFeature(RendererFeature::LIGHTS);
					hintDisplay->clearAndDisplayHint("Cheat Toggled: Lights", HintDisplayDuration::SHORT);
					worldManager->getWorld(playerCharacter->getLocation()->getWorld())->setUseLights(renderer->isUsingFeature(RendererFeature::LIGHTS));
					*/
					Scripts::setupLevel(1, 1);
					scriptManager->runScript("walked_on_0_0");
					break;
				case SDLK_6:
					// This is here for debugging!
					playerCharacter->addTeleportToQueue(Location(2, 0, 0, 0.5f, 0, 0.5f), TeleportationAnimation::RISE);
					// gameSaver->save("save_0");
					break;
				case SDLK_7:
					// This is here for debugging!
					playerCharacter->increaseMoney(1000);
					hintDisplay->clearAndDisplayHint("Player Money Increased To: " + std::to_string(playerCharacter->getMoney()), HintDisplayDuration::LONG);
					break;
				case SDLK_8: {
					// This is here for debugging!
					// This is here for debugging!
					// Scripts::setupLevel(1, 3);
					// player->showTileGun();
					// playerCharacter->addTeleportToQueue(Location(1, -22, 1, 1.5f, 0, 14.5f), TeleportationAnimation::RISE);
					// hintDisplay->clearAndDisplayHint("Cheat Activated: Rise Teleport", HintDisplayDuration::SHORT);
					// break;
						engine->getAudioLooper()->stopAllLoops();
						hintDisplay->clearAndDisplayHint("Stopped all audio loops", HintDisplayDuration::LONG);
					break;
				}
				case SDLK_9:
					if (gameMode == GameMode::MAP_EDITOR) {
						if (worldManager->exportCurrentWorld()) {
							hintDisplay->clearAndDisplayHint("Exported map: " + MAP_BEING_EDITED, HintDisplayDuration::SHORT);
						} else {
							hintDisplay->clearAndDisplayHint("Failed to export map: " + MAP_BEING_EDITED, HintDisplayDuration::SHORT);
						}
					}
					break;
				/*case SDLK_9:
					// This is here for debugging!
					AudioClip* clip;
					if (resourceManager->obtain<AudioClip>("res/audio/streams/Death of a Laboratory.wav", clip)) {
						// GameObjectProvider::getAudioPlayer()->playClip(clip);
						engine->getAudioLooper()->loopAudioInfinitely("res/audio/streams/Death of a Laboratory.wav", 244060);
						hintDisplay->clearAndDisplayHint("Now Playing: Death of a Laboratory", HintDisplayDuration::LONG);
					}
					break;*/
				
				default:
					break;
			}
		}
	}
}

void Game::AelaGame::setupScripts() {
	using namespace Scripts;

	// Add vital scripts here!
	scriptManager->addScript("setup script objects", std::bind(&setupScriptObjects));
	scriptManager->addScript("load materials", std::bind(&loadMaterials));
	scriptManager->addScript("load models", std::bind(&loadModels));
	scriptManager->addScript("load textures", std::bind(&loadTextures));
	scriptManager->addScript("load sprite sheets", std::bind(&loadSpriteSheets));
	scriptManager->addScript("load particles", std::bind(&loadParticles));
	scriptManager->addScript("load skyboxes", std::bind(&loadSkyboxes));
	scriptManager->addScript("load maps", std::bind(&loadMaps));
	scriptManager->addScript("load sound effects", std::bind(&loadSoundEffects));
	scriptManager->addScript("setup sound", std::bind(&setupSound));
	scriptManager->addScript("load fonts", std::bind(&loadFonts));
	scriptManager->addScript("load tile atlas", std::bind(&loadTileAtlas));
	// scriptManager->addScript("load all tiled maps", std::bind(&loadAllTiledMaps));
	scriptManager->addScript("load starting tiled maps", std::bind(&loadStartingTiledMaps));
	scriptManager->addScript("load scenes", std::bind(&setupScenes));
	scriptManager->addScript("load startup material animations", std::bind(&loadMaterialAnimations));
	scriptManager->addScript("unload resources", std::bind(&unloadResources));
	scriptManager->addScript("start main menu music", std::bind(&startPlayingMainMenuMusic));
	scriptManager->addScript("stop main menu music", std::bind(&stopPlayingMainMenuMusic));
}

void Game::AelaGame::clearTilesAtPlayerLocation() {
	Location* playerLocation = playerCharacter->getLocation();
	glm::vec3 tile = playerLocation->getTileGroup();
	glm::vec2 chunk = playerLocation->getChunk();
	World* worldPtr = worldManager->getWorld(playerLocation->getWorld());
	Chunk* chunkPtr = worldPtr->getChunk(chunk);

	if (chunkPtr == nullptr) {
		worldManager->createChunkInCurrentWorld(chunk);
		chunkPtr = worldPtr->getChunk(chunk);
	}

	TileGroup* tileGroup = chunkPtr->getTileGroup(tile);
	if (tileGroup == nullptr) {
		worldManager->createLayerInCurrentWorld(chunk, (unsigned int) tile.y);
		tileGroup = chunkPtr->getTileGroup(tile);
	}

	tileGroup->clear();
	worldManager->rebuildMapNextUpdate();
}

void Game::AelaGame::tileSelectLeftAction() {
	if (animator->get2DTrack("selector_box_movement") == nullptr) {
		player->getTileInventory()->increaseCurrentTileIfPossible();
		// tileInventoryDisplay->animateSelectorBox();
	}
	tileInventoryDisplay->refreshSubMenu();
	timeAtLastTileSelect = time->getCurrentTimeInNanos();
}

void Game::AelaGame::tileSelectRightAction() {
	if (animator->get2DTrack("selector_box_movement") == nullptr) {
		player->getTileInventory()->decreaseCurrentTileIfPossible();
		// tileInventoryDisplay->animateSelectorBox();
	}
	tileInventoryDisplay->refreshSubMenu();
	timeAtLastTileSelect = time->getCurrentTimeInNanos();
}

void Game::AelaGame::changePlayerAnimationToRunning() {
	AnimationTrack3D* playerTrack = animator->get3DTrack(playerCharacter->getName() + "/mv");
	if (playerTrack == nullptr) {
		return;
	}

	double originalTimeOfAnimation = 1000000.0f / playerCharacter->getWalkingSpeed();
	double percentLeft = (originalTimeOfAnimation - playerTrack->getPositionInTrack()) / originalTimeOfAnimation;
	auto newAnimationTime = (long long) ((1000000.0f / playerCharacter->getRunningSpeed()) * percentLeft
		+ playerTrack->getPositionInTrack());

	std::vector<std::pair<long long, KeyFrame3D>>* playerFrames = playerTrack->getKeyFrames();
	playerFrames->at(playerFrames->size() - 1).first = newAnimationTime;

	AnimationTrack3D* cameraTrack = animator->get3DTrack("camera_mv");
	if (cameraTrack == nullptr) {
		return;
	}

	std::vector<std::pair<long long, KeyFrame3D>>* cameraFrames = cameraTrack->getKeyFrames();
	cameraFrames->at(cameraFrames->size() - 1).first = newAnimationTime;
}

void Game::AelaGame::switchCameraMode(CameraMode cameraMode) {
	this->cameraMode = cameraMode;

	switch (cameraMode) {
		case CameraMode::PLAYER_LOCKED:
			camera->useControls(false);
			break;
		case CameraMode::INDEPENDENT:
			camera->useControls(true);
			break;
	}
}

bool Game::AelaGame::useTileSwitchGun() {
	if (tileSwitchGun.use(gameMode)) {
		if (!playerCharacter->isMoving()) {
			// playerCharacter->allowNewMovements(false);
			playerCharacter->setTexture("player_5");

			auto event = [this]() {
				playerCharacter->setTexture("player_3");
				// playerCharacter->allowNewMovements(true);
			};

			engine->getTimer()->scheduleEventInMillis(200, event);
		}
		return true;
	} else {
		return false;
	}
}

void Game::AelaGame::sceneWasSwitched(unsigned int sceneID) {
	if (currentScene == MAIN_MENU_SCENE && sceneID != currentScene) {
		// Stop main menu music.
		scriptManager->runScript("stop main menu music");
	}

	switch (sceneID) {
		case INTRO_SCENE:
			window->hideCursor();
			break;
		case MAIN_MENU_SCENE:
			// Some cool camera shot for the main menu:
			camera->setInUse(true);
			camera->setForceCursorToMiddle(false);
			camera->useControls(false);
			camera->setPosition(4.57369f, 0.124887f, -3.30828f);
			camera->setRotation(2.685f, 0.244202f, 0);

			// window->showCursor();
			window->hideCursor();

			// Start main menu music.
			scriptManager->runScript("start main menu music");
			break;
		case WORLD_GAMEPLAY_SCENE:
			// First person camera:
			window->hideCursor();
			camera->setInUse(true);
			camera->setForceCursorToMiddle(true);
			camera->useControls(false);
			camera->setRotation(0, (float) -PLAYER_CAMERA_ANGLE, 0);

			// This makes sure that the world is rebuilt immediately!
			worldManager->rebuildMap();
			break;
		case INVENTORY_SCENE:
			window->hideCursor();
			break;
		case PAUSE_SCENE:
			pauseScene->setMap(gameplayScene->getMap());
			// window->showCursor();
			window->hideCursor();
			break;
		case BATTLE_SCENE:
			window->hideCursor();
		default:
			break;
	}
	currentScene = sceneID; 
}

void Game::AelaGame::startNewGame() {
	gameMode = GameMode::GAMEPLAY;
	sceneManager->setCurrentScene(WORLD_GAMEPLAY_SCENE);
	sceneWasSwitched(WORLD_GAMEPLAY_SCENE);
	scriptManager->runScript("start_new_game");
	tileInventoryDisplay->refreshSubMenu();
	playerCharacter->setRunningSpeed(0.00575f);
}

void Game::AelaGame::continueGame() {
	// AelaErrorHandling::windowWarning("Sorry, this feature is not yet available.");

	gameMode = GameMode::GAMEPLAY;
	// scriptManager->runScript("load all tiled maps");
	// scriptManager->runScript("load starting tiled maps");
	sceneManager->setCurrentScene(WORLD_GAMEPLAY_SCENE);
	sceneWasSwitched(WORLD_GAMEPLAY_SCENE);
	scriptManager->runScript("continue_game");
	tileInventoryDisplay->refreshSubMenu();
	tileInventoryDisplay->setup();
	player->getTileInventory()->clear(3);
}

void Game::AelaGame::editMap() {
	gameMode = GameMode::MAP_EDITOR;
	Scripts::loadTiledMap(MAP_BEING_EDITED, MAP_BEING_EDITED_WORLD_ID);
	sceneManager->setCurrentScene(WORLD_GAMEPLAY_SCENE);
	sceneWasSwitched(WORLD_GAMEPLAY_SCENE);
	player->setupTileInventoryForMapEditor();
	scriptManager->runScript("edit_map");
	tileInventoryDisplay->refreshSubMenu();
	playerCharacter->setRunningSpeed(0.0075f);
	playerCharacter->setCollidable(false);
}

void AelaGame::startBattle(std::string fileName) {
	if (!player->isAlive()) {
		player->revive();
	}

	playerCharacter->setHealth(playerCharacter->getMaxHealth());
	engine->getAudioLooper()->stopAllLoops();

	currentBattle = Battle();
	currentBattle.setSrc(fileName);

	BattleLoader loader;
	if (!loader.loadBattleAsTextFile(RESOURCE_ROOT + BATTLE_LOCATION + fileName + ".txt", currentBattle)) {
		return;
	}

	dialogueDisplay->closeDialog();
	battlePlayer.prepareForBattle(&currentBattle);
	fadeToBattleScene();

	auto event = [this]() {
		battlePlayer.playBattle(&currentBattle);
	};

	engine->getTimer()->scheduleEventInMillis(2000, event);
}

void AelaGame::endBattle() {
	// I might put more stuff here later.
}

GameMode Game::AelaGame::getGameMode() {
	return gameMode;
}

void Game::AelaGame::setDeathMenuComponents(std::shared_ptr<RectComponent> deathRect, std::shared_ptr<Label> deathLabel,
                                            std::shared_ptr<Label> deathLabel2) {
	this->deathRect = std::move(deathRect);
	this->deathLabel = std::move(deathLabel);
	this->deathLabel2 = std::move(deathLabel2);
}

void Game::AelaGame::setFadeTeleportRect(std::shared_ptr<RectComponent> fadeTeleportRect) {
	this->fadeTeleportRect = std::move(fadeTeleportRect);
}

void Game::AelaGame::animatePlayerDeathScreen() {
	ColourRGBA transparentBlack(0.0f);
	ColourRGBA white(1, 1, 1, 1);
	deathRect->setTint(&transparentBlack);
	deathRect->show();

	AnimationTrack2D track;
	KeyFrame2D frame;
	frame.setObject(deathRect);
	frame.setTint(&white);
	track.addKeyFrameUsingMillis(1000, &frame);
	animator->addAnimationTrack2D(&track);

	deathLabel->setTint(&transparentBlack);
	deathLabel->show();

	AnimationTrack2D track2;
	KeyFrame2D frame2;
	frame2.setObject(deathLabel);
	frame2.setTint(&white);
	track2.addKeyFrameUsingMillis(1000, &frame2);
	animator->addAnimationTrack2D(&track2);

	deathLabel2->setTint(&transparentBlack);
	deathLabel2->show();

	AnimationTrack2D track3;
	KeyFrame2D frame3;
	frame3.setObject(deathLabel2);
	frame3.setTint(&white);
	track3.addKeyFrameUsingMillis(1000, &frame3);
	animator->addAnimationTrack2D(&track3);
}

void Game::AelaGame::hideDeathScreen() {
	deathRect->hide();
	deathLabel->hide();
	deathLabel2->hide();
}

void Game::AelaGame::animateFadeTeleport() {
	ColourRGBA transparentBlack(0.0f);
	ColourRGBA transparentWhite(0.0f);
	ColourRGBA white(1, 1, 1, 1);

	fadeTeleportRect->show();
	fadeTeleportRect->setTint(&transparentBlack);

	AnimationTrack2D rectTrack2D;
	KeyFrame2D rectFrame;
	rectFrame.setObject(fadeTeleportRect);
	rectFrame.setTint(&transparentWhite);
	rectTrack2D.addKeyFrameUsingMillis(300, &rectFrame);

	KeyFrame2D rectFrame2;
	rectFrame2.setObject(fadeTeleportRect);
	rectFrame2.setTint(&white);
	rectTrack2D.addKeyFrameUsingMillis(300, &rectFrame2);
	animator->addAnimationTrack2D(&rectTrack2D);

	KeyFrame2D rectFrame3;
	rectFrame3.setObject(fadeTeleportRect);
	rectFrame3.setTint(&white);
	rectTrack2D.addKeyFrameUsingMillis(500, &rectFrame3);
	animator->addAnimationTrack2D(&rectTrack2D);

	KeyFrame2D rectFrame4;
	rectFrame4.setObject(fadeTeleportRect);
	rectFrame4.setTint(&transparentWhite);
	rectTrack2D.addKeyFrameUsingMillis(300, &rectFrame4);
	rectTrack2D.setTag("game_fade");
	animator->addAnimationTrack2D(&rectTrack2D);
}

TileSwitchGun* Game::AelaGame::getTileSwitchGun() {
	return &tileSwitchGun;
}

int AelaGame::getDefaultWidth() {
	return DEFAULT_WINDOW_WIDTH;
}

int AelaGame::getDefaultHeight() {
	return DEFAULT_WINDOW_HEIGHT;
}

Rect<int>* AelaGame::getRenderingDimensions() {
	return &renderingDimensions;
}

void AelaGame::setAnimateDeaths(bool animateDeaths) {
	this->animateDeaths = animateDeaths;
}

bool AelaGame::isAnimatingDeaths() {
	return animateDeaths;
}

void AelaGame::fadeOut() {
	fadeOut(500);
}

void AelaGame::fadeOut(unsigned long long timeInMillis) {
	animator->delete2DTracksByTag("game_fade");

	ColourRGBA black(0.0f);
	ColourRGBA white(1);
	fadeTeleportRect->show();
	fadeTeleportRect->setTint(&black);

	AnimationTrack2D rectTrack2D;
	KeyFrame2D rectFrame;
	rectFrame.setObject(fadeTeleportRect);
	rectFrame.setTint(&white);
	rectTrack2D.addKeyFrameUsingMillis(timeInMillis, &rectFrame);
	rectTrack2D.setTag("game_fade");
	animator->addAnimationTrack2D(&rectTrack2D);
}

void AelaGame::fadeIn() {
	fadeIn(300);
}

void AelaGame::fadeIn(unsigned long long timeInMillis) {
	animator->delete2DTracksByTag("game_fade");
	
	ColourRGBA black(0.0f);
	ColourRGBA white(1);
	fadeTeleportRect->show();
	fadeTeleportRect->setTint(&white);

	AnimationTrack2D rectTrack2D;
	KeyFrame2D rectFrame;
	rectFrame.setObject(fadeTeleportRect);
	rectFrame.setTint(&black);
	rectTrack2D.addKeyFrameUsingMillis(timeInMillis, &rectFrame);
	rectTrack2D.setTag("game_fade");
	animator->addAnimationTrack2D(&rectTrack2D);
}

void AelaGame::fadeToBattleScene() {
	animator->delete2DTracksByTag("game_fade");
	battleDisplay->hideBattleComponents();
	fadeOut(700);

	auto event = [this]() {
		battleDisplay->startShowingBattle();
		sceneWasSwitched(BATTLE_SCENE);
		engine->getSceneManager()->setCurrentScene(BATTLE_SCENE);
	};

	engine->getTimer()->scheduleEventInMillis(1300, event);
}

void AelaGame::fadeOutOfBattleScene() {
	animator->delete2DTracksByTag("game_fade");
	battleDisplay->hideBattleComponents();

	auto event = [this]() {
		fadeIn(700);
		sceneWasSwitched(WORLD_GAMEPLAY_SCENE);
		engine->getSceneManager()->setCurrentScene(WORLD_GAMEPLAY_SCENE);
	};

	engine->getTimer()->scheduleEventInMillis(1700, event);
}

void AelaGame::showBlackness() {
	animator->delete2DTracksByTag("game_fade");
	ColourRGBA tint(0, 0, 0, 1);
	fadeTeleportRect->setTint(&tint);
	fadeTeleportRect->show();
}

void AelaGame::hideBlackness() {
	animator->delete2DTracksByTag("game_fade");
	ColourRGBA tint(0, 0, 0, 0);
	fadeTeleportRect->setTint(&tint);
	fadeTeleportRect->hide();
}

void AelaGame::resetFadeRect() {
	animator->delete2DTracksByTag("game_fade");
	ColourRGBA white(1, 1, 1, 1);
	fadeTeleportRect->setTint(&white);
}

void AelaGame::setFullscreen(bool fullscreen) {
	if (window->isFullscreen() == fullscreen) {
		return;
	}

	if (fullscreen) {
		int width, height;
		engine->getUserEnvironment()->getNativeDisplayResolution(0, &width, &height);
		window->setDimensions(width, height);
		int outputWidth = (int) (height * WIDTH_TO_HEIGHT_RATIO);
		renderer->setOutputToWindow(&Rect<int>((int) ((width - outputWidth) / 2.0f), 0, outputWidth, height));
		window->setFullscreen(WindowFlag::AELA_WINDOW_FULLSCREEN_DESKTOP);
	} else {
		window->setDimensions(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
		window->setFullscreen(WindowFlag::AELA_WINDOW_WINDOWED);
		renderer->outputToEntireWindow();
	}

	renderer->resolutionWasChanged();
	this->fullscreen = fullscreen;
}

void AelaGame::toggleFullscreen() {
	setFullscreen(!window->isFullscreen());
}