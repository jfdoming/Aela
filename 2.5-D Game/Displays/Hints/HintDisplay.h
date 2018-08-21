/*
* Class: Hint Display
* Author: Robert Ciborowski
* Date: 11/07/2018
* Description: A class used to display game-related hints.
*/

#pragma once
#include "HintDisplayDuration.h"
#include "../../Game Object Provider/GameObjectProvider.h"
#include "../../../Project Aela/Menus/ImageComponent.h"
#include <string>

namespace Game {
	class HintDisplay {
		public:
			HintDisplay();

			void displayHint(std::string text, HintDisplayDuration hintDisplayDuration);
			void displayHint(std::string text, HintDisplayDuration hintDisplayDuration, ColourRGBA* colour);
			void clear();

			void setHintLabel(std::shared_ptr<Label> hintLabel);
			void setHintBackdrop(std::shared_ptr<ImageComponent> hintBackdrop);
			std::string getCurrentHint();

		private:
			struct Hint {
				std::string text;
				HintDisplayDuration duration;
				ColourRGBA colour;
			};

			Timer* timer;
			std::shared_ptr<Label> hintLabel;
			std::shared_ptr<ImageComponent> hintBackdrop;
			ColourRGBA defaultColour;
			std::queue<Hint> hintQueue;

			bool displayingHint = false;

			void processNextHint();
			inline long long getDisplayDurationInMillis(HintDisplayDuration hintDisplayDuration);
	};
}