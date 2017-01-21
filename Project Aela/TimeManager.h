#pragma once
#include "time.h"

class TimeManager {
	public:
		TimeManager() {
			updateTime();
		}
		
		void updateTime() {
			// This updates the time and the time difference between the current and the last frame.
			lastTime = currentTime;
			currentTime = clock();
			deltaTime = float(currentTime - lastTime);
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