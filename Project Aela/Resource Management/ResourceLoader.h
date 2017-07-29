#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "../Lua/Exposable.h"
#include <unordered_map>

namespace Aela {
	class ResourceLoader {
		public:
			virtual bool load(std::unordered_map<std::string, Resource*>* resources, std::string src) = 0;

			std::string getErrorMessage();
		protected:
			std::string errorMessage;

			bool open(std::ifstream& in, std::string src);
	};
}