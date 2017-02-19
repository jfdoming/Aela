#pragma once
#include "time.h"

// The use of CLOCKS_PER_SECOND (that you get from time.h) is unimplemented! Implement this in order to
// get time accurately!!!!
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