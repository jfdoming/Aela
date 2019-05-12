/*
* Class: TileMovement
* Author: Robert Ciborowski
* Date: 22/12/2018
* Description: A class used to represent a character walking movement.
*/

#pragma once
#include "../Location/Location.h"
#include "Movement.h"
#include "WalkingMovement.h"

namespace Game {
	class TileMovement : protected WalkingMovement {
		public:
			TileMovement(TileDirection direction, bool collisionCheck);
			TileMovement(TileDirection direction, bool collisionCheck, bool running);
			virtual ~TileMovement();

			void start(Character* character) override;

		private:
			void onEnd(Character* character) override;
	};
}
