/*
* Class: Lock
* Author: Robert Ciborowski
* Date: 13/07/2018
* Description: A class used to represent a door lock.
*/

#pragma once

namespace Game {
	class Lock {
		public:
			Lock();
			Lock(bool locked);

			void setLocked(bool locked);
			bool isLocked();

		private:
			bool locked = true;
	};
}