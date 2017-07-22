/*
* Class: Planar Particle Emitter
* Author: Robert Ciborowski
* Date: 16/07/2017
* Description: A class used to emit particles along a plane The particles travel perpendicular to the plane.
*/

#pragma once

#include "ParticleEmitter.h"
#include "../../Utilities/Rect/Rect.h"

class PlanarParticleEmitter : public ParticleEmitter, public Object3D {
	public:
		PlanarParticleEmitter() {

		}

		// These are the functions that are overwritten from the ParticleEmitter class.
		void setupDimensions(Rect<GLfloat>* dimensions);
		void setupParticles(std::vector<GLuint>* textures, float particleWidth, float particleHeight, unsigned int amount);
		void update();

	private:
		// This stores the dimensions of the plane.
		Rect<GLfloat> dimensions;

		void setupParticlePositioning(unsigned int whichParticle);
};