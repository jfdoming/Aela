#include "FootstepAudioPlayer.h"
#include "../Scripts/Scripts to Move to LUA/ScriptObjects.h"

Game::FootstepAudioPlayer::FootstepAudioPlayer() {
}

void Game::FootstepAudioPlayer::setup() {
	gameAudioPlayer = GameObjectProvider::getGameAudioPlayer();
}

void Game::FootstepAudioPlayer::addSoundToTile(size_t tileID, std::string gameAudioPlayerTag) {
	auto associationPair = associations.find(tileID);
	if (associationPair == associations.end()) {
		associations[tileID] = std::vector<std::string>();
		previousSound[tileID] = 0;
	}

	associations[tileID].push_back(gameAudioPlayerTag);
	auto pair = playing.find(tileID);

	if (pair == playing.end()) {
		playing[tileID] = false;
	}
}

void Game::FootstepAudioPlayer::playSound(size_t tileID) {
	auto playingPair = playing.find(tileID);
	if (playingPair == playing.end() || playingPair->second) {
		return;
	}

	playingPair->second = true;
	auto associationPair = associations.find(tileID);

	if (associationPair == associations.end()) {
		gameAudioPlayer->playAudio(defaultSound);
	} else {
		size_t i = previousSound[tileID] + 1;

		if (i == associationPair->second.size()) {
			i = 0;
		}

		previousSound[tileID] = i;
		gameAudioPlayer->playAudio(associationPair->second[i]);
	}

	auto event = [this, tileID]() {
		auto playingPair = playing.find(tileID);
		if (playingPair == playing.end()) {
			return;
		}
		playingPair->second = false;
	};

	GameObjectProvider::getTimer()->scheduleEventInMillis(TIME_BETWEEN_SOUNDS, event);
}

void Game::FootstepAudioPlayer::setDefaultSound(std::string gameAudioPlayerTag) {
	this->defaultSound = gameAudioPlayerTag;
}
