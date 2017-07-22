/*
* Class: Particle Emitter
* Author: Robert Ciborowski
* Date: 15/07/2017
* Description: An interface used to emit particles. Because particles can be emitted in so many ways,
*              the implementation for how they are emitted should be implemented in a sub class.
*/

#include "ParticleEmitter.h"

void ParticleEmitter::setupParticles(std::vector<GLuint>* textures, float particleWidth, float particleHeight, unsigned int amount) {
	for (unsigned int i = 0; i < amount; i++) {
		Billboard particle;
		srand(timeManager->getCurrentTime());
		particle.setTexture(textures->at(rand() % textures->size()));
		particle.setProperty(Object3DProperty::X_SCALING, particleWidth);
		particle.setProperty(Object3DProperty::Y_SCALING, particleHeight);
		setupParticlePositioning(i);
	}
}

// The implementation of this function is to be done by the sub class.
void ParticleEmitter::update() {
}

std::vector<Billboard>* ParticleEmitter::getParticles() {
	return &particles;
}

void ParticleEmitter::setTimeManager(TimeManager* timeManager) {
	this->timeManager = timeManager;
}

TimeManager* ParticleEmitter::getTimeManager() {
	return timeManager;
}

// The implementation of this function is to be done by the sub class. The function is meant to reset the particle's position once it has completed
// its life. This creates the effect of spawning in a new particle, even though its actually just resetting the particle in the pool.
void ParticleEmitter::setupParticlePositioning(unsigned int whichParticle) {
}
