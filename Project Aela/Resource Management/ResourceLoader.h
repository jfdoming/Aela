#pragma once

#include "stdafx.h"
#include "Resource.h"

namespace Aela {
	class ResourceLoader {
		public:
		virtual bool isValid(std::ifstream &in);

		virtual Resource* load(std::ifstream &in) = 0;

		// potentially useful functions for reading from a stream
		std::string read(std::ifstream &in, int count);
	};
}