#pragma once
#include "Event.h"

namespace Aela {
	class Listener {
		public:
			virtual void onEvent(Event* event) = 0;
	};
}