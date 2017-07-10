/*
* Class: Simple 2D Framebuffer
* Author: Robert Ciborowski
* Date: 04/07/2017
* Description: A class used by Aela's 2D Renderer to represent a 2D frame buffer.
*/

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>
#include "../../3D/Texture/Texture.h"

class Simple2DFramebuffer {
	public:
		Simple2DFramebuffer() {

		}

		GLuint* getFramebuffer();
		void setFramebuffer(GLuint* framebuffer);
		GLuint* getMultisampledFramebuffer();
		void setMultisampledFramebuffer(GLuint* framebuffer);
		Texture* getFramebufferTexture();
		Texture* getMultisampledFramebufferTexture();
		unsigned int getMultisampling();
		void setMultisampling(unsigned int multisampling);

	private:
		GLuint framebuffer, multisampledFramebuffer;
		Texture framebufferTexture, multisampledFramebufferTexture;
		unsigned int multisampling = 0;
};