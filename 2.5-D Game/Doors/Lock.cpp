#include "Lock.h"

Game::Lock::Lock() {}

Game::Lock::Lock(bool locked) {
	this->locked = locked;
}

void Game::Lock::setLocked(bool locked) {
	this->locked = locked;
}

bool Game::Lock::isLocked() {
	return locked;
}
