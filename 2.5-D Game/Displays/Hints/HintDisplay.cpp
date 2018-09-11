#include "HintDisplay.h"

Game::HintDisplay::HintDisplay() : defaultColour(0.9f, 0.9f, 0.9f, 1.0f) {
	timer = GameObjectProvider::getTimer();
}

void Game::HintDisplay::displayHint(std::string text, HintDisplayDuration hintDisplayDuration) {
	Hint hint;
	hint.text = text;
	hint.duration = hintDisplayDuration;
	hint.colour = defaultColour;
	hintQueue.push(hint);

	if (!displayingHint || hint.duration == HintDisplayDuration::FOREVER) {
		displayingHint = true;

		auto processHint = [this]() {
				processNextHint();
		};

		// This forces the hint to be processed during the next major update() call on the main thread.
		timer->scheduleEventInMillis(0, processHint);
	}
}

void Game::HintDisplay::displayHint(std::string text, HintDisplayDuration hintDisplayDuration, ColourRGBA* colour) {
	Hint hint;
	hint.text = text;
	hint.duration = hintDisplayDuration;
	hint.colour = *colour;
	hintQueue.push(hint);

	if (!displayingHint || hint.duration == HintDisplayDuration::FOREVER) {
		displayingHint = true;

		auto processHint = [this]() {
			processNextHint();
		};

		// This forces the hint to be processed during the next major update() call on the main thread.
		timer->scheduleEventInMillis(0, processHint);
	}
}

void Game::HintDisplay::clearAndDisplayHint(std::string text, HintDisplayDuration hintDisplayDuration) {
	clear();
	displayHint(text, hintDisplayDuration);
}

void Game::HintDisplay::clearAndDisplayHint(std::string text, HintDisplayDuration hintDisplayDuration, ColourRGBA* colour) {
	clear();
	displayHint(text, hintDisplayDuration, colour);
}

void Game::HintDisplay::clear() {
	hintLabel->setText("");
	hintBackdrop->hide();
	displayingHint = false;
	while (!hintQueue.empty()) {
		hintQueue.pop();
	}
}

void Game::HintDisplay::setHintLabel(std::shared_ptr<Label> hintLabel) {
	this->hintLabel = hintLabel;
}

void Game::HintDisplay::setHintBackdrop(std::shared_ptr<ImageComponent> hintBackdrop) {
	this->hintBackdrop = hintBackdrop;
}

std::string Game::HintDisplay::getCurrentHint() {
	return hintLabel->getText();
}

void Game::HintDisplay::processNextHint() {
	if (hintQueue.size() == 0) {
		clear();
		return;
	}

	displayingHint = true;
	Hint hint = hintQueue.front();
	hintQueue.pop();
	std::string text = hint.text;
	hintLabel->setText(text);
	hintBackdrop->show();
	hintLabel->setColour(&hint.colour);

	int width = hintLabel->getFont()->getDimensionsOfText(text).getWidth();
	hintBackdrop->setProperty(Transformable2DProperty::RECT_WIDTH, (float) width);

	if (hint.duration != HintDisplayDuration::FOREVER) {
		auto closeHint = [this, text]() {
			if (hintLabel->getText() == text) {
				processNextHint();
			}
		};

		timer->scheduleEventInMillis(getDisplayDurationInMillis(hint.duration), closeHint);
	}
}

inline long long Game::HintDisplay::getDisplayDurationInMillis(HintDisplayDuration hintDisplayDuration) {
	switch (hintDisplayDuration) {
		case HintDisplayDuration::SHORT:
			return 1750;
		case HintDisplayDuration::LONG:
			return 3500;
		default:
			return 1000;
	}
}
