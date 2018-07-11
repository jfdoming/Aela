/*
* Class: Dialogue Handler
* Author: Robert Ciborowski
* Date: 04/01/2018
* Description: A class used to handle in-game dialogue.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "Menus/SubMenu.h"
#include "Menus/Label.h"
#include "DialogueOption.h"

namespace Game {
	enum class DialogueState {
		HIDDEN, MESSAGE, OPTIONS
	};

	class DialogueDisplay {
		public:
			DialogueDisplay();

			void setup();
			void update();

			// This is triggered on an event.
			void onEvent(Event* event);
		
			void showDialogue(std::string text, std::string scriptToRunOnceComplete);
			void showOptions(DialogueOption* option1, DialogueOption* option2), 
				showOptions(DialogueOption* option1, DialogueOption* option2, DialogueOption* option3),
				showOptions(DialogueOption* option1, DialogueOption* option2, DialogueOption* option3, DialogueOption* option4);
			void closeDialog();

			void setDialogueSubMenu(std::shared_ptr<SubMenu> dialogueSubMenu);
			void setDialogueLabels(std::shared_ptr<Label> label1, std::shared_ptr<Label> label2,
				std::shared_ptr<Label> label3, std::shared_ptr<Label> label4);

			bool dialogueIsBeingShown();
			bool hadJustFinishedDialogue();
		private:
			// These are obtained from GameObjectProvider.
			EventHandler* eventHandler;
			Time* time;
			ScriptManager* scriptManager;

			std::shared_ptr<SubMenu> dialogueSubMenu;
			std::shared_ptr<Label> label1, label2, label3, label4;

			DialogueState state = DialogueState::HIDDEN;

			// This handles the states of keys.
			bool pressingRight = false, pressingUp = false, pressingLeft = false, pressingDown = false;
			long long timeAtLastOptionSelect = 0, timeBetweenOptionSelects = 200000000;
			bool pressingReturn = false;

			bool justFinishedDialogue = false;

			// This is the max amount of characters that the handler will display per row of dialogue.
			const size_t MAX_CHARACTERS_PER_LINE = 46;

			// These are the lines that are to be displayed.
			std::string line1OfText = "", line2OfText = "";

			// This stores the script to run once a regular piece of dialogue is complete.
			std::string scriptOnDialogueEnd = "";

			// These are option-related properties.
			std::vector<DialogueOption> options;
			size_t numberOfCurrentOptions, currentOption;

			// This is the position (unit = characters) that the dialogue scrolling/appearing is at.
			size_t positionInDialogue = 0;
			
			// These store variables related to text scrolling/appearing and time.
			long long timeSinceNewCharacter;
			long long timeBetweenCharacterReveals = 10000000;

			void setupOptions();
			void pressUpAction(), pressDownAction(), pressLeftAction(), pressRightAction();
	};
}