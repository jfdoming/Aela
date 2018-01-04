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
#include "../2D/Animation/AnimationTrack2D.h"
#include "../3D/Animation/AnimationTrack3D.h"
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
		void addAnimationTrack3D(AnimationTrack3D* track);
		std::vector<AnimationTrack3D>* get3DKeyFrames();
		void addAnimationTrack2D(AnimationTrack2D* track);
		std::vector<AnimationTrack2D>* get2DKeyFrames();
		int delete3DTrackByTag(std::string tag), delete2DListsByTag(std::string tag), deleteListsByTag(std::string tag);
		bool trackWithTagExists(std::string tag);
		long long tracksWithTag(std::string tag);

	private:
		// These are the Aela objects that this class uses.
		TimeManager* timeManager;

		// These are basically the "tracks" in which the lists of keyframes are layed out. Note that 3D animation
		// uses a seperate track from 2D animation.
		std::vector<AnimationTrack3D> tracks3D;
		std::vector<AnimationTrack2D> tracks2D;
};