/*
* Class: Texture
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
#include "../../Resource Management/Resource.h"
#include "Image.h"

class Texture : public Aela::Resource {
	public:
		Texture(std::string src, GLuint& texId) : Resource(src) {
			image.setTexture(texId);
		}

		virtual ~Texture(){
		}

		// These are some getters and setters.
		void setDimensions(Rect<int>* dimensions) {
			image.setDimensions(dimensions);
		}

		void setDimensions(int x, int y, int width, int height) {
			image.setDimensions(x, y, width, height);
		}

		void setTexture(GLuint texture) {
			image.setTexture(texture);
		}

		Rect<int>* getDimensions() {
			return image.getDimensions();
		}

		Image* getImage() {
			return &image;
		}

		GLuint* getTexture() {
			return image.getTexture();
		}

		// This function returns if the object was initialised properly. If it is not
		// initialised then rendering propblems will occur. Note that a Texture is still
		// initialised if the output (dimensions on the screen) has a width and/or height
		// of zero.
		bool isInitialised() {
			return image.isInitialised();
		}

	private:
		Image image;
};
