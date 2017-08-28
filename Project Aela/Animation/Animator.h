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
#include "../2D/Animation/KeyFrame2DList.h"
#include "../3D/Animation/KeyFrame3DList.h"
#include "../Time Manager/TimeManager.h"

using namespace Aela;

class Animator {
	public:
		Animator() {

		}

		// This function should be run every loop in the thread specified for animations.
		void update();

		// These are getters and setters.
		void setTimeManager(TimeManager* timeManager);
		TimeManager* getTimeManager();
		void addKeyFrame3DList(KeyFrame3DList* keyFrameList);
		std::vector<KeyFrame3DList>* get3DKeyFrames();
		void addKeyFrame2DList(KeyFrame2DList* keyFrameList);
		std::vector<KeyFrame2DList>* get2DKeyFrames();

	private:
		// These are the Aela objects that this class uses.
		TimeManager* timeManager;

		// These are basically the "tracks" in which the lists of keyframes are layed out. Note that 3D animation
		// uses a seperate track from 2D animation.
		std::vector<KeyFrame3DList> keyFrame3DLists;
		std::vector<KeyFrame2DList> keyFrame2DLists;

		// This stores the time since the most recently finished key frame.
		unsigned int timeSinceLastKeyFrame3DList = 0;
		unsigned int timeSinceLastKeyFrame2DList = 0;
};