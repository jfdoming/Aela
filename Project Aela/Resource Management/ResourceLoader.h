#pragma once

#include "stdafx.h"
#include "Resource.h"
#include "../Lua/Exposable.h"

namespace Aela {
	class ResourceLoader {
		public:
			virtual Resource* load(std::ifstream& in) = 0;

			std::string getErrorMessage();
		protected:
			std::string errorMessage;
	};
}