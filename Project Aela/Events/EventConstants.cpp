#include "EventConstants.h"

using namespace Aela;

void EventConstants::expose(LuaManager& mgr) {
	luabridge::getGlobalNamespace(mgr.getLuaState())
		.beginClass<EventConstants>("EventConstants")
		.addStaticData("KEY_PRESSED", const_cast<int*>(&EventConstants::KEY_PRESSED), false)
		.addStaticData("KEY_RELEASED", const_cast<int*>(&EventConstants::KEY_RELEASED), false)
		.addStaticData("MOUSE_PRESSED", const_cast<int*>(&EventConstants::MOUSE_PRESSED), false)
		.addStaticData("MOUSE_RELEASED", const_cast<int*>(&EventConstants::MOUSE_RELEASED), false)
		.addStaticData("MOUSE_MOVED", const_cast<int*>(&EventConstants::MOUSE_MOVED), false)
		.endClass();
}