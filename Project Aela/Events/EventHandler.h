#pragma once

#include "SDL.h"
#include "../Window/Window.h"
#include "Event.h"
#include "EventListener.h"
#include "EventDispatcher.h"
#include "EventListenerList.h"

namespace Aela {
	class EventHandler {
		public:
			EventHandler();
			~EventHandler();

			void start();
			void stop();

			void updateSDLEvents();
			void addListener(int type, EventListener listener);

			void bindWindow(Window* window);
		private:
			SDL_Event event;
			Window* window;

			EventDispatcher dispatcher;
	};
}