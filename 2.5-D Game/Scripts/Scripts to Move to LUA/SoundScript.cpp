#include "SoundScript.h"
#include "ScriptObjects.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"

void Scripts::playElevatorSound() {
	engine->getAudioPlayer()->playClip(getAudioClip(""));
}

void Scripts::stopElevatorSound() {
	
}

void Scripts::playElevatorDing() {
	engine->getAudioPlayer()->playClip(getAudioClip(""));
}

void Scripts::playDoorSound() {
	engine->getAudioPlayer()->playClip(getAudioClip(""));
}

void Scripts::playGunSound() {
	engine->getAudioPlayer()->playClip(getAudioClip(""));
}

void Scripts::playTrack(std::string trackName) {
	engine->getAudioPlayer()->playClip(getAudioClip(DEFAULT_AUDIO_STREAM_PATH + trackName + ".wav"));
}

Aela::AudioClip* Scripts::getAudioClip(std::string path) {
	AudioClip* clip;
	if (!engine->getResourceManager()->obtain<AudioClip>(path, clip)) {
		return nullptr;
	}
	return clip;
}
