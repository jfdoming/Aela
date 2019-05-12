/*
* Name: Animation Scripts
* Author: Robert Ciborowski
* Date: 16/07/2018
* Description: A file which contains animation-related scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once
#include "../../Game Object Provider/GameObjectProvider.h"

namespace Scripts {
	using namespace Game;

	void loadMaterialAnimations();
	bool loadLogoMaterialAnimation(int whichLevel);
	bool animateTelevision();
	bool animateElevator();
	void stopAnimatingElevator();

	void loadAelaWatchAvatarAnimations();

	void setupMainAnimations();

	bool loopSpriteSheetAnimation(std::string spriteSheet, std::string materialToAnimate, int spriteSheetWidth,
			int startX, int startY, int endX, int endY, std::vector<int>* framesToPauseOn, std::vector<long long>* framePauseTimes,
		long long timeBetweenFrames);
	bool loopSpriteSheetAnimation(std::string spriteSheet, std::string materialToAnimate, int spriteSheetWidth,
			int startX, int startY, int endX, int endY, long long timeBetweenFrames);
}