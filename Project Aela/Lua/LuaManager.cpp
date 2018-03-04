#include "LuaManager.h"
#include <iostream>

using namespace luabridge;

Aela::LuaManager::LuaManager() {
	L = luaL_newstate();

	if (L) luaL_openlibs(L);
}

Aela::LuaManager::~LuaManager() {

}

lua_State* Aela::LuaManager::getLuaState() {
	return L;
}
