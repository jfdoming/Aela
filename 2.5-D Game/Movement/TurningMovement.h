/*
* Class: TurningMovement
* Author: Robert Ciborowski
* Date: 20/12/2018
* Description: A class used to represent a character turning movement.
*/

#pragma once
#include "../Location/Location.h"
#include "Movement.h"

namespace Game {
	class TurningMovement : Movement {
		public:
			TurningMovement(TileDirection direction);
			virtual ~TurningMovement();

			bool moveCharacter(Character* character) override;
	};
}
