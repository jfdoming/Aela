/*
* Class: Dialogue Option
* Author: Robert Ciborowski
* Date: 04/01/2018
* Description: A class used to represent an option in the dialogue.
*/

#pragma once
#include <string>

namespace Game {
	class DialogueOption {
		public:
			DialogueOption(std::string text, std::string actionOnSelection) {
				this->text = text;
				this->actionOnSelection = actionOnSelection;
			}

			std::string getText() {
				return text;
			}
			std::string getActionOnSelection() {
				return actionOnSelection;
			}

		private:
			std::string text;
			std::string actionOnSelection;
	};
	
}