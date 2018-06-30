/*
* Class: TileType
* Author: Robert Ciborowski
* Date: 06/12/2017
* Description: A class used to represent the properties of a type of tile.
*/

#pragma once
#include "3D/Materials/Material.h"

using namespace Aela;

namespace Game {
	enum class TileBehaviour {
		FLOOR, RAMP_RIGHT, RAMP_UP_RIGHT_DEPRESSED, RAMP_UP_RIGHT_ELEVATED, RAMP_UP, RAMP_UP_LEFT_DEPRESSED,
		RAMP_UP_LEFT_ELEVATED, RAMP_LEFT, RAMP_DOWN_LEFT_DEPRESSED, RAMP_DOWN_LEFT_ELEVATED, RAMP_DOWN,
		RAMP_DOWN_RIGHT_DEPRESSED, RAMP_DOWN_RIGHT_ELEVATED, BOX, WALL_RIGHT, WALL_FRONT, WALL_LEFT,
		WALL_CORNER_UP_LEFT, WALL_CORNER_UP_RIGHT, WALL_CORNER_DOWN_LEFT, WALL_CORNER_DOWN_RIGHT
	};

	enum class RampType {
		UP, FLAT, DOWN
	};

	class TileType {
		public:
			TileType();
			TileType(bool collidable, TileBehaviour behaviour, std::string name);

			bool isCollidable();
			TileBehaviour getBehaviour();
			std::string getName();

		private:
			bool collidable;
			TileBehaviour behaviour;
			std::string name;
	};
}