#pragma once
#include "LuaManager.h"

class Exposable {
	public:
		virtual void expose(LuaManager& mgr) = 0;
};