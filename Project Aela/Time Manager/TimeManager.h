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

			long long getCurrentTimeInNanos() {
				return std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime.time_since_epoch()).count();
			}

			long long getCurrentTimeInMicros() {
				return std::chrono::duration_cast<std::chrono::microseconds>(currentTime.time_since_epoch()).count();
			}

			long long getCurrentTimeInMillis() {
				return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();
			}

			long long getLastFrameTimeInNanos() {
				return std::chrono::duration_cast<std::chrono::nanoseconds>(lastTime.time_since_epoch()).count();
			}

			long long getLastFrameTimeInMicro() {
				return std::chrono::duration_cast<std::chrono::microseconds>(lastTime.time_since_epoch()).count();
			}

			long long getLastFrameTimeInMillis() {
				return std::chrono::duration_cast<std::chrono::milliseconds>(lastTime.time_since_epoch()).count();
			}

			long long getTimeBetweenFramesInNanos() {
				return deltaTime;
			}

			long long getTimeBetweenFramesInMicros() {
				return deltaTime / 1000;
			}

			long long getTimeBetweenFramesInMillis() {
				return deltaTime / 1000000;
			}

		private:
			time_point currentTime, lastTime;
			long long deltaTime;
	};
}
