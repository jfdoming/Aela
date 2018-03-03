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

using namespace Aela;

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
	Character playerCharacter;
	playerCharacter.setup(&Location(0, glm::ivec2(0, 0), glm::ivec3(15, 0, 1)));
	playerCharacter.setTextureNames("character_right", "character_forward", "character_left", "character_backward");
	playerCharacter.setName(PLAYER_NAME);
	size_t playerID;
	if (!characterManager->addCharacter(&playerCharacter, &playerID)) {
		// Is this even possible to reach?!
		AelaErrorHandling::windowError("Aela Game", (std::string) "There was a problem setting up the player. This error "
			+ "is supposed to be impossible to reach.");
	}
	player.setCharacterID(playerID);

	for (int i = 0; i < 3; i++) {
		player.getTileInventory()->addTile(&Tile(1));
	}

	GameScripts::runStartingScripts();

	refreshTileInventorySubMenu();

	// Tell the character manager that we've added all of our characters!
	characterManager->generateCharacterModels(resourceManager);

	// Setup the world manager!
	worldManager.setup(resourceManager, renderer, animator, camera, timeManager, &scriptManager, &dialogueHandler, player.getCharacter());

	// Setup the dialogue handler!
	dialogueHandler.setup(timeManager, eventHandler, &scriptManager);

	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(AelaGame::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(AelaGame::onEvent, this));
}

void Game::AelaGame::update() {
	// std::cout << "-------------Updating Aela Game-------------\n";
	// std::cout << player.isMoving() << " " << movingRight << " is isMoving & movingRight.\n";
	if (currentScene == WORLD_GAMEPLAY_SCENE) {
		glm::vec3 tile = player.getCharacter()->getLocation()->getTile();

		if (!player.isMoving() && !dialogueHandler.dialogueIsBeingShown()) {
			if (movingRight) {
				if (player.getDirectionFacing() != TileDirection::RIGHT) {
					player.turn(TileDirection::RIGHT);
					timeAtLastPlayerTurn = timeManager->getCurrentTimeInNanos();
				} else if (timeManager->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
					// std::cout << "Moving if possible.\n";
					player.moveIfPossible(TileDirection::RIGHT);
				}
			}
			if (movingForward) {
				if (player.getDirectionFacing() != TileDirection::FORWARD) {
					player.turn(TileDirection::FORWARD);
					timeAtLastPlayerTurn = timeManager->getCurrentTimeInNanos();
				} else if (timeManager->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
					player.moveIfPossible(TileDirection::FORWARD);
				}
			}
			if (movingLeft) {
				if (player.getDirectionFacing() != TileDirection::LEFT) {
					player.turn(TileDirection::LEFT);
					timeAtLastPlayerTurn = timeManager->getCurrentTimeInNanos();
				} else if (timeManager->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
					player.moveIfPossible(TileDirection::LEFT);
				}
			}
			if (movingBackward) {
				if (player.getDirectionFacing() != TileDirection::BACKWARD) {
					player.turn(TileDirection::BACKWARD);
					timeAtLastPlayerTurn = timeManager->getCurrentTimeInNanos();
				} else if (timeManager->getCurrentTimeInNanos() >= timeAtLastPlayerTurn + TIME_BETWEEN_PLAYER_TURNS) {
					player.moveIfPossible(TileDirection::BACKWARD);
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
							Location* playerLocation = player.getCharacter()->getLocation();
							glm::vec3 tile = playerLocation->getTile();
							glm::vec2 chunk = playerLocation->getChunk();
							worldManager.getCoordinateOfNeighbouringTile(tile, chunk, player.getDirectionFacing());
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
							refreshTileInventorySubMenu();
							worldManager.rebuildMapWhenPossible();
							pressingTileSwitch = true;
						}
						break;
					case SDLK_RETURN:
						if (!pressingReturn && !dialogueHandler.hadJustFinishedDialogue()) {
							Location* playerLocation = player.getCharacter()->getLocation();
							glm::vec3 tile = playerLocation->getTile();
							glm::vec2 chunk = playerLocation->getChunk();
							worldManager.getCoordinateOfNeighbouringTile(tile, chunk, player.getDirectionFacing());
							Location location(playerLocation->getWorld(), chunk, tile);
							worldManager.runPromptedScriptOfTile(&location);
							pressingReturn = true;
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
				case SDLK_UP:
					pressingTileSelectUp = false;
					break;
				case SDLK_DOWN:
					pressingTileSelectDown = false;
					break;
				case SDLK_RETURN:
					pressingReturn = false;
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

void Game::AelaGame::refreshTileInventorySubMenu() {
	int width = window->getWindowDimensions()->getWidth(), height = window->getWindowDimensions()->getHeight();
	int imageWidthAndHeight = height / 8;
	int currentTile = player.getTileInventory()->getCurrentTileIndex();

	tileInventoryLabel->setText(worldManager.getTileAtlas()->getTileType(player.getTileInventory()->getCurrentTile()->getType())->getName());

	for (size_t i = 0; i < player.getTileInventory()->getNumberOfTiles(); i++) {
		Tile* tile = player.getTileInventory()->getTile(i);

		if (tileInventoryImages.size() == i) {
			auto image = std::make_shared<ImageComponent>();
			GLTexture* texture;

			// Until Aela is designed better (which I don't have the time to do), we must do this to get a GLTexture* (as opposed to getting
			// a Texture* via tile->...->getTexture()). We have to get it from the tile atlas since 
			if (tile->getEntity() == nullptr) {
				resourceManager->obtain<GLTexture>(DEFAULT_MATERIAL_PATH + (std::string) "grass.dds", texture);
			} else if (!resourceManager->obtain<GLTexture>(worldManager.getTileAtlas()->getTileModel(tile->getType())->getSubModels()->at(0).getMaterial()->getTexture()->getSrc(), texture)) {
				break;
			}

			image->setDimensions(&Rect<int>((int) (width * 0.98 - (i * imageWidthAndHeight) - imageWidthAndHeight),
				(int) (height * 0.15 - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight));
			image->setTexture(texture);
			tileInventoryImages.push_back(image);
			tileInventorySubMenu->add(image);
		} else {
			GLTexture* texture;

			// Until Aela is designed better (which I don't have the time to do), we must do this to get a GLTexture* (as opposed to getting
			// a Texture* via tile->...->getTexture()).
			if (tile->getEntity() == nullptr) {
				resourceManager->obtain<GLTexture>(DEFAULT_MATERIAL_PATH + (std::string) "grass.dds", texture);
			} else {
				if (!resourceManager->obtain<GLTexture>(worldManager.getTileAtlas()->getTileModel(tile->getType())->getSubModels()->at(0).getMaterial()->getTexture()->getSrc(), texture)) {
					break;
				}
			}

			tileInventoryImages[i]->setDimensions(&Rect<int>((int) (width * 0.98 - (i * imageWidthAndHeight) - imageWidthAndHeight),
				(int) (height * 0.15 - (imageWidthAndHeight)), imageWidthAndHeight, imageWidthAndHeight));
			tileInventoryImages[i]->setTexture(texture);
		}
	}
}

void Game::AelaGame::animateSelectorBox() {
	AnimationTrack2D track;
	track.setTag("selector_box_movement");
	KeyFrame2D frame;
	frame.setObject(tileInventoryBoxImage);
	Rect<int> dimensions = *tileInventoryBoxImage->getDimensions();
	int imageWidthAndHeight = window->getWindowDimensions()->getHeight() / 8;
	dimensions.setX((int) (window->getWindowDimensions()->getWidth() * 0.98
		- (player.getTileInventory()->getCurrentTileIndex() * imageWidthAndHeight)
		- imageWidthAndHeight - imageWidthAndHeight * 0.1));
	frame.setDimensions(&dimensions);
	track.addKeyFrameUsingMillis((size_t) (TIME_FOR_SELECTOR_TO_MOVE), &frame);
	animator->addAnimationTrack2D(&track);
}

void Game::AelaGame::tileSelectUpAction() {
	if (animator->get2DTrack("selector_box_movement") == nullptr) {
		player.getTileInventory()->increaseCurrentTileIfPossible();
		animateSelectorBox();
	}
	refreshTileInventorySubMenu();
	timeAtLastTileSelect = timeManager->getCurrentTimeInNanos();
}

void Game::AelaGame::tileSelectDownAction() {
	if (animator->get2DTrack("selector_box_movement") == nullptr) {
		player.getTileInventory()->decreaseCurrentTileIfPossible();
		animateSelectorBox();
	}
	refreshTileInventorySubMenu();
	timeAtLastTileSelect = timeManager->getCurrentTimeInNanos();
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
			camera->setPosition(*player.getCharacter()->getEntity()->getPosition() + glm::vec3(0, sin(angleBetweenPlayerAndCamera)
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
	this->tileInventorySubMenu = tileInventorySubMenu;
	this->tileInventoryLabel = tileInventoryLabel;
	this->tileInventoryBoxImage = tileInventoryBoxImage;
}

void Game::AelaGame::cleanup() {
	unloadResources(resourceManager);
}