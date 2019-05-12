/*
* Class: TeleportationMovement
* Author: Robert Ciborowski
* Date: 20/12/2018
* Description: A class used to represent a character teleportation movement.
*/

#pragma once
#include "../Location/Location.h"
#include "TeleportationAnimation.h"
#include "Movement.h"
#include <queue>

namespace Game {
	class TeleportationMovement : Movement {
		public:
			TeleportationMovement(Location& destination, TeleportationAnimation animation);
			virtual ~TeleportationMovement();

			bool moveCharacter(Character* character) override;
			void setScriptOnHalfLife(std::string scriptOnHalfLife);

		private:
			TeleportationAnimation animation;
			bool currentTeleportIsSetup = false;
			std::queue<Location> teleportationQueue;
			bool aboutToEnd = false;
			bool complete = false;
			const unsigned long long FADE_TELEPORT_PAUSE = 50;
			std::string scriptOnHalfLife = "";
	};
}
