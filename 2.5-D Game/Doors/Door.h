/*
* Class: Door
* Author: Robert Ciborowski
* Date: 13/07/2018
* Description: A class used to represent a door.
*/

#pragma once
#include "Lock.h"
#include <vector>
#include <functional>

#define TIME_BETWEEN_DOOR_ANIMATION_FRAMES /*2000000000ll*/ 32000000ll

namespace Game {
	class Door {
		public:
			Door();
			explicit Door(int lockAmount);
			Door(std::initializer_list<bool> locked);

			void cleanup();

			// This adds a lock to the door and returns its ID.
			size_t addLock();
			size_t addLock(bool locked);
			void addLocks(int lockAmount, ...);
			size_t addLockWithoutCallingOnClose(bool locked);
			Lock* getLock(size_t id);
			std::vector<Lock*>* getLocks();

			// This is only meant for locks on the heap.
			size_t addLock(Lock* lock);

			// These lock/unlock locks and return the success.
			bool unlock(size_t lockID);
			void unlock();
			bool lock(size_t lockID);
			void lock();
			void lockWithoutLockingLocks();
			bool isOpen();

			void setOnOpen(std::function<void()>* onOpen);
			std::function<void()>* getOnOpen();
			void setOnClose(std::function<void()>* onClose);
			std::function<void()>* getOnClose();

			void makeBackupOfLockData();
			void useBackupOfLockData();

		private:
			std::vector<Lock*> locks;
			std::vector<bool> copyOfLockData;
			std::function<void()> onOpen = nullptr;
			std::function<void()> onClose = nullptr;

			bool open;
	};
}