#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include "../../Resource Management/ResourceLoader.h"

#define AELA_RESOURCE_TEXTURE_HEADER_SIZE 128
#define AELA_RESOURCE_TEXTURE_HEADER_START "DDS "

namespace Aela {
	enum class CompressionAlgorithms : unsigned int {
		FOURCC_DXT1 = 0x31545844, FOURCC_DXT3 = 0x33545844, FOURCC_DXT5 = 0x35545844
	};

	class TextureLoader : public ResourceLoader, public Exposable {
		public:
			TextureLoader();
			virtual ~TextureLoader();

			virtual void expose(LuaManager& mgr);

			virtual bool load(std::unordered_map<std::string, Resource*>* resources, std::string src);

		protected:
			void loadTexture(std::ifstream& in, GLuint* texID, GLenum target);
			void loadTexture(std::ifstream& in, GLuint* texID, GLenum target, unsigned int* width, unsigned int* height);
	};
}
