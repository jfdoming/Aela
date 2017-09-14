/*
* Class: AudioPlayer
* Author: Julian Dominguez-Schatz
* Date: 28/06/2017
* Description: Represents a class that manages all in-game audio.
*/

#pragma once
#include <al.h>
#include <alc.h>
#include <string>
#include "AudioListener.h"
#include "AudioClip.h"

namespace Aela {
	class AudioPlayer {
		public:
			AudioPlayer();
			~AudioPlayer();

			bool init();
			void playClip(AudioClip* clip);

			AudioListener getListener();

		private:
			ALCcontext* context = nullptr;
			ALCdevice* audioDevice = nullptr;

			void clearErrors();
			bool checkError(std::string* msg);
			void parseErrorMessage(ALuint error, std::string* msg);
	};
}
