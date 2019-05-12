#include "LockTimer.h"
#include "../../Game Object Provider/GameObjectProvider.h"
#include "TimerDisplay.h"
#include "TimerHandPosition.h"

Game::LockTimer::LockTimer() {
	duration = 1000;
}

Game::LockTimer::LockTimer(long long milliseconds, TimerColour colour, std::function<void()> onEnd) {
	duration = milliseconds;
	this->colour = colour;
	this->onEnd = onEnd;
}

void Game::LockTimer::start() {
	Timer* timer = GameObjectProvider::getTimer();
	TimerDisplay* display = GameObjectProvider::getTimerDisplay();
	size_t id = display->showTimer(this);


	auto event1 = [display, id]() {
		display->updateTimerHandPosition(id, TimerHandPosition::SEVEN);
	};

	timer->scheduleEventInMillis(duration / 8, event1);

	auto event2 = [display, id]() {
		display->updateTimerHandPosition(id, TimerHandPosition::SIX);
	};

	timer->scheduleEventInMillis(duration / 4, event2);

	auto event3 = [display, id]() {
		display->updateTimerHandPosition(id, TimerHandPosition::FIVE);
	};

	timer->scheduleEventInMillis(duration / 8 * 3, event3);

	auto event4 = [display, id]() {
		display->updateTimerHandPosition(id, TimerHandPosition::FOUR);
	};

	timer->scheduleEventInMillis(duration / 2, event4);

	auto event5 = [display, id]() {
		display->updateTimerHandPosition(id, TimerHandPosition::THREE);
	};

	timer->scheduleEventInMillis(duration / 8 * 5, event5);

	auto event6 = [display, id]() {
		display->updateTimerHandPosition(id, TimerHandPosition::TWO);
	};

	timer->scheduleEventInMillis(duration / 4 * 3, event6);

	auto event7 = [display, id]() {
		display->updateTimerHandPosition(id, TimerHandPosition::ONE);
	};

	timer->scheduleEventInMillis(duration / 8 * 7, event7);

	auto event8 = [this, display, id]() {
		complete = true;
		display->updateTimerHandPosition(id, TimerHandPosition::ZERO);
		onEnd();
	};

	timer->scheduleEventInMillis(duration, event8);

	auto event9 = [this, display, id]() {
		display->removeTimer(id);
	};

	timer->scheduleEventInMillis(duration + REMOVE_TIMER_TIME, event9);
}

Game::TimerColour Game::LockTimer::getColour() {
	return colour;
}

void Game::LockTimer::setBackgroundImage(std::shared_ptr<ImageComponent> backgroundImage) {
	this->backgroundImage = backgroundImage;
}

std::shared_ptr<ImageComponent> Game::LockTimer::getBackgroundImage() {
	return backgroundImage;
}

void Game::LockTimer::setClockHandImage(std::shared_ptr<ImageComponent> clockHandImage) {
	this->clockHandImage = clockHandImage;
}

std::shared_ptr<ImageComponent> Game::LockTimer::getClockHandImage() {
	return clockHandImage;
}

void Game::LockTimer::setRect(std::shared_ptr<RectComponent> rect) {
	this->rect = rect;
}

std::shared_ptr<RectComponent> Game::LockTimer::getRect() {
	return rect;
}

bool Game::LockTimer::isComplete() {
	return complete;
}
