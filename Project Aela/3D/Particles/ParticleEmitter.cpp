/*
* Class: Particle Emitter
* Author: Robert Ciborowski
* Date: 15/07/2017
* Description: An interface used to emit particles. Because particles can be emitted in so many ways,
*              the implementation for how they are emitted should be implemented in a sub class.
*/

#include "ParticleEmitter.h"

#include <algorithm>
#include <functional>

void ParticleEmitter::setupParticles(std::vector<GLuint>* textures, float particleWidthScaling, float particleHeightScaling, unsigned int amount) {
	for (unsigned int i = 0; i < amount; i++) {
		Particle particle;
		srand(timeManager->getCurrentTime() + i);
		particle.setTexture(textures->at(rand() % textures->size()));
		particle.setProperty(Transformable3DProperty::X_SCALING, particleWidthScaling);
		particle.setProperty(Transformable3DProperty::Y_SCALING, particleHeightScaling);
		particle.setSpeed(baseSpeed + (speedOffset * (rand() % 100) / 100));
		particle.setLifetime(baseLifetime + (lifetimeOffset * (rand() % 100) / 100));
		particles.push_back(particle);
		setupParticlePositioning(i, amount);
	}
	sortParticles();
}

// The implementation of this function is to be done by the sub class.
void ParticleEmitter::update() {
}

std::vector<Particle>* ParticleEmitter::getParticles() {
	return &particles;
}

void ParticleEmitter::setTimeManager(TimeManager* timeManager) {
	this->timeManager = timeManager;
}

TimeManager* ParticleEmitter::getTimeManager() {
	return timeManager;
}

float ParticleEmitter::getBaseSpeed() {
	return baseSpeed;
}

void ParticleEmitter::setBaseSpeed(float baseSpeed) {
	this->baseSpeed = baseSpeed;
}

unsigned int ParticleEmitter::getBaseLifetime() {
	return baseLifetime;
}

void ParticleEmitter::setBaseLifetime(unsigned int baseLifetime) {
	this->baseLifetime = baseLifetime;
}

float ParticleEmitter::getSpeedOffset() {
	return speedOffset;
}

void ParticleEmitter::setSpeedOffset(float speedOffset) {
	this->speedOffset = speedOffset;
}

unsigned int ParticleEmitter::getLifetimeOffset() {
	return lifetimeOffset;
}

void ParticleEmitter::setLifetimeOffset(unsigned int lifetimeOffset) {
	this->lifetimeOffset = lifetimeOffset;
}

float ParticleEmitter::getPathOffset() {
	return pathOffset;
}

void ParticleEmitter::setPathOffset(float pathOffset) {
	this->pathOffset = pathOffset;
}

// The implementation of this function is to be done by the sub class. The function is meant to reset the particle's position once it has completed
// its life. This creates the effect of spawning in a new particle, even though its actually just resetting the particle in the pool.
void ParticleEmitter::setupParticlePositioning(unsigned int whichParticle, unsigned int numberOfParticles) {
}

void ParticleEmitter::sortParticles() {
	// Implement your own sorting algorithms here!
}
