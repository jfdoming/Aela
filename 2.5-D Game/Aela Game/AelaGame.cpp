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
		case WORLD_GAMEPLAY_SCENE:
			// First person camera:
			engine->getWindow()->hideCursor();
			camera->setInUse(true);
			camera->setForceCursorToMiddle(false);
			camera->useControls(false);
			engine->getRenderer()->getCamera()->setRotation(0, -angleBetweenPlayerAndCamera, 0);
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

void Game::AelaGame::setup() {
	setupScripts();

	// These basically just run the scripts.
	loadResources();
	loadScenes();

	// Setup the scripts of the game!
	GameScripts::setupGameScripts(this, engine);

	engine->getRendererReference().activateFeature(RendererFeature::MSAA_2D_X4);
	engine->getRendererReference().activateFeature(RendererFeature::MSAA_3D_X4);

	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(AelaGame::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(AelaGame::onEvent, this));

	characterManager = worldManager.getCharacterManager();

	// Setup the player!
	Character playerCharacter;
	playerCharacter.setLocation(&Location(0, glm::ivec2(0, 0), glm::ivec3(15, 0, 1)));
	playerCharacter.setTextureNames("character_right", "character_forward", "character_left", "character_backward");
	playerCharacter.setName("player");
	unsigned long long playerID;
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
	worldManager.setup(resourceManager, renderer, animator, &scriptManager, player.getCharacter());

	camera->setPosition(*player.getCharacter()->getEntity()->getPosition() + glm::vec3(0, sin(angleBetweenPlayerAndCamera) * distanceBetweenPlayerAndCamera,
		-cos(angleBetweenPlayerAndCamera) * distanceBetweenPlayerAndCamera));
}

void Game::AelaGame::update() {
	if (currentScene == WORLD_GAMEPLAY_SCENE) {
		if (!player.isMoving()) {
			if (movingRight && player.moveIfPossible(TileDirection::RIGHT)) {
				animateCamera(glm::vec3(-1, 0, 0));
			}
			if (movingForward && player.moveIfPossible(TileDirection::FORWARD)) {
				animateCamera(glm::vec3(0, 0, 1));
			}
			if (movingLeft && player.moveIfPossible(TileDirection::LEFT)) {
				animateCamera(glm::vec3(1, 0, 0));
			}
			if (movingBackward && player.moveIfPossible(TileDirection::BACKWARD)) {
				animateCamera(glm::vec3(0, 0, -1));
			}
		} else if (!animator->trackWithTagExists("player_movement")) {
			player.stopMoving();
		}

		worldManager.update();
	}
}

void Game::AelaGame::onEvent(Event* event) {
	if (sceneManager->getCurrentSceneId() == WORLD_GAMEPLAY_SCENE) {
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
			}
		}
	}
}

void Game::AelaGame::animateCamera(glm::vec3 translation) {
	glm::vec3 cameraTrans = *camera->getPosition() + translation;

	AnimationTrack3D track;
	track.setTag("camera_movement");
	KeyFrame3D frame;
	frame.setObject(camera);
	frame.setTranslation(&cameraTrans);
	track.addKeyFrame((unsigned long long) (1000000.0f / player.getCharacter()->getWalkingSpeed()), &frame);
	animator->addAnimationTrack3D(&track);
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