#include "EventConstants.h"

using namespace Aela;

void EventConstants::expose(LuaManager& mgr) {
	luabridge::getGlobalNamespace(mgr.getLuaState())
		.beginClass<EventConstants>("EventConstants")
		.addStaticData("KEY_PRESSED", const_cast<int*>(&EventConstants::KEY_PRESSED), false)
		.addStaticData("KEY_RELEASED", const_cast<int*>(&EventConstants::KEY_RELEASED), false)
		.addStaticData("MOUSE_PRESSED", const_cast<int*>(&EventConstants::MOUSE_PRESSED), false)
		.addStaticData("MOUSE_RELEASED", const_cast<int*>(&EventConstants::MOUSE_RELEASED), false)
		.addStaticData("MOUSE_MOTIONED", const_cast<int*>(&EventConstants::MOUSE_MOTIONED), false)
		.addStaticData("MOUSE_ENTERED", const_cast<int*>(&EventConstants::MOUSE_ENTERED), false)
		.addStaticData("MOUSE_EXITED", const_cast<int*>(&EventConstants::MOUSE_EXITED), false)

		.addStaticData("MOUSE_ENTERED_COMPONENT", const_cast<int*>(&EventConstants::MOUSE_ENTERED_COMPONENT), false)
		.addStaticData("MOUSE_EXITED_COMPONENT", const_cast<int*>(&EventConstants::MOUSE_EXITED_COMPONENT), false)
		.endClass();
}
