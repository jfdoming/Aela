#include "LuaScript.h"
#include "../Error Handler/ErrorHandler.h"

using namespace luabridge;

Aela::LuaScript::LuaScript() {

}

Aela::LuaScript::~LuaScript() {
	if (L) lua_close(L);
}

void Aela::LuaScript::initLua(lua_State *_L) {
	L = _L;
}

int Aela::LuaScript::loadScript(std::string src) {
	const char * str = src.c_str();

	if (luaL_dofile(L, src.c_str())) {
		AelaErrorHandling::windowError("Lua Error: File IO", "Could not load " + src);
		return -1;
	}

	return 0;
}

int Aela::LuaScript::callFunction(std::string name) {
	LuaRef func = getGlobal(L, name.c_str());

	if (func.isNil()) {
		AelaErrorHandling::windowError("Lua Error: Nil Reference", "Function \"" + name + "\" is Nil");
		return -1;
	}

	func();

	return 0;
}