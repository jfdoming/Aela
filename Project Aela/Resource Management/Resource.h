/*
* Class: Resource
* Author: Julian Dominguez-Schatz
* Date: 01/01/2017
* Description: Keeps track of the scene system.
*/

#pragma once

#include "stdafx.h"
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>

namespace Aela {
	class Resource {
		public:

		Resource();
		~Resource();
	};

	class TextureResource : public Resource {
	public:
		GLuint data = 0;

		TextureResource();
		~TextureResource();
	};

	class OBJResource : public Resource {
	public:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> UVs;
		std::vector<glm::vec3> normals;

		OBJResource();
		~OBJResource();
	};
}