/*
* Class: Project Aela's Keyed 3D Animator
* Author: Robert Ciborowski
* Date: 20/08/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*              The transformations occur based on the user's key inputs.
*/

#include "KeyedAnimator3D.h"
#include "../../Events/KeyEvent.h"
#include "../../Events/EventConstants.h"

void KeyedAnimator::onEvent(Event* event) {
	KeyEvent* keyEvent = dynamic_cast<KeyEvent*>(event);

	if (window->isFocused()) {
		if (keyEvent->getKeycode() == 225) {
			currentSpeed = superSpeed;
		} else {
			currentSpeed = speed;
		}

		switch (keyEvent->getType()) {
		case EventConstants::KEY_PRESSED:
			if (keyEvent->getKeycode() == SDLK_w) {
				forward = true;
			}

			if (keyEvent->getKeycode() == SDLK_s) {
				backward = true;
			}

			if (keyEvent->getKeycode() == SDLK_d) {
				right = true;
			}

			if (keyEvent->getKeycode() == SDLK_a) {
				left = true;
			}

			if (keyEvent->getKeycode() == SDLK_SPACE) {
				up = true;
			}

			if (keyEvent->getKeycode() == SDLK_LCTRL) {
				down = true;
			}
			break;
		case EventConstants::KEY_RELEASED:
			if (keyEvent->getKeycode() == SDLK_w) {
				forward = false;
			}

			if (keyEvent->getKeycode() == SDLK_s) {
				backward = false;
			}

			if (keyEvent->getKeycode() == SDLK_d) {
				right = false;
			}

			if (keyEvent->getKeycode() == SDLK_a) {
				left = false;
			}

			if (keyEvent->getKeycode() == SDLK_SPACE) {
				up = false;
			}

			if (keyEvent->getKeycode() == SDLK_LCTRL) {
				down = false;
			}
			break;
		}
	}
}

void KeyedAnimator::update() {
	float deltaTime = timeManager->getTimeBetweenFrames();
	for (auto pair : transformables) {
		Transformable3D* transformable = pair.second;
		float amount = currentSpeed * deltaTime;

		if (forward) {
			*transformable->getPosition() += glm::vec3(0, 0, amount);
		}
		if (backward) {
			*transformable->getPosition() -= glm::vec3(0, 0, amount);
		}
		if (right) {
			*transformable->getPosition() -= glm::vec3(amount, 0, 0);
		}
		if (left) {
			*transformable->getPosition() += glm::vec3(amount, 0, 0);
		}
		if (up) {
			*transformable->getPosition() += glm::vec3(0, amount, 0);
		}
		if (down) {
			*transformable->getPosition() -= glm::vec3(0, amount, 0);
		}
	}
}

void KeyedAnimator::setTimeManager(TimeManager* timeManager) {
	this->timeManager = timeManager;
}

TimeManager* KeyedAnimator::getTimeManager() {
	return timeManager;
}

void KeyedAnimator::setWindow(Window* window) {
	this->window = window;
}

Window* KeyedAnimator::getWindow() {
	return window;
}

bool KeyedAnimator::addTransformable(int key, Transformable3D* transformable) {
	if (transformables.find(key) == transformables.end()) {
		transformables[key] = transformable;
		return true;
	}
	return false;
}

int KeyedAnimator::addTransformable(Transformable3D* transformable) {
	transformables[transformables.size()] = transformable;
	return transformables.size();
}

bool KeyedAnimator::removeTransformable(int key) {
	auto position = transformables.find(key);
	if (position != transformables.end()) {
		transformables.erase(position);
		return true;
	}
	return false;
}
