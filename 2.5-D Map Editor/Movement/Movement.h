/*
* Class: Movement
* Author: Robert Ciborowski
* Date: 18/04/2018
* Description: A class used to represent a character movement.
*/

#pragma once
#include "../Location/Location.h"

namespace Game {
	class Movement {
		public:
			Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation);

			Location* getDestination();
			glm::vec3* getWorldspaceTranslation();
			TileDirection getDirection();
			bool isATeleportation();

		private:
			Location destination;
			glm::vec3 worldSpaceTranslation;
			TileDirection direction;
			bool teleportation;
	};
}