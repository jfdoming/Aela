#pragma once

#include "stdafx.h"
#include <GL/glew.h>

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
}