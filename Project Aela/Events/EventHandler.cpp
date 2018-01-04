#include <iostream>

#include "EventHandler.h"
#include "EventConstants.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

#include <memory>

using namespace Aela;

EventHandler::EventHandler() {
}

EventHandler::~EventHandler() {
	// dispatcher's destructor will be called to verify thread stops, no need to do anything
}

void EventHandler::start() {
	dispatcher.runAsync();
}

void EventHandler::stop() {
	dispatcher.stopBlocking();
}

void EventHandler::updateSDLEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				dispatcher.stopAsync();
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
				dispatcher.fireEvent(new KeyEvent(EventConstants::KEY_PRESSED, event.key.keysym.sym, event.key.keysym.mod));
				break;
			case SDL_KEYUP:
				dispatcher.fireEvent(new KeyEvent(EventConstants::KEY_RELEASED, event.key.keysym.sym, event.key.keysym.mod));
				break;
			case SDL_MOUSEBUTTONDOWN:
				dispatcher.fireEvent(new MouseEvent(EventConstants::MOUSE_PRESSED,
											event.button.button,
											event.key.keysym.mod,
											event.button.clicks,
											event.button.x,
											event.button.y,
											event.wheel.y));
				break;
			case SDL_MOUSEBUTTONUP:
				dispatcher.fireEvent(new MouseEvent(EventConstants::MOUSE_RELEASED, 
											event.button.button,
											event.key.keysym.mod,
											event.button.clicks,
											event.button.x,
											event.button.y,
											event.wheel.y));
				break;
			case SDL_MOUSEMOTION:
				dispatcher.fireEvent(new MouseEvent(EventConstants::MOUSE_MOTIONED,
											event.button.button,
											event.key.keysym.mod,
											event.button.clicks,
											event.button.x,
											event.button.y,
											event.wheel.y));
				break;
			case SDL_MOUSEWHEEL:
				dispatcher.fireEvent(new MouseEvent(EventConstants::MOUSE_WHEEL,
											event.button.button,
											event.key.keysym.mod,
											event.button.clicks,
											event.button.x,
											event.button.y,
											event.wheel.y));
				break;
		}
	}
}

void EventHandler::addListener(int type, EventListener listener) {
	dispatcher.addListener(type, listener);
}

void EventHandler::bindWindow(Window* window) {
	this->window = window;
}