/*
* Class: Time Manager
* Author: Robert Ciborowski
* Date: November 2016
* Description: A header-only class for time management.
*/

#pragma once
#include <chrono>
#include <iostream>

using steady_clock = std::chrono::steady_clock;
using time_point = std::chrono::time_point<steady_clock>;

namespace Aela {
	class TimeManager {
		public:
			TimeManager() {
				updateTime();
			}

			void updateTime() {
				// This updates the time and the time difference between the current and the last frame.
				lastTime = currentTime;
				currentTime = steady_clock::now();
				deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
			}

			long long getCurrentTime() {
				return std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime.time_since_epoch()).count();
			}

			long long getLastFrameTime() {
				return std::chrono::duration_cast<std::chrono::nanoseconds>(lastTime.time_since_epoch()).count();
			}

			long long getTimeBetweenFrames() {
				return deltaTime;
			}

		private:
			time_point currentTime, lastTime;
			long long deltaTime;
	};
}
