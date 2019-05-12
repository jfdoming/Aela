#include "TimerDisplay.h"
#include "../../../Project Aela/Utilities/enumut.h"
#include "../../../Project Aela/Menus/RectComponent.h"
#include "../../Aela Game/AelaGame.h"

Game::TimerDisplay::TimerDisplay() {
}

void Game::TimerDisplay::setup() {
}

size_t Game::TimerDisplay::showTimer(LockTimer* timer) {
	if (clockHandTextures.size() == 0) {
		loadResources();
	}

	size_t id = 0;
	bool animate = true;
	if (timers.size() > 0) {
		while (true) {
			auto iter = timers.find(id);
			if (iter == timers.end()) {
				break;
			}
			// if (iter->second->isComplete()) {
			// 	animate = false;
			// 	removeTimer(id);
			// 	break;
			// }
			id++;
		}
	}

	GLTexture* backgroundTexture = backgroundTextures[enumToInteger(timer->getColour())];
	GLTexture* clockHandTexture = clockHandTextures[0];
	Rect<int> renderDimensions = *GameObjectProvider::getGame()->getRenderingDimensions();
	Rect<int> endDimensions((int) (renderDimensions.getWidth() * 0.9), (int) (renderDimensions.getHeight() / 7.0 * (2 + id)),
		(int) (renderDimensions.getHeight() / 8.0), (int) (renderDimensions.getHeight() / 8.0));
	Rect<int> rectEndDimensions((int) (renderDimensions.getWidth() * 0.885), (int) (renderDimensions.getHeight() / 7.0 * (1.85 + id)),
		(int) (renderDimensions.getHeight() / 4.0), (int) (renderDimensions.getHeight() / 6.0));

	auto backgroundImage = std::make_shared<ImageComponent>();
	backgroundImage->setPositioningMode(PositioningMode2D::TOP_LEFT);
	backgroundImage->setTexture(backgroundTexture);

	auto clockHandImage = std::make_shared<ImageComponent>();
	clockHandImage->setPositioningMode(PositioningMode2D::TOP_LEFT);
	clockHandImage->setTexture(clockHandTexture);

	auto rect = std::make_shared<RectComponent>();
	rect->setPositioningMode(PositioningMode2D::TOP_LEFT);
	rect->setDimensions(&rectEndDimensions);
	ColourRGBA rectColour(0.169f, 0.169f, 0.169f, 1);
	rect->setColour(&rectColour);

	GameObjectProvider::getGameplayScene()->getMenu()->add(rect);
	GameObjectProvider::getGameplayScene()->getMenu()->add(backgroundImage);
	GameObjectProvider::getGameplayScene()->getMenu()->add(clockHandImage);

	if (animate) {
		Rect<int> startDimensions((int) renderDimensions.getWidth(), (int) (renderDimensions.getHeight() / 7.0 * (2 + id)),
		(int) (renderDimensions.getHeight() / 8.0), (int) (renderDimensions.getHeight() / 8.0));
		Rect<int> rectStartDimensions((int) renderDimensions.getWidth(), (int) (renderDimensions.getHeight() / 7.0 * (1.85 + id)),
		(int) (renderDimensions.getHeight() / 4.0), (int) (renderDimensions.getHeight() / 6.0));
		backgroundImage->setDimensions(&startDimensions);
		clockHandImage->setDimensions(&startDimensions);
		rect->setDimensions(&rectStartDimensions);

		Animator* animator = GameObjectProvider::getAnimator();
		AnimationTrack2D track;
		KeyFrame2D frame;
		frame.setObject(backgroundImage);
		frame.setDimensions(&endDimensions);
		track.addKeyFrameUsingMillis(ANIMATION_TIME, &frame);
		animator->addAnimationTrack2D(&track);

		AnimationTrack2D track2;
		KeyFrame2D frame2;
		frame2.setObject(clockHandImage);
		frame2.setDimensions(&endDimensions);
		track2.addKeyFrameUsingMillis(ANIMATION_TIME, &frame2);
		animator->addAnimationTrack2D(&track2);

		AnimationTrack2D track3;
		KeyFrame2D frame3;
		frame3.setObject(rect);
		frame3.setDimensions(&rectEndDimensions);
		track3.addKeyFrameUsingMillis(ANIMATION_TIME, &frame3);
		animator->addAnimationTrack2D(&track3);

	} else {
		backgroundImage->setDimensions(&endDimensions);
		clockHandImage->setDimensions(&endDimensions);
		rect->setDimensions(&rectEndDimensions);
	}

	timer->setRect(rect);
	timer->setBackgroundImage(backgroundImage);
	timer->setClockHandImage(clockHandImage);
	timers.insert(std::make_pair(id, timer));
	return id;
}

void Game::TimerDisplay::updateTimerHandPosition(size_t id, TimerHandPosition position) {
	if (timers.find(id) == timers.end()) {
		return;
	}
	
	if (position == TimerHandPosition::ZERO) {
		timers[id]->getClockHandImage()->setTexture(clockHandTextures[0]);
		return;
	}
	timers[id]->getClockHandImage()->setTexture(clockHandTextures[enumToInteger(position)]);
}

bool Game::TimerDisplay::removeTimer(size_t id) {
	auto iter = timers.find(id);
	if (iter != timers.end()) {
		LockTimer* timer = timers[id];
		if (timer->isComplete()) {
			Rect<int> renderingDimensions = *GameObjectProvider::getGame()->getRenderingDimensions();
			Rect<int> endDimensions((int) renderingDimensions.getWidth(), (int) (renderingDimensions.getHeight() / 7.0 * (2 + id)),
				(int) (renderingDimensions.getHeight() / 8.0), (int) (renderingDimensions.getHeight() / 8.0));
			Rect<int> rectEndDimensions((int) renderingDimensions.getWidth(), (int) (renderingDimensions.getHeight() / 7.0 * (1.85 + id)),
				(int) (renderingDimensions.getHeight() / 4.0), (int) (renderingDimensions.getHeight() / 6.0));
			auto backgroundImage = timer->getBackgroundImage();
			auto clockHandImage = timer->getClockHandImage();
			auto rect = timer->getRect();

			auto onEnd = [this, timer, id]() {
				GameObjectProvider::getGameplayScene()->getMenu()->remove(timer->getBackgroundImage());
				GameObjectProvider::getGameplayScene()->getMenu()->remove(timer->getClockHandImage());
				GameObjectProvider::getGameplayScene()->getMenu()->remove(timer->getRect());
				auto iter = timers.find(id);
				if (iter != timers.end()) {
					timers.erase(iter);
					delete timer;
				}
			};

			Animator* animator = GameObjectProvider::getAnimator();
			AnimationTrack2D track;
			KeyFrame2D frame;
			frame.setObject(backgroundImage);
			frame.setDimensions(&endDimensions);
			frame.setEndingAction(onEnd);
			track.addKeyFrameUsingMillis(ANIMATION_TIME, &frame);
			animator->addAnimationTrack2D(&track);

			AnimationTrack2D track2;
			KeyFrame2D frame2;
			frame2.setObject(clockHandImage);
			frame2.setDimensions(&endDimensions);
			track2.addKeyFrameUsingMillis(ANIMATION_TIME, &frame2);
			animator->addAnimationTrack2D(&track2);

			AnimationTrack2D track3;
			KeyFrame2D frame3;
			frame3.setObject(rect);
			frame3.setDimensions(&rectEndDimensions);
			track3.addKeyFrameUsingMillis(ANIMATION_TIME, &frame3);
			animator->addAnimationTrack2D(&track3);
			return true;
		}
	}
	return false;
}

void Game::TimerDisplay::clear() {
	for (auto timer : timers) {
		GameObjectProvider::getGameplayScene()->getMenu()->remove(timer.second->getBackgroundImage());
		GameObjectProvider::getGameplayScene()->getMenu()->remove(timer.second->getClockHandImage());
		GameObjectProvider::getGameplayScene()->getMenu()->remove(timer.second->getRect());
	}
}

void Game::TimerDisplay::loadResources() {
	ResourceManager* resourceManager = GameObjectProvider::getResourceManager();
	GLTexture* texture;

	for (unsigned int i = 0; i < NUMBER_OF_TIMER_COLOURS; i++) {
		if (resourceManager->obtain<GLTexture>(ICONS_LOCATION + std::to_string(i) + "/0.png", texture)) {
			backgroundTextures.push_back(texture);
		}
	}

	for (unsigned int i = 0; i < NUMBER_OF_TIMER_POSITIONS; i++) {
		if (resourceManager->obtain<GLTexture>(ICONS_LOCATION + std::to_string(i) + "/1.png", texture)) {
			clockHandTextures.push_back(texture);
		}
	}
}
