/*
* Class: Particle
* Author: Robert Ciborowski
* Date: 15/07/2017
* Description: A class used to store properties of a particle.
*/

#include "Particle.h"

float Particle::getSpeed() {
	return baseSpeed;
}

void Particle::setSpeed(float baseSpeed) {
	this->baseSpeed = baseSpeed;
}

unsigned int Particle::getLifetime() {
	return baseLifetime;
}

void Particle::setLifetime(unsigned int baseLifetime) {
	this->baseLifetime = baseLifetime;
}
