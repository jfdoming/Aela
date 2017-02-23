/*
* Name: Project Aela's Texture Class
* Author: Ekkon Games
* Date: 21/02/2017
* Description: A class used by Aela's Renderer to store properties of a texture.
*/

#pragma once
#include "ErrorHandler.h"
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include "Rect.h"

class Texture {
	public:
		Texture() {
			dimensions.setValues(0, 0, 0, 0);
		}

		void setDimensions(Rect<unsigned int> dimensions) {
			this->dimensions = dimensions;
		}

		void setDimensions(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
			dimensions.setValues(x, y, width, height);
		}

		Rect<unsigned int> getDimensions() {
			return dimensions;
		}

		void setTexture(GLuint texture) {
			this->texture = texture;
		}

		GLuint getTexture() {
			return texture;
		}

		bool isInitialised() {
			return (dimensions.getWidth() > 0 && dimensions.getHeight() > 0 && texture != NULL);
		}

	private:
		Rect<unsigned int> dimensions;
		GLuint texture = NULL;
};
