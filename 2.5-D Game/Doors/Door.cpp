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
}

Game::Door::Door(int lockAmount, bool locked...) {
	va_list args;
	va_start(args, lockAmount);

	for (int i = 0; i < lockAmount; i++) {
		addLock(va_arg(args, bool));
	}

	va_end(args);
}

void Game::Door::cleanup() {
	for (auto* lock : locks) {
		std::cout << lock << " " << lock->locked << "C";
		delete lock;
		std::cout << "D";
	}
}

size_t Game::Door::addLock() {
	return addLock(true);
}

size_t Game::Door::addLock(bool locked) {
	std::cout << "BAD!!!\n";
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
	std::cout << "GOOD!\n";
	locks.push_back(lock);

	if (lock->isLocked()) {
		open = false;

		if (onClose != nullptr) {
			onClose();
		}
	}

	std::cout << "ADDED: " << locks.size() << "\n";

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

Game::Lock* Game::Door::getLock(size_t id) {
	return locks[id];
}

bool Game::Door::unlock(size_t lockID) {
	std::cout << lockID << " " << locks.size() << "...\n";
	if (lockID >= locks.size()) {
		return false;
	}
	std::cout << "1...\n";
	locks[lockID]->locked = false;

	bool containsLockedLock = false;
	for (Lock lock : locks) {
		if (lock.isLocked()) {
			containsLockedLock = true;
		}
	}

	std::cout << open << " " << containsLockedLock << "2...\n";

	if (!open && !containsLockedLock) {
		open = true;

		std::cout << "3...\n";

		if (onOpen != nullptr) {
			onOpen();
		}
	}

	return true;
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

void Game::Door::setOnOpen(std::function<void()>* onOpen) {
	this->onOpen = *onOpen;
}

void Game::Door::setOnClose(std::function<void()>* onClose) {
	this->onClose = *onClose;
}
