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

namespace Game {
	class Door {
		public:
			Door();
			Door(int lockAmount, ...);

			// This adds a lock to the door and returns its ID.
			size_t addLock();
			size_t addLock(bool locked);
			void addLocks(int lockAmount, ...);

			// These lock/unlock locks and return the success.
			bool unlock(size_t lockID);
			bool lock(size_t lockID);

			void setOnOpen(std::function<void()>* onOpen);
			void setOnClose(std::function<void()>* onClose);

		private:
			std::vector<Lock> locks;
			std::function<void()> onOpen;
			std::function<void()> onClose;

			bool open;
	};
}