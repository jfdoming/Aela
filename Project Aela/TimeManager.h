/*
* Name: Project Aela's Time Manager
* Author: Ekkon Games
* Date: November 2016
* Description: A header-only class for time management.
*/

#pragma once
#include "time.h"

// WARNING! PLEASE READ!
// The use of CLOCKS_PER_SECOND (that you get from time.h) is unimplemented! Implement this in order to
// get time accurately!!!! Or use chronos.

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
