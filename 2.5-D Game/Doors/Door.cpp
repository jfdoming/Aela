#include "Door.h"
#include <cstdarg>
#include <iostream>

Game::Door::Door() {
	open = true;
}

Game::Door::Door(int lockAmount) {
	for (int i = 0; i < lockAmount; i++) {
		locks.push_back(new Lock(true));
	}

	open = false;
}

Game::Door::Door(std::initializer_list<bool> locked) {
	open = true;

	for (bool lockIsLocked : locked) {
		if (lockIsLocked) {
			open = false;
		}
		addLock(lockIsLocked);
	}
}

void Game::Door::cleanup() {
	for (auto* lock : locks) {
		delete lock;
	}
}

size_t Game::Door::addLock() {
	return addLock(true);
}

size_t Game::Door::addLock(bool locked) {
	// Why is VS giving a warning about forcing a ptr to a bool (performance warning)?
	locks.push_back(new Lock(locked));

	if (locked) {
		open = false;

		if (onClose != nullptr) {
			onClose();
		}
	}

	return locks.size() - 1;
}

size_t Game::Door::addLock(Lock* lock) {
	locks.push_back(lock);

	if (lock->isLocked()) {
		open = false;

		if (onClose != nullptr) {
			onClose();
		}
	} else if (locks.empty()) {
		open = true;
		if (onOpen != nullptr) {
			onOpen();
		}
	}

	return locks.size() - 1;
}

void Game::Door::addLocks(int lockAmount, ...) {
	va_list args;
	va_start(args, lockAmount);

	for (int i = 0; i < lockAmount; i++) {
		addLock((bool) va_arg(args, int));
	}

	va_end(args);
}

size_t Game::Door::addLockWithoutCallingOnClose(bool locked) {
	locks.push_back(new Lock(locked));

	if (locked) {
		open = false;
	}

	return locks.size() - 1;
}

Game::Lock* Game::Door::getLock(size_t id) {
	return locks[id];
}

std::vector<Game::Lock*>* Game::Door::getLocks() {
	return &locks;
}

bool Game::Door::unlock(size_t lockID) {
	if (open) {
		return true;
	}

	if (lockID >= locks.size()) {
		return false;
	}

	locks[lockID]->locked = false;
	bool containsLockedLock = false;

	for (Lock* lock : locks) {
		if (lock->isLocked()) {
			containsLockedLock = true;
		}
	}

	if (!open && !containsLockedLock) {
		open = true;

		if (onOpen != nullptr) {
			onOpen();
		}
	}

	return true;
}

void Game::Door::unlock() {
	if (!open) {
		open = true;

		if (onOpen != nullptr) {
			onOpen();
		}
	}

	for (auto lock : locks) {
		lock->locked = false;
	}
}

bool Game::Door::lock(size_t lockID) {
	if (lockID >= locks.size()) {
		return false;
	}


	locks[lockID]->locked = true;

	if (open) {
		open = false;

		if (onClose != nullptr) {
			onClose();
		}
	}
	return true;
}

void Game::Door::lock() {
	for (auto lock : locks) {
		lock->locked = false;
	}

	if (open) {
		open = false;

		if (onClose != nullptr) {
			onClose();
		}
	}
}

void Game::Door::lockWithoutLockingLocks() {
	if (open) {
		open = false;

		if (onClose != nullptr) {
			onClose();
		}
	}
}

bool Game::Door::isOpen() {
	return open;
}

void Game::Door::setOnOpen(std::function<void()>* onOpen) {
	this->onOpen = *onOpen;
}

std::function<void()>* Game::Door::getOnOpen() {
	return &onOpen;
}

void Game::Door::setOnClose(std::function<void()>* onClose) {
	this->onClose = *onClose;
}

std::function<void()>* Game::Door::getOnClose() {
	return &onClose;
}

void Game::Door::makeBackupOfLockData() {
	copyOfLockData.clear();

	for (auto lock : locks) {
		copyOfLockData.push_back(lock->isLocked());
	}
}

void Game::Door::useBackupOfLockData() {
	if (copyOfLockData.size() != locks.size()) {
		// Something went wrong.
		return;
	}

	for (size_t i = 0; i < copyOfLockData.size(); i++) {
		locks[i]->locked = copyOfLockData[i];
	}
}
