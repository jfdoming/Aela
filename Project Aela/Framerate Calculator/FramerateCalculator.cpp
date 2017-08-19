/*
* Class: Framerate Calculator
* Author: Robert Ciborowski
* Date: 18/08/2017
* Description: A class used to keep track of framerate.
*/

#include "FramerateCalculator.h"

void FramerateCalculator::calculate(clock_t currentTime, float timeBetweenFrames) {
	// This does some simple math for framerate calculating.
	if (currentTime - timeOfLastFrameCheck >= timeBetweenFrameChecks) {
		if (trueFPS == -1 && timeBetweenFrames != 0) {
			trueFPS = 1000.0f / timeBetweenFrames;
			smoothedFPS = trueFPS;
		} else if (timeBetweenFrames != 0) {
			trueFPS = 1000.0f / timeBetweenFrames;
			smoothedFPS = (smoothedFPS * fpsSmoothingAmount) + ((1000.0f / timeBetweenFrames) * (1.0f - fpsSmoothingAmount));
			timeOfLastFrameCheck = currentTime;
		} else {
			// timeBetweenFrames = 0? Whoa, your computer is THAT fast? If you're really that rich, buy me a new PC!
			trueFPS = 1000;
			smoothedFPS = (smoothedFPS * fpsSmoothingAmount) + (1000.0f * (1.0f - fpsSmoothingAmount));
			timeOfLastFrameCheck = currentTime;
		}
	}
}

float FramerateCalculator::getTrueFPS() {
	return trueFPS;
}

float FramerateCalculator::getSmoothedFPS() {
	return trueFPS;
}

float FramerateCalculator::getFPSSmoothingAmount() {
	return fpsSmoothingAmount;
}

void FramerateCalculator::setFPSSmoothing(float fpsSmoothingAmount) {
	this->fpsSmoothingAmount = fpsSmoothingAmount;
}

void FramerateCalculator::setTimeBetweenFrameChecks(unsigned int timeBetweenFrameChecks) {
	this->timeBetweenFrameChecks = timeBetweenFrameChecks;
}