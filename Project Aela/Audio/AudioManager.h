/*
* Class: AudioManager
* Author: Julian Dominguez-Schatz
* Date: 28/06/2017
* Description: Represents a class that manages all in-game audio.
*/

#pragma once
#include <al.h>
#include <alc.h>
#include "Listener.h"

class AudioManager {
	public:
		AudioManager();
		~AudioManager();

		bool init();
		Listener getListener();

	private:
		ALCcontext* context;
		ALCdevice* audioDevice;

		void clearErrors();
};
