#include <utility>

#include <utility>

/*
* Class: Dialogue Handler
* Author: Robert Ciborowski
* Date: 04/01/2018
* Description: A class used to handle in-game dialogue.
*/

#include "DialogueDisplay.h"
#include "../Scripts/ScriptManager.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"
#include "../../Menus/FontSizes.h"
#include "../../Project Aela/Time/Clock.h"
#include "../../Audio/GameAudioPlayer.h"
#include "../../Scripts/Scripts to Move to LUA/SceneIDs.h"
#include "../../Aela Game/AelaGame.h"

Game::DialogueDisplay::DialogueDisplay() : MultiLineTextDisplay(2) {
	eventHandler = GameObjectProvider::getEventHandler();
	usingFontSize = true;

	eventHandler->addListener(EventConstants::KEY_RELEASED, bindListener(DialogueDisplay::onEvent, this));
	eventHandler->addListener(EventConstants::KEY_PRESSED, bindListener(DialogueDisplay::onEvent, this));
}

Game::DialogueDisplay::~DialogueDisplay() {
}

void Game::DialogueDisplay::setup() {
	MultiLineTextDisplay::setup();
	animator = GameObjectProvider::getAnimator();
	maxWidthOfText = (int) (GameObjectProvider::getWindow()->getDimensions()->getWidth() * 0.7f);
}

void Game::DialogueDisplay::update() {
	MultiLineTextDisplay::update();
	AvatarDisplay::update();

	if (optionWasSelected) {
		if (options[currentOption].getActionOnSelection().empty()) {
			closeDialog();
		} else {
			scriptManager->runScript(options[currentOption].getActionOnSelection());
		}
		optionWasSelected = false;
	}

	if (finishDialogueAnimations) {
		finishDialogueAnimationImmediately();
		finishDialogueAnimations = false;
	}

	if (state == DialogueState::OPTIONS && time->getCurrentTimeInMillis() > timeAtLastOptionSelect + timeBetweenOptionSelects) {
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
		auto* keyEvent = dynamic_cast<KeyEvent*>(event);
		switch (keyEvent->getKeycode()) {
			case SDLK_RETURN:
				if (!pressingReturn) {
					if (state == DialogueState::MESSAGE && continueOnReturnPress && GameObjectProvider::getSceneManager()->getCurrentSceneId() == WORLD_GAMEPLAY_SCENE) {
						if (positionInDialogue > 1 && positionInDialogue < maxCharactersOfText) {
							// Note that the dialogue must have scrolled past the first character in order for this to trigger. Otherwise,
							// this could trigger right after the dialogue's initiation by AelaGame after a RETURN release.
							// positionInDialogue = maxCharactersOfText - 1;
							// timeSinceNewCharacter = 0;
							finishDialogueAnimations = true;
						} else if (textScrollingHasCompleted()) {
							if (scriptOnDialogueEnd.empty()) {
								closeDialog();
							} else {
								runScript = true;
							}
							justSkippedDialogue = true;
						}
					} else if (state == DialogueState::OPTIONS) {
						optionWasSelected = true;
					}
					pressingReturn = true;
				}
				break;
			case SDLK_w:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressingUp = true;
				}
				return;
			case SDLK_s:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressingDown = true;
				}
				return;
			case SDLK_a:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressingLeft = true;
				}
				return;
			case SDLK_d:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressingRight = true;
				}
				return;
			case SDLK_UP:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressingUp = true;
				}
				return;
			case SDLK_DOWN:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressingDown = true;
				}
				return;
			case SDLK_LEFT:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressingLeft = true;
				}
				return;
			case SDLK_RIGHT:
				if (state == DialogueState::OPTIONS && !(pressingUp || pressingDown || pressingLeft || pressingRight)) {
					pressingRight = true;
				}
				return;
			default:
				break;
		}
	} else if (event->getType() == EventConstants::KEY_RELEASED) {
		auto* keyEvent = dynamic_cast<KeyEvent*>(event);
		switch (keyEvent->getKeycode()) {
			case SDLK_RETURN:
				pressingReturn = false;
				justSkippedDialogue = false;
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
			case SDLK_UP:
				pressingUp = false;
				break;
			case SDLK_DOWN:
				pressingDown = false;
				break;
			case SDLK_LEFT:
				pressingLeft = false;
				break;
			case SDLK_RIGHT:
				pressingRight = false;
				break;
			default:
				break;
		}
	}
}

void Game::DialogueDisplay::showDialogue(std::string name, std::string text, std::string scriptToRunOnceComplete) {
	showDialogue("", "", std::move(text), std::move(scriptToRunOnceComplete));
}

void Game::DialogueDisplay::showDialogue(std::string name, std::string avatarSrc, std::string text, std::string scriptToRunOnceComplete) {
	showDialogue(name, avatarSrc, "", text, scriptToRunOnceComplete);
}

void Game::DialogueDisplay::showDialogue(std::string name, std::string avatarSrc, std::string blinkingAvatarSrc,
	std::string text, std::string scriptToRunOnceComplete) {
	MultiLineTextDisplay::showDialogue(name, text, scriptToRunOnceComplete);

	auto onAnimationComplete = [this, name, avatarSrc, blinkingAvatarSrc]() {
		// We might not end up actually showing the name.
		// nameLabel->setText(name);
		nameLabel->setText("");
		showAvatar(avatarSrc, blinkingAvatarSrc);
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

	selectorImage->hide();
	state = DialogueState::MESSAGE;
	subMenu->show();

	// Lower any music volume.
	GameObjectProvider::getGameAudioPlayer()->setAllStreamVolumes(0.6f);
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
	MultiLineTextDisplay::closeDialog();

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
		labels[0]->setText("");
		labels[1]->setText("");
		labels[2]->setText("");
		labels[3]->setText("");
		nameLabel->setText("");
		stopShowingAvatar();
	}
	state = DialogueState::HIDDEN;

	GameObjectProvider::getGameAudioPlayer()->setAllStreamVolumes(1);
}

void Game::DialogueDisplay::setSubMenu(std::shared_ptr<SubMenu> setSubMenu) {
	this->subMenu = std::move(setSubMenu);
}

void Game::DialogueDisplay::setBackdrop(std::shared_ptr<ImageComponent> backdrop) {
	this->backdrop = backdrop;
	backdropExpandedDimensions = *backdrop->getDimensions();
	backdropClosedDimensions = *backdrop->getDimensions();
	backdropClosedDimensions.setWidth(0);
	backdrop->setDimensions(&backdropClosedDimensions);
}

void Game::DialogueDisplay::setNameLabel(std::shared_ptr<Label> nameLabel) {
	this->nameLabel = std::move(nameLabel);
}

void Game::DialogueDisplay::setSelectorImage(std::shared_ptr<ImageComponent> selectorImage) {
	this->selectorImage = selectorImage;
}

bool Game::DialogueDisplay::hadJustSkippedDialogue() {
	return justSkippedDialogue;
}

void Game::DialogueDisplay::setContinueOnReturnPress(bool continueOnReturnPress) {
	this->continueOnReturnPress = continueOnReturnPress;
}

float Game::DialogueDisplay::getDialogueTextStartX() {
	return DIALOGUE_TEXT_START_X;
}

void Game::DialogueDisplay::setupOptions() {
	labels[0]->setText(options[0].getText());
	labels[1]->setText(options[1].getText());

	if (numberOfCurrentOptions > 2) {
			labels[2]->setText(options[2].getText());
	} else {
		labels[2]->setText("");
	}

	if (numberOfCurrentOptions > 3) {
		labels[3]->setText(options[3].getText());
	} else {
		labels[3]->setText("");
	}

	for (auto label : labels) {
		label->setFont(defaultFont);
		label->setSize(defaultFont->getDefaultSize());
	}

	updateSelectorImagePosition();
	subMenu->show();
	selectorImage->show();
	state = DialogueState::OPTIONS;
}

void Game::DialogueDisplay::pressUpAction() {
	int oldOption = currentOption;

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

	/*if (currentOption != oldOption) {
		GameObjectProvider::getGameAudioPlayer()->playAudio("option");
	}*/

	timeAtLastOptionSelect = time->getCurrentTimeInMillis();
}

void Game::DialogueDisplay::pressDownAction() {
	int oldOption = currentOption;

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

	/*if (currentOption != oldOption) {
		GameObjectProvider::getGameAudioPlayer()->playAudio("option");
	}*/

	timeAtLastOptionSelect = time->getCurrentTimeInMillis();
}

void Game::DialogueDisplay::pressLeftAction() {
	int oldOption = currentOption;
	currentOption -= 2;

	if (currentOption < 0) {
		currentOption += 4;
	}

	if (currentOption < numberOfCurrentOptions) {
		setupOptions();
	} else {
		currentOption = oldOption;
	}

	/*if (currentOption != oldOption) {
		GameObjectProvider::getGameAudioPlayer()->playAudio("option");
	}*/

	timeAtLastOptionSelect = time->getCurrentTimeInMillis();
}

void Game::DialogueDisplay::pressRightAction() {
	int oldOption = currentOption;
	currentOption += 2;

	if (currentOption > 3) {
		currentOption -= 4;
	}

	if (currentOption < numberOfCurrentOptions) {
		setupOptions();
	} else {
		currentOption = oldOption;
	}

	/*if (currentOption != oldOption) {
		GameObjectProvider::getGameAudioPlayer()->playAudio("option");
	}*/

	timeAtLastOptionSelect = time->getCurrentTimeInMillis();
}

void Game::DialogueDisplay::updateSelectorImagePosition() {
	Rect<int>* dimensions;

	if (currentOption == 0) {
		dimensions = labels[0]->getDimensions();
	} else if (currentOption == 1) {
		dimensions = labels[1]->getDimensions();
	} else if (currentOption == 2) {
		dimensions = labels[2]->getDimensions();
	} else {
		dimensions = labels[3]->getDimensions();
	}

	selectorImage->setXY(dimensions->getX() - selectorImage->getDimensions()->getWidth() - SELECTOR_IMAGE_X_OFFSET, dimensions->getY() - dimensions->getHeight());
}
