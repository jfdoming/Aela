/*
* Class: Timer
* Author: Robert Ciborowski
* Date: 20/03/2018
* Description: A class for timed events.
*/

#pragma once
#include "../Time.h"
#include <string>
#include <unordered_map>
#include <functional>

namespace Aela {
	class Timer {
		public:
			Timer(Time* time);

			void update();

			std::string scheduleEventInNanos(long long waitTime, std::function<void()> event);
			std::string scheduleEventInMicros(long long waitTime, std::function<void()> event);
			std::string scheduleEventInMillis(long long waitTime, std::function<void()> event);
			std::string scheduleEventInSeconds(long long waitTime, std::function<void()> event);

			void scheduleEventInNanos(std::string tag, long long waitTime, std::function<void()> event);
			void scheduleEventInMicros(std::string tag, long long waitTime, std::function<void()> event);
			void scheduleEventInMillis(std::string tag, long long waitTime, std::function<void()> event);
			void scheduleEventInSeconds(std::string tag, long long waitTime, std::function<void()> event);

			bool removeEvent(std::string tag);

		private:
			struct TimedEvent {
				long long time;
				std::function<void()> event;
				std::string tag;
			};

			Time* time;

			std::vector<TimedEvent> timedEvents;
	};
}
