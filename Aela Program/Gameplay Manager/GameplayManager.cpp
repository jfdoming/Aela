/*
* Class: Gameplay Manager
* Author: Robert Ciborowski
* Date: 28/08/2017
* Description: A class used to keep track of gameplay-related properties.
*/


#include "GameplayManager.h"

#define EKKON_INTRO_SCENE 1
#define MAIN_MENU_SCENE 2
#define GAMEPLAY_SCENE 3

void GameplayManager::setup() {
	if (currentMap != nullptr) {
		ModelEntity playerEntity;
		Model* playerModel;
		bool success = resourceManager->obtain<Model>("res/models/meme_mug.obj", playerModel);
		if (success) {
			playerEntity.setModel(playerModel);
		}

		// Note: this function copies, so now we should get a pointer to the player entity.
		currentMap->addModel(0, &playerEntity);
		player.storeEntityIDInMap(0);
		ModelEntity* mapPlayerEntity = currentMap->getModel(0);
		player.setEntity(mapPlayerEntity);
		player.getEntity()->getPosition()->y = 0;
		player.getEntity()->generateBoundingBox();

		camera->setPosition(0, 10, -10);
		camera->setRotation(0, glm::pi<float>() / -6, 0);
		engine->getWindow()->hideCursor();
		camera->setInUse(true);
	}

	// TODO refactor the onEvent function to separate functions specific to certain events (see menus for style)
	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(GameplayManager::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(GameplayManager::onEvent, this));
}

void GameplayManager::update() {
	switch (sceneManager->getCurrentSceneId()) {
		case GAMEPLAY_SCENE:
			updatePlayerMovements();
			break;
	}

	previousMovingLeft = movingLeft;
	previousMovingRight = movingRight;
	previousMovingUp = movingUp;
	previousMovingDown = movingDown;
}

void GameplayManager::updatePlayerMovements() {
	// Note: movement of the player in third person will work somewhat differently and in a more realistic manner, but this
	// movement is also good.
	std::vector<float> directions;
	if (movingDown) {
		if (movingRight) {
			directions.push_back(0);
		} else {
			directions.push_back(glm::pi<float>() * 2);
		}
	}
	if (movingLeft) {
		directions.push_back(glm::pi<float>() * 3 / 2);
	}
	if (movingUp) {
		directions.push_back(glm::pi<float>());
	}
	if (movingRight) {
		directions.push_back(glm::pi<float>() / 2);
	}
	float rotation = 0;
	for (float d : directions) {
		rotation += d;
	}
	glm::vec3 rotationVector = *player.getEntity()->getRotation();
	if (directions.size() != 0) {
		rotation /= directions.size();
		directionToMovePlayerTowards = rotation;
	} else {
		directionToMovePlayerTowards = rotationVector.y;
	}
	if (rotationVector.y - directionToMovePlayerTowards > glm::pi<float>()) {
		rotationVector.y = directionToMovePlayerTowards + glm::pi<float>() * 2;
	} else if (rotationVector.y - directionToMovePlayerTowards < -glm::pi<float>()) {
		rotationVector.y = directionToMovePlayerTowards - glm::pi<float>() * 2;
	} else {
		rotationVector.y = directionToMovePlayerTowards;
	}
	glm::vec3 newRotation = *player.getEntity()->getRotation();
	if (rotationVector.y > newRotation.y) {
		newRotation.y += player.getRotationSpeed() * timeManager->getTimeBetweenFramesInNanos();
		if (rotationVector.y < newRotation.y) {
			newRotation.y = rotationVector.y;
		}
	} else if (rotationVector.y < newRotation.y) {
		newRotation.y -= player.getRotationSpeed() * timeManager->getTimeBetweenFramesInNanos();
		if (rotationVector.y > newRotation.y) {
			newRotation.y = rotationVector.y;
		}
	}

	player.getEntity()->setRotation(newRotation);

	// Since physics calculations are performed using the bounding box that is bound to the player's ModelEntity, this modifies
	// that bounding box but keeps a copy of the original just in case the physics calculations detect a collision and does
	// not allow the player to moveSimple.
	BoundingBox3D originalBox = *player.getEntity()->getBoundingBox();
	BoundingBox3D* playerBox = player.getEntity()->getBoundingBox();
	if (movingLeft) {
		playerBox->getPosition()->x += player.getWalkingSpeed() * timeManager->getTimeBetweenFramesInNanos();
	}
	if (movingRight) {
		playerBox->getPosition()->x -= player.getWalkingSpeed() * timeManager->getTimeBetweenFramesInNanos();
	}
	if (movingUp) {
		playerBox->getPosition()->z += player.getWalkingSpeed() * timeManager->getTimeBetweenFramesInNanos();
	}
	if (movingDown) {
		playerBox->getPosition()->z -= player.getWalkingSpeed() * timeManager->getTimeBetweenFramesInNanos();
	}
	player.getEntity()->getBoundingBox()->setPosition(*playerBox->getPosition());
	player.getEntity()->getBoundingBox()->generateVertices();

	// This literally just checks to see if the player's bounding box is inside of another model's bounding box. If the player
	// is moving fast enough, they could moveSimple past another model's bounding box.
	// Actually, for now, forget collision.
	// if (!physicsManager->collidingInMap(player.getEntityIDInMap(), currentMap)) {
		// There was not a collision! Move the player!
		player.getEntity()->setPosition(*playerBox->getPosition());
	/*} else {
		// There was a collision! Reset the player's bounding box to what it was before!
		player.getEntity()->getBoundingBox()->setPosition(*originalBox.getPosition());
		player.getEntity()->getBoundingBox()->setPosition(*originalBox.getPosition());
		player.getEntity()->getBoundingBox()->generateVertices();
	}*/
	camera->getPosition()->x = player.getEntity()->getPosition()->x;
	camera->getPosition()->y = player.getEntity()->getPosition()->y + 8.6603f;
	camera->getPosition()->z = player.getEntity()->getPosition()->z - 15;
}

void GameplayManager::onEvent(Event* event) {
	if (sceneManager->getCurrentSceneId() == GAMEPLAY_SCENE) {
		if (event->getType() == EventConstants::KEY_PRESSED) {
			KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
				case SDLK_a:
					movingLeft = true;
					movingRight = false;
					break;
				case SDLK_d:
					movingRight = true;
					movingLeft = false;
					break;
				case SDLK_w:
					movingUp = true;
					movingDown = false;
					break;
				case SDLK_s:
					movingDown = true;
					movingUp = false;
					break;
			}
		} else if (event->getType() == EventConstants::KEY_RELEASED) {
			KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
			case SDLK_a:
				movingLeft = false;
				break;
			case SDLK_d:
				movingRight = false;
				break;
			case SDLK_w:
				movingUp = false;
				break;
			case SDLK_s:
				movingDown = false;
				break;
			}
		}
	}
}

Player* GameplayManager::getPlayer() {
	return &player;
}

CharacterManager* GameplayManager::getNPCManager() {
	return &characterManager;
}

void GameplayManager::setCurrentMap(Map3D* map) {
	currentMap = map;
}
