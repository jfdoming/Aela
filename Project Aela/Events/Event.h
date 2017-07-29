#pragma once

namespace Aela {
	class Event {
		private:
			int type;
		public:
			Event(int _type);
			virtual ~Event();

			int getType();
	};
}