#pragma once

#include "stdafx.h"
#include "Resource.h"

namespace Aela {
	class ResourceLoader {
	public:
		virtual bool isValid(std::ifstream& in);
		virtual Resource* load(std::ifstream& in) = 0;
	};
}