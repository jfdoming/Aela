/*
* Class: TileDestroyer
* Author: Robert Ciborowski
* Date: 13/09/2018
* Description: A class which represents an enemy that will (in addition
*              to destroying the player) destroy tiles in its line of sight.
*/

#pragma once
#include "Enemy.h"

namespace Game {
	class TileDestroyer : public Enemy {
		public:
			TileDestroyer();
			TileDestroyer(std::string name, const Location& location, unsigned int range);
			~TileDestroyer();

			void destroyMatter();

			void setDestructionAngle(TileDirection direction, bool detect);
			void setDestructionAngles(bool right, bool forward, bool left, bool backward);


		protected:
			virtual void attack(Character* character);
			virtual void update();

		private:
			unsigned int range = 0;
			std::unordered_map<TileDirection, bool> destructionAngles;
	};
}

