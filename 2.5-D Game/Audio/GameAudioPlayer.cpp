#include "GameAudioPlayer.h"
#include "../Game Object Provider/GameObjectProvider.h"

Game::GameAudioPlayer::GameAudioPlayer() {
	resourceManager = GameObjectProvider::getResourceManager();
	audioPlayer = GameObjectProvider::getAudioPlayer();
	audioLooper = GameObjectProvider::getAudioLooper();
}

bool Game::GameAudioPlayer::addAudioAsClip(std::string tag, std::string src) {
	Aela::AudioClip* clip;
	if (resourceManager->obtain<AudioClip>("res/audio/clips/" + src, clip)) {
		clips[tag] = clip;
		return true;
	}
	std::cout << "Did not find effect " << "res/audio/clips/" + src << "\n";
	return false;
}

bool Game::GameAudioPlayer::addAudioAsStream(std::string tag, std::string src) {
	// Once streams are added, we'll use streams. For now, we're just loading streams as clips.
	Aela::AudioClip* clip;
	if (resourceManager->obtain<AudioClip>("res/audio/streams/" + src, clip)) {
		streams[tag] = clip;
		return true;
	}
	return false;
}

bool Game::GameAudioPlayer::playAudio(std::string tag) {
	bool success = false;
	auto pair = clips.find(tag);
	if (pair != clips.end()) {
		audioPlayer->playClip(pair->second);
		success = true;
	}

	auto pair2 = streams.find(tag);
	if (pair2 != streams.end()) {
		audioPlayer->playClip(pair2->second);
		success = true;
	}

	return success;
}

bool Game::GameAudioPlayer::playAudioIfNotPlaying(std::string tag) {
	if (!isPlayingClip(tag) && !isPlayingStream(tag)) {
		return playAudio(tag);
	}
	return false;
}

bool Game::GameAudioPlayer::setClipVolume(std::string tag, float volume) {
	auto pair = clips.find(tag);

	if (pair == clips.end()) {
		return false;
	}

	pair->second->setDefaultVolume(volume);
	return audioPlayer->changeVolumeOfPlayingClip(pair->second, volume);
}

void Game::GameAudioPlayer::loopClip(std::string tag, unsigned numberOfLoops, unsigned long long locationOfLoopEnd,
	unsigned long long waitTimeBeforeStart) {
	auto pair = clips.find(tag);

	if (pair == clips.end()) {
		AelaErrorHandling::consoleWindowWarning("Game Audio Looper", "Could not find clip " + tag + ".");
		return;
	}

	audioLooper->loopAudio(pair->second, numberOfLoops, locationOfLoopEnd, waitTimeBeforeStart);
}

void Game::GameAudioPlayer::loopStream(std::string tag, unsigned numberOfLoops, unsigned long long locationOfLoopEnd,
	unsigned long long waitTimeBeforeStart) {
	auto pair = streams.find(tag);

	if (pair == streams.end()) {
		AelaErrorHandling::consoleWindowWarning("Game Audio Looper", "Could not find stream " + tag + ".");
		return;
	}

	audioLooper->loopAudio(pair->second, numberOfLoops, locationOfLoopEnd, waitTimeBeforeStart);
}

void Game::GameAudioPlayer::loopClipInfinitely(std::string tag, unsigned long long locationOfLoopEnd,
	unsigned long long waitTimeBeforeStart) {
	auto pair = clips.find(tag);

	if (pair == clips.end()) {
		AelaErrorHandling::consoleWindowWarning("Game Audio Looper", "Could not find clip " + tag + ".");
		return;
	}

	audioLooper->loopAudioInfinitely(pair->second, locationOfLoopEnd, waitTimeBeforeStart);
}

void Game::GameAudioPlayer::loopStreamInfinitely(std::string tag, unsigned long long locationOfLoopEnd,
	unsigned long long waitTimeBeforeStart) {
	auto pair = streams.find(tag);

	if (pair == streams.end()) {
		AelaErrorHandling::consoleWindowWarning("Game Audio Looper", "Could not find stream " + tag + ".");
		return;
	}

	audioLooper->loopAudioInfinitely(pair->second, locationOfLoopEnd, waitTimeBeforeStart);
}

void Game::GameAudioPlayer::stopClip(std::string tag) {
	auto pair = clips.find(tag);

	if (pair == clips.end()) {
		AelaErrorHandling::consoleWindowWarning("Game Audio Looper", "Could not find clip " + tag + ".");
		return;
	}

	if (!audioLooper->stopLoop(pair->second)) {
		// The audio was not being looped but could have been played normally.
		audioPlayer->stopClip(pair->second);
	}
}

void Game::GameAudioPlayer::stopStream(std::string tag) {
	auto pair = streams.find(tag);

	if (pair == streams.end()) {
		AelaErrorHandling::consoleWindowWarning("Game Audio Looper", "Could not find stream " + tag + ".");
		return;
	}

	if (!audioLooper->stopLoop(pair->second)) {
		// The audio was not being looped but could have been played normally.
		audioPlayer->stopClip(pair->second);
	}
}

void Game::GameAudioPlayer::stopAllLoops() {
	audioLooper->stopAllLoops();
}

void Game::GameAudioPlayer::stopEverything() {
	audioLooper->stopAllLoops();

	for (auto& pair : clips) {
		audioPlayer->stopClip(pair.second);
	}

	for (auto& pair : streams) {
		audioPlayer->stopClip(pair.second);
	}
}

bool Game::GameAudioPlayer::setStreamVolume(std::string tag, float volume) {
	auto pair = streams.find(tag);

	if (pair == streams.end()) {
		AelaErrorHandling::consoleWindowWarning("Game Audio Looper", "Could not find stream " + tag + ".");
		return false;
	}

	pair->second->setDefaultVolume(volume);
	bool success = audioPlayer->changeVolumeOfPlayingClip(pair->second, volume);
	return success;
}

void Game::GameAudioPlayer::setAllClipVolumes(float volume) {
	for (auto& pair : clips) {
		pair.second->setDefaultVolume(volume);
		audioPlayer->changeVolumeOfPlayingClip(pair.second, volume);
	}
}

void Game::GameAudioPlayer::setAllStreamVolumes(float volume) {
	for (auto& pair : streams) {
		pair.second->setDefaultVolume(volume);
		audioPlayer->changeVolumeOfPlayingClip(pair.second, volume);
	}
}

bool Game::GameAudioPlayer::isPlayingClip(std::string tag) {
	auto pair = clips.find(tag);

	if (pair == clips.end()) {
		return false;
	}

	return audioPlayer->isPlaying(pair->second);
}

bool Game::GameAudioPlayer::isPlayingStream(std::string tag) {
	auto pair = streams.find(tag);

	if (pair == streams.end()) {
		return false;
	}

	return audioPlayer->isPlaying(pair->second);
}

bool Game::GameAudioPlayer::unloadAudio(std::string tag) {
	bool success = false;
	auto pair = clips.find(tag);
	if (pair != clips.end()) {
		resourceManager->unload(pair->second->getSrc());
		success = true;
	}

	auto pair2 = streams.find(tag);
	if (pair2 != streams.end()) {
		resourceManager->unload(pair2->second->getSrc());
		success = true;
	}

	return success;
}

void Game::GameAudioPlayer::unloadAllClips() {
	for (auto pair : clips) {
		resourceManager->unload(pair.second->getSrc());
	}

	clips.clear();
}

void Game::GameAudioPlayer::unloadAllStreams() {
	for (auto pair : streams) {
		resourceManager->unload(pair.second->getSrc());
	}

	streams.clear();
}

Aela::AudioClip* Game::GameAudioPlayer::getClip(std::string tag) {
	auto pair = clips.find(tag);
	if (pair != clips.end()) {
		return pair->second;
	}
	return nullptr;
}
