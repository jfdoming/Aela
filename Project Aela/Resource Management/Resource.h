#pragma once

#include "stdafx.h"
#include <GL/glew.h>
#include <GLM/glm.hpp>

#define AELA_RESOURCE_DEFAULT_BUFFER_SIZE 8192

namespace Aela {
	class Resource {
		public:
		std::string src;

		Resource(std::string src);
		~Resource();
	};

	class TextureResource : public Resource {
	public:
		GLuint data = 0;

		TextureResource(std::string src);
		~TextureResource();
	};

	class OBJResource : public Resource {
	public:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> UVs;
		std::vector<glm::vec3> normals;

		OBJResource(std::string src);
		~OBJResource();
	};
}