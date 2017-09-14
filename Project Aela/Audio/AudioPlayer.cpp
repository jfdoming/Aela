#include "AudioPlayer.h"
#include "../Error Handler/ErrorHandler.h"

using namespace Aela;

void AudioPlayer::clearErrors() {
	alGetError();
}

bool AudioPlayer::checkError(std::string* msg) {
	ALenum error = alGetError();
	bool cond = error != AL_NO_ERROR;
	if (cond) {
		parseErrorMessage(error, msg);
	}
	return cond;
}

void AudioPlayer::parseErrorMessage(ALuint error, std::string* msg) {
	switch (error) {
		case AL_NO_ERROR:
			*msg = "No error occurred.";
			break;
		case AL_INVALID_NAME:
			*msg = "openAL received an invalid name as a parameter.";
			break;
		case AL_INVALID_ENUM:
			*msg = "openAL received an invalid enum as a parameter.";
			break;
		case AL_INVALID_VALUE:
			*msg = "openAL received an invalid value as a parameter.";
			break;
		case AL_INVALID_OPERATION:
			*msg = "The program attempted an invalid operation.";
			break;
		case AL_OUT_OF_MEMORY:
			*msg = "The program ran out of memory.";
			break;
		default:
			*msg = "An unknown error occurred.";
			break;
	}
}

#define alDoWithErrorCheck_void(expr) {								\
	expr;															\
	std::string msg;												\
	if (checkError(&msg)) {											\
		AelaErrorHandling::consoleWindowError("AudioPlayer", msg);	\
		return;														\
	}																\
}

#define alDoWithErrorCheck_noret(expr) {							\
	expr;															\
	std::string msg;												\
	if (checkError(&msg)) {											\
		AelaErrorHandling::consoleWindowError("AudioPlayer", msg);	\
	}																\
}

AudioPlayer::AudioPlayer() {
}

AudioPlayer::~AudioPlayer() {
	if (context != nullptr) {
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
	}

	if (audioDevice != nullptr) {
		alcCloseDevice(audioDevice);
	}
}

bool AudioPlayer::init() {
	audioDevice = alcOpenDevice(nullptr);
	if (!audioDevice) {
		return false;
	}

	context = alcCreateContext(audioDevice, nullptr);
	alcMakeContextCurrent(context);

	if (!context) {
		return false;
	}

	return true;
}

void AudioPlayer::playClip(AudioClip* clip) {
	if (clip == nullptr) {
		AelaErrorHandling::consoleWindowError("AudioPlayer", "AudioClip was null.");
		return;
	}

	// TEMPORARY audio playing code
	// TODO refactor to use a source pool, and a buffer pool
	// TODO determine cause of corruption noise

	clearErrors();

	ALuint buffer, source;
	ALint source_state;
	alDoWithErrorCheck_void(alGenBuffers(1, &buffer));
	alDoWithErrorCheck_void(alBufferData(buffer, clip->getFormat(), clip->getData(), clip->getSize(), clip->getSampleRate()));

	alDoWithErrorCheck_void(alGenSources(1, &source));

	alDoWithErrorCheck_void(alSourcei(source, AL_BUFFER, buffer));
	alDoWithErrorCheck_void(alSourcei(source, AL_LOOPING, AL_FALSE));

	alDoWithErrorCheck_void(alSourcef(source, AL_PITCH, 1.0F));
	alDoWithErrorCheck_void(alSourcef(source, AL_GAIN, 1.0F));

	alDoWithErrorCheck_void(alSource3f(source, AL_POSITION, 0, 0, 0));
	alDoWithErrorCheck_void(alSource3f(source, AL_VELOCITY, 0, 0, 0));
	
	alDoWithErrorCheck_noret(alSourcePlay(source));
	alDoWithErrorCheck_noret(alGetSourcei(source, AL_SOURCE_STATE, &source_state));

	while (source_state == AL_PLAYING) {
		alDoWithErrorCheck_noret(alGetSourcei(source, AL_SOURCE_STATE, &source_state));
	}
	
	alDoWithErrorCheck_noret(alDeleteSources(1, &source));
	alDoWithErrorCheck_noret(alDeleteBuffers(1, &buffer));
}