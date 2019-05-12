#include "Movement.h"
#include "../Character/Character.h"

using namespace Game;

Movement::Movement() {
}

Movement::~Movement() {
}

bool Movement::moveCharacter(Character* character) {
	return false;
}

bool Movement::isStarted() {
	return started;
}

void Movement::start(Character* character) {
	started = true;
}

void Movement::setDirection(TileDirection direction) {
	this->direction = direction;
}

TileDirection Movement::getDirection() {
	return direction;
}
