/*
* Class: Particle
* Author: Robert Ciborowski
* Date: 15/07/2017
* Description: A class used to store properties of a particle.
*/

#pragma once

#include "../Billboards/Billboards.h"

class Particle : public Billboard {
	public:
		Particle() {

		}

		float getSpeed();
		void setSpeed(float baseSpeed);
		unsigned int getLifetime();
		void setLifetime(unsigned int baseLifetime);

	private:
		float baseSpeed = 0.001f;
		unsigned int baseLifetime = 1000;
};