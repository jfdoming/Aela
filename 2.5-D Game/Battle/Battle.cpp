#include "Battle.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../../Project Aela/Utilities/enumut.h"

Game::Battle::Battle() {
}

Game::Battle::~Battle() {
	for (auto pair : notes) {
		delete pair.second;
	}
}

void Game::Battle::addNote(unsigned long long timeOfArrival, BattleNote& note) {
	int random = -1;

	if (note.getDirection() == NoteDirection::RANDOM) {
		srand((unsigned int) (GameObjectProvider::getTime()->getCurrentTimeInNanos() + notes.size()));
		random = rand() % 4;
	} else if (note.getDirection() == NoteDirection::RANDOM_LEFT_RIGHT) {
		srand((unsigned int) (GameObjectProvider::getTime()->getCurrentTimeInNanos() + notes.size()));
		random = rand() % 2;

		if (random == 1) {
			random = 2;
		}
	} else if (note.getDirection() == NoteDirection::RANDOM_UP_DOWN) {
		srand((unsigned int) (GameObjectProvider::getTime()->getCurrentTimeInNanos() + notes.size()));
		random = rand() % 2 + 1;

		if (random == 2) {
			random = 3;
		}
	}

	switch (random) {
		case 0:
			note.setDirection(NoteDirection::RIGHT);
			break;
		case 1:
			note.setDirection(NoteDirection::UP);
			break;
		case 2:
			note.setDirection(NoteDirection::LEFT);
			break;
		case 3:
			note.setDirection(NoteDirection::DOWN);
			break;
	}

	BattleNote* note2 = new BattleNote(note);
	notes.insert(std::pair<unsigned long long, BattleNote*>(timeOfArrival, note2));
}

void Game::Battle::addNotes(std::multimap<unsigned long long, BattleNote> notes) {
	for (auto& pair : notes) {
		addNote(pair.first, pair.second);
	}
}

void Game::Battle::addKeyMovement(unsigned long long timing, KeyMovement& movement) {
	keyMovements.insert(std::pair<unsigned long long, KeyMovement>(timing, movement));
}

void Game::Battle::setAvatar(std::string avatarSrc) {
	this->avatarSrc = avatarSrc;
}

void Game::Battle::setTextOnEnd(std::string textOnEnd) {
	this->textOnEnd = textOnEnd;
}

std::string Game::Battle::getTextOnEnd() {
	return textOnEnd;
}

void Game::Battle::setTextOnDeath(std::string textOnDeath) {
	this->textOnDeath = textOnDeath;
}

std::string Game::Battle::getTextOnDeath() {
	return textOnDeath;
}

void Game::Battle::setRewardOnEnd(int rewardOnEnd) {
	this->rewardOnEnd = rewardOnEnd;
}

int Game::Battle::getRewardOnEnd() {
	return rewardOnEnd;
}

void Game::Battle::setSrc(std::string src) {
	this->src = src;
}

std::string Game::Battle::getSrc() {
	return src;
}

void Game::Battle::setFadeOnReturn(bool fadeOnReturn) {
	this->fadeOnReturn = fadeOnReturn;
}

bool Game::Battle::getFadeOnReturn() {
	return fadeOnReturn;
}

std::string Game::Battle::getAvatarSrc() {
	return avatarSrc;
}

void Game::Battle::setBlinkingAvatar(std::string blinkingAvatarSrc) {
	this->blinkingAvatarSrc = blinkingAvatarSrc;
}

std::string Game::Battle::getBlinkingAvatarSrc() {
	return blinkingAvatarSrc;
}

void Game::Battle::setTrack(std::string avatarSrc) {
	this->trackSrc = avatarSrc;
}

std::string Game::Battle::getTrackSrc() {
	return trackSrc;
}

std::multimap<unsigned long long, Game::BattleNote*>* Game::Battle::getNotes() {
	return &notes;
}

std::multimap<unsigned long long, Game::KeyMovement>* Game::Battle::getKeyMovements() {
	return &keyMovements;
}

void Game::Battle::addDialogue(unsigned long long timeInNanos, BattleDialogue dialogue) {
	this->dialogue[timeInNanos] = dialogue;
}

void Game::Battle::addAvatarChange(unsigned long long timeInNanos, std::string avatarSrc, std::string blinkingAvatarSrc) {
	avatarChanges[timeInNanos] = std::make_pair(avatarSrc, blinkingAvatarSrc);
}

void Game::Battle::addScriptRun(unsigned long long timeInNanos, std::string script) {
	scriptRuns[timeInNanos] = script;
}

void Game::Battle::addBackgroundChange(unsigned long long timeInNanos, unsigned long long durationInMillis, std::string backgroundSrc) {
	BackgroundChange change;
	change.src = backgroundSrc;
	change.duration = durationInMillis;
	backgroundChanges[timeInNanos] = change;
}

void Game::Battle::setDefaultBackground(std::string defaultBackground) {
	this->defaultBackground = defaultBackground;
}

std::string Game::Battle::getDefaultBackground() {
	return defaultBackground;
}

std::map<unsigned long long, Game::BackgroundChange>* Game::Battle::getBackgroundChanges() {
	return &backgroundChanges;
}

void Game::Battle::setDialogueHideTimes(std::vector<unsigned long long>* dialogueHideTimes) {
	this->dialogueHideTimes = *dialogueHideTimes;
	std::sort(this->dialogueHideTimes.begin(), this->dialogueHideTimes.end());
}

std::vector<unsigned long long>* Game::Battle::getDialogueHideTimes() {
	return &dialogueHideTimes;
}

void Game::Battle::setDuration(unsigned long long duration) {
	this->duration = duration;
}

unsigned long long Game::Battle::getDuration() {
	return duration;
}

void Game::Battle::setScriptOnEnd(std::string scriptOnEnd) {
	this->scriptOnEnd = scriptOnEnd;
}

std::string Game::Battle::getScriptOnEnd() {
	return scriptOnEnd;
}

void Game::Battle::setScriptOnReturnToGameplay(std::string scriptOnReturnToGameplay) {
	this->scriptOnReturnToGameplay = scriptOnReturnToGameplay;
}

std::string Game::Battle::getScriptOnReturnToGameplay() {
	return scriptOnReturnToGameplay;
}

std::map<unsigned long long, Game::BattleDialogue>* Game::Battle::getDialogue() {
	return &dialogue;
}

std::map<unsigned long long, std::pair<std::string, std::string>>* Game::Battle::getAvatarChanges() {
	return &avatarChanges;
}

std::map<unsigned long long, std::string>* Game::Battle::getScriptRuns() {
	return &scriptRuns;
}
