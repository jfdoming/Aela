/*
* Class: Teleporter
* Author: Robert Ciborowski
* Date: 18/04/2018
* Description: A class that is able to teleport characters.
*/

#pragma once
#include "../Location/Location.h"

namespace Game {
	class Teleporter {
		public:
			Teleporter();
			Teleporter(const Location& location);

			Location* getDestination();

		private:
			Location destination;
	};
}