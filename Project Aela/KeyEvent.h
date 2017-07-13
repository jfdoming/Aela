#pragma once

#include "Event.h"

namespace Aela {
	class KeyEvent : public Event {
		private:

		public:
			KeyEvent(int _type) : Event(_type);
	};
}