#include "Door.h"
#include <cstdarg>
#include <iostream>

Game::Door::Door() {
	open = true;
}

Game::Door::Door(int lockAmount, ...) {
	addLocks(lockAmount, locks);
}

size_t Game::Door::addLock() {
	return addLock(true);
}

size_t Game::Door::addLock(bool locked) {
	// Why is VS giving a warning about forcing a bool to a bool (performance warning)?
	locks.push_back(&Lock(locked));

	if (locked) {
		open = false;
		onClose();
	}

	return locks.size() - 1;
}

void Game::Door::addLocks(int lockAmount, ...) {
	va_list args;
	va_start(args, lockAmount);

	for (int i = 0; i < lockAmount; i++) {
		addLock(va_arg(args, bool));
	}

	va_end(args);
}

bool Game::Door::unlock(size_t lockID) {
	if (lockID >= locks.size()) {
		return false;
	}

	locks[lockID].setLocked(false);

	bool containsLockedLock = false;
	for (Lock lock : locks) {
		if (lock.isLocked()) {
			containsLockedLock = true;
		}
	}

	std::cout << "HERE: " << open << " " << containsLockedLock << "\n";

	if (!open && !containsLockedLock) {
		open = true;
		onOpen();
	}

	return true;
}

bool Game::Door::lock(size_t lockID) {
	if (lockID >= locks.size()) {
		return false;
	}

	locks[lockID].setLocked(true);

	if (open) {
		open = false;
		onClose();
	}
	return true;
}

void Game::Door::setOnOpen(std::function<void()>* onOpen) {
	this->onOpen = *onOpen;
}

void Game::Door::setOnClose(std::function<void()>* onClose) {
	this->onClose = *onClose;
}
