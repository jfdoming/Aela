#pragma once

#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "LuaScript.h"
#include "../Entities/Entity.h"

namespace Aela {
	class LuaManager {
		private:
			lua_State* L;
			public:
			LuaManager();
			~LuaManager();

			template <typename T>
			void exposeObject(T* obj, std::string name) {
				luabridge::push(L, obj);
				lua_setglobal(L, name.c_str());
			}

			lua_State* getLuaState();
	};
}