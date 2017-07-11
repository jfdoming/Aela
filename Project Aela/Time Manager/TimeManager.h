/*
* Class: Time Manager
* Author: Robert Ciborowski
* Date: November 2016
* Description: A header-only class for time management.
*/

#pragma once
#include "time.h"
#include <iostream>

namespace Aela {
	class TimeManager {
		public:
		TimeManager() {
			updateTime();
		}

		void updateTime() {
			// This updates the time and the time difference between the current and the last frame.
			lastTime = currentTime;
			currentTime = clock();
			deltaTime = float(currentTime - lastTime) * (CLOCKS_PER_SEC / 1000);
		}

		clock_t getCurrentTime() {
			return currentTime;
		}

		clock_t getLastFrameTime() {
			return lastTime;
		}

		float getTimeBetweenFrames() {
			return deltaTime;
		}


		private:
		clock_t currentTime, lastTime;
		float deltaTime;
	};
}
