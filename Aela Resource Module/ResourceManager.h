#pragma once

#include "stdafx.h"
#include "Resource.h"
#include <unordered_map>

namespace Aela {
	class ResourceManager {
		public:
			MYLIBRARY_EXPORT ResourceManager(int resourceCount);
			MYLIBRARY_EXPORT ~ResourceManager();

			MYLIBRARY_EXPORT void loadText(string src);
			MYLIBRARY_EXPORT Resource obtain(string src);

			static const string TEXT_LOAD_PATH;
			static const string IMAGE_LOAD_PATH;
		private:
			std::unordered_map<string, Resource> resources;
	};
}