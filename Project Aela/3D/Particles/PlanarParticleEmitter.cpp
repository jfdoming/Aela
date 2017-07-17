/*
* Class: Planar Particle Emitter
* Author: Robert Ciborowski
* Date: 16/07/2017
* Description: A class used to emit particles along a plane The particles travel perpendicular to the plane.
*/

#include "PlanarParticleEmitter.h"

void PlanarParticleEmitter::setupDimensions(Rect<GLfloat>* dimensions) {
	this->dimensions = *dimensions;
}

void PlanarParticleEmitter::setupParticles(std::vector<GLuint>* textures, float particleWidth, float particleHeight, unsigned int amount) {
	for (unsigned int i = 0; i < amount; i++) {
		Billboard particle;
		srand(timeManager->getCurrentTime());
		particle.setTexture(textures->at(rand() % textures->size()));
		particle.setProperty(Object3DProperty::X_SCALING, particleWidth);
		particle.setProperty(Object3DProperty::Y_SCALING, particleHeight);
		setupParticlePositioning(i);
	}
}

void PlanarParticleEmitter::update() {
}

void PlanarParticleEmitter::setupParticlePositioning(unsigned int whichParticle) {

}
