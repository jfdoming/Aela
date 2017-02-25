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

		void setDimensions(Rect<unsigned int>* dimensions) {
			this->dimensions = *dimensions;
		}

		void setDimensions(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
			dimensions.setValues(x, y, width, height);
		}

		void setOutput(Rect<unsigned int>* output) {
			this->output = *output;
		}

		void setOutput(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
			output.setValues(x, y, width, height);
		}

		Rect<unsigned int>* getDimensions() {
			return &dimensions;
		}

		Rect<unsigned int>* getOutput() {
			return &output;
		}

		void setTexture(GLuint* texture) {
			if (texture != NULL) {
				glDeleteTextures(1, &(this->texture));
			}
			this->texture = *texture;
		}

		void deleteTexture() {
			glDeleteTextures(1, &texture);
		}

		GLuint getTexture() {
			return texture;
		}

		bool isInitialised() {
			return (dimensions.getWidth() > 0 && dimensions.getHeight() > 0 && texture != NULL);
		}

	private:
		Rect<unsigned int> dimensions;
		Rect<unsigned int> output;
		GLuint texture = NULL;
};
