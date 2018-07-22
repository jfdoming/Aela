#include "HintDisplay.h"

Game::HintDisplay::HintDisplay() : defaultColour(0.9f, 0.9f, 0.9f, 1.0f) {
	timer = GameObjectProvider::getTimer();
}

void Game::HintDisplay::displayHint(std::string text, HintDisplayDuration hintDisplayDuration) {
	hintLabel->setText(text);
	hintLabel->setColour(&defaultColour);

	if (hintDisplayDuration != HintDisplayDuration::FOREVER) {
		auto closeHint = [this]() {
			hide();
		};

		timer->scheduleEventInMillis(getDisplayDurationInMillis(hintDisplayDuration), closeHint);
	}
}

void Game::HintDisplay::displayHint(std::string text, HintDisplayDuration hintDisplayDuration, ColourRGBA* colour) {
	hintLabel->setText(text);
	hintLabel->setColour(colour);

	if (hintDisplayDuration != HintDisplayDuration::FOREVER) {
		auto closeHint = [this]() {
			hide();
		};

		timer->scheduleEventInMillis(getDisplayDurationInMillis(hintDisplayDuration), closeHint);
	}
}

void Game::HintDisplay::hide() {
	hintLabel->setText("");
}

void Game::HintDisplay::setHintLabel(std::shared_ptr<Label> hintLabel) {
	this->hintLabel = hintLabel;
}

inline long long Game::HintDisplay::getDisplayDurationInMillis(HintDisplayDuration hintDisplayDuration) {
	switch (hintDisplayDuration) {
		case HintDisplayDuration::SHORT:
			return 1000;
		case HintDisplayDuration::LONG:
			return 3000;
		default:
			return 1000;
	}
}
