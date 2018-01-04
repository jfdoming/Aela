/*
* Class: TileType
* Author: Robert Ciborowski
* Date: 06/12/2017
* Description: A header-only class used to represent the properties of a type of tile.
*/

#pragma once
#include "3D/Materials/Material.h"

using namespace Aela;

namespace Game {
	enum class TileBehaviour {
		FLOOR, RAMP_RIGHT, RAMP_UP_RIGHT_DEPRESSED, RAMP_UP_RIGHT_ELEVATED, RAMP_UP, RAMP_UP_LEFT_DEPRESSED,
		RAMP_UP_LEFT_ELEVATED, RAMP_LEFT, RAMP_DOWN_LEFT_DEPRESSED, RAMP_DOWN_LEFT_ELEVATED, RAMP_DOWN,
		RAMP_DOWN_RIGHT_DEPRESSED, RAMP_DOWN_RIGHT_ELEVATED
	};

	class TileType {
		public:
			TileType() {
				collidable = false;
			}

			TileType(bool collidable, TileBehaviour behaviour) {
				this->collidable = collidable;
				this->behaviour = behaviour;
			}

			bool isCollidable() {
				return collidable;
			}

			TileBehaviour getBehaviour() {
				return behaviour;
			}

		private:
			bool collidable;
			TileBehaviour behaviour;
	};
}