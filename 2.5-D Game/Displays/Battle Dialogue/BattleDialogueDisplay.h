/*
* Class: Battle Dialogue Display
* Author: Robert Ciborowski
* Date: 14/01/2019
* Description: A class used to handle battle dialogue.
*/

#pragma once

#include "../Multi Line Text Display/MultiLineTextDisplay.h"
#include "../Avatar/AvatarDisplay.h"

namespace Game {
	class BattleDialogueDisplay : public MultiLineTextDisplay, public AvatarDisplay {
		public:
			BattleDialogueDisplay();
			virtual ~BattleDialogueDisplay();

			void setup() override;
			void update() override;

			void showDialogue(std::string name, std::string text, std::string scriptToRunOnceComplete, bool sound);
			void showDialogue(std::string name, std::string text, std::string scriptToRunOnceComplete) override;
			void closeDialogue();

			void setSubMenu(std::shared_ptr<SubMenu> subMenu);

		private:
			std::shared_ptr<SubMenu> subMenu;
	};
}
