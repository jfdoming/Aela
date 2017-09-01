/*
* Class: Planar Particle Emitter
* Author: Robert Ciborowski
* Date: 16/07/2017
* Description: A class used to emit particles along a plane. The particles travel perpendicular to the plane.
*/

#include "PlanarParticleEmitter.h"

#include <glm/gtc/constants.hpp>

#define PI 3.14159265358979323846

void PlanarParticleEmitter::setupDimensions(Rect<GLfloat>* dimensions) {
	this->dimensions = *dimensions;
}

void PlanarParticleEmitter::setupParticles(std::vector<Texture*>* textures, float particleWidth, float particleHeight, unsigned int amount) {
	ParticleEmitter::setupParticles(textures, particleWidth, particleHeight, amount);
}

void PlanarParticleEmitter::update() {
	// Note: the particle positioning is relative to the emitter. The rotation, position and scaling of the emitter is looked at by the renderer,
	// which renders the particles while taking these transformations into account.
	for (unsigned int i = 0; i < particles.size(); i++) {
		Particle* particle = &particles.at(i);
		if (particle->getProperty(Transformable3DProperty::Y_POSITION) >= particle->getLifetime()) {
			setupParticlePositioning(i, particles.size());
		} else {
			particle->translate(0, timeManager->getTimeBetweenFrames() * particle->getSpeed(), 0);
		}
	}
}

Camera3D* PlanarParticleEmitter::getCamera() {
	return camera;
}

void PlanarParticleEmitter::setCamera(Camera3D* camera) {
	this->camera = camera;
}

void PlanarParticleEmitter::setupParticlePositioning(unsigned int whichParticle, unsigned int numberOfParticles) {
	// This chooses the position of the particle, relative to the emitter.
	srand(timeManager->getCurrentTime() + whichParticle);
	float particleZ = dimensions.getHeight() - (dimensions.getHeight() / numberOfParticles) * whichParticle;
	particles[whichParticle].setPosition((rand() % 100) / 100.0f * dimensions.getWidth(), pathOffset * (rand() % 100) / 100.0f, particleZ);
	for (unsigned int i = 0; i < particles.size(); i++) {
		if (particles[i].getPosition()->z >= particles[whichParticle].getPosition()->z) {
			particles.insert(particles.begin() + i, particles[whichParticle]);
			particles.erase(particles.begin() + whichParticle + 1);
		}
	}
}
