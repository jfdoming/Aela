#pragma once
#include "../Lua/Exposable.h"
#include "../Lua/LuaManager.h"

namespace Aela {
	class EventConstants : Exposable {
		public:
			static const int KEY_PRESSED = 0;
			static const int KEY_RELEASED = 1;
			static const int MOUSE_PRESSED = 2;
			static const int MOUSE_RELEASED = 3;

			void expose(LuaManager& mgr);
	};
}