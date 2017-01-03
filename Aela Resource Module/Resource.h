#pragma once

#include "stdafx.h"

#define AELA_RESOURCE_DEFAULT_BUFFER_SIZE 8192

namespace Aela {
	class AELA_EXPORT Resource {
		public:
		const char * src;

		Resource(const char * src);
		~Resource();
	};

	class AELA_EXPORT TextResource : public Resource {
		public:
		std::string data = "";

		TextResource(const char * src);
		~TextResource();

		char * getData() {
			data.c_str();
		}
	};

	class TextureResource : public Resource {
		public:
		GLuint data = 0;

		TextureResource(const char * src);
		~TextureResource();
	};
}