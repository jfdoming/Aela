#pragma once

#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "LuaScript.h"
#include "../3D/Entity/Entity3D.h"

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
