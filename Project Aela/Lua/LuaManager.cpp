#include "LuaManager.h"
#include <iostream>

using namespace luabridge;

LuaManager::LuaManager() {
	L = luaL_newstate();

	if (L) luaL_openlibs(L);
}

LuaManager::~LuaManager() {

}

lua_State* LuaManager::getLuaState() {
	return L;
}
