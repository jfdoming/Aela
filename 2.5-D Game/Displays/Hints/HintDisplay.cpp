#include "HintDisplay.h"
#include "../../Scripts/Scripts to Move to LUA/ScriptObjects.h"

Game::HintDisplay::HintDisplay() : defaultColour(0.9f, 0.9f, 0.9f, 1.0f) {
	timer = GameObjectProvider::getTimer();
	GameObjectProvider::getEventHandler()->addListener(EventConstants::KEY_PRESSED, bindListener(HintDisplay::onEvent, this));

}

void Game::HintDisplay::displayHint(std::string text, HintDisplayDuration hintDisplayDuration) {
	std::cout << "DISPLAYING HINT: " << text << "\n";
	Hint hint;
	hint.text = text;
	hint.duration = hintDisplayDuration;
	hint.colour = defaultColour;
	hintQueue.push(hint);
	keycodesToClearOn.push_back(std::vector<int>());

	if (!blockUntilKey && (!displayingHint || hint.duration == HintDisplayDuration::FOREVER)) {
		displayingHint = true;

		if (hint.duration == HintDisplayDuration::UNTIL_KEY) {
			blockUntilKey = true;
		}

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
	keycodesToClearOn.push_back(std::vector<int>());

	if (!blockUntilKey && (!displayingHint || hint.duration == HintDisplayDuration::FOREVER)) {
		displayingHint = true;

		if (hint.duration == HintDisplayDuration::UNTIL_KEY) {
			blockUntilKey = true;
		}

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
	keycodesToClearOn.clear();
	while (!hintQueue.empty()) {
		hintQueue.pop();
	}
}

void Game::HintDisplay::displayMostRecentHintUntilKeyPress(int keycode) {
	if (!keycodesToClearOn.empty()) {
		keycodesToClearOn[keycodesToClearOn.size() - 1] = std::vector<int>(1, keycode);
	}
}

void HintDisplay::displayMostRecentHintUntilKeyPresses(std::initializer_list<int> keycodes) {
	if (!keycodesToClearOn.empty()) {
		keycodesToClearOn[keycodesToClearOn.size() - 1] = keycodes;
	}
}

void Game::HintDisplay::onEvent(Event* event) {
	if (event->getType() == EventConstants::KEY_PRESSED) {
		auto* keyEvent = dynamic_cast<KeyEvent*>(event);

		if (!keycodesToClearOn.empty()) {
			for (int keycode : keycodesToClearOn[0]) {
				if (keyEvent->getKeycode() == keycode) {
					blockUntilKey = false;
					auto processHint = [this]() {
						processNextHint();
					};

					// This forces the hint to be processed during the next major update() call on the main thread.
					timer->scheduleEventInMillis(0, processHint);
				}
			}
		}
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

void HintDisplay::setSpacing(int spacing) {
	this->spacing = spacing;
}

bool HintDisplay::isDisplayingHint() {
	return hintBackdrop->isVisible();
}

void Game::HintDisplay::processNextHint() {
	if (hintQueue.size() == 0) {
		clear();
		return;
	}

	if (hintBackdrop->isVisible()) {
		keycodesToClearOn.erase(keycodesToClearOn.begin());
	}

	displayingHint = true;
	Hint hint = hintQueue.front();
	hintQueue.pop();
	std::string text = hint.text;
	hintLabel->setText(text);
	hintBackdrop->show();
	hintLabel->setColour(&hint.colour);

	int width = hintLabel->getFont()->getDimensionsOfText(text, hintLabel->getSize()).getWidth() + spacing * 4;
	hintBackdrop->setProperty(Transformable2DProperty::RECT_WIDTH, (float) width);

	if (hint.duration != HintDisplayDuration::FOREVER && hint.duration != HintDisplayDuration::UNTIL_KEY) {
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
			return 4000;
		default:
			return 1000;
	}
}
