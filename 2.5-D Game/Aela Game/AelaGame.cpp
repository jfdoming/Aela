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

using namespace Aela;

Game::AelaGame::AelaGame(Engine* engine) : engine(engine), tileInventoryDisplay(engine) {
	// This just sets properties.
	window = engine->getWindow();
	renderer = engine->getRenderer();
	eventHandler = engine->getEventHandler();
	timeManager = engine->getTime();
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
}

void Game::AelaGame::loadResources() {
	scriptManager.runScript("load materials");
	scriptManager.runScript("load models");
	scriptManager.runScript("load textures");
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

	characterManager = worldManager.getCharacterManager();

	// Setup the player!
	Character temporaryPlayerCharacter;
	temporaryPlayerCharacter.setup(&Location(0, glm::ivec2(0, 0), glm::ivec3(15, 0, 1)));
	temporaryPlayerCharacter.setTextureName("character");
	temporaryPlayerCharacter.setName(PLAYER_NAME);
	size_t playerID;
	if (!characterManager->addCharacter(&temporaryPlayerCharacter, &playerID)) {
		// Is this even possible to reach?!
		AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up the player. This error "
			+ "is supposed to be impossible to reach.");
	}

	for (int i = 0; i < 3; i++) {
		player.getTileInventory()->addTile(&Tile(1));
	}

	GameScripts::runStartingScripts();

	tileInventoryDisplay.setGameObjects(&player, &worldManager);
	tileInventoryDisplay.refreshSubMenu();

	// Tell the character manager that we've added all of our characters!
	characterManager->generateCharacterModels(resourceManager);

	// Setup the world manager!
	worldManager.setup(engine, &scriptManager, &dialogueHandler, playerCharacter);

	// Setup the dialogue handler!
	dialogueHandler.setup(timeManager, eventHandler, &scriptManager);

	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(AelaGame::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(AelaGame::onEvent, this));

	playerCharacter = characterManager->getCharacterByID(playerID);
	player.setCharacter(playerCharacter);
	player.setCharacterID(playerID);
}

void Game::AelaGame::update() {
	// std::cout << "-------------Updating Aela Game-------------\n";
	// std::cout << player.isMoving() << " " << movingRight << " is isMoving & movingRight.\n";
	if (currentScene == WORLD_GAMEPLAY_SCENE) {
		glm::vec3 tile = playerCharacter->getLocation()->getTile();

		if (!playerCharacter->isMoving() && !dialogueHandler.dialogueIsBeingShown()) {
			if (movingRight) {
				std::cout << (playerCharacter->getDirectionFacing() != TileDirection::RIGHT) << " " << !playerCharacter->animationHasJustEnded() << "\n";
				if (playerCharacter->getDirectionFacing() != TileDirection::RIGHT && !playerCharacter->animationHasJustEnded()) {
					characterManager->turn(playerCharacter, TileDirection::RIGHT);
					timeAtLastPlayerTurn = timeManager->getCurrentTimeInNanos();
				} else if (timeManager->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
					// std::cout << "Moving if possible.\n";
					worldManager.moveCharacterIfPossible(player.getCharacterID(), TileDirection::RIGHT);
				}
			}
			if (movingForward) {
				if (playerCharacter->getDirectionFacing() != TileDirection::FORWARD && !playerCharacter->animationHasJustEnded()) {
					characterManager->turn(playerCharacter, TileDirection::FORWARD);
					timeAtLastPlayerTurn = timeManager->getCurrentTimeInNanos();
				} else if (timeManager->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
					worldManager.moveCharacterIfPossible(player.getCharacterID(), TileDirection::FORWARD);
				}
			}
			if (movingLeft) {
				if (playerCharacter->getDirectionFacing() != TileDirection::LEFT && !playerCharacter->animationHasJustEnded()) {
					characterManager->turn(playerCharacter, TileDirection::LEFT);
					timeAtLastPlayerTurn = timeManager->getCurrentTimeInNanos();
				} else if (timeManager->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
					worldManager.moveCharacterIfPossible(player.getCharacterID(), TileDirection::LEFT);
				}
			}
			if (movingBackward) {
				if (playerCharacter->getDirectionFacing() != TileDirection::BACKWARD && !playerCharacter->animationHasJustEnded()) {
					characterManager->turn(playerCharacter, TileDirection::BACKWARD);
					timeAtLastPlayerTurn = timeManager->getCurrentTimeInNanos();
				} else if (timeManager->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
					worldManager.moveCharacterIfPossible(player.getCharacterID(), TileDirection::BACKWARD);
				}
			}
			if (pressingTileSelectUp && timeManager->getCurrentTimeInNanos() > timeAtLastTileSelect + timeBetweenTileSelects) {
				tileSelectUpAction();
			}
			if (pressingTileSelectDown && timeManager->getCurrentTimeInNanos() > timeAtLastTileSelect + timeBetweenTileSelects) {
				tileSelectDownAction();
			}
		}

		worldManager.update();
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
					case SDLK_d:
						pressingRight = true;
						movingRight = true;
						movingForward = false;
						movingLeft = false;
						movingBackward = false;
						break;
					case SDLK_w:
						pressingForward = true;
						movingRight = false;
						movingForward = true;
						movingLeft = false;
						movingBackward = false;
						break;
					case SDLK_a:
						pressingLeft = true;
						movingRight = false;
						movingForward = false;
						movingLeft = true;
						movingBackward = false;
						break;
					case SDLK_s:
						pressingBackward = true;
						movingRight = false;
						movingForward = false;
						movingLeft = false;
						movingBackward = true;
						break;
					case SDLK_UP:
						if (pressingTileSelectUp || pressingTileSelectDown) {
							break;
						}
						tileSelectUpAction();
						pressingTileSelectUp = true;
						break;
					case SDLK_DOWN:
						if (pressingTileSelectUp || pressingTileSelectDown) {
							break;
						}
						tileSelectDownAction();
						pressingTileSelectDown = true;
						break;
					case SDLK_BACKSLASH:
						if (!pressingTileSwitch) {
							Location* playerLocation = playerCharacter->getLocation();
							glm::vec3 tile = playerLocation->getTile();
							glm::vec2 chunk = playerLocation->getChunk();
							worldManager.getCoordinateOfNeighbouringTile(tile, chunk, playerCharacter->getDirectionFacing());
							Location location(playerLocation->getWorld(), chunk, tile);
							World* worldPtr = worldManager.getWorld(playerLocation->getWorld());
							if (worldPtr == nullptr) {
								break;
							}
							Chunk* chunkPtr = worldPtr->getChunk(chunk);
							if (chunkPtr == nullptr) {
								break;
							}
							Tile* tilePtr = worldPtr->getChunk(chunk)->getTile(tile);
							if (tilePtr == nullptr) {
								break;
							}
							player.getTileInventory()->switchCurrentTile(tilePtr);
							tileInventoryDisplay.refreshSubMenu();
							worldManager.rebuildMapWhenPossible();
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
					case SDLK_LSHIFT:
						if (!playerCharacter->getRunning()) {
							playerCharacter->setRunning(true);
							changePlayerAnimationToRunning();
						}
						break;
				}
			}
		}

		// This processes key released events, no matter if the player is in dialogue or not.
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
				case SDLK_UP: {
					pressingTileSelectUp = false;
					break;
				}
				case SDLK_DOWN: {
					pressingTileSelectDown = false;
					break;
				}
				case SDLK_RETURN:
					pressingReturn = false;
					break;
				case SDLK_LSHIFT:
					playerCharacter->setRunning(false);
					break;
			}
		}
	}
}

void Game::AelaGame::setupScripts() {
	// Add vital scripts here!
	scriptManager.addScript("load materials", std::bind(&loadMaterials, resourceManager));
	scriptManager.addScript("load models", std::bind(&loadModels, resourceManager));
	scriptManager.addScript("load textures", std::bind(&loadTextures, resourceManager));
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
	timeAtLastTileSelect = timeManager->getCurrentTimeInNanos();
}

void Game::AelaGame::tileSelectDownAction() {
	if (animator->get2DTrack("selector_box_movement") == nullptr) {
		player.getTileInventory()->decreaseCurrentTileIfPossible();
		tileInventoryDisplay.animateSelectorBox();
	}
	tileInventoryDisplay.refreshSubMenu();
	timeAtLastTileSelect = timeManager->getCurrentTimeInNanos();
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

			camera->setPosition(*playerCharacter->getEntity()->getPosition() + glm::vec3(0, sin(angleBetweenPlayerAndCamera)
				* distanceBetweenPlayerAndCamera, -cos(angleBetweenPlayerAndCamera) * distanceBetweenPlayerAndCamera));
			camera->setRotation(0, -angleBetweenPlayerAndCamera, 0);
			break;
	}
	currentScene = sceneID;
}

Game::WorldManager* Game::AelaGame::getWorldManager() {
	return &worldManager;
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

void Game::AelaGame::cleanup() {
	unloadResources(resourceManager);
}