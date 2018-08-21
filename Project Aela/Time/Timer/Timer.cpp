#include "Timer.h"

using namespace Aela;

Timer::Timer(Time* time) {
	this->time = time;
}

std::string Aela::Timer::scheduleEventInNanos(long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime;
	std::string tag = std::to_string((size_t) &event);
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
	return tag;
}

std::string Aela::Timer::scheduleEventInMicros(long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000;
	std::string tag = std::to_string((size_t) &event);
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
	return tag;
}

std::string Timer::scheduleEventInMillis(long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000000;
	std::string tag = std::to_string((size_t) &event);
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
	return tag;
}

std::string Aela::Timer::scheduleEventInSeconds(long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000000000;
	std::string tag = std::to_string((size_t) &event);
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
	return tag;
}

void Aela::Timer::scheduleEventInNanos(std::string tag, long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime;
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
}

void Aela::Timer::scheduleEventInMicros(std::string tag, long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000;
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
}

void Aela::Timer::scheduleEventInMillis(std::string tag, long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000000;
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
}

void Aela::Timer::scheduleEventInSeconds(std::string tag, long long waitTime, std::function<void()> event) {
	long long timeOfEvent = time->getCurrentTimeInNanos() + waitTime * 1000000000;
	TimedEvent timedEvent;
	timedEvent.time = timeOfEvent;
	timedEvent.event = event;
	timedEvent.tag = tag;
	timedEvents.push_back(timedEvent);
}

bool Aela::Timer::removeEvent(std::string tag) {
	for (size_t i = 0; i < timedEvents.size(); i++) {
		if (timedEvents[i].tag == tag) {
			timedEvents.erase(timedEvents.begin() + i);
			return true;
		}
	}

	return false;
}

void Timer::update() {
	long long currentTime = time->getCurrentTimeInNanos();

	for (size_t i = 0; i < timedEvents.size(); i++) {
		TimedEvent event = timedEvents[i];
		if (event.time <= currentTime) {
			event.event();
			timedEvents.erase(timedEvents.begin() + i);
			i--;
		}
	}
}