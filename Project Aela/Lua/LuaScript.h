#pragma once

#include <string>
#include "LuaManager.h"
#include "../Error Handler/ErrorHandler.h"

namespace Aela {
	class LuaScript {
		private:
			lua_State * L;
			public:
			LuaScript();
			~LuaScript();

			void initLua(lua_State *_L);
			int loadScript(std::string src);

			// Calls a function in lua that takes no params
			int callFunction(std::string name);

			// Calls a function in lua that takes parameters, up to 8 apparently
			template <typename T>
			int callFunction(std::string name, T args...) {
				luabridge::LuaRef func = luabridge::getGlobal(L, name.c_str());

				if (func.isNil()) {
					AelaErrorHandling::windowError("Lua Error: Nil Reference", "Function \"" + name + "\" is Nil");
					return -1;
				}

				func(args);

				return 0;
			}
	};
}