/*
* Class: TimerDisplay
* Author: Robert Ciborowski
* Date: 02/12/2018
* Description: A class used to represent the display which shows how much
*              each timer has left.
*/

#pragma once
#include "../Tiles/TileInventory.h"
#include "../../Project Aela/Menus/ImageComponent.h"
#include "TimerHandPosition.h"
#include "LockTimer.h"
using namespace Aela;

namespace Game {
	class TimerDisplay {
		public:
			TimerDisplay();

			void setup();

			size_t showTimer(LockTimer* timer);
			void updateTimerHandPosition(size_t id, TimerHandPosition position);
			bool removeTimer(size_t id);
			void clear();

		private:
			std::map<size_t, LockTimer*> timers;
			std::vector<GLTexture*> backgroundTextures;
			std::vector<GLTexture*> clockHandTextures;
			const std::string ICONS_LOCATION = "res/textures/icons_1/";
			const unsigned int NUMBER_OF_TIMER_COLOURS = 8;
			const unsigned int NUMBER_OF_TIMER_POSITIONS = 8;
			const long long ANIMATION_TIME = 250;
			void loadResources();
	};
}