#pragma once

namespace Aela {
	enum class EventTypes {
		KEY_PRESSED, KEY_RELEASED, 
	};

	class Event {
		private:
			int type;
		public:
			Event(int type);
			virtual ~Event();

			int getType();
	};
}