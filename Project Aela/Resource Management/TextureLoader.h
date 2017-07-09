#pragma once

#include "ResourceLoader.h"

#define AELA_RESOURCE_TEXTURE_HEADER_SIZE 128
#define AELA_RESOURCE_TEXTURE_HEADER_START "DDS "

namespace Aela {
	enum class CompressionAlgorithms : unsigned int {
		FOURCC_DXT1 = 0x31545844, FOURCC_DXT3 = 0x33545844, FOURCC_DXT5 = 0x35545844
	};

	class TextureLoader : public ResourceLoader, public Exposable {
		public:
			static TextureLoader& getInstance() {
				// This is guaranteed to be destroyed, and instantiated on first use.
				static TextureLoader instance;
				return instance;
			}

			void expose(LuaManager& mgr);

			TextureLoader(TextureLoader const&) = delete;
			void operator=(TextureLoader const&) = delete;

			virtual Resource* load(std::ifstream& in);
	private:
		TextureLoader() {}
	};
}
