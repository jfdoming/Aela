/*
* Class: Light3D
* Author: Robert Ciborowski
* Date: 07/04/2017
* Description: A class used to store the properties of a light.
*/

#pragma once
#include "../Transformable/Transformable3D.h"
#include "../../Utilities/Colour/ColourRGB.h"

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

class Light3D : public Transformable3D {
	public:
		Light3D() {
			// If no parameters are set, these values are chosen for the light.
			position = glm::vec3(0.0, 0.0, 0.0);
			rotation = glm::vec3(0.0, 0.0, 0.0);
			colour.setValues(1.0, 1.0, 1.0);
			power = 1.0;
		}

		Light3D(glm::vec3 position, glm::vec3 rotation, ColourRGB colour, float power) {
			this->position = position;
			this->rotation = rotation;
			this->colour = colour;
			this->power = power;
		}

		// These are getters and setters.
		void setColour(ColourRGB* colour);
		void setColour(float r, float g, float b);
		void setPower(float power);
		ColourRGB* getColour();
		float getPower();
		GLuint* getShadowMapTexture();
		void setShadowMapTexture(GLuint* shadowMapTexture);
		GLuint* getShadowMapBuffer();
		void setShadowMapBuffer(GLuint* shadowMapBuffer);

	private:
		ColourRGB colour;
		float power;
		GLuint shadowMapTexture, shadowMapBuffer;
};