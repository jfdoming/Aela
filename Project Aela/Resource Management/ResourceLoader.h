#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "../Lua/Exposable.h"

namespace Aela {
	class ResourceLoader {
		public:
			virtual Resource* load(std::string src) = 0;

			std::string getErrorMessage();
		protected:
			std::string errorMessage;

			bool open(std::ifstream& in, std::string src);
	};
}