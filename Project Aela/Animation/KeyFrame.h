/*
* Class: Project Aela's Key Frame
* Author: Robert Ciborowski
* Date: 27/08/2017
* Description: A class used for keyframing with the animator.
*/

#pragma once
#include <functional>

namespace Aela {
	enum class KeyFrameType {
		GENERIC, TWO_DIMENSIONAL, THREE_DIMENSIONAL, MATERIAL, MODEL
	};

	class KeyFrame {
		public:
			KeyFrame() {}

			virtual KeyFrameType getType() {
				return KeyFrameType::GENERIC;
			}

			virtual void start();
			bool hasBeenStarted();

			void setEndingAction(std::function<void()> endingAction);
			std::function<void()> getEndingAction();

		protected:
			// A keyframe can run an std::function after its life has been completed.
			std::function<void()> endingAction = nullptr;

			bool started = false;
	};
}