#include "EventHandler.h"
#include "EventConstants.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

using namespace Aela;

EventHandler::EventHandler() {
	
}

EventHandler::~EventHandler() {

}

void EventHandler::start() {
	running = true;
	std::thread eventThread(&EventHandler::dispatchEvents, this);
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
				fireEvent(new KeyEvent(EventConstants::KEY_PRESSED, event.key.keysym.sym, event.key.keysym.mod));
				break;
			case SDL_KEYUP:
				fireEvent(new KeyEvent(EventConstants::KEY_RELEASED, event.key.keysym.sym, event.key.keysym.mod));
				break;
			case SDL_MOUSEBUTTONDOWN:
				fireEvent(new MouseEvent(EventConstants::MOUSE_PRESSED,
											event.button.button,
											event.key.keysym.mod,
											event.button.clicks,
											event.button.x,
											event.button.y));
				break;
			case SDL_MOUSEBUTTONUP:
				fireEvent(new MouseEvent(EventConstants::MOUSE_RELEASED, 
											event.button.button,
											event.key.keysym.mod,
											event.button.clicks,
											event.button.x,
											event.button.y));
				break;
		}
	}
}

void EventHandler::dispatchEvents() {
	while (running) {
		if (!eventQueue.empty()) {
			Event* event = eventQueue.front();

			auto iter = listeners.find(event->getType());
			if (iter != listeners.end()) {
				std::forward_list<Listener*> list = iter->second;

				for (auto i = list.begin(); i != list.end(); ++i) {
					(*i)->onEvent(event);
				}
			}

			eventQueue.pop();
			delete event;
		}
	}
}

void EventHandler::fireEvent(Event* event) {
	eventQueue.push(event);
}

void EventHandler::addListener(int type, Listener* listener) {
	auto iter = listeners.find(type);
	if (iter != listeners.end()) {
		iter->second.push_front(listener);
	} else {
		std::forward_list<Listener*> list;
		list.push_front(listener);
		listeners.insert(std::make_pair(type, list));
	}
}

void EventHandler::bindWindow(Window* window) {
	this->window = window;
}
