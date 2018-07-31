/*
* Class: Tile Switch Particle Emitter
* Author: Robert Ciborowski
* Date: 14/04/2018
* Description: A class used to emit particles when a tile is switched.
*/

#include "TileSwitchParticleEmitter.h"
#include "../Utilities/MathConstants.h"
#include <glm/gtc/constants.hpp>
#include "BulletParticleEmitter.h"

using namespace Game;

Game::TileSwitchParticleEmitter::TileSwitchParticleEmitter(Time* time) : ParticleEmitter(time) {
}

Game::TileSwitchParticleEmitter::~TileSwitchParticleEmitter() {

}

void TileSwitchParticleEmitter::setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount) {
	ParticleEmitter::setupParticles(textures, particleWidth, particleHeight, amount);
}

void TileSwitchParticleEmitter::update() {
	// Note: the particle positioning is relative to the emitter. The rotation, position and scaling of the emitter is looked at by the renderer,
	// which renders the particles while taking these transformations into account.
	for (size_t i = 0; i < particles.size(); i++) {
		Particle* particle = &particles.at(i);
		if (particle->getProperty(Transformable3DProperty::Y_POSITION) >= particle->getDistance()) {
			setupParticlePositioning(i, particles.size());
		} else {
			particle->translate(0, time->getTimeBetweenFramesInNanos() * particle->getSpeed(), 0);
			// float scaling = (particle->getDistance() - particle->getScaling()->y) / particle->getDistance();
			// std::cout << scaling * particle->getScaling()->x << "\n";
			// particle->rotate(glm::vec3(0, time->getTimeBetweenFramesInNanos() * particle->getSpeed() / 4, 0));
			// particle->setScaling(glm::vec3(scaling * particle->getScaling()->x, 1, scaling * particle->getScaling()->z));
			// particle->setRotation(glm::vec3(HALF_PI, scaling * DOUBLE_PI, 0));
		}
	}
}

void Game::TileSwitchParticleEmitter::setupDimensions(Rect<GLfloat>* dimensions) {
	this->dimensions = *dimensions;
}

void TileSwitchParticleEmitter::setupParticlePositioning(size_t whichParticle, size_t numberOfParticles) {
	particles[whichParticle].getPosition()->x = dimensions.getX() + particles[whichParticle].getScaling()->x / 2;
	particles[whichParticle].getPosition()->z = dimensions.getY() + particles[whichParticle].getScaling()->y / 2;
	particles[whichParticle].getPosition()->y = pathOffset;
	particles[whichParticle].setRotation(glm::vec3(HALF_PI, 0, 0));
	std::cout << "SET UP!\n";
}
