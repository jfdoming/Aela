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

	GameScripts::runStartingScripts();

	// Tell the character manager that we've added all of our characters!
	characterManager->generateCharacterModels(resourceManager);

	// Setup the world manager!
	worldManager.setup(resourceManager, renderer, animator, camera, &scriptManager, &dialogueHandler, player.getCharacter());

	// Setup the dialogue handler!
	dialogueHandler.setup(timeManager, eventHandler, &scriptManager);

	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(AelaGame::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(AelaGame::onEvent, this));
}

void Game::AelaGame::update() {
	if (currentScene == WORLD_GAMEPLAY_SCENE) {
		if (!player.isMoving()) {
			if (movingRight) {
				player.moveIfPossible(TileDirection::RIGHT);
			}
			if (movingForward) {
				player.moveIfPossible(TileDirection::FORWARD);
			}
			if (movingLeft) {
				player.moveIfPossible(TileDirection::LEFT);
			}
			if (movingBackward) {
				player.moveIfPossible(TileDirection::BACKWARD);
			}
		}

		worldManager.update();
		dialogueHandler.update();
	}
}

void Game::AelaGame::onEvent(Event* event) {
	if (sceneManager->getCurrentSceneId() == WORLD_GAMEPLAY_SCENE) {
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
				}
			} else if (event->getType() == EventConstants::KEY_RELEASED) {
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
					case SDLK_RETURN:
						Location* playerLocation = player.getCharacter()->getLocation();
						glm::vec3 tile = playerLocation->getTile();
						switch (player.getDirectionFacing()) {
							case TileDirection::RIGHT:
								tile += glm::ivec3(-1, 0, 0);
								break;
							case TileDirection::FORWARD:
								tile += glm::ivec3(0, 0, 1);
								break;
							case TileDirection::LEFT:
								tile += glm::ivec3(1, 0, 0);
								break;
							case TileDirection::BACKWARD:
								tile += glm::ivec3(0, 0, -1);
								break;
						}
						Location location(playerLocation->getWorld(), playerLocation->getChunk(), tile);
						worldManager.runPromptedScriptOfTile(&location);
						break;
				}
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

void Game::AelaGame::cleanup() {
	unloadResources(resourceManager);
}