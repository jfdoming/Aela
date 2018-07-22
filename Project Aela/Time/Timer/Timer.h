/*
* Class: Timer
* Author: Robert Ciborowski
* Date: 20/03/2018
* Description: A class for timed events.
*/

#pragma once
#include "../Time.h"
#include <vector>
#include <functional>

namespace Aela {
	class Timer {
		public:
			Timer(Time* time);

			void update();

			void scheduleEventInNanos(long long waitTime, std::function<void()> event);
			void scheduleEventInMicros(long long waitTime, std::function<void()> event);
			void scheduleEventInMillis(long long waitTime, std::function<void()> event);
			void scheduleEventInSeconds(long long waitTime, std::function<void()> event);

		private:
			Time* time;

			std::vector<std::pair<long long, std::function<void()>>> timedEvents;
	};
}
