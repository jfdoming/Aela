/*
* Class: Movement
* Author: Robert Ciborowski
* Date: 18/04/2018
* Description: A class used to represent a character movement.
*/

#pragma once
#include "../Tiles/Tile.h"

namespace Game {
	class Character;
	class Movement {
		public:
			Movement();
			virtual ~Movement();

			// This lets the movement do its thing on a character and returns whether the movement has completed.
			virtual bool moveCharacter(Character* character);

			bool isStarted();
			virtual void start(Character* character);
			void setDirection(TileDirection direction);
			TileDirection getDirection();

		protected:
			bool started = false;
			TileDirection direction = TileDirection::NONE;
	};
}
