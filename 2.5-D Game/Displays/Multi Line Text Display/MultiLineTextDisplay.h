/*
* Class: Multi Line Text Display
* Author: Robert Ciborowski
* Date: 13/01/2019
* Description: A class used to handle in-game text displays that specifically represent speech.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "../../../Project Aela/Menus/SubMenu.h"
#include "../../../Project Aela/Menus/Label.h"
#include "../../../Project Aela/Menus/ImageComponent.h"

namespace Game {
	class MultiLineTextDisplay {
		public:
			MultiLineTextDisplay(size_t numberOfLines);
			virtual ~MultiLineTextDisplay();

			virtual void setup();
			virtual void update();

			// This is to be called by children classes whenever they want to show dialogue.
			virtual void showDialogue(std::string name, std::string text, std::string scriptToRunOnceComplete);

			// This is to be called in order to instantly show the rest of the dialogue.
			virtual void finishDialogueAnimationImmediately();

			// This is to be called by children whenever they want to clear/close dialogue.
			virtual void closeDialog();
		
			// This changes the current font. Note that passing in nullptr will change it to the default font.
			void changeCurrentFont(Font* currentFont);

			void setDialogueLabels(std::vector<std::shared_ptr<Label>>* labels);
			void setDialogueLabels(std::initializer_list<std::shared_ptr<Label>> labels);
			void setDefaultFont(Font* defaultFont);
			void setDefaultSpeechSound(std::string defaultSpeechSound);
			void setTimeBetweenCharacterReveals(unsigned long long timeBetweenCharacterReveals);
			void resetTimeBetweenCharacterReveals();
			void setPlaySound(bool playSound);

			void associateCharacterWithFont(std::string name, Font* font);
			void associateCharacterWithSpeechSound(std::string name, std::string speechSound);

			bool isShowingDialogue();
			std::string getLineOfText(size_t whichLine);

		protected:
			// These are obtained from GameObjectProvider.
			Clock* time;
			ScriptManager* scriptManager;

			std::shared_ptr<ImageComponent> backdrop;
			std::vector<std::shared_ptr<Label>> labels;

			int maxWidthOfText = 32;
			size_t maxCharactersOfText = 32;

			// This is set to true typically when an SDLK_RETURN event is received. It lets update()
			// (which runs on the main thread) know that it should run the script on dialogue end.
			bool runScript = false;

			bool playSound = true;

			// This is the position (unit = characters) that the dialogue scrolling/appearing is at.
			size_t positionInDialogue = 0;
			size_t positionInLine = 0;
			size_t currentLine = 0;

			unsigned long long timeSinceNewCharacter;

			// This stores the script to run once a regular piece of dialogue is complete.
			std::string scriptOnDialogueEnd = "";

			bool usingFontSize = false;

			Font* defaultFont = nullptr;
			Font* currentFont = nullptr;

			bool textScrollingHasCompleted();

		private:
			std::unordered_map<std::string, Font*> characterFontAssociations;
			std::unordered_map<std::string, std::string> characterSoundAssociations;
			std::string defaultSpeechSound = "speech 1";
			std::string currentSpeechSound;

			bool showingDialogue = false;

			// These are the lines that are to be displayed.
			std::vector<std::string> linesOfText;
			
			// These store variables related to text scrolling/appearing and time in milliseconds.
			unsigned long long timeBetweenCharacterReveals = 22;
			const unsigned long long DEFAULT_TIME_BETWEEN_CHARACTER_REVEALS = 22;

			// This will return true for most (but not all) punctuation marks.
			bool requiresBreathInSentence(char character);
	};
}