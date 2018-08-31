/*
* Class: Bullet
* Author: Robert Ciborowski
* Date: 08/04/2018
* Description: A class used to represent a bullet.
*/

#pragma once

namespace Game {
	class Bullet {
		public:
			Bullet();
			explicit Bullet(double speed);

			void setSpeed(double speed);
			double getSpeed();

		private:
			double speed;
	};
}