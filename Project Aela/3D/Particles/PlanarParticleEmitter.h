/*
* Class: Planar Particle Emitter
* Author: Robert Ciborowski
* Date: 16/07/2017
* Description: A class used to emit particles along a plane The particles travel perpendicular to the plane.
*/

#pragma once

#include "ParticleEmitter.h"
#include "../../Utilities/Rect/Rect.h"
#include "../Camera/Camera3D.h"

class PlanarParticleEmitter : public ParticleEmitter {
	public:
		PlanarParticleEmitter() {

		}

		// These are the functions that are overwritten from the ParticleEmitter class.
		void setupDimensions(Rect<GLfloat>* dimensions);
		void setupParticles(std::vector<GLuint>* textures, float particleWidth, float particleHeight, unsigned int amount);
		void update();

		// These are getters and setters.
		Camera3D* getCamera();
		void setCamera(Camera3D* camera);

	private:
		// This stores the dimensions of the plane.
		Rect<GLfloat> dimensions;

		// This class requires a pointer to the camera so that it can properly sort particles.
		Camera3D* camera;

		void setupParticlePositioning(unsigned int whichParticle, unsigned int numberOfParticles);
		void sortParticles();
};