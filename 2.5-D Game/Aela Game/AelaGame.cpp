#include <utility>

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
#include "../Utilities/GameConstants.h"
#include "../Resources/ResourceInfo.h"
#include "../Tiles/TileBehaviourExecutor.h"
#include "../Worlds/WorldExporter.h"
#include "../Scripts/Scripts to Move to LUA/ScriptObjects.h"
#include "../Camera/CameraController.h"
#include "../Displays/Hints/HintDisplay.h"
#include "../Doors/DoorProvider.h"
#include "../Save States/GameSaver.h"
#include "../../Project Aela/Events/EventListener.h"
#include "../Enemies/TileDestroyer.h"
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
	auto* doorProvider = new DoorProvider();
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
	scriptManager->runScript("load audio");
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

	// renderer->activateFeature(RendererFeature::MSAA_2D_X4);
	// renderer->activateFeature(RendererFeature::MSAA_3D_X4);

	using namespace Game;

	// Setup the player!
	playerCharacter = new Character();
	playerCharacter->setTexture("player_1");
	playerCharacter->setName(PLAYER_NAME);
	playerCharacter->setMaxHealth(1);
	playerCharacter->setHealth(1);
	size_t playerID;
	if (!characterProvider->addCharacter(playerCharacter, &playerID)) {
		// Is this even possible to reach?!
		AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up the player-> This error "
			+ "is supposed to be impossible to reach.");
	}

	tileInventoryDisplay->refreshSubMenu();

	// Tell the character manager that we've added all of our characters!
	// characterProvider->generateCharacterModelsForAllCharacters();

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
					if (playerCharacter->getDirectionFacing() != TileDirection::RIGHT && !playerCharacter->hasMovementJustEnded()) {
						playerCharacter->turnImmediately(TileDirection::RIGHT);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						playerCharacter->moveIfPossible(TileDirection::RIGHT);
					}
				}
				if (movingForward) {
					if (playerCharacter->getDirectionFacing() != TileDirection::FORWARD && !playerCharacter->hasMovementJustEnded()) {
						playerCharacter->turnImmediately(TileDirection::FORWARD);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						playerCharacter->moveIfPossible(TileDirection::FORWARD);
					}
				}
				if (movingLeft) {
					if (playerCharacter->getDirectionFacing() != TileDirection::LEFT && !playerCharacter->hasMovementJustEnded()) {
						playerCharacter->turnImmediately(TileDirection::LEFT);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						playerCharacter->moveIfPossible(TileDirection::LEFT);
					}
				}
				if (movingBackward) {
					if (playerCharacter->getDirectionFacing() != TileDirection::BACKWARD && !playerCharacter->hasMovementJustEnded()) {
						playerCharacter->turnImmediately(TileDirection::BACKWARD);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS
						&& !playerCharacter->isMoving()) {
						playerCharacter->moveIfPossible(TileDirection::BACKWARD);
					}
				}
				if (pressedTileSwitch) {
					pressedTileSwitch = false;
					useTileSwitchGun();
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
			}
		}

		if (pressedReturn) {
			Location* playerLocation = playerCharacter->getLocation();
			glm::ivec3 playerTile = playerLocation->getTileGroup();
			glm::ivec2 chunk = playerLocation->getChunk();
			worldManager->getCoordinateOfNeighbouringTile(playerTile, chunk, playerCharacter->getDirectionFacing());
			worldManager->runPromptedScriptOfTile(Location(playerLocation->getWorld(), chunk, playerTile));
			enemyProvider->returnWasPressed();
			pressingReturn = true;
			pressedReturn = false;
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
				auto* keyEvent = dynamic_cast<KeyEvent*>(event);
				switch (keyEvent->getKeycode()) {
					case SDLK_BACKSLASH:
						if (!pressingTileSwitch) {
							pressedTileSwitch = true;
							pressingTileSwitch = true;
						}
						break;
					case SDLK_RETURN:
						if (!pressingReturn && !dialogueDisplay->hadJustFinishedDialogue()) {
							pressedReturn = true;
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
						pressingBackspace = true;
						if (gameMode == GameMode::MAP_EDITOR) {
							clearTilesAtPlayerLocation();
						}
						break;
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
					if (!pressingPauseButton) {
						sceneManager->setCurrentScene(sceneBeforePause);
						switchScene(sceneBeforePause);
						pressingPauseButton = true;
						animator->unpause3DAnimations();
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
						switchScene(WORLD_GAMEPLAY_SCENE);
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
				case SDLK_ESCAPE:
					if (!pressingPauseButton && (currentScene == WORLD_GAMEPLAY_SCENE || currentScene == INVENTORY_SCENE)) {
						sceneBeforePause = currentScene;
						sceneManager->setCurrentScene(PAUSE_SCENE);
						switchScene(PAUSE_SCENE);
						pressingPauseButton = true;
						animator->pause3DAnimations();
					}
					break;
				default:
					break;
			}
		}
		switch (keyEvent->getKeycode()) {
			case SDLK_LSHIFT:
				if (!playerCharacter->getRunning() && playerCharacter->areNewMovementsAllowed()) {
					playerCharacter->setRunning(true);
					changePlayerAnimationToRunning();
				}
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
				playerCharacter->setRunning(false);
				break;
			case SDLK_ESCAPE:
				pressingPauseButton = false;
				break;
			case SDLK_i:
				pressingInventoryButton = false;
				break;
			case SDLK_r:
				if (gameSaver->load("save_0")) {
					hintDisplay->displayHint("Loaded last savepoint!", HintDisplayDuration::LONG);
				} else {
					hintDisplay->displayHint("Failed to load savepoint!", HintDisplayDuration::LONG);
				}
				break;
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
				Scripts::setupLevel(1, 2);
				player->showTileGun();
				playerCharacter->teleportWithAnimation(Location(1, glm::ivec2(-7, 0), glm::ivec3(14, 0, 1)), TeleportationAnimation::RISE);
				hintDisplay->clearAndDisplayHint("Cheat Activated: Rise Teleport", HintDisplayDuration::SHORT);
				break;
			}
			case SDLK_2: {
				// This is here for debugging!
				Scripts::setupLevel(1, 3);
				player->showTileGun();
				playerCharacter->teleportWithAnimation(Location(1, glm::ivec2(-15, 0), glm::ivec3(3, 0, 3)), TeleportationAnimation::RISE);
				hintDisplay->clearAndDisplayHint("Cheat Activated: Rise Teleport", HintDisplayDuration::SHORT);
				break;
			}
			case SDLK_3:
				// This is here for debugging!
				player->kill();
				hintDisplay->clearAndDisplayHint("Cheat Activated: Kill Player", HintDisplayDuration::SHORT);
				break;
			case SDLK_4:
				// This is here for debugging!
				player->getCharacter()->toggleCollidability();
				hintDisplay->clearAndDisplayHint("Cheat Activated: Toggle Player Collidability", HintDisplayDuration::SHORT);
				break;
			case SDLK_5:
				// This is here for debugging!
				renderer->toggleFeature(RendererFeature::LIGHTS);
				hintDisplay->clearAndDisplayHint("Cheat Activated: Toggle Lights", HintDisplayDuration::SHORT);
				break;
			case SDLK_6:
				// This is here for debugging!
				gameSaver->save("save_0");
				break;
			case SDLK_7:
				// This is here for debugging!
				// gameSaver->load("save_0");
				break;
			case SDLK_8: {
				// This is here for debugging!
				// This is here for debugging!
				Scripts::setupLevel(1, 3);
				player->showTileGun();
				playerCharacter->teleportWithAnimation(Location(1, glm::ivec2(-22, 1), glm::ivec3(1, 0, 14)), TeleportationAnimation::RISE);
				hintDisplay->clearAndDisplayHint("Cheat Activated: Rise Teleport", HintDisplayDuration::SHORT);
				break;
			}
			case SDLK_9:
				// This is here for debugging!
				/*AudioClip* clip;
				if (resourceManager->obtain<AudioClip>("res/audio/streams/Even the Tutorial Can Be Serious.wav", clip)) {
					GameObjectProvider::getAudioPlayer()->playClip(clip);
					hintDisplay->clearAndDisplayHint("Now Playing: Even the Tutorial Can Be Serious", HintDisplayDuration::LONG);
				}*/

				dynamic_cast<TileDestroyer*>(enemyProvider->getEnemy("destroyer_1"))->toggleHostility();
				scriptManager->runScript("walked_on_4_0");
				break;
			default:
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
	scriptManager->addScript("load audio", std::bind(&loadAudio));
	scriptManager->addScript("load fonts", std::bind(&loadFonts));
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

void Game::AelaGame::tileSelectLeftAction() {
	if (animator->get2DTrack("selector_box_movement") == nullptr) {
		player->getTileInventory()->increaseCurrentTileIfPossible();
		tileInventoryDisplay->animateSelectorBox();
	}
	tileInventoryDisplay->refreshSubMenu();
	timeAtLastTileSelect = time->getCurrentTimeInNanos();
}

void Game::AelaGame::tileSelectRightAction() {
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

void Game::AelaGame::switchScene(unsigned int sceneID) {
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
		default:
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
	AelaErrorHandling::windowWarning("Sorry, this feature is not yet available.");

//	gameMode = GameMode::GAMEPLAY;
//
//	sceneManager->setCurrentScene(WORLD_GAMEPLAY_SCENE);
//	switchScene(WORLD_GAMEPLAY_SCENE);
//
//	scriptManager->runScript("continue_game");
//	tileInventoryDisplay->refreshSubMenu();
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
	animator->addAnimationTrack2D(&rectTrack2D);
}

TileSwitchGun* Game::AelaGame::getTileSwitchGun() {
	return &tileSwitchGun;
}

void AelaGame::fadeOut() {
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
}

void AelaGame::fadeIn() {
	ColourRGBA transparentBlack(0.0f);
	ColourRGBA transparentWhite(0.0f);
	ColourRGBA white(1, 1, 1, 1);

	fadeTeleportRect->show();
	fadeTeleportRect->setTint(&white);

	AnimationTrack2D rectTrack2D;
	KeyFrame2D rectFrame;
	rectFrame.setObject(fadeTeleportRect);
	rectFrame.setTint(&white);
	rectTrack2D.addKeyFrameUsingMillis(500, &rectFrame);
	animator->addAnimationTrack2D(&rectTrack2D);

	KeyFrame2D rectFrame2;
	rectFrame2.setObject(fadeTeleportRect);
	rectFrame2.setTint(&transparentWhite);
	rectTrack2D.addKeyFrameUsingMillis(300, &rectFrame2);
	animator->addAnimationTrack2D(&rectTrack2D);
}

void AelaGame::resetFadeRect() {
	ColourRGBA white(1, 1, 1, 1);
	fadeTeleportRect->setTint(&white);
}
