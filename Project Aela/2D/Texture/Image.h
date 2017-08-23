/*
* Class: Image
* Author: Julian Dominguez-Schatz, adapted from Texture (Robert Ciborowski)
* Date: 21/02/2017
* Description: A class used by Aela's Renderer to store properties of an OpenGL texture.
*/

#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include "../../Utilities/Rect/Rect.h"

class Image {
	public:
		Image() {
			dimensions.setValues(0, 0, 0, 0);
		}

		virtual ~Image(){
			deleteImage();
		}

		// These are some getters and setters.
		void setDimensions(Rect<int>* dimensions) {
			this->dimensions = *dimensions;
		}

		void setDimensions(int x, int y, int width, int height) {
			dimensions.setValues(x, y, width, height);
		}

		void setTexture(GLuint texture) {
			if (texture != 0) {
				deleteImage();
			}
			this->texture = texture;
		}

		Rect<int>* getDimensions() {
			return &dimensions;
		}

		GLuint* getTexture() {
			return &texture;
		}

		// This function returns if the object was initialised properly. If it is not
		// initialised then rendering propblems will occur. Note that a Texture is still
		// initialised if the output (dimensions on the screen) has a width and/or height
		// of zero.
		bool isInitialised() {
			return (dimensions.getWidth() > 0 && dimensions.getHeight() > 0 && texture != 0);
		}

	private:
		Rect<int> dimensions;
		GLuint texture = 0;

		void deleteImage() {
			glDeleteTextures(1, &texture);
		}
};
