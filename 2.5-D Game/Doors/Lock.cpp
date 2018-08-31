#include "Lock.h"

Game::Lock::Lock() = default;

Game::Lock::Lock(bool locked) {
	this->locked = locked;
}

Game::Lock::~Lock() = default;

bool Game::Lock::isLocked() {
	return locked;
}
