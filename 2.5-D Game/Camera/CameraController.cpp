#include "CameraController.h"
#include "../Player/Player.h"
#include "../Utilities/GameConstants.h"

Game::CameraController::CameraController() {
	camera = GameObjectProvider::getCamera();
}

void Game::CameraController::setup() {
	player = GameObjectProvider::getPlayer();
	playerCharacter = GameObjectProvider::getPlayer()->getCharacter();
}

void Game::CameraController::update() {
	if (!camera->isUsingKeyboardControls() && lockCameraToPlayer && GameObjectProvider::getPlayer()->isAlive()) {
		// The camera has not been taken over by the user using a cheat-code.
		glm::vec3 entityPosition = *playerCharacter->getEntity()->getPosition();
		camera->setPosition(entityPosition + glm::vec3(0, sin(PLAYER_CAMERA_ANGLE)
			* PLAYER_CAMERA_DISTANCE, -cos(PLAYER_CAMERA_ANGLE) * PLAYER_CAMERA_DISTANCE));
		camera->setRotation(0, -PLAYER_CAMERA_ANGLE, 0);
	}
}

void Game::CameraController::setLockCameraToPlayer(bool lockCameraToPlayer) {
	this->lockCameraToPlayer = lockCameraToPlayer;
}