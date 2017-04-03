#pragma once

#include "ResourceLoader.h"

#define AELA_RESOURCE_TEXTURE_HEADER_SIZE 128
#define AELA_RESOURCE_TEXTURE_HEADER_START "DDS "

namespace Aela {
	enum CompressionAlgorithms {
		FOURCC_DXT1 = 0x31545844, FOURCC_DXT3 = 0x33545844, FOURCC_DXT5 = 0x35545844
	};

	class TextureLoader : public ResourceLoader {
		public:
		bool isValid(std::ifstream &in);
		Resource* load(std::ifstream &in);
	};
}
