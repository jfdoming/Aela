/*
* Class: Material
* Author: Robert Ciborowski
* Date: 27/07/2017
* Description: A class for a material that may be used by an object.
*/

#pragma once

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>
#include <glm\glm.hpp>
#include <vector>

namespace Aela {
	class Material {
		public:
			Material() {

			}

			GLuint getTexture();
			void setTexture(GLuint texture);

		private:
			GLuint texture;

			// These have not been properly implemented yet.
			/*glm::vec3 ambientColour;
			glm::vec3 diffuseColour;
			glm::vec3 specularColour;
			float specularExponent;
			float transparency;
			unsigned short illuminationModel;*/
	};
}