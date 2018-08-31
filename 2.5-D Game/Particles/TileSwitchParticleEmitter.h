/*
* Class: Tile Switch Particle Emitter
* Author: Robert Ciborowski
* Date: 14/04/2018
* Description: A class used to emit particles when a tile is switched.
*/

#pragma once

#include "../../Project Aela/3D/Particles/ParticleEmitter.h"
#include "../../Project Aela/Utilities/Rect/Rect.h"

namespace Game {
	class TileSwitchParticleEmitter : public ParticleEmitter {
		public:
			TileSwitchParticleEmitter(Clock* time);
			virtual ~TileSwitchParticleEmitter();

			// These are the functions that are overwritten from the ParticleEmitter class.
			virtual void setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount);
			virtual void update();

			void setupDimensions(Rect<GLfloat>* dimensions);

		private:
			// This stores the dimensions of the plane.
			Rect<GLfloat> dimensions;

			virtual void setupParticlePositioning(size_t whichParticle, size_t numberOfParticles);
	};
}