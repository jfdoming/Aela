/*
* Class: Dialogue Display
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
#include "../Multi Line Text Display/MultiLineTextDisplay.h"
#include "../Avatar/AvatarDisplay.h"

namespace Game {
	enum class DialogueState {
		HIDDEN, MESSAGE, OPTIONS
	};

	class DialogueDisplay : public MultiLineTextDisplay, public AvatarDisplay {
		public:
			DialogueDisplay();
			virtual ~DialogueDisplay();

			void setup() override;
			void update() override;

			// This is triggered on an event.
			void onEvent(Event* event);
		
			void showDialogue(std::string name, std::string text, std::string scriptToRunOnceComplete) override;
			void showDialogue(std::string name, std::string avatarSrc, std::string text, std::string scriptToRunOnceComplete);
			void showDialogue(std::string name, std::string avatarSrc, std::string blinkingAvatarSrc, std::string text, std::string scriptToRunOnceComplete);
			void showOptions(DialogueOption* option1, DialogueOption* option2), 
				showOptions(DialogueOption* option1, DialogueOption* option2, DialogueOption* option3),
				showOptions(DialogueOption* option1, DialogueOption* option2, DialogueOption* option3, DialogueOption* option4);
			void closeDialog() override;

			void setSubMenu(std::shared_ptr<SubMenu> subMenu);
			void setBackdrop(std::shared_ptr<ImageComponent> backdrop);
			void setNameLabel(std::shared_ptr<Label> nameLabel);
			void setSelectorImage(std::shared_ptr<ImageComponent> selectorImage);

			bool hadJustSkippedDialogue();
			void setContinueOnReturnPress(bool continueOnReturnPress);
			float getDialogueTextStartX();

		private:
			// These are obtained from GameObjectProvider.
			EventHandler* eventHandler;
			Animator* animator;

			std::shared_ptr<SubMenu> subMenu;
			std::shared_ptr<Label> nameLabel;
			std::shared_ptr<ImageComponent> selectorImage;

			const int SELECTOR_IMAGE_X_OFFSET = 5;

			Rect<int> backdropExpandedDimensions, backdropClosedDimensions;

			DialogueState state = DialogueState::HIDDEN;

			// This handles the states of keys.
			std::atomic<bool> pressingRight = false, pressingUp = false, pressingLeft = false, pressingDown = false;
			unsigned long long timeAtLastOptionSelect = 0, timeBetweenOptionSelects = 200;
			std::atomic<bool> pressingReturn = false;
			std::atomic<bool> finishDialogueAnimations = false;
			std::atomic<bool> justSkippedDialogue = false;

			// This is a multiplier for how much of the window width the dialogue text's x position is.
			const float DIALOGUE_TEXT_START_X = 0.25f;

			bool optionWasSelected = false;
			bool continueOnReturnPress = true;

			// These are option-related properties.
			std::vector<DialogueOption> options;
			int numberOfCurrentOptions, currentOption;
			
			const long long BACKDROP_ANIMATION_PERIOD = 150;

			void setupOptions();
			void pressUpAction(), pressDownAction(), pressLeftAction(), pressRightAction();

			void updateSelectorImagePosition();
	};
}
