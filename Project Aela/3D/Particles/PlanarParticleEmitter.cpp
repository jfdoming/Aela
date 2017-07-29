/*
* Class: Planar Particle Emitter
* Author: Robert Ciborowski
* Date: 16/07/2017
* Description: A class used to emit particles along a plane The particles travel perpendicular to the plane.
*/

#include "PlanarParticleEmitter.h"

#include <glm/gtc/constants.hpp>

#define PI 3.14159265358979323846

void PlanarParticleEmitter::setupDimensions(Rect<GLfloat>* dimensions) {
	this->dimensions = *dimensions;
}

void PlanarParticleEmitter::setupParticles(std::vector<GLuint>* textures, float particleWidth, float particleHeight, unsigned int amount) {
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
	sortParticles();
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
	particles.at(whichParticle).setPosition((rand() % 100) / 100.0f * dimensions.getWidth(), pathOffset * (rand() % 100) / 100.0f, particleZ);
}

// This sorts particles so that the particles that have a greater depth are earlier in the particle list and are rendered first.
// This only matters if the particles have any sort of transparency (if they don't have any transparency, they must be opaque rectangles).
void PlanarParticleEmitter::sortParticles() {
	bool useFastAlgorithm = true;

	/*if (useFastAlgorithm) {
		// This is a fast algorithm that sorts the particles based on their distane to the camera. It is mostly accurate and looks
		// better with smaller particles.
		for (unsigned int i = 0; i < particles.size(); i++) {
			for (unsigned int j = 0; j < particles.size() - 1; j++) {
				if (glm::distance(*camera->getPosition(), *particles.at(j).getPosition()) < glm::distance(*camera->getPosition(), *particles.at(j + 1).getPosition())) {
					std::iter_swap(particles.begin() + j, particles.begin() + j + 1);
				}
			}
		}
	} else {
		for (unsigned int i = 0; i < particles.size(); i++) {
			for (unsigned int j = 0; j < particles.size() - 1; j++) {
				// This is not done yet! It's supposed to find the actual on-screen depth of the particle.
				glm::vec3 cameraAngle = glm::vec3(camera->getRotation()->y, camera->getRotation()->x, camera->getRotation()->z) / glm::vec3((float) glm::pi<float>() * 2);
				glm::vec3 cameraToParticleA = glm::normalize(*particles.at(j).getPosition() - *camera->getPosition());
				glm::vec3 cameraToParticleB = glm::normalize(*particles.at(j + 1).getPosition() - *camera->getPosition());
				float cosA = glm::dot(cameraAngle, cameraToParticleA);
				float cosB = glm::dot(cameraAngle, cameraToParticleB);

				std::cout << cosA << " " << cosB << " \n";

				if (glm::distance(*camera->getPosition(), *particles.at(j).getPosition()) < glm::distance(*camera->getPosition(), *particles.at(j + 1).getPosition())) {
					std::iter_swap(particles.begin() + j, particles.begin() + j + 1);
				}
			}
		}
	}*/
}
