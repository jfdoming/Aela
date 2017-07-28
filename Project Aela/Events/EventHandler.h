#pragma once
#include <forward_list>
#include <queue>
#include <thread>
#include <unordered_map>
#include "SDL.h"
#include "lua.hpp"
#include "../Lua/LuaBridge/LuaBridge.h"
#include "../Window/Window.h"
#include "Event.h"
#include "Listener.h"

namespace Aela {
	class EventHandler {
		private:
			SDL_Event event;
			Window* window;

			bool running;

			std::queue<Event*> eventQueue;
			std::unordered_map<int, std::forward_list<Listener*>> listeners; // Listeners defined in C++
			//std::unordered_map<int, luabridge::LuaRef> callbacks; // Listeners defined in Lua

		public:
			EventHandler();
			~EventHandler();

			void start();
			void updateSDLEvents();
			void fireEvent(Event* event);
			void dispatchEvents();
			void addListener(int type, Listener* listener);

			void bindWindow(Window* window);
	};
}