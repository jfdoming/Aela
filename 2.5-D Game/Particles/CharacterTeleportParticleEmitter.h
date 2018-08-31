/*
* Class: Character Teleport Particle Emitter
* Author: Robert Ciborowski
* Date: 19/04/2018
* Description: A particle emitter that is used to make a cool character teleport effect.
*              (It's quick and sort of dirty, for now).
*/

#pragma once
#include <functional>
#include "../../Project Aela/3D/Particles/ParticleEmitter.h"
#include "../../Project Aela/Utilities/Rect/Rect.h"
#include "../Character/Character.h"

namespace Game {
	class CharacterTeleportParticleEmitter : public ParticleEmitter {
		public:
			CharacterTeleportParticleEmitter(Clock* time);
			virtual ~CharacterTeleportParticleEmitter();

			// These are the functions that are overwritten from the ParticleEmitter class.
			virtual void setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount);
			virtual void update();

			void setCharacter(Character* character);
			void setLocations(Location* oldLocation, Location* newLocation);
			void setActionOnHalfLife(std::function<void()> onHalfLife);

			void setSecondaryPathOffset(float secondaryPathOffset);

		private:
			// This stores the dimensions of the plane.
			Rect<GLfloat> dimensions1, dimensions2;

			bool halfLifeHasPassed = false;

			GLTexture* texture;
			float particleWidth, particleHeight;
			unsigned int amount;
			float secondaryPathOffset;

			Character* character;

			std::function<void()> onHalfLife = NULL;

			virtual void setupParticlePositioning(size_t whichParticle, size_t numberOfParticles);
	};
}