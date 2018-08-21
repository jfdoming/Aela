/*
* Class: Dialogue Handler
* Author: Robert Ciborowski
* Date: 04/01/2018
* Description: A class used to handle in-game dialogue.
*/

#include "DialogueDisplay.h"
#include "../Scripts/ScriptManager.h"
#include "Time/Time.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"

Game::DialogueDisplay::DialogueDisplay() {
	eventHandler = GameObjectProvider::getEventHandler();
	time = GameObjectProvider::getTime();

	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(DialogueDisplay::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(DialogueDisplay::onEvent, this));
}

void Game::DialogueDisplay::setup() {
	scriptManager = GameObjectProvider::getScriptManager();
	animator = GameObjectProvider::getAnimator();
	maxWidthOfText = (int) (GameObjectProvider::getWindow()->getDimensions()->getWidth() * 0.7f);
}

void Game::DialogueDisplay::update() {
	if (state == DialogueState::MESSAGE && positionInDialogue < maxCharactersOfText
		&& time->getCurrentTimeInNanos() >= timeSinceNewCharacter + timeBetweenCharacterReveals) {
		positionInDialogue++;
		timeSinceNewCharacter = time->getCurrentTimeInNanos();
		if (positionInDialogue < maxCharactersInLabel1) {
			label1->setText(line1OfText.substr(0, positionInDialogue));
			label2->setText("");
			//if (positionInDialogue == maxCharactersPerLine - 1 && line2OfText == "") {
			//	// To indidcate that the one-liner is completely shown.
			//	positionInDialogue = maxCharactersPerLine * 2;
			//}
		} else {
			label1->setText(line1OfText.substr(0, maxCharactersInLabel1));
			label2->setText(line2OfText.substr(0, positionInDialogue - maxCharactersInLabel1));
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

void Game::DialogueDisplay::onEvent(Event* event) {
	if (event->getType() == EventConstants::KEY_PRESSED) {
		KeyEvent* keyEvent = static_cast<KeyEvent*>(event);
		switch (keyEvent->getKeycode()) {
			case SDLK_RETURN:
				if (!pressingReturn) {
					if (state == DialogueState::MESSAGE) {
						if (positionInDialogue > 1 && positionInDialogue < maxCharactersOfText) {
							// Note that the dialogue must have scrolled past the first character in order for this to trigger. Otherwise,
							// this could trigger right after the dialogue's initiation by AelaGame after a RETURN release.
							positionInDialogue = maxCharactersOfText - 1;
						} else if (positionInDialogue == maxCharactersOfText) {
							if (scriptOnDialogueEnd == "") {
								closeDialog();
							} else {
								scriptManager->runScript(scriptOnDialogueEnd);
							}
							justFinishedDialogue = true;
						}
					} else if (state == DialogueState::OPTIONS) {
						if (options[currentOption].getActionOnSelection() == "") {
							closeDialog();
						} else {
							scriptManager->runScript(options[currentOption].getActionOnSelection());
						}
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

void Game::DialogueDisplay::showDialogue(std::string text, std::string scriptToRunOnceComplete) {
	showDialogue("", "", text, scriptToRunOnceComplete);
}

void Game::DialogueDisplay::showDialogue(std::string name, std::string avatarSrc, std::string text, std::string scriptToRunOnceComplete) {
	auto onAnimationComplete = [this, name, avatarSrc]() {
		state = DialogueState::MESSAGE;
		nameLabel->setText(name);

		GLTexture* texture;
		if (avatarSrc != "" && GameObjectProvider::getResourceManager()->obtain<GLTexture>(DEFAULT_TEXTURE_PATH + avatarSrc, texture)) {
			avatar->setTexture(texture);
			avatar->show();
		} else {
			avatar->hide();
		}
	};
	
	if (state == DialogueState::HIDDEN) {
		AnimationTrack2D track;
		KeyFrame2D frame;
		frame.setObject(backdrop);
		frame.setDimensions(&backdropExpandedDimensions);
		frame.setEndingAction(onAnimationComplete);
		track.addKeyFrame(BACKDROP_ANIMATION_PERIOD, &frame);
		animator->addAnimationTrack2D(&track);
	} else {
		onAnimationComplete();
	}

	std::string splitText = text;
	TextFont* font = label1->getFont();
	int width = FontManager::dimensionsOfText(font, splitText).getWidth();
	while (width > maxWidthOfText) {
		size_t position = splitText.find_last_of(' ');
		if (position == std::string::npos) {
			break;
		}
		splitText = splitText.substr(0, position);
		width = FontManager::dimensionsOfText(font, splitText).getWidth();
	}

	maxCharactersInLabel1 = (int) splitText.size();
	maxCharactersOfText = (int) text.size();

	if (maxCharactersInLabel1 == maxCharactersOfText) {
		line1OfText = text;
		line2OfText = "";
	} else {
		line1OfText = splitText;
		line2OfText = text.substr(splitText.size() + 1, text.size() - splitText.size());
	}

	std::cout << "---------------\n";
	std::cout << "-" << line1OfText << "-\n";
	std::cout << "-" << line2OfText << "-\n";
	std::cout << "---------------\n";

	//if (text.size() > maxCharactersPerLine) {
	//	size_t position = text.substr(0, maxCharactersPerLine).find_last_of(' ');
	//	if (/*position >= maxCharactersPerLine || */position == std::string::npos) {
	//		line1OfText = text.substr(0, maxCharactersPerLine);
	//		position = maxCharactersPerLine;
	//	} else {
	//		line1OfText = text.substr(0, position);
	//		if (text.at(position) == ' ') {
	//			text.erase(text.begin() + position);
	//		}
	//	}
	//	if (text.size() > maxCharactersPerLine * 2) {
	//		line2OfText = text.substr(position, maxCharactersPerLine * 2);
	//	} else {
	//		line2OfText = text.substr(position, text.size());
	//	}
	//} else {
	//	line1OfText = text;
	//	line2OfText = "";
	//}

	positionInDialogue = 0;
	scriptOnDialogueEnd = scriptToRunOnceComplete;
	timeSinceNewCharacter = time->getCurrentTimeInNanos();
	label1->setText("");
	label2->setText("");
	label3->setText("");
	label4->setText("");
	subMenu->show();
}

void Game::DialogueDisplay::showOptions(DialogueOption* option1, DialogueOption* option2) {
	options.clear();
	options.push_back(*option1);
	options.push_back(*option2);
	numberOfCurrentOptions = 2;
	currentOption = 0;
	setupOptions();
}

void Game::DialogueDisplay::showOptions(DialogueOption* option1, DialogueOption* option2, DialogueOption* option3) {
	options.clear();
	options.push_back(*option1);
	options.push_back(*option2);
	options.push_back(*option3);
	numberOfCurrentOptions = 3;
	currentOption = 0;
	setupOptions();
}

void Game::DialogueDisplay::showOptions(DialogueOption* option1, DialogueOption* option2, DialogueOption* option3,
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

void Game::DialogueDisplay::closeDialog() {
	// Note that if the submenu changes visibility, it has to be rerendered. If it is already hidden,
	// there's no point in rerendering it as hidden again as that is a waste of resources.
	if (state == DialogueState::MESSAGE || state == DialogueState::OPTIONS) {
		auto endingAction = [this]() {
			subMenu->hide();
		};

		AnimationTrack2D track;
		KeyFrame2D frame;
		frame.setObject(backdrop);
		frame.setDimensions(&backdropClosedDimensions);
		frame.setEndingAction(endingAction);
		track.addKeyFrame(BACKDROP_ANIMATION_PERIOD, &frame);
		animator->addAnimationTrack2D(&track);
		label1->setText("");
		label2->setText("");
		label3->setText("");
		label4->setText("");
		nameLabel->setText("");
		avatar->hide();
	}
	state = DialogueState::HIDDEN;
}

void Game::DialogueDisplay::setSubMenu(std::shared_ptr<SubMenu> setSubMenu) {
	this->subMenu = setSubMenu;
}

void Game::DialogueDisplay::setBackdrop(std::shared_ptr<ImageComponent> backdrop) {
	this->backdrop = backdrop;
	backdropExpandedDimensions = *backdrop->getDimensions();
	backdropClosedDimensions = *backdrop->getDimensions();
	backdropClosedDimensions.setWidth(0);
	backdrop->setDimensions(&backdropClosedDimensions);
}

void Game::DialogueDisplay::setAvatar(std::shared_ptr<ImageComponent> avatar) {
	this->avatar = avatar;
}

void Game::DialogueDisplay::setDialogueLabels(std::shared_ptr<Label> label1, std::shared_ptr<Label> label2,
	std::shared_ptr<Label> label3, std::shared_ptr<Label> label4) {
	this->label1 = label1;
	this->label2 = label2;
	this->label3 = label3;
	this->label4 = label4;
}

void Game::DialogueDisplay::setNameLabel(std::shared_ptr<Label> nameLabel) {
	this->nameLabel = nameLabel;
}

bool Game::DialogueDisplay::dialogueIsBeingShown() {
	return state != DialogueState::HIDDEN;
}

bool Game::DialogueDisplay::hadJustFinishedDialogue() {
	return justFinishedDialogue;
}

float Game::DialogueDisplay::getDialogueTextStartX() {
	return DIALOGUE_TEXT_START_X;
}

void Game::DialogueDisplay::setupOptions() {
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
	subMenu->show();
	state = DialogueState::OPTIONS;
}

void Game::DialogueDisplay::pressUpAction() {
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

void Game::DialogueDisplay::pressDownAction() {
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

void Game::DialogueDisplay::pressLeftAction() {
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

void Game::DialogueDisplay::pressRightAction() {
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
