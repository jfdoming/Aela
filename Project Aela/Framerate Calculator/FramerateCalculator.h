/*
* Class: Framerate Calculator
* Author: Robert Ciborowski
* Date: 18/08/2017
* Description: A class used to keep track of framerate.
*/

#pragma once
#include <time.h>

class FramerateCalculator {
	public:
		FramerateCalculator() {

		}

		void calculate(clock_t currentTime, float timeBetweenFrames);

		// These are getters and setters.
		float getTrueFPS(), getSmoothedFPS(), getFPSSmoothingAmount();
		void setFPSSmoothing(float fpsSmoothingAmount), setTimeBetweenFrameChecks(unsigned int timeBetweenFrameChecks);

	private:
		float trueFPS = -1, smoothedFPS = -1, fpsSmoothingAmount = 0.9f;
		unsigned int timeBetweenFrameChecks = 250;
		clock_t timeOfLastFrameCheck = 0;
};