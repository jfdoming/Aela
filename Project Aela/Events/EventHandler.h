#pragma once
#include "SDL.h"
#include <vector>
#include <functional>
#include <unordered_map>
#include <iterator>
#include "../Window/Window.h"
#include "../Control Manager/ControlManager.h"

class EventHandler {
	private:
		std::unordered_map<Uint32, std::unordered_map<int, std::vector<std::function<void()>>>> listeners;

		SDL_Event event;
		Window* window;
		ControlManager* controlManager;

		const Uint8* keystate;

	public:
		EventHandler();
		~EventHandler();

		void updateEvents();
		void bindWindow(Window* window);
		void bindControlManager(ControlManager* controlManager);

		void bindFunction(Uint32 type, int key, std::function<void()> func);

		template <class T>
		void bindMemberFunction(Uint32 type, int key, std::function<void(T&)> func, T obj) {
			bindFunction(type, key, std::bind(func, obj));
		}

		void invokeBoundFunctions(Uint32 type, int key);

		const Uint8* getKeystate();
};