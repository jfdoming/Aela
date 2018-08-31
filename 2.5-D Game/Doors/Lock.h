/*
* Class: Lock
* Author: Robert Ciborowski
* Date: 13/07/2018
* Description: A class used to represent a door lock.
*/

#pragma once

namespace Game {
	class Lock {
		friend class Door;
		public:
			Lock();
			explicit Lock(bool locked);
			virtual ~Lock();

			bool isLocked();

		protected:
			bool locked = true;
	};
}