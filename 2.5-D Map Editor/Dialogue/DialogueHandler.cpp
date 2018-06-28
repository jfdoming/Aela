/*
* Class: Dialogue Handler
* Author: Robert Ciborowski
* Date: 04/01/2018
* Description: A class used to handle in-game dialogue.
*/

#include "DialogueHandler.h"

void Game::DialogueHandler::setup(Time* time, EventHandler* eventHandler, ScriptManager* scriptManager) {
	this->time = time;
	this->scriptManager = scriptManager;
	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(DialogueHandler::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(DialogueHandler::onEvent, this));
}

void Game::DialogueHandler::update() {
	if (state == DialogueState::MESSAGE && positionInDialogue < MAX_CHARACTERS_PER_LINE * 2
		&& time->getCurrentTimeInNanos() >= timeSinceNewCharacter + timeBetweenCharacterReveals) {
		positionInDialogue++;
		timeSinceNewCharacter = time->getCurrentTimeInNanos();
		if (positionInDialogue < MAX_CHARACTERS_PER_LINE) {
			label1->setText(line1OfText.substr(0, positionInDialogue));
			label2->setText("");
			if (positionInDialogue == MAX_CHARACTERS_PER_LINE - 1 && line2OfText == "") {
				// To indidcate that the one-liner is completely shown.
				positionInDialogue = MAX_CHARACTERS_PER_LINE * 2;
			}
		} else {
			label1->setText(line1OfText.substr(0, MAX_CHARACTERS_PER_LINE));
			label2->setText(line2OfText.substr(0, positionInDialogue - MAX_CHARACTERS_PER_LINE));
		}
	}

	if (time->getCurrentTimeInNanos() > timeAtLastOptionSelect + timeBetweenOptionSelects) {
		if (pressingUp) {
			pressUpAction();
		}

		if (pressingDown) {
			pressDownAction();
		}

		if (pressingLeft) {
			pressLeftAction();
		}

		if (pressingRight) {
			pressRightAction();
		}
	}
}

void Game::DialogueHandler::onEvent(Event* event) {
	if (event->getType() == EventConstants::KEY_PRESSED) {
		KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
		switch (keyEvent->getKeycode()) {
			case SDLK_RETURN:
				if (!pressingReturn) {
					if (state == DialogueState::MESSAGE) {
						if (positionInDialogue > 1 && positionInDialogue < MAX_CHARACTERS_PER_LINE * 2) {
							// Note that the dialogue must have scrolled past the first character in order for this to trigger. Otherwise,
							// this could trigger right after the dialogue's initiation by AelaGame after a RETURN release.
							positionInDialogue = MAX_CHARACTERS_PER_LINE * 2 - 1;
						} else if (positionInDialogue == MAX_CHARACTERS_PER_LINE * 2) {
							closeDialog();
							scriptManager->runScript(scriptOnDialogueEnd);
							justFinishedDialogue = true;
						}
					} else if (state == DialogueState::OPTIONS) {
						closeDialog();
						scriptManager->runScript(options[currentOption].getActionOnSelection());
					}
					pressingReturn = true;
				}
				break;
			case SDLK_w:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressUpAction();
					pressingUp = true;
				}
				return;
			case SDLK_s:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressDownAction();
					pressingDown = true;
				}
				return;
			case SDLK_a:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressLeftAction();
					pressingLeft = true;
				}
				return;
			case SDLK_d:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressRightAction();
					pressingRight = true;
				}
				return;
		}
	} else if (event->getType() == EventConstants::KEY_RELEASED) {
		KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
		switch (keyEvent->getKeycode()) {
			case SDLK_RETURN:
				pressingReturn = false;
				justFinishedDialogue = false;
				break;
			case SDLK_w:
				pressingUp = false;
				break;
			case SDLK_s:
				pressingDown = false;
				break;
			case SDLK_a:
				pressingLeft = false;
				break;
			case SDLK_d:
				pressingRight = false;
				break;
		}
	}
}

void Game::DialogueHandler::showDialogue(std::string text, std::string scriptToRunOnceComplete) {
	scriptOnDialogueEnd = scriptToRunOnceComplete;
	state = DialogueState::MESSAGE;

	if (text.size() > MAX_CHARACTERS_PER_LINE) {
		size_t position = text.substr(0, MAX_CHARACTERS_PER_LINE).find_last_of(' ');
		if (position >= MAX_CHARACTERS_PER_LINE || position == std::string::npos) {
			line1OfText = text.substr(0, MAX_CHARACTERS_PER_LINE);
			position = MAX_CHARACTERS_PER_LINE;
		} else {
			line1OfText = text.substr(0, position);
			if (text.at(position) == ' ') {
				text.erase(text.begin() + position);
			}
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
	timeSinceNewCharacter = time->getCurrentTimeInNanos();
	label1->setText(line1OfText.substr(0, 1));
	label2->setText("");
	label3->setText("");
	label4->setText("");
	dialogueSubMenu->show();
}

void Game::DialogueHandler::showOptions(DialogueOption* option1, DialogueOption* option2) {
	options.clear();
	options.push_back(*option1);
	options.push_back(*option2);
	numberOfCurrentOptions = 2;
	currentOption = 0;
	setupOptions();
}

void Game::DialogueHandler::showOptions(DialogueOption* option1, DialogueOption* option2, DialogueOption* option3) {
	options.clear();
	options.push_back(*option1);
	options.push_back(*option2);
	options.push_back(*option3);
	numberOfCurrentOptions = 3;
	currentOption = 0;
	setupOptions();
}

void Game::DialogueHandler::showOptions(DialogueOption* option1, DialogueOption* option2, DialogueOption* option3,
	DialogueOption* option4) {
	options.clear();
	options.push_back(*option1);
	options.push_back(*option2);
	options.push_back(*option3);
	options.push_back(*option4);
	numberOfCurrentOptions = 4;
	currentOption = 0;
	setupOptions();
}

void Game::DialogueHandler::closeDialog() {
	// Note that if the submenu changes visibility, it has to be rerendered. If it is already hidden,
	// there's no point in rerendering it as hidden again as that is a waste of resources.
	if (dialogueSubMenu->isVisible()) {
		dialogueSubMenu->hide();
		positionInDialogue = MAX_CHARACTERS_PER_LINE * 2;
	}
	state = DialogueState::HIDDEN;
}

void Game::DialogueHandler::setDialogueSubMenu(std::shared_ptr<SubMenu> dialogueSubMenu) {
	this->dialogueSubMenu = dialogueSubMenu;
}

void Game::DialogueHandler::setDialogueLabels(std::shared_ptr<Label> label1, std::shared_ptr<Label> label2,
	std::shared_ptr<Label> label3, std::shared_ptr<Label> label4) {
	this->label1 = label1;
	this->label2 = label2;
	this->label3 = label3;
	this->label4 = label4;
}

bool Game::DialogueHandler::dialogueIsBeingShown() {
	return state != DialogueState::HIDDEN;
}

bool Game::DialogueHandler::hadJustFinishedDialogue() {
	return justFinishedDialogue;
}

void Game::DialogueHandler::setupOptions() {
	if (currentOption == 0) {
		label1->setText("> " + options[0].getText());
	} else {
		label1->setText(options[0].getText());
	}
	if (currentOption == 1) {
		label2->setText("> " + options[1].getText());
	} else {
		label2->setText(options[1].getText());
	}
	if (numberOfCurrentOptions > 2) {
		if (currentOption == 2) {
			label3->setText("> " + options[2].getText());
		} else {
			label3->setText(options[2].getText());
		}
	} else {
		label3->setText("");
	}
	if (numberOfCurrentOptions > 3) {
		if (currentOption == 0) {
			label4->setText("> " + options[3].getText());
		} else {
			label4->setText(options[3].getText());
		}
	} else {
		label4->setText("");
	}
	dialogueSubMenu->show();
	state = DialogueState::OPTIONS;
}

void Game::DialogueHandler::pressUpAction() {
	size_t oldOption = currentOption;
	if (currentOption % 2 == 0) {
		currentOption++;
	} else {
		currentOption--;
	}
	if (currentOption < numberOfCurrentOptions) {
		setupOptions();
	} else {
		currentOption = oldOption;
	}
	timeAtLastOptionSelect = time->getCurrentTimeInNanos();
}

void Game::DialogueHandler::pressDownAction() {
	size_t oldOption = currentOption;
	if (currentOption % 2 == 0) {
		currentOption++;
	} else {
		currentOption--;
	}
	if (currentOption < numberOfCurrentOptions) {
		setupOptions();
	} else {
		currentOption = oldOption;
	}
	timeAtLastOptionSelect = time->getCurrentTimeInNanos();
}

void Game::DialogueHandler::pressLeftAction() {
	size_t oldOption = currentOption;
	currentOption -= 2;
	if (currentOption < 0) {
		currentOption += 4;
	}
	if (currentOption < numberOfCurrentOptions) {
		setupOptions();
	} else {
		currentOption = oldOption;
	}
	timeAtLastOptionSelect = time->getCurrentTimeInNanos();
}

void Game::DialogueHandler::pressRightAction() {
	size_t oldOption = currentOption;
	currentOption += 2;
	if (currentOption > 3) {
		currentOption -= 4;
	}
	if (currentOption < numberOfCurrentOptions) {
		setupOptions();
	} else {
		currentOption = oldOption;
	}
	timeAtLastOptionSelect = time->getCurrentTimeInNanos();
}
