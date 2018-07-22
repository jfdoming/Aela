/*
* Class: Movement
* Author: Robert Ciborowski
* Date: 18/04/2018
* Description: A class used to represent a character movement.
*/

#pragma once
#include "../Location/Location.h"
#include "TeleportationAnimation.h"

namespace Game {
	class Movement {
		public:
			Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation);
			Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation,
				bool animated);
			Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation,
				TeleportationAnimation teleportationAnimation);

			Location* getDestination();
			glm::vec3* getWorldspaceTranslation();
			TileDirection getDirection();
			bool isATeleportation();
			void setAnimated(bool animated);
			bool isAnimated();
			void setTeleportationAnimation(TeleportationAnimation teleportationAnimation);
			TeleportationAnimation getTeleportationAnimation();

		private:
			Location destination;
			glm::vec3 worldSpaceTranslation;
			TileDirection direction;
			TeleportationAnimation teleportationAnimation;
			bool teleportation;
			bool animated = true;
	};
}