#pragma once

#include "Event.h"

namespace Aela {
	class KeyEvent : public Event {
		private:
			const int ID = 1;
		public:
			KeyEvent();
			~KeyEvent();
	};
}