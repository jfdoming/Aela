/*
* Class: Dialogue Handler
* Author: Robert Ciborowski
* Date: 04/01/2018
* Description: A class used to handle in-game dialogue.
*/

#pragma once
#include "../Scripts/ScriptManager.h"
#include "Time Manager\TimeManager.h"
#include "Menus/SubMenu.h"
#include "Menus/Label.h"
#include "DialogueOption.h"

namespace Game {
	class DialogueHandler {
		public:
			DialogueHandler() {}

			void setup(TimeManager* timeManager, ScriptManager* scriptManager);
			void update();
		
			void showDialogue(std::string text, std::string scriptToRunOnceComplete);
			void showOptions(DialogueOption option1, DialogueOption option2), 
				showOptions(DialogueOption option1, DialogueOption option2, DialogueOption option3),
				showOptions(DialogueOption option1, DialogueOption option2, DialogueOption option3, DialogueOption option4);
			void closeDialog();

			void setDialogueSubMenu(std::shared_ptr<SubMenu> dialogueSubMenu);
			void setDialogueLabels(std::shared_ptr<Label> label1, std::shared_ptr<Label> label2);
		private:
			TimeManager* timeManager;
			ScriptManager* scriptManager;
			std::shared_ptr<SubMenu> dialogueSubMenu;
			std::shared_ptr<Label> label1, label2;

			// This is the max amount of characters that the handler will display per row of dialogue.
			const size_t MAX_CHARACTERS_PER_LINE = 46;

			// These are the lines that are to be displayed.
			std::string line1OfText = "", line2OfText = "";

			// This is the position (unit = characters) that the dialogue scrolling/appearing is at.
			size_t positionInDialogue = 0;
			
			// These store variables related to text scrolling/appearing and time.
			long long timeSinceNewCharacter;
			long long timeBetweenCharacterReveals = 10000000;
	};
}