#include "Lock.h"
#include <iostream>

Game::Lock::Lock() {}

Game::Lock::Lock(bool locked) {
	this->locked = locked;
}

Game::Lock::~Lock() {
	std::cout << "DELETING... lock\n";
}

bool Game::Lock::isLocked() {
	return locked;
}
