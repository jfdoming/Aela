#include "AudioLooper.h"
#include "../../Project Aela/Resource Management/ResourceManager.h"

Aela::AudioLooper::AudioLooper() {}

void Aela::AudioLooper::update() {
	if (loopingClips.empty()) {
		return;
	}

	std::vector<AudioClip*> clipsToErase;

	for (auto& pair : loopingClips) {
		auto& data = pair.second;
		if (!data.started && data.timeWhenToStart <= clock->getCurrentTimeInMillis()) {
			audioPlayer->playClip(pair.first);
			data.timeOfStart = clock->getCurrentTimeInMillis();
			data.started = true;
		}

		if (data.started && data.locationOfLoopEnd <= clock->getCurrentTimeInMillis() - data.timeOfStart) {
			audioPlayer->stopClip(pair.first);

			if (data.numberOfLoops != 0) {
				data.whichLoop++;

				if (data.whichLoop == data.numberOfLoops) {
					clipsToErase.push_back(pair.first);
					continue;
				}
			}

			audioPlayer->playClip(pair.first);
			data.timeOfStart = clock->getCurrentTimeInMillis();
		}
	}

	for (AudioClip* clip : clipsToErase) {
		loopingClips.erase(clip);
	}
}

void Aela::AudioLooper::loopAudio(std::string src, unsigned numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	AudioClip* clip;
	if (resourceManager->obtain<AudioClip>(src, clip)) {
		loopAudio(clip, numberOfLoops, locationOfLoopEnd, waitTimeBeforeStart);
	}
}

void Aela::AudioLooper::loopAudio(Aela::AudioClip* clip, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	AudioLoopData data;
	data.whichLoop = 0;
	data.numberOfLoops = numberOfLoops;
	data.started = false;
	data.timeWhenToStart = waitTimeBeforeStart + clock->getCurrentTimeInMillis();
	data.locationOfLoopEnd = locationOfLoopEnd;
	loopingClips[clip] = data;
}

void Aela::AudioLooper::loopAudioInfinitely(std::string src, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	AudioClip* clip;
	if (resourceManager->obtain<AudioClip>(src, clip)) {
		loopAudioInfinitely(clip, locationOfLoopEnd, waitTimeBeforeStart);
	}
}

void Aela::AudioLooper::loopAudioInfinitely(Aela::AudioClip* clip, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart) {
	// Note: a number of loops of zero means that the audio loops infinitely.
	AudioLoopData data;
	data.whichLoop = 0;
	data.numberOfLoops = 0;
	data.started = false;
	data.timeWhenToStart = waitTimeBeforeStart + clock->getCurrentTimeInMillis();
	data.locationOfLoopEnd = locationOfLoopEnd;
	loopingClips[clip] = data;
	std::cout << "NOW LOOPING: " << clip << "\n";
}

bool Aela::AudioLooper::stopLoop(std::string src) {
	AudioClip* clip;
	if (resourceManager->obtain<AudioClip>(src, clip)) {
		stopLoop(clip);
		return true;
	}

	return false;
}

bool Aela::AudioLooper::stopLoop(Aela::AudioClip* clip) {
	auto pair = loopingClips.find(clip);

	if (pair != loopingClips.end()) {
		audioPlayer->stopClip(clip);
		loopingClips.erase(pair);
		return true;
	}

	return false;
}

void Aela::AudioLooper::stopAllLoops() {
	for (auto pair : loopingClips) {
		audioPlayer->stopClip(pair.first);
	}

	loopingClips.clear();
}

bool AudioLooper::isLoopingAnyAudio() {
	return !loopingClips.empty();
}

size_t AudioLooper::getNumberOfCurrentLoops() {
	return loopingClips.size();
}

AudioClip* AudioLooper::getFirstLoop() {
	return loopingClips.begin()->first;
}

AudioLoopData* AudioLooper::getFirstLoopData() {
	return &loopingClips.begin()->second;
}

void AudioLooper::setAudioPlayer(AudioPlayer* audioPlayer) {
	this->audioPlayer = audioPlayer;
}

void AudioLooper::setResourceManager(ResourceManager* resourceManager) {
	this->resourceManager = resourceManager;
}

void AudioLooper::setClock(Clock* clock) {
	this->clock = clock;
}
