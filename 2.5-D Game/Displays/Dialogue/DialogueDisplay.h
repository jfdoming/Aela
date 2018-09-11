/*
* Class: Dialogue Handler
* Author: Robert Ciborowski
* Date: 04/01/2018
* Description: A class used to handle in-game dialogue.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "../../../Project Aela/Menus/SubMenu.h"
#include "../../../Project Aela/Menus/Label.h"
#include "../../../Project Aela/Menus/ImageComponent.h"
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
			void showDialogue(std::string name, std::string avatarSrc, std::string text, std::string scriptToRunOnceComplete);
			void showOptions(DialogueOption* option1, DialogueOption* option2), 
				showOptions(DialogueOption* option1, DialogueOption* option2, DialogueOption* option3),
				showOptions(DialogueOption* option1, DialogueOption* option2, DialogueOption* option3, DialogueOption* option4);
			void closeDialog();

			void setSubMenu(std::shared_ptr<SubMenu> subMenu);
			void setBackdrop(std::shared_ptr<ImageComponent> backdrop);
			void setAvatar(std::shared_ptr<ImageComponent> avatar);
			void setDialogueLabels(std::shared_ptr<Label> label1, std::shared_ptr<Label> label2,
				std::shared_ptr<Label> label3, std::shared_ptr<Label> label4);
			void setNameLabel(std::shared_ptr<Label> nameLabel);

			bool dialogueIsBeingShown();
			bool hadJustFinishedDialogue();

			float getDialogueTextStartX();
		private:
			// These are obtained from GameObjectProvider.
			EventHandler* eventHandler;
			Clock* time;
			ScriptManager* scriptManager;
			Animator* animator;

			std::shared_ptr<SubMenu> subMenu;
			std::shared_ptr<ImageComponent> backdrop;
			std::shared_ptr<ImageComponent> avatar;
			std::shared_ptr<Label> label1, label2, label3, label4, nameLabel;

			Rect<int> backdropExpandedDimensions, backdropClosedDimensions;

			DialogueState state = DialogueState::HIDDEN;

			// This handles the states of keys.
			bool pressingRight = false, pressingUp = false, pressingLeft = false, pressingDown = false;
			long long timeAtLastOptionSelect = 0, timeBetweenOptionSelects = 200000000;
			bool pressingReturn = false;

			bool justFinishedDialogue = false;

			int maxWidthOfText;
			size_t maxCharactersInLabel1, maxCharactersOfText;

			// This is a multiplier for how much of the window width the dialogue text's x position is.
			const float DIALOGUE_TEXT_START_X = 0.25f;

			// These are the lines that are to be displayed.
			std::string line1OfText = "", line2OfText = "";

			// This stores the script to run once a regular piece of dialogue is complete.
			std::string scriptOnDialogueEnd = "";

			// This is set to true typically when an SDLK_RETURN event is received. It lets update()
			// (which runs on the main thread) know that it should run the script on dialogue end.
			bool runScript = false;

			bool optionWasSelected = false;

			// These are option-related properties.
			std::vector<DialogueOption> options;
			size_t numberOfCurrentOptions, currentOption;

			// This is the position (unit = characters) that the dialogue scrolling/appearing is at.
			size_t positionInDialogue = 0;
			
			// These store variables related to text scrolling/appearing and time.
			long long timeSinceNewCharacter;
			long long timeBetweenCharacterReveals = 10000000;
			const long long BACKDROP_ANIMATION_PERIOD = 150000000;

			void setupOptions();
			void pressUpAction(), pressDownAction(), pressLeftAction(), pressRightAction();
	};
}