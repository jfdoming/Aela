#pragma once

#include "ResourceLoader.h"

namespace Aela {
	class TextureLoader : public ResourceLoader {
		public:
		bool isValid(std::ifstream &in);
		Resource* load(std::ifstream &in);
	};
}