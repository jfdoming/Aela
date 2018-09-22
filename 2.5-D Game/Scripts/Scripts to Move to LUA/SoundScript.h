/*
* Name: Sound Script
* Author: Robert Ciborowski
* Date: 12/09/2018
* Description: A file which contains sound-related scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once
#include <string>
#include "../../../Project Aela/Audio/AudioClip.h"

namespace Scripts {
	void playElevatorSound();
	void stopElevatorSound();
	void playElevatorDing();
	void playDoorSound();
	void playGunSound();

	void playTrack(std::string trackName);
	Aela::AudioClip* getAudioClip(std::string path);
}
