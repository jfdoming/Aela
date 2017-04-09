#pragma once
#include "SDL.h"
#include "../Window/Window.h"
#include "../Control Manager/ControlManager.h"

class EventHandler {
private:
	SDL_Event event;
	Window* window;
	ControlManager* controlManager;

public:
	EventHandler();
	~EventHandler();

	void updateEvents();
	void bindWindow(Window* window);
	void bindControlManager(ControlManager* controlManager);
	SDL_Event getEvent();
};