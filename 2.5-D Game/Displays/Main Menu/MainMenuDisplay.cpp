#include "MainMenuDisplay.h"
#include "../../Game Object Provider/GameObjectProvider.h"
#include "../Scripts/Scripts to Move to LUA/SceneIDs.h"
#include "../../Audio/GameAudioPlayer.h"

Game::MainMenuDisplay::MainMenuDisplay() {
}

void Game::MainMenuDisplay::setup() {
	GameObjectProvider::getEventHandler()->addListener(EventConstants::KEY_PRESSED, bindListener(MainMenuDisplay::onEvent, this));
}

void Game::MainMenuDisplay::update() {
	if (pressedReturn) {
		pressedReturn = false;
		// GameObjectProvider::getGameAudioPlayer()->playAudio("select");
		onSelects[currentOption]();
	}
}

void Game::MainMenuDisplay::addOption(std::shared_ptr<Label> label, std::function<void()> onEnd) {
	labels.push_back(label);
	onSelects.push_back(onEnd);
}

void Game::MainMenuDisplay::setSelectorImage(std::shared_ptr<ImageComponent> selectorImage) {
	this->selectorImage = selectorImage;

	if (!labels.empty()) {
		updateSelectorImagePosition();
	}
}

void Game::MainMenuDisplay::onEvent(Event* event) {
	if (!enabled) {
		return;
	}

	if (GameObjectProvider::getSceneManager()->getCurrentSceneId() == MAIN_MENU_SCENE) {
		if (event->getType() == EventConstants::KEY_PRESSED) {
			auto* keyEvent = dynamic_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
				case SDLK_RETURN:
					pressedReturn = true;
					break;
				case SDLK_w:
					selectPrevious();
					break;
				case SDLK_s:
					selectNext();
					break;
				case SDLK_a:
					selectPrevious();
					break;
				case SDLK_d:
					selectNext();
					break;
				case SDLK_UP:
					selectPrevious();
					break;
				case SDLK_DOWN:
					selectNext();
					break;
				case SDLK_LEFT:
					selectPrevious();
					break;
				case SDLK_RIGHT:
					selectNext();
					break;
			}
		}
	}
}

void Game::MainMenuDisplay::disable() {
	enabled = false;
}

void Game::MainMenuDisplay::enable() {
	enabled = true;
}

void Game::MainMenuDisplay::selectNext() {
	if (labels.empty()) {
		return;
	}

	currentOption++;

	if (currentOption == labels.size()) {
		currentOption = 0;
	}

	GameObjectProvider::getGameAudioPlayer()->playAudio("option");
	updateSelectorImagePosition();
}

void Game::MainMenuDisplay::selectPrevious() {
	if (labels.empty()) {
		return;
	}

	if (currentOption == 0) {
		currentOption = labels.size() - 1;
	} else {
		currentOption--;
	}

	GameObjectProvider::getGameAudioPlayer()->playAudio("option");
	updateSelectorImagePosition();
}

void Game::MainMenuDisplay::updateSelectorImagePosition() {
	auto dimensions = labels[currentOption]->getDimensions();
	selectorImage->setXY(dimensions->getX() - selectorImage->getDimensions()->getWidth() - SELECTOR_IMAGE_X_OFFSET, dimensions->getY() - dimensions->getHeight());
}
