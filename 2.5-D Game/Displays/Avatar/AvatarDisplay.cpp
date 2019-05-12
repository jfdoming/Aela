#include "AvatarDisplay.h"
#include "../../Game Object Provider/GameObjectProvider.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"

Game::AvatarDisplay::AvatarDisplay() {
}

Game::AvatarDisplay::~AvatarDisplay() {
}

void Game::AvatarDisplay::update() {
	if (blinkingAvatarTexture != nullptr && avatarTexture != nullptr) {
		long long currentTime = GameObjectProvider::getTime()->getCurrentTimeInMillis();
		if (blinking && lastAvatarUpdateTime + BLINK_TIME < currentTime) {
			blinking = false;
			lastAvatarUpdateTime = currentTime;
			avatarImage->setTexture(avatarTexture);
		} else if (!blinking && lastAvatarUpdateTime + TIME_BETWEEN_BLINKS < currentTime) {
			blinking = true;
			lastAvatarUpdateTime = currentTime;
			avatarImage->setTexture(blinkingAvatarTexture);
		}
	}
}

void Game::AvatarDisplay::showAvatar(std::string openEyeSrc) {
	showAvatar(openEyeSrc, "");
}

void Game::AvatarDisplay::showAvatar(std::string openEyeSrc, std::string blinkingEyeSrc) {
	GLTexture* texture;
	if (!openEyeSrc.empty() && GameObjectProvider::getResourceManager()->obtain<GLTexture>(DEFAULT_TEXTURE_PATH + openEyeSrc, texture)) {
		avatarImage->setTexture(texture);
		avatarImage->setCropping(texture->getDimensions());
		avatarTexture = texture;
		avatarImage->show();
	} else {
		avatarImage->hide();
		avatarTexture = nullptr;
	}

	if (blinkingEyeSrc.empty()) {
		blinkingAvatarTexture = nullptr;
	} else if (GameObjectProvider::getResourceManager()->obtain<GLTexture>(DEFAULT_TEXTURE_PATH + blinkingEyeSrc, texture)) {
		blinkingAvatarTexture = texture;
		lastAvatarUpdateTime = GameObjectProvider::getTime()->getCurrentTimeInMillis();
		blinking = false;
	} else {
		blinkingAvatarTexture = nullptr;
	}
}

void Game::AvatarDisplay::stopShowingAvatar() {
	showAvatar("");
}

void Game::AvatarDisplay::setAvatarImage(std::shared_ptr<ImageComponent> avatarImage) {
	this->avatarImage = avatarImage;
}
