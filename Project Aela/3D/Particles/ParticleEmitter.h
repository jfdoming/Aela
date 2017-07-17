/*
* Class: Particle Emitter
* Author: Robert Ciborowski
* Date: 15/07/2017
* Description: An interface used to emit particles. Because particles can be emitted in so many ways,
*              the implementation for how they are emitted should be implemented in a sub class.
*/

#pragma once

#include "../Billboards/Billboards.h"
#include "../../Time Manager/TimeManager.h"

using namespace Aela;

class ParticleEmitter {
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
		std::vector<Billboard>* getParticles();
		void setTimeManager(TimeManager* timeManager);
		TimeManager* getTimeManager();

	protected:
		// These are the properties of the class that use Aela classes.
		std::vector<Billboard> particles;
		TimeManager* timeManager;

		// This is meant to reset a particle's position once it has completed its life.
		virtual void setupParticlePositioning(unsigned int whichParticle);
};