/*
* Class: Particle Emitter
* Author: Robert Ciborowski
* Date: 15/07/2017
* Description: An interface used to emit particles. Because particles can be emitted in so many ways,
*              the implementation for how they are emitted should be implemented in a sub class.
*/

#pragma once

#include "Particle.h"
#include "../../Time Manager/TimeManager.h"

using namespace Aela;

class ParticleEmitter : public Transformable3D {
	public:
		ParticleEmitter() {
			
		}

		virtual ~ParticleEmitter() {

		}

		// This sets up a particle. Basic implementation has been done on this function.
		virtual void setupParticles(std::vector<GLuint>* textures, float particleWidth, float particleHeight, unsigned int amount);

		// This is the update function, which has no actual implementation.
		virtual void update();

		// These are getters and setters.
		std::vector<Particle>* getParticles();
		void setTimeManager(TimeManager* timeManager);
		TimeManager* getTimeManager();
		float getBaseSpeed();
		void setBaseSpeed(float baseSpeed);
		unsigned int getBaseLifetime();
		void setBaseLifetime(unsigned int baseLifetime);
		float getSpeedOffset();
		void setSpeedOffset(float speedOffset);
		unsigned int getLifetimeOffset();
		void setLifetimeOffset(unsigned int lifetimeOffset);
		float getPathOffset();
		void setPathOffset(float pathOffset);

	protected:
		// These are the properties of the class that use Aela classes.
		std::vector<Particle> particles;
		TimeManager* timeManager;

		// This defines properties of the particles. Speed = distance / millisecond, lifetime = distance.
		float baseSpeed = 0.001f;
		unsigned int baseLifetime = 1000;

		// These are offsets that allow some particles to be slightly different in their behaviour compared to other particles.
		float speedOffset = 0;
		unsigned int lifetimeOffset = 0;
		float pathOffset = 0;

		// This is meant to reset a particle's position once it has completed its life.
		virtual void setupParticlePositioning(unsigned int whichParticle, unsigned int numberOfParticles);

		// This function sorts particles so that they may be rendered properly.
		virtual void sortParticles();
};