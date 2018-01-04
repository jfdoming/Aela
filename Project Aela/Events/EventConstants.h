/*
* Class: Aela Game
* Author: Waseef Nayeem
* Date: We would know if Waseef put a header when he made this.
* Description: A class used to hold contansts.
*/

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
			// "MOUSE_MOVED" would be better, but Visual Studio interprets that as a macro or something,
			// meaning that projects which use Aela won't compile!
			static const int MOUSE_MOTIONED = 4;
			static const int MOUSE_ENTERED = 5;
			static const int MOUSE_EXITED = 6;

			static const int MOUSE_ENTERED_COMPONENT = 7;
			static const int MOUSE_EXITED_COMPONENT = 8;
			static const int MOUSE_WHEEL = 9;

			virtual void expose(LuaManager& mgr);
	};
}
