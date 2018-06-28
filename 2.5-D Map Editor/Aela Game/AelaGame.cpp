/*
* Class: Aela Game
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used to represent a game which uses the Aela Software Engine.
*/

#include "AelaGame.h"
#include "Events/EventListener.h"
#include "../Scripts/Scripts to Move to LUA/GameScript.h"
#include "../Scripts/Scripts to Move to LUA/ResourceScript.h"
#include "../Scripts/Scripts to Move to LUA/SceneScript.h"
#include "../Scripts/Scripts to Move to LUA/MapScript.h"
#include "../Resources/ResourceInfo.h"
#include "../Utilities/MathConstants.h"
#include "../Particles/TileSwitchParticleEmitter.h"
#include "../Utilities/GameConstants.h"

using namespace Aela;

Game::AelaGame::AelaGame(Engine* engine) : engine(engine), tileInventoryDisplay(engine) {
	// This just sets properties.
	window = engine->getWindow();
	renderer = engine->getRenderer();
	eventHandler = engine->getEventHandler();
	time = engine->getTime();
	sceneManager = engine->getSceneManager();
	resourceManager = engine->getResourceManager();
	audioPlayer = engine->getAudioPlayer();
	animator = engine->getAnimator();
	userEnvironment = engine->getUserEnvironment();
	framerateCalculator = engine->getFramerateCalculator();
	camera = engine->getRendererReference().getCamera();
	// luaManager = engine->getLuaManager();
	// animator = engine->getAnimator();

	// This isn't actually necessary since RESOURCE_ROOT is the default for the ResourceManager.
	// However, if ResourceManager's RESOURCE_ROOT changes in Project Aela, then this will keep
	// the game from breaking.
	resourceManager->setResourceRoot(RESOURCE_ROOT);

	cameraMode = CameraMode::PLAYER_LOCKED;
}

void Game::AelaGame::loadResources() {
	scriptManager.runScript("load materials");
	scriptManager.runScript("load models");
	scriptManager.runScript("load textures");
	scriptManager.runScript("load sprite sheets");
	scriptManager.runScript("load particles");
	scriptManager.runScript("load skyboxes");
	scriptManager.runScript("load startup map");
	scriptManager.runScript("load tiled maps");
}

void Game::AelaGame::loadScenes() {
	scriptManager.runScript("load scenes");
}

void Game::AelaGame::setup() {
	setupScripts();

	// These basically just run the scripts.
	loadResources();
	loadScenes();

	// Setup the scripts of the game!
	GameScripts::setupGameScripts(this, engine);

	engine->getRendererReference().activateFeature(RendererFeature::MSAA_2D_X4);
	engine->getRendererReference().activateFeature(RendererFeature::MSAA_3D_X4);

	characterTracker = worldManager.getCharacterTracker();

	// Setup the player!
	playerCharacter = new Character();
	playerCharacter->setup(&Location(0, glm::ivec2(0, 0), glm::ivec3(1, 0, 1)));
	playerCharacter->setTextureName("character");
	playerCharacter->setName(PLAYER_NAME);
	playerCharacter->setMaxHealth(100);
	playerCharacter->setHealth(100);
	size_t playerID;
	if (!characterTracker->trackCharacter(playerCharacter, &playerID)) {
		// Is this even possible to reach?!
		AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up the player. This error "
			+ "is supposed to be impossible to reach.");
	}
	characterTracker->setPlayer(playerID);

	enemyRegistrar.setPlayer(&player);
	enemyRegistrar.setAelaObjects(engine);

	GameScripts::runStartingScripts();

	// Tell the character manager that we've added all of our characters!
	characterTracker->generateCharacterModels(resourceManager);

	// Setup the world manager!
	worldManager.setup(engine, &scriptManager, &dialogueHandler, playerCharacter);
	map = worldManager.getMap3D();

	// Setup the dialogue handler!
	dialogueHandler.setup(time, eventHandler, &scriptManager);

	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(AelaGame::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(AelaGame::onEvent, this));

	player.setupTileInventory(worldManager.getTileAtlas());
	tileInventoryDisplay.setGameObjects(&player, &worldManager);
	tileInventoryDisplay.refreshSubMenu();

	player.setCharacter(playerCharacter);
	player.setCharacterID(playerID);
}

void Game::AelaGame::update() {
	if (currentScene == WORLD_GAMEPLAY_SCENE) {
		glm::vec3 tile = playerCharacter->getLocation()->getTile();

		if (!characterTracker->isPlayerDead()) {
			/*float tint = playerCharacter->getHealth() / 100.0f;
			renderer->set3DTint(&ColourRGBA(1, tint, tint, 1));*/

			if (!playerCharacter->isMoving() && !dialogueHandler.dialogueIsBeingShown() && cameraMode == CameraMode::PLAYER_LOCKED
				&& !playerCharacter->isFrozen()) {
				if (movingRight) {
					if (playerCharacter->getDirectionFacing() != TileDirection::RIGHT && !playerCharacter->animationHasJustEnded()) {
						characterTracker->turnTrackedCharacter(playerCharacter, TileDirection::RIGHT);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
						worldManager.moveCharacterIfPossible(player.getCharacterID(), TileDirection::RIGHT);
					}
				}
				if (movingForward) {
					if (playerCharacter->getDirectionFacing() != TileDirection::FORWARD && !playerCharacter->animationHasJustEnded()) {
						characterTracker->turnTrackedCharacter(playerCharacter, TileDirection::FORWARD);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
						worldManager.moveCharacterIfPossible(player.getCharacterID(), TileDirection::FORWARD);
					}
				}
				if (movingLeft) {
					if (playerCharacter->getDirectionFacing() != TileDirection::LEFT && !playerCharacter->animationHasJustEnded()) {
						characterTracker->turnTrackedCharacter(playerCharacter, TileDirection::LEFT);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
						worldManager.moveCharacterIfPossible(player.getCharacterID(), TileDirection::LEFT);
					}
				}
				if (movingBackward) {
					if (playerCharacter->getDirectionFacing() != TileDirection::BACKWARD && !playerCharacter->animationHasJustEnded()) {
						characterTracker->turnTrackedCharacter(playerCharacter, TileDirection::BACKWARD);
						timeAtLastPlayerTurn = time->getCurrentTimeInNanos();
					} else if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
						worldManager.moveCharacterIfPossible(player.getCharacterID(), TileDirection::BACKWARD);
					}
				}
				if (movingUp) {
					if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
						worldManager.moveCharacterIfPossible(player.getCharacterID(), TileDirection::UP);
					}
				}
				if (movingDown) {
					if (time->getCurrentTimeInNanos() >= timeAtLastPlayerTurn) {
						worldManager.moveCharacterIfPossible(player.getCharacterID(), TileDirection::DOWN);
					}
				}
				if (pressingTileSelectLeft && time->getCurrentTimeInNanos() > timeAtLastTileSelect + timeBetweenTileSelects) {
					tileSelectUpAction();
				}
				if (pressingTileSelectRight && time->getCurrentTimeInNanos() > timeAtLastTileSelect + timeBetweenTileSelects) {
					tileSelectDownAction();
				}
			}
		}

		worldManager.update();
		enemyRegistrar.updateRegisteredEnemies();
		dialogueHandler.update();
	}
}

void Game::AelaGame::onEvent(Event* event) {
	if (sceneManager->getCurrentSceneId() == WORLD_GAMEPLAY_SCENE) {
		// This processes events if the player is not in dialogue.
		if (!dialogueHandler.dialogueIsBeingShown()) {
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
						if (!pressingReturn && !dialogueHandler.hadJustFinishedDialogue()) {
							Location* playerLocation = playerCharacter->getLocation();
							glm::vec3 tile = playerLocation->getTile();
							glm::vec2 chunk = playerLocation->getChunk();
							worldManager.getCoordinateOfNeighbouringTile(tile, chunk, playerCharacter->getDirectionFacing());
							Location location(playerLocation->getWorld(), chunk, tile);
							worldManager.runPromptedScriptOfTile(&location);
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
		if (!dialogueHandler.dialogueIsBeingShown()) {
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
					movingForward = true;
					movingRight = false;
					movingLeft = false;
					movingBackward = false;
					movingUp = false;
					movingDown = false;
					break;
				case SDLK_a:
					pressingLeft = true;
					movingLeft = true;
					movingRight = false;
					movingForward = false;
					movingBackward = false;
					movingUp = false;
					movingDown = false;
					break;
				case SDLK_s:
					pressingBackward = true;
					movingBackward = true;
					movingRight = false;
					movingForward = false;
					movingLeft = false;
					movingUp = false;
					movingDown = false;
					break;
				case SDLK_LEFT:
					if (characterTracker->isPlayerDead() || pressingTileSelectLeft || pressingTileSelectRight) {
						break;
					}
					tileSelectUpAction();
					pressingTileSelectLeft = true;
					break;
				case SDLK_RIGHT: {
					if (characterTracker->isPlayerDead() || pressingTileSelectLeft || pressingTileSelectRight) {
						break;
					}
					tileSelectDownAction();
					pressingTileSelectRight = true;
					break;
				}
				case SDLK_UP: {
					movingUp = true;
					movingDown = false;
					movingRight = false;
					movingForward = false;
					movingLeft = false;
					movingBackward = false;
					break;
				}	
				case SDLK_DOWN:
					movingUp = false;
					movingDown = true;
					movingRight = false;
					movingForward = false;
					movingLeft = false;
					movingBackward = false;
					break;
				case SDLK_LSHIFT:
					if (!playerCharacter->getRunning()) {
						playerCharacter->setRunning(true);
						changePlayerAnimationToRunning();
					}
					break;
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
			case SDLK_UP:
				movingUp = false;
				break;
			case SDLK_DOWN:
				movingDown = false;
				break;
			case SDLK_BACKSLASH:
				pressingTileSwitch = false;
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
		}
	}
}

void Game::AelaGame::setupScripts() {
	// Add vital scripts here!
	scriptManager.addScript("load materials", std::bind(&loadMaterials, resourceManager));
	scriptManager.addScript("load models", std::bind(&loadModels, resourceManager));
	scriptManager.addScript("load textures", std::bind(&loadTextures, resourceManager));
	scriptManager.addScript("load sprite sheets", std::bind(&loadSpriteSheets, resourceManager));
	scriptManager.addScript("load particles", std::bind(&loadParticles, resourceManager));
	scriptManager.addScript("load skyboxes", std::bind(&loadSkyboxes, resourceManager));
	scriptManager.addScript("load tiled maps", std::bind(&loadTiledMaps, resourceManager, &worldManager));
	scriptManager.addScript("load startup map", std::bind(&loadStartupMap, resourceManager, renderer));
	scriptManager.addScript("load scenes", std::bind(&setupScenes, engine, this));
}

void Game::AelaGame::tileSelectUpAction() {
	if (animator->get2DTrack("selector_box_movement") == nullptr) {
		player.getTileInventory()->increaseCurrentTileIfPossible();
		tileInventoryDisplay.animateSelectorBox();
	}
	tileInventoryDisplay.refreshSubMenu();
	timeAtLastTileSelect = time->getCurrentTimeInNanos();
}

void Game::AelaGame::tileSelectDownAction() {
	if (animator->get2DTrack("selector_box_movement") == nullptr) {
		player.getTileInventory()->decreaseCurrentTileIfPossible();
		tileInventoryDisplay.animateSelectorBox();
	}
	tileInventoryDisplay.refreshSubMenu();
	timeAtLastTileSelect = time->getCurrentTimeInNanos();
}

void Game::AelaGame::changePlayerAnimationToRunning() {
	AnimationTrack3D* playerTrack = animator->get3DTrack(playerCharacter->getName() + "_mv");
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

void Game::AelaGame::addTileSwitchParticleEmitter(Location* location, GLTexture* texture) {
	glm::vec3 worldSpacePosition = location->getWorldSpaceLocation();
	TileSwitchParticleEmitter* particleEmitter = new TileSwitchParticleEmitter(time);
	particleEmitter->setBaseDistance(worldSpacePosition.y + 10);
	particleEmitter->setBaseSpeed(0.00000002f);
	particleEmitter->setPathOffset(worldSpacePosition.y);
	particleEmitter->setLifeTime(500000000);
	particleEmitter->setupDimensions(&Rect<float>(worldSpacePosition.x, worldSpacePosition.z, 1, 1));
	
	std::vector<GLTexture*> textures;
	textures.push_back(texture);

	particleEmitter->setupParticles(&textures, 1, 1, 1);
	gameplayScene->putParticleEmitter(particleEmitter);
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
	Location* playerLocation = playerCharacter->getLocation();
	glm::vec3 tile = playerLocation->getTile();
	glm::vec2 chunk = playerLocation->getChunk();
	World* worldPtr = worldManager.getWorld(playerLocation->getWorld());
	Chunk* chunkPtr = worldPtr->getChunk(chunk);
	if (chunkPtr == nullptr) {
		worldManager.createChunkInCurrentWorld(chunk);
		chunkPtr = worldPtr->getChunk(chunk);
	}

	Tile* tilePtr = chunkPtr->getTile(tile);
	if (tilePtr == nullptr) {
		worldManager.createLayerInCurrentWorld(chunk, tile.y);
		tilePtr = chunkPtr->getTile(tile);
	}

	//if (worldManager.getTileAtlas()->getTileType(tilePtr->getType())->getBehaviour() != TileBehaviour::FLOOR
	//	/*|| tilePtr->getType() == 0 || tilePtr->getType() == player.getTileInventory()->getCurrentTile()->getType()*/) {
	//	return false;
	//}

	player.getTileInventory()->replaceTile(tilePtr);
	// player.getTileInventory()->switchCurrentTile(tilePtr);
	tileInventoryDisplay.refreshSubMenu();
	worldManager.rebuildMapWhenPossible();
	return true;
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

			camera->setPosition(*playerCharacter->getEntity()->getPosition() + glm::vec3(0, sin(PLAYER_CAMERA_ANGLE)
				* PLAYER_CAMERA_DISTANCE, -cos(PLAYER_CAMERA_ANGLE) * PLAYER_CAMERA_DISTANCE));
			camera->setRotation(0, -PLAYER_CAMERA_ANGLE, 0);
			break;
		case INVENTORY_SCENE:
			engine->getWindow()->hideCursor();
			break;
		case PAUSE_SCENE:
			engine->getWindow()->showCursor();
			break;
	}
	currentScene = sceneID; 
}

Game::WorldManager* Game::AelaGame::getWorldManager() {
	return &worldManager;
}

Game::EnemyRegistrar* Game::AelaGame::getEnemyRegistrar() {
	return &enemyRegistrar;
}

Game::ScriptManager* Game::AelaGame::getScriptManager() {
	return &scriptManager;
}

Game::DialogueHandler* Game::AelaGame::getDialogueHandler() {
	return &dialogueHandler;
}

void Game::AelaGame::setTileInventoryMenuItems(std::shared_ptr<SubMenu> tileInventorySubMenu,
	std::shared_ptr<Label> tileInventoryLabel, std::shared_ptr<ImageComponent> tileInventoryBoxImage) {
	tileInventoryDisplay.setMenuItems(tileInventorySubMenu, tileInventoryLabel, tileInventoryBoxImage);
}

void Game::AelaGame::setGameplayScene(Scene* gameplayScene) {
	this->gameplayScene = gameplayScene;
	worldManager.setGameplayScene(gameplayScene);
	enemyRegistrar.setGameplayScene(gameplayScene);
}

void Game::AelaGame::cleanup() {
	unloadResources(resourceManager);
}