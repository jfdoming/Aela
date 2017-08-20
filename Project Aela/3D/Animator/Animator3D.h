/*
* Class: Project Aela's 3D Animator
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*              Note: The position of the point used when rotating a Transformable3D using
*              PointRotation3D is relative to the Transformable3D (as opposed to being relative
*              to (0, 0, 0) in worldspace).
*/

#pragma once

#include <vector>

#include "KeyFrame3DList.h"
#include "../../Time Manager/TimeManager.h"

using namespace Aela;

class Animator3D {
	public:
		Animator3D() {

		}

		// This function should be run every loop in the thread specified for animations.
		void update();

		// These are getters and setters.
		void setTimeManager(TimeManager* timeManager);
		TimeManager* getTimeManager();
		void addKeyFrameList(KeyFrame3DList* keyFrameList);
		std::vector<KeyFrame3DList>* getKeyFrames();

	private:
		// These are the Aela objects that this class uses.
		TimeManager* timeManager;
		std::vector<KeyFrame3DList> keyFrameLists;

		// This stores the time since the most recently finished key frame.
		unsigned int timeSinceLastKeyFrameList = 0;
};