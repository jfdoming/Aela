#pragma once

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include "../../Resource Management/ResourceLoader.h"
#include "../../2D/Texture/Texture.h"

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

			virtual bool load(ResourceMap& resources, std::string src);

		protected:
			bool loadTexture(Texture*& result, std::string src);
			bool loadTextureToBoundId(std::ifstream& in, GLenum target);
			bool loadTextureToBoundId(std::ifstream& in, GLenum target, unsigned int* width, unsigned int* height);
			bool loadTextureUsingFILE(std::string path, GLenum target);
			bool loadTextureUsingFILE(std::string path, GLenum target, unsigned int* width, unsigned int* height);

		private:
			const std::string defaultTexturePath = "res/textures/";
	};
}
