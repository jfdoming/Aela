/*
* Class: AudioManager
* Author: Julian Dominguez-Schatz
* Date: 28/06/2017
* Description: Represents a class that manages all in-game audio.
*/

#pragma once
#include <al.h>
#include <alc.h>
#include <string>
#include "Listener.h"
#include "AudioClip.h"

namespace Aela {
	class AudioManager {
		public:
			AudioManager();
			~AudioManager();

			bool init();
			void playClip(AudioClip* clip);

			Listener getListener();

		private:
			ALCcontext* context;
			ALCdevice* audioDevice;

			void clearErrors();
			bool checkError(std::string* msg);
			void parseErrorMessage(ALuint error, std::string* msg);
	};
}
