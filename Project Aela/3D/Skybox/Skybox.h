#pragma once
#include <glm/glm.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

class Skybox {
	public:
		Skybox();

		GLuint* getTexture();
		void setTexture(GLuint* texture);
		float* getVertices();

	private:
		GLuint texture;
		float vertices;
};