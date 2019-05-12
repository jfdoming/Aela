/*
* Class: LockTimer
* Author: Robert Ciborowski
* Date: 02/12/2018
* Description: A class used to represent a timer.
*/

#pragma once
#include <functional>
#include "TimerColour.h"
#include "../../../Project Aela/Menus/ImageComponent.h"
#include "../../../Project Aela/Menus/RectComponent.h"

namespace Game {
	class LockTimer {
		public:
			LockTimer();
			LockTimer(long long milliseconds, TimerColour colour, std::function<void()> onEnd);

			void start();

			TimerColour getColour();
			void setBackgroundImage(std::shared_ptr<ImageComponent> backgroundImage);
			std::shared_ptr<ImageComponent> getBackgroundImage();
			void setClockHandImage(std::shared_ptr<ImageComponent> clockHandImage);
			std::shared_ptr<ImageComponent> getClockHandImage();
			void setRect(std::shared_ptr<RectComponent> rect);
			std::shared_ptr<RectComponent> getRect();
			bool isComplete();
			
		private:
			long long startTime, duration;
			const long long REMOVE_TIMER_TIME = 1000;
			bool complete = false;
			TimerColour colour;
			std::function<void()> onEnd;
			std::shared_ptr<ImageComponent> backgroundImage;
			std::shared_ptr<ImageComponent> clockHandImage;
			std::shared_ptr<RectComponent> rect;
	};}
