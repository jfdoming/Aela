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
#include "../Utilities/MathConstants.h"
#include "../Particles/TileSwitchParticleEmitter.h"
#include "../Utilities/GameConstants.h"
#include "../Resources/ResourceInfo.h"
#include "../Tiles/TileBehaviourExecuter.h"
#include "../Worlds/WorldExporter.h"
#include "../Scripts/Scripts to Move to LUA/ScriptObjects.h"
#include "../Camera/CameraController.h"
#include "../Displays/Hint Display/HintDisplay.h"
#include "../Doors/DoorProvider.h"
#include "../Save States/GameSaver.h"
#include "../../Project Aela/Events/EventListener.h"

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
	TileAtlas* tileAtlas = new TileAtlas();
	WorldExporter* worldExporter = new WorldExporter();
	cameraController = new CameraController();
	tileBehaviourExecuter = new TileBehaviourExecuter();
	hintDisplay = new HintDisplay();
	DoorProvider* doorProvider = new DoorProvider();
	gameSaver = new GameSaver();

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
	GameObjectProvider::setDoorProvider(doorProvider);
	GameObjectProvider::setGameSaver(gameSaver);
	GameObjectProvider::setGame(this);

	engine = GameObjectProvider::getEngine();
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
	scriptManager->runScript("load tiled maps");
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
	dialogueDisplay->setup();
	enemyProvider->setup();
	tileInventoryDisplay->setup();
	tileSwitchGun.setup();

	// Setup the scripts of the game!
	setupScripts();

	// This runs the scripts that were set up.
	loadResources();
	loadScenes();
	loadAnimations();

	Scripts::setupScripts();

	renderer->activateFeature(RendererFeature::MSAA_2D_X4);
	renderer->activateFeature(RendererFeature::MSAA_3D_X4);

	using namespace Game;

	// Setup the player!
	playerCharacter = new Character();
	playerCharacter->setTextureName("character");
	playerCharacter->setName(PLAYER_NAME);
	playerCharacter->setMaxHealth(100);
	playerCharacter->setHealth(100);
	size_t playerID;
	if (!characterProvider->addCharacter(playerCharacter, &playerID)) {
		// Is this even possible to reach?!
		AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up the player-> This error "
			+ "is supposed to be impossible to reach.");
	}

	tileInventoryDisplay->refreshSubMenu();

	// Tell the character manager that we've added all of our characters!
	characterProvider->generateCharacterModelsForAllCharacters(resourceManager);

	player->setCharacter(playerCharacter);
	player->setCharacterID(playerID);

	// Setup the world manager!
	worldManager->setup();
	map = worldManager->getMap3D();
	gameplayScene->setMap(map);
	pauseScene->setMap(map);
	cameraController->setup();
	tileBehaviourExecuter->setup();

	worldManager->rebuildMap();

	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(AelaGame::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(AelaGame::onEvent, this));

	Scripts::runGameStartingScripts();
}

void Game::AelaGame::update() {
	if (currentScene == WORLD_GAMEPLAY_SCENE) {
		glm::vec3 tile = playerCharacter->getLocation()->getTileGroup();

		if (player->isAlive()) {
			/*float tint = playerCharacter->getHealth() / 100.0f;
			renderer->set3DTint(&ColourRGBA(1, tint, tint, 1));*/

			if (!playerCharacter->isMoving() && !dialogueDisplay->dialogueIsBeingShown() && cameraMode == CameraMode::PLAYER_LOCKED
				&& playerCharacter->areNewMovementsAllowed()) {
				if (movingRight) {
					if (playerCharacter->getDirectionFacing() != TileDirection::RIGHT && !playerCharacter->animationHasJustEnded()) {
						playerCharacter->turnImmediately(TileDirection::RIGHT);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
						playerCharacter->moveIfPossible(TileDirection::RIGHT);
					}
				}
				if (movingForward) {
					if (playerCharacter->getDirectionFacing() != TileDirection::FORWARD && !playerCharacter->animationHasJustEnded()) {
						playerCharacter->turnImmediately(TileDirection::FORWARD);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
						playerCharacter->moveIfPossible(TileDirection::FORWARD);
					}
				}
				if (movingLeft) {
					if (playerCharacter->getDirectionFacing() != TileDirection::LEFT && !playerCharacter->animationHasJustEnded()) {
						playerCharacter->turnImmediately(TileDirection::LEFT);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
						playerCharacter->moveIfPossible(TileDirection::LEFT);
					}
				}
				if (movingBackward) {
					if (playerCharacter->getDirectionFacing() != TileDirection::BACKWARD && !playerCharacter->animationHasJustEnded()) {
						playerCharacter->turnImmediately(TileDirection::BACKWARD);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
						playerCharacter->moveIfPossible(TileDirection::BACKWARD);
					}
				}
				if (gameMode == GameMode::MAP_EDITOR) {
					if (movingUp) {
						if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
							playerCharacter->moveIfPossible(TileDirection::UP);
						}
					}
					if (movingDown) {
						if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn) {
							playerCharacter->moveIfPossible(TileDirection::DOWN);
						}
					}
				}
				if (pressingTileSelectLeft && time->getCurrentTimeInNanos() > timeAtLastTileSelect + timeBetweenTileSelects) {
					tileSelectUpAction();
					timeBetweenTileSelects = TIME_BETWEEN_TILE_SELECTS_ON_HOLD;
				}
				if (pressingTileSelectRight && time->getCurrentTimeInNanos() > timeAtLastTileSelect + timeBetweenTileSelects) {
					tileSelectDownAction();
					timeBetweenTileSelects = TIME_BETWEEN_TILE_SELECTS_ON_HOLD;
				}
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
	}
}

void Game::AelaGame::onEvent(Event* event) {
	if (sceneManager->getCurrentSceneId() == WORLD_GAMEPLAY_SCENE) {
		// This processes events if the player is not in dialogue.
		if (!dialogueDisplay->dialogueIsBeingShown()) {
			if (event->getType() == EventConstants::KEY_PRESSED) {
				KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
				switch (keyEvent->getKeycode()) {
					case SDLK_BACKSLASH:
						if (!pressingTileSwitch) {
							useTileSwitchGun();
							pressingTileSwitch = true;
						}
						break;
					case SDLK_RETURN:
						if (!pressingReturn && !dialogueDisplay->hadJustFinishedDialogue()) {
							Location* playerLocation = playerCharacter->getLocation();
							glm::ivec3 tile = playerLocation->getTileGroup();
							glm::ivec2 chunk = playerLocation->getChunk();
							worldManager->getCoordinateOfNeighbouringTile(tile, chunk, playerCharacter->getDirectionFacing());
							Location location(playerLocation->getWorld(), chunk, tile);
							worldManager->runPromptedScriptOfTile(&location);
							pressingReturn = true;
						}
						break;
					case SDLK_i:
						if (!pressingInventoryButton) {
							sceneManager->setCurrentScene(INVENTORY_SCENE);
							switchScene(INVENTORY_SCENE);
							pressingInventoryButton = true;
							// animator->pause3DAnimations();
						}
						break;
					case SDLK_BACKSPACE:
						if (gameMode == GameMode::MAP_EDITOR) {
							clearTilesAtPlayerLocation();
						}
						break;
				}
			}
		}
	} else if (currentScene == PAUSE_SCENE) {
		if (event->getType() == EventConstants::KEY_PRESSED) {
			KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
			case SDLK_ESCAPE:
				if (!pressingPauseButton) {
					sceneManager->setCurrentScene(sceneBeforePause);
					switchScene(sceneBeforePause);
					pressingPauseButton = true;
					animator->unpause3DAnimations();
				}
				break;
			}
		}
	} else if (currentScene == INVENTORY_SCENE) {
		if (event->getType() == EventConstants::KEY_PRESSED) {
			KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
			case SDLK_i:
				if (!pressingInventoryButton) {
					sceneManager->setCurrentScene(WORLD_GAMEPLAY_SCENE);
					switchScene(WORLD_GAMEPLAY_SCENE);
					pressingInventoryButton = true;
					// animator->unpause3DAnimations();
				}
				break;
			}
		}
	}

	// This occurs for any scene.
	if (event->getType() == EventConstants::KEY_PRESSED) {
		KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
		if (!dialogueDisplay->dialogueIsBeingShown()) {
			switch (keyEvent->getKeycode()) {
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
				case SDLK_LEFT:
					if (!player->isAlive() || pressingTileSelectLeft || pressingTileSelectRight) {
						break;
					}
					tileSelectUpAction();
					pressingTileSelectLeft = true;
					timeBetweenTileSelects = TIME_BETWEEN_TILE_SELECTS_ON_FIRST_PRESS;
					break;
				case SDLK_RIGHT:
					if (!player->isAlive() || pressingTileSelectLeft || pressingTileSelectRight) {
						break;
					}
					tileSelectDownAction();
					pressingTileSelectRight = true;
					timeBetweenTileSelects = TIME_BETWEEN_TILE_SELECTS_ON_FIRST_PRESS;
					break;
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
				case SDLK_ESCAPE:
					if (!pressingPauseButton && (currentScene == WORLD_GAMEPLAY_SCENE || currentScene == INVENTORY_SCENE)) {
						sceneBeforePause = currentScene;
						sceneManager->setCurrentScene(PAUSE_SCENE);
						switchScene(PAUSE_SCENE);
						pressingPauseButton = true;
						animator->pause3DAnimations();
					}
					break;
			}
		}
		switch (keyEvent->getKeycode()) {
			case SDLK_LSHIFT:
				if (!playerCharacter->getRunning()) {
					playerCharacter->setRunning(true);
					changePlayerAnimationToRunning();
				}
				break;
		}
	}

	if (event->getType() == EventConstants::KEY_RELEASED) {
		KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
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

			case SDLK_BACKSLASH:
				pressingTileSwitch = false;
				break;
			case SDLK_LCTRL:
				movingDown = false;
				break;
			case SDLK_SPACE:
				movingUp = false;
				break;
			case SDLK_LEFT: {
				pressingTileSelectLeft = false;
				break;
			}
			case SDLK_RIGHT: {
				pressingTileSelectRight = false;
				break;
			}
			case SDLK_RETURN:
				pressingReturn = false;
				break;
			case SDLK_LSHIFT:
				playerCharacter->setRunning(false);
				break;
			case SDLK_ESCAPE:
				pressingPauseButton = false;
				break;
			case SDLK_i:
				pressingInventoryButton = false;
				break;
			case SDLK_0:
				// This is here for debugging!
				if (cameraMode == CameraMode::PLAYER_LOCKED) {
					switchCameraMode(CameraMode::INDEPENDENT);
				} else {
					switchCameraMode(CameraMode::PLAYER_LOCKED);
				}
				break;
			case SDLK_1:
				// This is here for debugging!
				playerCharacter->teleportWithAnimation(&Location(1, glm::ivec2(-7, 0), glm::ivec3(14, 0, 1)),
					TeleportationAnimation::RISE);
				break;
			case SDLK_2:
				// This is here for debugging!
				playerCharacter->teleportWithAnimation(&Location(1, glm::ivec2(-15, 0), glm::ivec3(3, 0, 3)),
					TeleportationAnimation::RISE);
				break;
			case SDLK_3:
				// This is here for debugging!
				player->kill();
				break;
			case SDLK_4:
				// This is here for debugging!
				renderer->toggleFeature(RendererFeature::SHADOWS);
				break;
			case SDLK_5:
				// This is here for debugging!
				renderer->toggleFeature(RendererFeature::LIGHTS);
				break;
			case SDLK_6:
				// This is here for debugging!
				gameSaver->save("save_0");
				break;
			case SDLK_7:
				// This is here for debugging!
				gameSaver->load("save_0");
				break;
			case SDLK_8:
				// This is here for debugging!
				playerCharacter->teleportWithAnimation(&Location(1, glm::ivec2(-5, 0), glm::ivec3(14, 0, 12)),
					TeleportationAnimation::FADE);
				break;
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
	scriptManager->addScript("load tiled maps", std::bind(&loadTiledMaps));
	scriptManager->addScript("load maps", std::bind(&loadMaps));
	scriptManager->addScript("load scenes", std::bind(&setupScenes));
	scriptManager->addScript("load startup material animations", std::bind(&loadMaterialAnimations));
	scriptManager->addScript("unload resources", std::bind(&unloadResources));
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

void Game::AelaGame::tileSelectUpAction() {
	if (animator->get2DTrack("selector_box_movement") == nullptr) {
		player->getTileInventory()->increaseCurrentTileIfPossible();
		tileInventoryDisplay->animateSelectorBox();
	}
	tileInventoryDisplay->refreshSubMenu();
	timeAtLastTileSelect = time->getCurrentTimeInNanos();
}

void Game::AelaGame::tileSelectDownAction() {
	if (animator->get2DTrack("selector_box_movement") == nullptr) {
		player->getTileInventory()->decreaseCurrentTileIfPossible();
		tileInventoryDisplay->animateSelectorBox();
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
	long long newAnimationTime = (long long) ((1000000.0f / playerCharacter->getRunningSpeed()) * percentLeft
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
	return tileSwitchGun.use(gameMode);
}

void Game::AelaGame::switchScene(int sceneID) {
	switch (sceneID) {
		case MAIN_MENU_SCENE:
			// Some cool camera shot for the main menu:
			camera->setInUse(true);
			camera->setForceCursorToMiddle(false);
			camera->useControls(false);
			camera->setPosition(4.57369f, 0.124887f, -3.30828f);
			camera->setRotation(2.685f, 0.244202f, 0);
			break;
		case WORLD_GAMEPLAY_SCENE:
			// First person camera:
			engine->getWindow()->hideCursor();
			camera->setInUse(true);
			camera->setForceCursorToMiddle(true);
			camera->useControls(false);
			camera->setRotation(0, -PLAYER_CAMERA_ANGLE, 0);
			break;
		case INVENTORY_SCENE:
			engine->getWindow()->hideCursor();
			break;
		case PAUSE_SCENE:
			pauseScene->setMap(gameplayScene->getMap());
			engine->getWindow()->showCursor();
			break;
	}
	currentScene = sceneID; 
}

void Game::AelaGame::startNewGame() {
	gameMode = GameMode::GAMEPLAY;
	sceneManager->setCurrentScene(WORLD_GAMEPLAY_SCENE);
	switchScene(WORLD_GAMEPLAY_SCENE);
	scriptManager->runScript("start_new_game");
	tileInventoryDisplay->refreshSubMenu();
}

void Game::AelaGame::continueGame() {
	gameMode = GameMode::GAMEPLAY;
	sceneManager->setCurrentScene(WORLD_GAMEPLAY_SCENE);
	switchScene(WORLD_GAMEPLAY_SCENE);
	scriptManager->runScript("continue_game");
	tileInventoryDisplay->refreshSubMenu();
}

void Game::AelaGame::editMap() {
	gameMode = GameMode::MAP_EDITOR;
	sceneManager->setCurrentScene(WORLD_GAMEPLAY_SCENE);
	switchScene(WORLD_GAMEPLAY_SCENE);
	player->setupTileInventoryForMapEditor();
	scriptManager->runScript("edit_map");
	tileInventoryDisplay->refreshSubMenu();
}

GameMode Game::AelaGame::getGameMode() {
	return gameMode;
}

void Game::AelaGame::setTileInventoryMenuItems(std::shared_ptr<SubMenu> tileInventorySubMenu,
	std::shared_ptr<Label> tileInventoryLabel, std::shared_ptr<ImageComponent> tileInventoryBoxImage) {
	tileInventoryDisplay->setMenuItems(tileInventorySubMenu, tileInventoryLabel, tileInventoryBoxImage);
}

void Game::AelaGame::setDeathMenuItems(std::shared_ptr<RectComponent> overlayRect, std::shared_ptr<Label> overlayText) {
	this->overlayRect = overlayRect;
	this->overlayText = overlayText;
}

void Game::AelaGame::animatePlayerDeathScreen() {
	overlayRect->setColour(&ColourRGBA(0.15f, 0.15f, 0.15f, 0.95f));

	AnimationTrack2D rectTrack2D;
	KeyFrame2D rectFrame;
	rectFrame.setObject(overlayRect);
	rectFrame.setTint(&ColourRGBA(1, 1, 1, 1));
	rectTrack2D.addKeyFrameUsingMillis(1000, &rectFrame);
	animator->addAnimationTrack2D(&rectTrack2D);

	AnimationTrack2D textTrack2D;
	KeyFrame2D textFrame;
	textFrame.setObject(overlayText);
	textFrame.setTint(&ColourRGBA(1, 1, 1, 1));
	textTrack2D.addKeyFrameUsingMillis(1000, &textFrame);
	animator->addAnimationTrack2D(&textTrack2D);
}

void Game::AelaGame::animateFadeTeleport() {
	overlayRect->setColour(&ColourRGBA(0.05f, 0.05f, 0.05f, 1.0f));

	AnimationTrack2D rectTrack2D;
	KeyFrame2D rectFrame;
	rectFrame.setObject(overlayRect);
	rectFrame.setTint(&ColourRGBA(1, 1, 1, 1));
	rectTrack2D.addKeyFrameUsingMillis(650, &rectFrame);

	KeyFrame2D rectFrame2;
	rectFrame2.setObject(overlayRect);
	rectFrame2.setTint(&ColourRGBA(1, 1, 1, 1));
	rectTrack2D.addKeyFrameUsingMillis(200, &rectFrame);

	KeyFrame2D rectFrame3;
	rectFrame3.setObject(overlayRect);
	rectFrame3.setTint(&ColourRGBA(1, 1, 1, 0));
	rectTrack2D.addKeyFrameUsingMillis(650, &rectFrame);
	animator->addAnimationTrack2D(&rectTrack2D);

	AnimationTrack2D textTrack2D;
	KeyFrame2D textFrame;
	textFrame.setObject(overlayText);
	textFrame.setTint(&ColourRGBA(1, 1, 1, 1));
	textTrack2D.addKeyFrameUsingMillis(650, &textFrame);

	KeyFrame2D textFrame2;
	textFrame2.setObject(overlayText);
	textFrame2.setTint(&ColourRGBA(1, 1, 1, 1));
	textTrack2D.addKeyFrameUsingMillis(200, &textFrame);

	KeyFrame2D textFrame3;
	textFrame3.setObject(overlayText);
	textFrame3.setTint(&ColourRGBA(1, 1, 1, 0));
	textTrack2D.addKeyFrameUsingMillis(650, &textFrame);
	animator->addAnimationTrack2D(&textTrack2D);
}

TileSwitchGun* Game::AelaGame::getTileSwitchGun() {
	return &tileSwitchGun;
}
