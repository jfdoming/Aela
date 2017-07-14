#include "EventHandler.h"

using namespace Aela;

EventHandler::EventHandler() {

}

EventHandler::~EventHandler() {

}

void EventHandler::updateSDLEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				window->quit();
				break;
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_FOCUS_GAINED:
						window->setFocus(true);
						break;
					case SDL_WINDOWEVENT_FOCUS_LOST:
						window->setFocus(false);
						break;
				}
				break;
			case SDL_KEYDOWN:

				break;
			case SDL_KEYUP:
				break;
		}
	}
}

void EventHandler::bindWindow(Window* window) {
	this->window = window;
}
