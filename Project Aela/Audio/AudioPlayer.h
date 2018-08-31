/*
* Class: AudioPlayer
* Author: Julian Dominguez-Schatz
* Date: 28/06/2017
* Description: Represents a class that facilitates playing all in-game audio.
*/

#pragma once
#include <al.h>
#include <alc.h>
#include <string>
#include <thread>
#include <vector>
#include "AudioListener.h"
#include "AudioClip.h"
//#include "ctpl_stl.h"

namespace Aela {
	class AudioPlayer {
		public:
			AudioPlayer();
			~AudioPlayer();

			bool init();
			void die();
			void playClip(AudioClip* clip);

			void update();

			AudioListener getListener();

		private:
			ALCcontext* context = nullptr;
			ALCdevice* audioDevice = nullptr;

			void clearErrors();
			bool checkError(std::string* msg);
			void parseErrorMessage(ALuint error, std::string* msg);

			std::thread backgroundThread;
			std::vector<AudioClip*> playingClips;
	};
}
