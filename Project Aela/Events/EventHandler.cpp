#include "EventHandler.h"

EventHandler::EventHandler() {

}

EventHandler::~EventHandler() {

}

void EventHandler::updateEvents() {
	while (SDL_PollEvent(&event)) {
		keystate = SDL_GetKeyboardState(NULL);

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
				invokeBoundFunctions(SDL_KEYDOWN, event.key.keysym.scancode);
				break;
			case SDL_KEYUP:
				invokeBoundFunctions(SDL_KEYUP, event.key.keysym.scancode);
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

void EventHandler::bindFunction(Uint32 type, int key, std::function<void()> func) {
	auto iter = listeners.find(type);

	if (iter == listeners.end()) {
		std::unordered_map<int, std::vector<std::function<void()>>> _boundFuncs;
		std::vector<std::function<void()>> _funcs;

		_funcs.push_back(func);
		_boundFuncs.emplace(key, _funcs);
		listeners.emplace(type, _boundFuncs);
	} else {
		auto _iter = iter->second.find(key);

		if (_iter == iter->second.end()) {
			std::vector<std::function<void()>> _funcs;

			_funcs.push_back(func);
			iter->second.emplace(key, _funcs);
		} else {
			_iter->second.push_back(func);
		}
	}
}

void EventHandler::invokeBoundFunctions(Uint32 type, int key) {
	auto iter = listeners.find(type);

	if (iter == listeners.end()) {
		return;
	}

	auto _iter = iter->second.find(key);

	if (_iter == iter->second.end()) {
		return;
	}

	std::function<void()> func;

	for (int i = 0; i < _iter->second.size(); i++) {
		func = _iter->second.at(i);
		func();
	}
}

const Uint8* EventHandler::getKeystate() {
	return keystate;
}
