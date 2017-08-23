/*
* Name: Project Aela's Skybox Class
* Author: Robert Ciborowski
* Date: June 2017
* Description: A class used by Aela's Renderer to store properties of a skybox.
*/

#pragma once
#include <glm/glm.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>
#include <string>

#include "../../Resource Management/Resource.h"

namespace Aela {
	class Skybox : public Resource {
	public:
		Skybox(std::string src);

		GLuint* getTexture();
		float* getVertices();

	private:
		GLuint texture;
		float vertices;
	};
}