#include "AudioListener.h"

using namespace Aela;

AudioListener::AudioListener() {
}

AudioListener::~AudioListener() {
}

bool AudioListener::setPosition(ALfloat x, ALfloat y, ALfloat z) {
	orientation[0] = x;
	orientation[1] = y;
	orientation[2] = z;

	alListener3f(AL_POSITION, x, y, z);
	if (alGetError() != AL_NO_ERROR) {
		return false;
	}
	return true;
}

bool AudioListener::setVelocity(ALfloat x, ALfloat y, ALfloat z) {
	alListener3f(AL_VELOCITY, x, y, z);
	if (alGetError() != AL_NO_ERROR) {
		return false;
	}
	return true;
}

bool AudioListener::setOrientation(ALfloat x, ALfloat y, ALfloat z) {
	orientation[3] = x;
	orientation[4] = y;
	orientation[5] = z;

	alListenerfv(AL_ORIENTATION, orientation);
	if (alGetError() != AL_NO_ERROR) {
		return false;
	}
	return true;
}