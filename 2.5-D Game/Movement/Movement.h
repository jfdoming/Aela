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
			explicit Movement(TileDirection direction);
			Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation);
			Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation,
				bool animated);
			Movement(Location* destination, glm::vec3* worldSpaceTranslation, TileDirection direction, bool teleportation,
				TeleportationAnimation teleportationAnimation);

			void setDestination(Location* destination);
			Location* getDestination();
			void setWorldspaceTranslation(glm::vec3* worldSpaceTranslation);
			glm::vec3* getWorldspaceTranslation();
			void setDirection(TileDirection direction);
			TileDirection getDirection();
			bool isATeleportation();
			void setAnimated(bool animated);
			bool isAnimated();
			void setTeleportationAnimation(TeleportationAnimation teleportationAnimation);
			TeleportationAnimation getTeleportationAnimation();
			void setTurnOnly(bool turnOnly);
			bool isATurnOnly();
			void setPreprocessed(bool preprocessed);
			bool isPreprocessed();
			void setScriptOnCompletion(std::string scriptOnCompletion);
			std::string getScriptOnCompletion();
			void setCollidable(bool collidable);
			bool isCollidable();

		private:
			Location destination;
			glm::vec3 worldSpaceTranslation;
			TileDirection direction;
			TeleportationAnimation teleportationAnimation;
			std::string scriptOnCompletion = "";
			bool teleportation = false;
			bool animated = true;
			bool turnOnly = false;
			bool preprocessed = false;
			bool collidable = true;
	};
}