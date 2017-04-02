/*
* Name: Project Aela's Texture Class
* Author: Robert Ciborowski
* Date: 21/02/2017
* Description: A class used by Aela's Renderer to store properties of an OpenGL texture.
*/

#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include "../../Utilities/Rect/Rect.h"

class Texture {
	public:
		Texture() {
			dimensions.setValues(0, 0, 0, 0);
		}

		// These are some getters and setters.
		void setDimensions(Rect<int>* dimensions) {
			this->dimensions = *dimensions;
		}

		void setDimensions(int x, int y, int width, int height) {
			dimensions.setValues(x, y, width, height);
		}

		void setOutput(Rect<int>* output) {
			this->output = *output;
		}

		void setOutput(int x, int y, int width, int height) {
			output.setValues(x, y, width, height);
		}

		Rect<int>* getDimensions() {
			return &dimensions;
		}

		Rect<int>* getOutput() {
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

		GLuint* getTexture() {
			return &texture;
		}

		// This function returns if the object was initialised properly. If it is not
		// initialised then rendering propblems will occur. Note that a Texture is still
		// initialised if the output (dimensions on the screen) has a width and/or height
		// of zero.
		bool isInitialised() {
			return (dimensions.getWidth() > 0 && dimensions.getHeight() > 0 && texture != NULL);
		}

	private:
		Rect<int> dimensions;
		Rect<int> output;
		GLuint texture = NULL;
};
