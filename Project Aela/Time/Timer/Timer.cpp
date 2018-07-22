#include "Timer.h"

using namespace Aela;

Timer::Timer(Time* time) {
	this->time = time;
}

void Aela::Timer::scheduleEventInNanos(long long waitTime, std::function<void()> event) {
	timedEvents.push_back(std::pair<long long, std::function<void()>>(time->getCurrentTimeInNanos()
		+ waitTime, event));
}

void Aela::Timer::scheduleEventInMicros(long long waitTime, std::function<void()> event) {
	timedEvents.push_back(std::pair<long long, std::function<void()>>(time->getCurrentTimeInNanos()
		+ waitTime * 1000, event));
}


void Timer::scheduleEventInMillis(long long waitTime, std::function<void()> event) {
	timedEvents.push_back(std::pair<long long, std::function<void()>>(time->getCurrentTimeInNanos()
		+ waitTime * 1000000, event));
}

void Aela::Timer::scheduleEventInSeconds(long long waitTime, std::function<void()> event) {
	timedEvents.push_back(std::pair<long long, std::function<void()>>(time->getCurrentTimeInNanos()
		+ waitTime * 1000000000, event));
}

void Timer::update() {
	long long currentTime = time->getCurrentTimeInNanos();
	
	for (size_t i = 0; i < timedEvents.size(); i++) {
		auto pair = timedEvents[i];
		if (pair.first <= currentTime) {
			pair.second();
			timedEvents.erase(timedEvents.begin() + i);
			i--;
		}
	}
}