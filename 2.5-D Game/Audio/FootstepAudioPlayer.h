/*
* Class: Footstep Audio Player
* Author: Robert Ciborowski
* Date: 04/02/2019
* Description: A class used to play audio when a footstep occurs.
*/

#pragma once
#include <string>
#include "GameAudioPlayer.h"

namespace Game {
	class FootstepAudioPlayer {
		public:
			FootstepAudioPlayer();

			void setup();

			void addSoundToTile(size_t tileID, std::string gameAudioPlayerTag);
			void playSound(size_t tileID);
			void setDefaultSound(std::string gameAudioPlayerTag);

		private:
			GameAudioPlayer* gameAudioPlayer;
			std::unordered_map<size_t, std::vector<std::string>> associations;
			std::unordered_map<size_t, bool> playing;
			std::unordered_map<size_t, size_t> previousSound;
			std::string defaultSound = "default footstep";
			const long long TIME_BETWEEN_SOUNDS = 200;
	};
}
