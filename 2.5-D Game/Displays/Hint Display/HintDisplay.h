/*
* Class: Hint Display
* Author: Robert Ciborowski
* Date: 11/07/2018
* Description: A class used to display game-related hints.
*/

#pragma once
#include "HintDisplayDuration.h"
#include "../../Game Object Provider/GameObjectProvider.h"
#include <string>

namespace Game {
	class HintDisplay {
		public:
			HintDisplay();

			void displayHint(std::string text, HintDisplayDuration hintDisplayDuration);
			void displayHint(std::string text, HintDisplayDuration hintDisplayDuration, ColourRGBA* colour);
			void hide();

			void setHintLabel(std::shared_ptr<Label> hintLabel);

		private:
			Timer* timer;
			std::shared_ptr<Label> hintLabel;
			ColourRGBA defaultColour;

			inline long long getDisplayDurationInMillis(HintDisplayDuration hintDisplayDuration);
	};
}