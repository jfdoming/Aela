/*
* Class: MainMenu Display
* Author: Robert Ciborowski
* Date: 04/01/2019
* Description: A class used to display the main menu and give it functionality.
*/

#pragma once
#include "../../../Project Aela/Menus/Label.h"
#include "../../../Project Aela/Menus/ImageComponent.h"

namespace Game {
	class MainMenuDisplay {
		public:
			MainMenuDisplay();

			void setup();
			void update();
		
			void addOption(std::shared_ptr<Label> label, std::function<void()> onEnd);
			void setSelectorImage(std::shared_ptr<ImageComponent> selectorImage);
		
			// This is triggered on an event.
			void onEvent(Event* event);

			void disable();
			void enable();
		
		private:
			std::shared_ptr<ImageComponent> selectorImage;
			std::vector<std::shared_ptr<Label>> labels;
			std::vector<std::function<void()>> onSelects;

			bool pressedReturn = false;
			size_t currentOption = 0;
			const int SELECTOR_IMAGE_X_OFFSET = 5;
			std::atomic<bool> enabled = false;

			void selectNext();
			void selectPrevious();
			void updateSelectorImagePosition();
	};
}
