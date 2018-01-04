/*
* Class: Project Aela's Animation Track
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class that stores 2D keyframes.
*/

#pragma once
#include <vector>
#include <utility>

template <class T> class AnimationTrack {
	public:
		AnimationTrack() {}

		void updatePositionInTrack(long long timeToAdd) {
			positionInTrack += timeToAdd;
		}

		void resetPosition() {
			positionInTrack = 0;
		}

		long long getPositionInTrack() {
			return positionInTrack;
		}

		bool addKeyFrame(long long time, T* keyFrame) {
			if (time <= positionInTrack) {
				// Nice try, but your time has passed.
				return false;
			}

			for (auto& pair : keyFrames) {
				if (pair.first > time) {
					keyFrames.push_back(std::pair<long long, T>(time, *keyFrame));
					return true;
				}
			}

			keyFrames.push_back(std::pair<long long, T>(time, *keyFrame));
			return true;
		}

		bool addKeyFrameUsingMillis(long long timeInMillis, T* keyFrame) {
			return addKeyFrame(timeInMillis * 1000000, keyFrame);
		}

		bool addKeyFrameUsingSeconds(long long timeInSeconds, T* keyFrame) {
			return addKeyFrame(timeInMillis * 1000000000, keyFrame);
		}

		std::vector<std::pair<long long, T>>* getKeyFrames() {
			return &keyFrames;
		}

		void setTag(std::string tag) {
			this->tag = tag;
		}

		std::string getTag() {
			return tag;
		}

	protected:
		// This specifies the time that this key frame list should fire after the previous one.
		long long positionInTrack = 0;

		// This is the list of KeyFrames.
		std::vector<std::pair<long long, T>> keyFrames;

		// This is used in order to let the Animator perform actions upon this list. For example, if something tells the Animator
		// to delete all tracks with the tag "camera_animation" in order to get rid of all camera animations and this list has that
		// exact tag, then this list will be deleted.
		std::string tag = "list";
};