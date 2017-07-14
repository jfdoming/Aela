#pragma once
#include "SDL.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include <iterator>
#include "../Window/Window.h"
#include "../Control Manager/ControlManager.h"

namespace Aela {
	class EventHandler {

		SDL_Event event;
		Window* window;

		public:
		EventHandler();
		~EventHandler();

		void bindWindow(Window* window);
		void updateSDLEvents();
		void fireEvents();
	};
}