/*
* Class: Project Aela's 2D Key Frame List
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class that stores 2D keyframes.
*/

#pragma once

#include <vector>
#include "../../Animation/KeyFrameList.h"
#include "KeyFrame2D.h"

class KeyFrame2DList : public KeyFrameList<KeyFrame2D> {
	public:
		KeyFrame2DList() {

		}

		std::vector<ColourRGBA>* getOriginalTints();
		void storeOriginalTransformations();

	private:
		// This stores the original properties of an object before it was modified by the key frames.
		std::vector<ColourRGBA> originalTints;
};