#include "MultiLineTextDisplay.h"
#include "../../Scripts/ScriptManager.h"
#include "../../Audio/GameAudioPlayer.h"

Game::MultiLineTextDisplay::MultiLineTextDisplay(size_t numberOfLines) {
	for (size_t i = 0; i < numberOfLines; i++) {
		linesOfText.push_back("");
	}

	time = GameObjectProvider::getTime();
}

Game::MultiLineTextDisplay::~MultiLineTextDisplay() {
}

void Game::MultiLineTextDisplay::setup() {
	scriptManager = GameObjectProvider::getScriptManager();
}

void Game::MultiLineTextDisplay::update() {
	if (runScript) {
		scriptManager->runScript(scriptOnDialogueEnd);
		runScript = false;
	}

	if (isShowingDialogue() && positionInDialogue < maxCharactersOfText && time->getCurrentTimeInMillis() >= timeSinceNewCharacter + timeBetweenCharacterReveals) {
		positionInDialogue++;
		positionInLine++;
		timeSinceNewCharacter = time->getCurrentTimeInMillis();
		labels[currentLine]->setText(linesOfText[currentLine].substr(0, positionInLine));

		if (playSound) {
			GameObjectProvider::getGameAudioPlayer()->playAudioIfNotPlaying(currentSpeechSound);
		}

		if (positionInLine == linesOfText[currentLine].size()) {
			positionInLine = 0;
			currentLine++;
		} else if (requiresBreathInSentence(linesOfText[currentLine][positionInLine - 1])) {
			timeSinceNewCharacter += timeBetweenCharacterReveals * 6;
		}
	}
}

void Game::MultiLineTextDisplay::setDialogueLabels(std::vector<std::shared_ptr<Label>>* labels) {
	this->labels = *labels;

	for (auto label : this->labels) {
		label->setText("");
		if (usingFontSize) {
			label->useFontSize();
		}
	}
}

void Game::MultiLineTextDisplay::setDialogueLabels(std::initializer_list<std::shared_ptr<Label>> labels) {
	for (auto label : labels) {
		label->setText("");
		if (usingFontSize) {
			label->useFontSize();
		}
		this->labels.push_back(label);
	}
}

void Game::MultiLineTextDisplay::setDefaultFont(Font* defaultFont) {
	this->defaultFont = defaultFont;
}

void Game::MultiLineTextDisplay::setDefaultSpeechSound(std::string defaultSpeechSound) {
	this->defaultSpeechSound = defaultSpeechSound;
}

void Game::MultiLineTextDisplay::associateCharacterWithSpeechSound(std::string name, std::string speechSound) {
	characterSoundAssociations[name] = speechSound;
}

void Game::MultiLineTextDisplay::setTimeBetweenCharacterReveals(unsigned long long timeBetweenCharacterReveals) {
	this->timeBetweenCharacterReveals = timeBetweenCharacterReveals;
}

void Game::MultiLineTextDisplay::resetTimeBetweenCharacterReveals() {
	this->timeBetweenCharacterReveals = DEFAULT_TIME_BETWEEN_CHARACTER_REVEALS;
}

void Game::MultiLineTextDisplay::setPlaySound(bool playSound) {
	this->playSound = playSound;
}

void Game::MultiLineTextDisplay::associateCharacterWithFont(std::string name, Font * font) {
	characterFontAssociations[name] = font;
}

bool Game::MultiLineTextDisplay::isShowingDialogue() {
	return showingDialogue;
}

std::string Game::MultiLineTextDisplay::getLineOfText(size_t whichLine) {
	return linesOfText[whichLine];
}

void Game::MultiLineTextDisplay::showDialogue(std::string name, std::string text, std::string scriptToRunOnceComplete) {
	currentLine = 0;
	positionInDialogue = 0;
	positionInLine = 0;
	showingDialogue = true;

	for (auto& line : linesOfText) {
		line = "";
	}
	
	auto pair = characterFontAssociations.find(name);
	if (pair != characterFontAssociations.end()) {
		changeCurrentFont(pair->second);
	} else {
		changeCurrentFont(defaultFont);
	}

	auto pair2 = characterSoundAssociations.find(name);
	if (pair2 != characterSoundAssociations.end()) {
		currentSpeechSound = pair2->second;
	} else {
		currentSpeechSound = defaultSpeechSound;
	}

	for (auto& label : labels) {
		label->setText("");

		if (usingFontSize) {
			label->useFontSize();
		}
	}

	std::string splitText = text;
	int width = currentFont->getDimensionsOfText(splitText, labels[0]->getSize()).getWidth();

	if (width > maxWidthOfText) {
		size_t whichLine = 0;
		size_t lastPosOfSpace = 0;
		for (size_t i = 0; i < text.size(); i++) {
			if (text[i] == ' ' && currentFont->getDimensionsOfText(linesOfText[whichLine] + text.substr(lastPosOfSpace, i - lastPosOfSpace), labels[0]->getSize()).getWidth() <= maxWidthOfText) {
				linesOfText[whichLine] += text.substr(lastPosOfSpace, i - lastPosOfSpace);
				lastPosOfSpace = i;
			} else if (text[i] == ' ') {
				whichLine++;
				if (whichLine == linesOfText.size()) {
					linesOfText[whichLine - 1] += text.substr(lastPosOfSpace, text.size() - lastPosOfSpace);
					break;
				}
			} else if (i == text.size() - 1) {
				if (whichLine != linesOfText.size() - 1 && currentFont->getDimensionsOfText(linesOfText[whichLine] + text.substr(lastPosOfSpace, i - lastPosOfSpace), labels[0]->getSize()).getWidth() > maxWidthOfText) {
					whichLine++;
				}
				linesOfText[whichLine] += text.substr(lastPosOfSpace, text.size() - lastPosOfSpace);
			}
		}
	} else {
		linesOfText[0] = text;
	}

	maxCharactersOfText = 0;

	for (auto& line : linesOfText) {
		if (!line.empty()) {
			if (line[0] == ' ') {
				line = line.substr(1, line.size() - 1);
			}
			if (line[line.size() - 1] == ' ') {
				line = line.substr(0, line.size() - 1);
			}
			maxCharactersOfText += line.size();
		}
	}
	

	scriptOnDialogueEnd = std::move(scriptToRunOnceComplete);
	timeSinceNewCharacter = time->getCurrentTimeInMillis();
}

void Game::MultiLineTextDisplay::finishDialogueAnimationImmediately() {
	positionInDialogue = maxCharactersOfText;

	for (size_t i = 0; i < linesOfText.size(); i++) {
		labels[i]->setText(linesOfText[i]);
	}
}

void Game::MultiLineTextDisplay::closeDialog() {
	showingDialogue = false;
}

void Game::MultiLineTextDisplay::changeCurrentFont(Font* currentFont) {
	this->currentFont = currentFont;

	for (auto label : labels) {
		label->setFont(currentFont);
	}
}

bool Game::MultiLineTextDisplay::textScrollingHasCompleted() {
	return positionInDialogue == maxCharactersOfText;
}

bool Game::MultiLineTextDisplay::requiresBreathInSentence(char character) {
	return character == '.' || character == ',' || character == '?' || character == ';' || character == ':'
		|| character == '(' || character == '!';
}
