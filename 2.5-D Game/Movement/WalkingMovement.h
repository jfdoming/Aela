/*
* Class: WalkingMovement
* Author: Robert Ciborowski
* Date: 20/12/2018
* Description: A class used to represent a character walking movement.
*/

#pragma once
#include "../Location/Location.h"
#include "Movement.h"

namespace Game {
	class WalkingMovement : protected Movement {
		public:
			WalkingMovement(float magnitude, TileDirection direction, bool collisionCheck);
			WalkingMovement(float magnitude, TileDirection direction, bool collisionCheck, bool running);
			virtual ~WalkingMovement();

			bool moveCharacter(Character* character) override;
			void start(Character* character) override;

		protected:
			float magnitude;
			float distanceTravelled = 0;
			bool collisionCheck;
			bool running = false;
			bool previousRunning;

			bool translate(Character* character, float amount);
			virtual void onEnd(Character* character);
	};
}
