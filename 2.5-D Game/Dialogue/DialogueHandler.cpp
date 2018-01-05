/*
* Class: Dialogue Handler
* Author: Robert Ciborowski
* Date: 04/01/2018
* Description: A class used to handle in-game dialogue.
*/

#include "DialogueHandler.h"

void Game::DialogueHandler::setup(TimeManager* timeManager, ScriptManager* scriptManager) {
	this->timeManager = timeManager;
	this->scriptManager = scriptManager;
}

void Game::DialogueHandler::update() {
	if (positionInDialogue < MAX_CHARACTERS_PER_LINE * 2
		&& timeManager->getCurrentTimeInNanos() >= timeSinceNewCharacter + timeBetweenCharacterReveals) {
		positionInDialogue++;
		timeSinceNewCharacter = timeManager->getCurrentTimeInNanos();
		if (positionInDialogue < MAX_CHARACTERS_PER_LINE) {
			label1->setText(line1OfText.substr(0, positionInDialogue));
			label2->setText("");
		} else {
			label1->setText(line1OfText.substr(0, MAX_CHARACTERS_PER_LINE));
			label2->setText(line2OfText.substr(0, positionInDialogue - MAX_CHARACTERS_PER_LINE));
		}
	}
}

void Game::DialogueHandler::showDialogue(std::string text, std::string scriptToRunOnceComplete) {
	if ((signed long long) text.size() > MAX_CHARACTERS_PER_LINE) {
		size_t position = text.substr(0, MAX_CHARACTERS_PER_LINE).find_last_of(' ');
		if (position >= MAX_CHARACTERS_PER_LINE || position == std::string::npos) {
			line1OfText = text.substr(0, MAX_CHARACTERS_PER_LINE);
			position = MAX_CHARACTERS_PER_LINE;
		} else {
			line1OfText = text.substr(0, position);
		}
		if (text.size() > MAX_CHARACTERS_PER_LINE * 2) {
			line2OfText = text.substr(position, MAX_CHARACTERS_PER_LINE * 2);
		} else {
			line2OfText = text.substr(position, text.size());
		}
	} else {
		line1OfText = text;
		line2OfText = "";
	}

	positionInDialogue = 1;
	timeSinceNewCharacter = timeManager->getCurrentTimeInNanos();
	label1->setText(line1OfText.substr(0, 1));
	label2->setText("");
	dialogueSubMenu->show();
}

void Game::DialogueHandler::showOptions(DialogueOption option1, DialogueOption option2) {}

void Game::DialogueHandler::showOptions(DialogueOption option1, DialogueOption option2, DialogueOption option3, DialogueOption option4) {}

void Game::DialogueHandler::closeDialog() {
	// Note that if the submenu changes visibility, it has to be rerendered. If it is already hidden,
	// there's no point in rerendering it as hidden again as that is a waste of resources.
	if (dialogueSubMenu->isVisible()) {
		dialogueSubMenu->hide();
		positionInDialogue = MAX_CHARACTERS_PER_LINE * 2;
	}
}

void Game::DialogueHandler::setDialogueSubMenu(std::shared_ptr<SubMenu> dialogueSubMenu) {
	this->dialogueSubMenu = dialogueSubMenu;
}

void Game::DialogueHandler::setDialogueLabels(std::shared_ptr<Label> label1, std::shared_ptr<Label> label2) {
	this->label1 = label1;
	this->label2 = label2;
}

void Game::DialogueHandler::showOptions(DialogueOption option1, DialogueOption option2, DialogueOption option3) {}
