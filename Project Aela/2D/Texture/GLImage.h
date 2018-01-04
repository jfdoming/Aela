/*
* Class: Image
* Author: Julian Dominguez-Schatz, adapted from GLTexture (Robert Ciborowski)
* Date: 21/02/2017
* Description: A class used by Aela's Renderer to store properties of an Open texture.
*/

#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include "Image.h"

class GLImage : public Image {
	public:
		GLImage() {
			dimensions.setValues(0, 0, 0, 0);
		}

		virtual ~GLImage() {
			deleteImage();
		}

		void setTexture(unsigned int texture) {
			if (texture != 0) {
				deleteImage();
			}
			this->texture = texture;
		}

	private:
		void deleteImage() {
			glDeleteTextures(1, &texture);
		}
};
