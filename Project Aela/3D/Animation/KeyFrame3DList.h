/*
* Class: Project Aela's 3D Key Frame List
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class that stores 3D keyframes.
*/

#pragma once

#include <vector>
#include "../../Animation/KeyFrameList.h"
#include "KeyFrame3D.h"

class KeyFrame3DList : public KeyFrameList<KeyFrame3D> {
	public:
		KeyFrame3DList() {

		}

		std::vector<glm::vec3>* getOriginalPositions();
		std::vector<glm::vec3>* getOriginalRotations();
		std::vector<glm::vec3>* getOriginalScalings();
		void storeOriginalTransformations();

	private:
		// This stores the original properties of an object before it was modified by the key frames.
		std::vector<glm::vec3> originalPositions;
		std::vector<glm::vec3> originalRotations;
		std::vector<glm::vec3> originalScalings;
};