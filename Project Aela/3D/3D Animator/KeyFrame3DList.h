/*
* Class: Project Aela's 3D Key Frame List
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class that stores 3D keyframes.
*/

#pragma once

#include <vector>

#include "KeyFrame3D.h"

class KeyFrame3DList {
	public:
		KeyFrame3DList() {

		}

		void setTimeAfterPreviousKeyFrame(unsigned int time);
		unsigned int getTimeAfterPreviousKeyFrame();
		void addKeyFrame(KeyFrame3D* keyFrame);
		std::vector<KeyFrame3D>* getKeyFrames();
		std::vector<glm::vec3>* getOriginalPositions();
		std::vector<glm::vec3>* getOriginalRotations();
		void storeOriginalTransformations();

	private:
		// This specifies the time that this key frame list should fire after the previous one.
		unsigned int timeAfterPreviousKeyFrame = 0;

		std::vector<KeyFrame3D> keyFrames;

		// This stores the original properties of an object before it was modified by the key frames.
		std::vector<glm::vec3> originalPositions;
		std::vector<glm::vec3> originalRotations;
};