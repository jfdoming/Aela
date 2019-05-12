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
			void clearAndDisplayHint(std::string text, HintDisplayDuration hintDisplayDuration);
			void clearAndDisplayHint(std::string text, HintDisplayDuration hintDisplayDuration, ColourRGBA* colour);
			void clear();

			// This will display the next hint when this key is pressed. If the key is pressed again, nothing will happen.
			void displayMostRecentHintUntilKeyPress(int keycode);

			// This will display the next hint on any one of these keypresses. If any of the keys are pressed again,
			// nothing will happen.
			void displayMostRecentHintUntilKeyPresses(std::initializer_list<int> keycodes);

			void setHintLabel(std::shared_ptr<Label> hintLabel);
			void setHintBackdrop(std::shared_ptr<ImageComponent> hintBackdrop);
			std::string getCurrentHint();
			void setSpacing(int spacing);
			bool isDisplayingHint();

			// This is triggered on an event.
			void onEvent(Event* event);

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

			int spacing = 0;

			bool displayingHint = false;
			std::atomic<bool> blockUntilKey = false;
			std::vector<std::vector<int>> keycodesToClearOn;

			void processNextHint();
			inline long long getDisplayDurationInMillis(HintDisplayDuration hintDisplayDuration);
	};
}