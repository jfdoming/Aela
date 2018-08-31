/*
* Class: Bullet Particle Emitter
* Author: Robert Ciborowski
* Date: 22/04/2018
* Description: A class used to emit particles that represent bullets.
*/

#pragma once

#include "../../Project Aela/3D/Particles/ParticleEmitter.h"
#include "../Location/Location.h"

namespace Game {
	class BulletParticleEmitter : public ParticleEmitter {
	public:
		BulletParticleEmitter(Clock* time);
		virtual ~BulletParticleEmitter();

		// These are the functions that are overwritten from the ParticleEmitter class.
		virtual void setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount);
		virtual void update();

		void setDirection(TileDirection direction);

	private:
		// This stores the dimensions of the plane.
		TileDirection direction;
		glm::vec3 particleRotation;

		virtual void setupParticlePositioning(size_t whichParticle, size_t numberOfParticles);
	};
}