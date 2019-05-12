/*
* Class: Battle Player
* Author: Robert Ciborowski
* Date: 07/01/2019
* Description: A class used to play a battle.
*/

#pragma once
#include "../Character/Character.h"
#include "Battle.h"
#include "BattleNote.h"

namespace Game {
	class BattlePlayer {
		public:
			BattlePlayer();

			void setup();
			void update();

			// This is triggered on an event.
			void onEvent(Event* event);

			void prepareForBattle(Battle* battle);
			void playBattle(Battle* battle);

			bool isInBattle();

		private:
			Battle* currentBattle;
			BattleDisplay* battleDisplay;
			BattleDialogueDisplay* battleDialogueDisplay;
			Character* playerCharacter;
			ScriptManager* scriptManager;
			Clock* clock;

			std::unordered_map<NoteDirection, std::multimap<unsigned long long, BattleNote*>> notesOnDisplay;
			std::unordered_map<NoteDirection, unsigned long long> holdEnds;
			std::unordered_map<NoteDirection, BattleNote*> holdNotes;

			bool playing = false;
			bool inBattle = false;
			std::atomic<bool> pressingRight = false, pressingUp = false, pressingLeft = false, pressingDown = false;
			std::atomic<bool> justPressedRight = false, justPressedUp = false, justPressedLeft = false, justPressedDown = false;
			std::atomic<bool> justReleasedRight = false, justReleasedUp = false, justReleasedLeft = false, justReleasedDown = false;
			
			unsigned long long battleStartTime = 0;
			unsigned long long battleDuration = 0;
			std::unordered_map<NoteDirection, unsigned long long> timeSinceLastHoldReward;
			const unsigned long long WAIT_TIME_AFTER_NOTE_ARRIVAL = 400000000;
			const unsigned long long WAIT_INTERVAL_FOR_HOLDS = 250000000;

			int penaltyForMissing = 10;
			int penaltyForLate = 5;
			int rewardForGood = 1;
			int rewardForGreat = 2;
			int rewardForPerfect = 3;
			int rewardForHold = 1;
			unsigned int currentScore = 0;

			const long long REQUIREMENT_FOR_LATE = WAIT_TIME_AFTER_NOTE_ARRIVAL;
			const long long REQUIREMENT_FOR_GOOD = 325000000;
			const long long REQUIREMENT_FOR_GREAT = 175000000;
			const long long REQUIREMENT_FOR_PERFECT = 64000000;

			void processNote(unsigned long long timeOfEntry, BattleNote& note);
			bool pressedNote(unsigned long long timing, BattleNote* note);
			void heldTail();
			void noteWasMissed(BattleNote* note);
			void endBattle();
			void playerDied();
	};
}
