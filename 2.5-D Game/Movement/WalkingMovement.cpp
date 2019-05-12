#include "WalkingMovement.h"
#include "../Character/Character.h"

Game::WalkingMovement::WalkingMovement(float magnitude, TileDirection direction, bool collisionCheck) {
	this->magnitude = magnitude;
	this->direction = direction;
	this->collisionCheck = collisionCheck;
}

Game::WalkingMovement::WalkingMovement(float magnitude, TileDirection direction, bool collisionCheck, bool running) : WalkingMovement(magnitude, direction, collisionCheck) {
	this->running = running;
}

Game::WalkingMovement::~WalkingMovement() {
}

bool Game::WalkingMovement::moveCharacter(Character* character) {
	float amount = (GameObjectProvider::getTime()->getTimeBetweenFramesInMillis()) * character->getCurrentSpeed();
	return translate(character, amount);
}

void Game::WalkingMovement::start(Character* character) {
	started = true;

	if (direction == TileDirection::NONE) {
		magnitude = 0;
	} else {
		previousRunning = character->isRunning();
		character->setRunning(running);
		if (direction != TileDirection::UP && direction != TileDirection::DOWN) {
			character->turn(direction);
		}
	}
}

bool Game::WalkingMovement::translate(Character* character, float amount) {
	bool finished = false;

	if (distanceTravelled + amount > magnitude) {
		amount = magnitude - distanceTravelled;
		finished = true;
		onEnd(character);
	}

	if (amount == 0) {
		return finished;
	}

	glm::vec3 translation;
	switch (direction) {
		case TileDirection::RIGHT:
			translation.x -= amount;
			break;
		case TileDirection::FORWARD:
			translation.z += amount;
			break;
		case TileDirection::LEFT:
			translation.x += amount;
			break;
		case TileDirection::BACKWARD:
			translation.z -= amount;
			break;
		case TileDirection::UP:
			translation.y += amount;
			break;
		case TileDirection::DOWN:
			translation.y -= amount;
			break;
		default:
			return finished;
	}

	character->translate(translation);
	distanceTravelled += amount;
	return finished;
}

void Game::WalkingMovement::onEnd(Character* character) {
	character->setRunning(previousRunning);
}
