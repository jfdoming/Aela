#pragma once

#include "Event.h"

namespace Aela {
	class MouseEvent : public Event {
	private:
		int button;
		int modifier;
		int clicks;
		int x, y;
	public:
		MouseEvent(int _type, int _button, int _modifier, int _clicks, int _x, int _y);
		virtual ~MouseEvent();

		int getButton();
		int getModifier();
		int getClicks();
		int getMouseX();
		int getMouseY();
	};
}