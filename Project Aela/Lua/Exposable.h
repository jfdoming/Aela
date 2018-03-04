#pragma once
#include "LuaManager.h"

namespace Aela {
	class Exposable {
		public:
			virtual void expose(LuaManager& mgr) = 0;
	};
}