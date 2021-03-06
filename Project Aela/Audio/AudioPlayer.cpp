#include "AudioPlayer.h"
#include "../Error Handler/ErrorHandling.h"
#include <iostream>

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
	die();
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

	backgroundThread = std::thread(&AudioPlayer::update, this);

	return true;
}

void AudioPlayer::update() {
	ALint source_state;

	// This is temporary stuff until the pools are added. I just want to listen
	// to audio while doing other things.
	while (true) {
		{
			std::lock_guard<std::mutex> guard(mutex);
			if (closeThread) {
				break;
			}
		}
		for (size_t i = 0; i < playingBuffers.size(); i++) {
			{

				std::lock_guard<std::mutex> guard(mutex);
				AudioBuffer& audioBuffer = playingBuffers[i];

				alDoWithErrorCheck_noret(alGetSourcei(audioBuffer.getSource(), AL_SOURCE_STATE, &source_state));

				if (source_state != AL_PLAYING) {
					ALuint source = audioBuffer.getSource();
					ALuint buffer = audioBuffer.getBuffer();
					alDoWithErrorCheck_noret(alDeleteSources(1, &source));
					alDoWithErrorCheck_noret(alDeleteBuffers(1, &buffer));

					playingBuffers.erase(playingBuffers.begin() + i);
					i--;
				}
			}
		}
	}




	// The old stuff:
	for (auto clip : playingClips) {
		while (source_state == AL_PLAYING) {
			// TODO make source object
//			alDoWithErrorCheck_noret(alGetSourcei(source, AL_SOURCE_STATE, &source_state));



		}
	}
}

void AudioPlayer::die() {
	if (context != nullptr) {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
	}

	if (audioDevice != nullptr) {
		alcCloseDevice(audioDevice);
	}

	// I think you might want to do this lol
	// - Robert
	{
		std::lock_guard<std::mutex> guard(mutex);
		closeThread = true;
	}
	backgroundThread.join();
}

void AudioPlayer::playClip(AudioClip* clip) {
	if (clip == nullptr) {
		AelaErrorHandling::consoleWindowError("AudioPlayer", "AudioClip was null.");
		return;
	}

	// std::cout << "Now playing clip: " << clip << "\n";

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

	alSourcef(source, AL_GAIN, clip->getDefaultVolume());

	AudioBuffer audioBuffer(buffer, source, clip);
	{
		std::lock_guard<std::mutex> guard(mutex);
		playingBuffers.push_back(audioBuffer);
	}

//	while (source_state == AL_PLAYING) {
//		alDoWithErrorCheck_noret(alGetSourcei(source, AL_SOURCE_STATE, &source_state));
//	}
//
//	alDoWithErrorCheck_noret(alDeleteSources(1, &source));
//	alDoWithErrorCheck_noret(alDeleteBuffers(1, &buffer));
}

void AudioPlayer::stopClip(AudioClip* clip) {
	{
		std::lock_guard<std::mutex> guard(mutex);

		// I don't think that playingClips was ever finished, so more stuff
		// will have to be done inside of the if statement of this loop.
		for (size_t i = 0; i < playingClips.size(); i++) {
			if (playingClips[i] == clip) {
			}
		}

		for (size_t i = 0; i < playingBuffers.size(); i++) {
			if (playingBuffers[i].getClip() == clip) {
				alSourceStop(playingBuffers[i].getSource());
			}
		}
	}
}

bool AudioPlayer::isPlaying(AudioClip* clip) {
	// return std::find(playingClips.begin(), playingClips.end(), clip) != playingClips.end();

	for (size_t i = 0; i < playingBuffers.size(); i++) {
		if (playingBuffers[i].getClip() == clip) {
			return true;
		}
	}
	return false;
}

bool AudioPlayer::changeVolumeOfPlayingClip(AudioClip* clip, float volume) {
	// This kind of linear search sucks. Maybe we should instead use some sort of a map that uses AudioClip*.
	for (size_t i = 0; i < playingBuffers.size(); i++) {
		if (playingBuffers[i].getClip() == clip) {
			alSourcef(playingBuffers[i].getSource(), AL_GAIN, volume);
			return true;
		}
	}

	return false;
}
