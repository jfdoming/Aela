#include "AudioManager.h"

AudioManager::AudioManager() {
}

AudioManager::~AudioManager() {
}

void AudioManager::clearErrors() {
	alGetError();
}

bool AudioManager::init() {
	ALCdevice *device;

	device = alcOpenDevice(nullptr);
	if (!device) {
		return false;
	}

	context = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(context);

	if (!context) {
		return false;
	}
}