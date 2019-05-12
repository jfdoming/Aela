/*
* Name: Sound Script
* Author: Robert Ciborowski
* Date: 12/09/2018
* Description: A file which contains sound-related scripts.
*              The contents of this file will be moved to LUA once we can get LUA to work!
*/

#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace Scripts {
	void setupSound();
	void setupMusicForLevel(int stage, int level);
	void startPlayingMainMenuMusic();
	void stopPlayingMainMenuMusic();
}
