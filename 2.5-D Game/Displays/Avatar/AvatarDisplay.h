/*
* Class: Avatar Display
* Author: Robert Ciborowski
* Date: 21/01/2019
* Description: A class used to display a blinking avatar.
*/

#pragma once
#include "../../../Project Aela/Menus/ImageComponent.h"
#include "../../../Project Aela/Menus/Label.h"

namespace Game {
	class AvatarDisplay {
		public:
			AvatarDisplay();
			virtual ~AvatarDisplay();

			void update();

			void showAvatar(std::string openEyeSrc);
			void showAvatar(std::string openEyeSrc, std::string blinkingEyeSrc);
			void stopShowingAvatar();

			void setAvatarImage(std::shared_ptr<ImageComponent> avatarImage);

		private:
			std::shared_ptr<ImageComponent> avatarImage;

			// These are timings related to the avatar blinking. Note that this uses milliseconds.
			const long long BLINK_TIME = 200, TIME_BETWEEN_BLINKS = 2250;
			long long lastAvatarUpdateTime = 0;

			// This stores whether the avatar is currently using the blinking texture or not.
			bool blinking = false;

			// These store avatar textures.
			GLTexture* avatarTexture = nullptr, *blinkingAvatarTexture = nullptr;
	};
}
