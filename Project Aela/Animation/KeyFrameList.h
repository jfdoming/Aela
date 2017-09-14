/*
* Class: Project Aela's Frame List
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class that stores 2D keyframes.
*/

#pragma once

#include <vector>

template <class T> class KeyFrameList {
	public:
		KeyFrameList() {

		}

		void setTimeAfterPreviousKeyFrame(unsigned int time) {
			if (time != 0) {
				timeAfterPreviousKeyFrame = time;
			}
		}

		void setTimeAfterPreviousKeyFrameInMillis(unsigned int time) {
			if (time != 0) {
				timeAfterPreviousKeyFrame = time * 1000000;
			}
		}

		unsigned int getTimeAfterPreviousKeyFrame() {
			return timeAfterPreviousKeyFrame;
		}

		void addKeyFrame(T* keyFrame) {
			keyFrames.push_back(*keyFrame);
		}

		std::vector<T>* getKeyFrames() {
			return &keyFrames;
		}

		void setTag(std::string tag) {
			this->tag = tag;
		}

		std::string getTag() {
			return tag;
		}

		// This function is meant to store the original properties of an object. See examples of this in KeyFrameList3D
		// and KeyFrameList2D for more.
		virtual void storeOriginalTransformations() = 0;

	protected:
		// This specifies the time that this key frame list should fire after the previous one.
		unsigned int timeAfterPreviousKeyFrame = 0;

		// This is the list of KeyFrames.
		std::vector<T> keyFrames;

		// This is used in order to let the Animator perform actions upon this list. For example, if something tells the Animator
		// to delete all lists with the tag "camera_animation" in order to get rid of all camera animations and this list has that
		// exact tag, then this list will be deleted.
		std::string tag = "list";
};