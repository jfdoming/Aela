#pragma once

#include "stdafx.h"

namespace Aela {
	class Resource {
		public:
		string src;

		Resource(string src);
		~Resource();
	};

	class TextResource : public Resource {
		public:
		string data = "";

		TextResource(string src);
		~TextResource();
	};

	class TextureResource : public Resource {
		public:
		GLuint data = 0;

		TextureResource(string src);
		~TextureResource();
	};
}