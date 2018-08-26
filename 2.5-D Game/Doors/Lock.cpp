#include "Lock.h"
#include <iostream>

Game::Lock::Lock() {}

Game::Lock::Lock(bool locked) {
	this->locked = locked;
}

Game::Lock::~Lock() {}

bool Game::Lock::isLocked() {
	return locked;
}
