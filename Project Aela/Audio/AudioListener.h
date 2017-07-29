/*
* Class: Listener
* Author: Julian Dominguez-Schatz
* Date: 29/06/2017
* Description: Represents a class that manages all in-game audio.
*/

#pragma once
#include <al.h>
#include <alc.h>
#include <glm/glm.hpp>

class AudioListener {
	public:
		AudioListener();
		~AudioListener();

		bool setPosition(ALfloat x, ALfloat y, ALfloat z);
		bool setVelocity(ALfloat x, ALfloat y, ALfloat z);
		bool setOrientation(ALfloat x, ALfloat y, ALfloat z);

	private:
		ALfloat orientation[6];
};
