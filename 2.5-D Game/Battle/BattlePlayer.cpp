#include "BattlePlayer.h"
#include "../../Project Aela/Menus/ImageComponent.h"
#include "../Displays/Battle/BattleDisplay.h"
#include "../Scripts/Scripts to Move to LUA/SceneIDs.h"
#include "../Scripts/Scripts to Move to LUA/ScriptObjects.h"
#include "../../Project Aela/Resource Management/ResourcePaths.h"
#include "../Displays/Battle Dialogue/BattleDialogueDisplay.h"
#include "../Aela Game/AelaGame.h"
#include "../Scripts/ScriptManager.h"
#include "../../Project Aela/Utilities/enumut.h"

Game::BattlePlayer::BattlePlayer() {
	notesOnDisplay[NoteDirection::RIGHT] = std::multimap<unsigned long long, BattleNote*>();
	notesOnDisplay[NoteDirection::UP] = std::multimap<unsigned long long, BattleNote*>();
	notesOnDisplay[NoteDirection::LEFT] = std::multimap<unsigned long long, BattleNote*>();
	notesOnDisplay[NoteDirection::DOWN] = std::multimap<unsigned long long, BattleNote*>();
	timeSinceLastHoldReward[NoteDirection::RIGHT] = 0;
	timeSinceLastHoldReward[NoteDirection::UP] = 0;
	timeSinceLastHoldReward[NoteDirection::LEFT] = 0;
	timeSinceLastHoldReward[NoteDirection::DOWN] = 0;
	holdEnds[NoteDirection::RIGHT] = 0;
	holdEnds[NoteDirection::UP] = 0;
	holdEnds[NoteDirection::LEFT] = 0;
	holdEnds[NoteDirection::DOWN] = 0;
}

void Game::BattlePlayer::setup() {
	playerCharacter = GameObjectProvider::getPlayer()->getCharacter();
	clock = GameObjectProvider::getEngine()->getTime();
	battleDisplay = GameObjectProvider::getBattleDisplay();
	battleDialogueDisplay = GameObjectProvider::getBattleDialogueDisplay();
	scriptManager = GameObjectProvider::getScriptManager();
	
	
	GameObjectProvider::getEventHandler()->addListener(EventConstants::KEY_PRESSED, bindListener(BattlePlayer::onEvent, this));
	GameObjectProvider::getEventHandler()->addListener(EventConstants::KEY_RELEASED, bindListener(BattlePlayer::onEvent, this));
}

void Game::BattlePlayer::update() {
	if (!playing) {
		return;
	}

	battleDuration = clock->getCurrentTimeInNanos() - battleStartTime;

	if (currentBattle->getDuration() <= battleDuration) {
		// Do stuff.
		endBattle();
	}
	
	while (true) {
		auto notes = currentBattle->getNotes();

		if (notes->empty()) {
			break;
		}

		auto pair = notes->begin();

		if (pair->first * 1000000 <= battleDuration) {
			processNote(pair->first * 1000000, *pair->second);
			notes->erase(pair);

			if (notes->empty()) {
				break;
			}
		} else {
			break;
		}
	}

	/*while (true) {
		auto keyMovements = currentBattle->getKeyMovements();

		if (keyMovements->empty()) {
			break;
		}

		auto pair = keyMovements->begin();

		if (pair->first <= battleDuration) {
			battleDisplay->moveKey(pair->second);
			keyMovements->erase(pair);
		} else {
			break;
		}
	}*/

	for (auto& multimap : notesOnDisplay) {
		for (auto& pair : multimap.second) {
			if (pair.second->isPressed() && pair.first <= battleDuration) {
				multimap.second.erase(pair.first);
				battleDisplay->removeNote(pair.second);
			} else if (pair.first + WAIT_TIME_AFTER_NOTE_ARRIVAL <= battleDuration) {
				noteWasMissed(pair.second);
				multimap.second.erase(pair.first);
			} else {
				break;
			}
		}
	}

	if (!currentBattle->getDialogue()->empty()) {
		auto dialoguePair = currentBattle->getDialogue()->begin();
		if (dialoguePair->first <= battleDuration) {
			battleDialogueDisplay->showDialogue(dialoguePair->second.characterName, dialoguePair->second.text, "", dialoguePair->second.playSound);
			currentBattle->getDialogue()->erase(dialoguePair);
		}
	}

	if (!currentBattle->getDialogueHideTimes()->empty()) {
		auto value = currentBattle->getDialogueHideTimes()->at(0);
		if (value <= battleDuration) {
			battleDialogueDisplay->closeDialogue();
			currentBattle->getDialogueHideTimes()->erase(currentBattle->getDialogueHideTimes()->begin());
		}
	}

	if (!currentBattle->getAvatarChanges()->empty()) {
		auto pair = currentBattle->getAvatarChanges()->begin();
		if (pair->first <= battleDuration) {
			battleDialogueDisplay->showAvatar(pair->second.first, pair->second.second);
			currentBattle->getAvatarChanges()->erase(pair);
		}
	}

	if (!currentBattle->getScriptRuns()->empty()) {
		auto pair = currentBattle->getScriptRuns()->begin();
		if (pair->first <= battleDuration) {
			scriptManager->runScript(pair->second);
			currentBattle->getScriptRuns()->erase(pair);
		}
	}

	if (!currentBattle->getBackgroundChanges()->empty()) {
		auto pair = currentBattle->getBackgroundChanges()->begin();
		if (pair->first <= battleDuration) {
			battleDisplay->changeBackground(pair->second);
			currentBattle->getBackgroundChanges()->erase(pair);
		}
	}

	if (justPressedRight) {
		for (auto& pair : notesOnDisplay[NoteDirection::RIGHT]) {
			if (pair.second->isPressed()) {
				continue;
			}

			pressedNote(pair.first, pair.second);
			break;
		}
		justPressedRight = false;
	}

	if (justPressedUp) {
		for (auto& pair : notesOnDisplay[NoteDirection::UP]) {
			if (pair.second->isPressed()) {
				continue;
			}

			pressedNote(pair.first, pair.second);
			break;
		}
		justPressedUp = false;
	}

	if (justPressedLeft) {
		for (auto& pair : notesOnDisplay[NoteDirection::LEFT]) {
			if (pair.second->isPressed()) {
				continue;
			}

			pressedNote(pair.first, pair.second);
			break;
		}
		justPressedLeft = false;
	}

	if (justPressedDown) {
		for (auto& pair : notesOnDisplay[NoteDirection::DOWN]) {
			if (pair.second->isPressed()) {
				continue;
			}

			pressedNote(pair.first, pair.second);
			break;
		}
		justPressedDown = false;
	}

	if (pressingRight) {
		if (holdEnds[NoteDirection::RIGHT] >= battleDuration) {
			if (battleDuration >= timeSinceLastHoldReward[NoteDirection::RIGHT] + WAIT_INTERVAL_FOR_HOLDS) {
				timeSinceLastHoldReward[NoteDirection::RIGHT] = battleDuration;
				heldTail();
			}
			battleDisplay->tailWasPressed(holdNotes[NoteDirection::RIGHT]);
		}
	}

	if (pressingUp) {
		if (holdEnds[NoteDirection::UP] >= battleDuration) {
			if (battleDuration >= timeSinceLastHoldReward[NoteDirection::UP] + WAIT_INTERVAL_FOR_HOLDS) {
				timeSinceLastHoldReward[NoteDirection::UP] = battleDuration;
				heldTail();
			}
			battleDisplay->tailWasPressed(holdNotes[NoteDirection::UP]);
		}
	}

	if (pressingLeft) {
		if (holdEnds[NoteDirection::LEFT] >= battleDuration) {
			if (battleDuration >= timeSinceLastHoldReward[NoteDirection::LEFT] + WAIT_INTERVAL_FOR_HOLDS) {
				timeSinceLastHoldReward[NoteDirection::LEFT] = battleDuration;
				heldTail();
			}
			battleDisplay->tailWasPressed(holdNotes[NoteDirection::LEFT]);
		}
	}

	if (pressingDown) {
		if (holdEnds[NoteDirection::DOWN] >= battleDuration) {
			if (battleDuration >= timeSinceLastHoldReward[NoteDirection::DOWN] + WAIT_INTERVAL_FOR_HOLDS) {
				timeSinceLastHoldReward[NoteDirection::DOWN] = battleDuration;
				heldTail();
			}
			battleDisplay->tailWasPressed(holdNotes[NoteDirection::DOWN]);
		}
	}

	if (justReleasedRight) {
		if (holdEnds[NoteDirection::RIGHT] >= battleDuration) {
			battleDisplay->tailWasReleased(holdNotes[NoteDirection::RIGHT]);
		}
		justReleasedRight = false;
	}

	if (justReleasedUp) {
		if (holdEnds[NoteDirection::UP] >= battleDuration) {
			battleDisplay->tailWasReleased(holdNotes[NoteDirection::UP]);
		}
		justReleasedUp = false;
	}

	if (justReleasedLeft) {
		if (holdEnds[NoteDirection::LEFT] >= battleDuration) {
			battleDisplay->tailWasReleased(holdNotes[NoteDirection::LEFT]);
		}
		justReleasedLeft = false;
	}

	if (justReleasedDown) {
		if (holdEnds[NoteDirection::DOWN] >= battleDuration) {
			battleDisplay->tailWasReleased(holdNotes[NoteDirection::DOWN]);
		}
		justReleasedDown = false;
	}
}

void Game::BattlePlayer::onEvent(Event* event) {
	if (GameObjectProvider::getSceneManager()->getCurrentSceneId() == BATTLE_SCENE) {
		if (event->getType() == EventConstants::KEY_PRESSED) {
			auto* keyEvent = dynamic_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
				case SDLK_d:
					if (!pressingRight) {
						pressingRight = true;
						justPressedRight = true;
					}
					break;
				case SDLK_w:
					if (!pressingUp) {
						pressingUp = true;
						justPressedUp = true;
					}
					break;
				case SDLK_a:
					if (!pressingLeft) {
						pressingLeft = true;
						justPressedLeft = true;
					}
					break;
				case SDLK_s:
					if (!pressingDown) {
						pressingDown = true;
						justPressedDown = true;
					}
					break;
			}
		} else if (event->getType() == EventConstants::KEY_RELEASED) {
			auto* keyEvent = dynamic_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
				case SDLK_d:
					justReleasedRight = true;
					pressingRight = false;
					break;
				case SDLK_w:
					justReleasedUp = true;
					pressingUp = false;
					break;
				case SDLK_a:
					justReleasedLeft = true;
					pressingLeft = false;
					break;
				case SDLK_s:
					justReleasedDown = true;
					pressingDown = false;
					break;
			}
		}
	}
}

void BattlePlayer::prepareForBattle(Battle* battle) {
	inBattle = true;
	battleDialogueDisplay->showAvatar(battle->getAvatarSrc(), battle->getBlinkingAvatarSrc());
	currentScore = 0;
	battleDisplay->showTotalScore(0);
	battleDisplay->prepareForBattle(battle->getDefaultBackground());
	GameObjectProvider::getGame()->setAnimateDeaths(false);
}

void Game::BattlePlayer::playBattle(Battle* battle) {
	playing = true;
	currentBattle = battle;
	battleStartTime = clock->getCurrentTimeInNanos();
	battleDisplay->showTimingScore("");
	battleDisplay->setupKeyMovementAnimations(battle);

	AudioClip* clip;
	if (Scripts::resourceManager->obtain<AudioClip>(DEFAULT_AUDIO_STREAM_PATH + battle->getTrackSrc(), clip)) {
		GameObjectProvider::getAudioPlayer()->playClip(clip);
	}
}

bool BattlePlayer::isInBattle() {
	return inBattle;
}

void Game::BattlePlayer::processNote(unsigned long long timeOfEntry, BattleNote& note) {
	notesOnDisplay[note.getDirection()].insert(std::pair<unsigned long long, BattleNote*>(timeOfEntry + note.getTiming() * 1000000, &note));
	battleDisplay->addNewNote(&note, timeOfEntry, battleStartTime);
}

bool Game::BattlePlayer::pressedNote(unsigned long long timing, BattleNote* note) {
	long long timeDifference = abs((signed long long) (timing - (clock->getCurrentTimeInNanos() - battleStartTime)));

	if (note->getTailLength() != 0) {
		holdEnds[note->getDirection()] = battleDuration + note->getTailLength() * 1000000;
		holdNotes[note->getDirection()] = note;
	}

	if (timeDifference <= REQUIREMENT_FOR_PERFECT) {
		playerCharacter->increaseHealth(rewardForPerfect);
		currentScore += rewardForPerfect;
		battleDisplay->showTimingScore("Perfect!");
	} else if (timeDifference <= REQUIREMENT_FOR_GREAT) {
		playerCharacter->increaseHealth(rewardForGreat);
		currentScore += rewardForGreat;
		battleDisplay->showTimingScore("Great!");
	} else if (timeDifference <= REQUIREMENT_FOR_GOOD) {
		playerCharacter->increaseHealth(rewardForGood);
		currentScore += rewardForGood;
		battleDisplay->showTimingScore("Good!");
	} else if (timeDifference <= REQUIREMENT_FOR_LATE) {
		playerCharacter->decreaseHealth(penaltyForLate);
		battleDisplay->showTimingScore("Bad");

		if (!playerCharacter->isAlive()) {
			playerDied();
		}
	} else {
		return false;
	}

	note->press();
	battleDisplay->noteWasPressed(note);
	battleDisplay->showTotalScore(currentScore);
	return true;
}

void BattlePlayer::heldTail() {
	playerCharacter->increaseHealth(rewardForHold);
	currentScore += rewardForHold;
	battleDisplay->showTotalScore(currentScore);
}

void BattlePlayer::noteWasMissed(BattleNote* note) {
	battleDisplay->removeNote(note);
	playerCharacter->decreaseHealth(penaltyForMissing);
	battleDisplay->showTimingScore("Missed!");

	if (note->getTailLength() != 0) {
		holdEnds[note->getDirection()] = battleDuration + note->getTiming() * 1000000 + note->getTailLength() * 1000000;
		holdNotes[note->getDirection()] = note;
	}

	if (!playerCharacter->isAlive()) {
		playerDied();
	}
}

void Game::BattlePlayer::endBattle() {
	playing = false;
	inBattle = false;
	battleDisplay->battleWasFinished(currentBattle, currentScore);
	playerCharacter->increaseMoney(currentBattle->getRewardOnEnd());
	GameObjectProvider::getGame()->endBattle();
	GameObjectProvider::getGame()->setAnimateDeaths(true);
	GameObjectProvider::getScriptManager()->runScript(currentBattle->getScriptOnEnd());
}

void BattlePlayer::playerDied() {
	playing = false;

	AudioClip* clip;
	if (Scripts::resourceManager->obtain<AudioClip>(DEFAULT_AUDIO_STREAM_PATH + currentBattle->getTrackSrc(), clip)) {
		GameObjectProvider::getAudioPlayer()->stopClip(clip);
	}

	battleDisplay->diedDuringBattle(currentBattle->getSrc(), currentScore, currentBattle->getTextOnDeath());
}
