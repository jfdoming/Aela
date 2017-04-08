#include "EventHandler.h"

EventHandler::EventHandler() {

}

EventHandler::~EventHandler() {

}

void EventHandler::updateEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				window->quit();
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				window->setFocus(true);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				window->setFocus(false);
				break;
			case SDL_KEYDOWN:
				controlManager->updateEvents(&event);
				break;
			case SDL_KEYUP:
				controlManager->updateEvents(&event);
				break;
		}
	}
}

void EventHandler::bindWindow(Window* window) {
	this->window = window;
}

void EventHandler::bindControlManager(ControlManager* controlManager) {
	this->controlManager = controlManager;
}

SDL_Event EventHandler::getEvent() {
	return event;
}