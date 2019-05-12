/*
* Class: Battle
* Author: Robert Ciborowski
* Date: 07/01/2019
* Description: A class used to represent a battle.
*/

#pragma once
#include "BattleNote.h"
#include <map>
#include <vector>
#include "KeyMovement.h"

#define DEFAULT_BACKGROUND "battle_1_1.png"

namespace Game {
	struct BattleDialogue {
		std::string text;
		std::string characterName;
		bool playSound = true;
	};

	struct BackgroundChange {
		std::string src;
		unsigned long long duration;
	};

	class Battle {
		public:
			Battle();
			~Battle();

			void addNote(unsigned long long timeOfArrival, BattleNote& note);
			void addNotes(std::multimap<unsigned long long, BattleNote> notes);
			void addKeyMovement(unsigned long long timing, KeyMovement& movement);
			void addDialogue(unsigned long long timeInNanos, BattleDialogue dialogue);
			void addAvatarChange(unsigned long long timeInNanos, std::string avatarSrc, std::string blinkingAvatarSrc);
			void addScriptRun(unsigned long long timeInNanos, std::string script);
			void addBackgroundChange(unsigned long long timeInNanos, unsigned long long durationInMillis, std::string backgroundSrc);

			void setDialogueHideTimes(std::vector<unsigned long long>* dialogueHideTimes);
			void setAvatar(std::string avatarSrc);
			std::string getAvatarSrc();
			void setBlinkingAvatar(std::string blinkingAvatarSrc);
			std::string getBlinkingAvatarSrc();
			void setTrack(std::string avatarSrc);
			std::string getTrackSrc();
			void setDefaultBackground(std::string defaultBackground);
			std::string getDefaultBackground();
			std::multimap<unsigned long long, BattleNote*>* getNotes();
			std::multimap<unsigned long long, KeyMovement>* getKeyMovements();
			std::map<unsigned long long, BattleDialogue>* getDialogue();
			std::map<unsigned long long, std::pair<std::string, std::string>>* getAvatarChanges();
			std::map<unsigned long long, std::string>* getScriptRuns();
			std::map<unsigned long long, BackgroundChange>* getBackgroundChanges();
			std::vector<unsigned long long>* getDialogueHideTimes();
			void setDuration(unsigned long long duration);
			unsigned long long getDuration();
			void setScriptOnEnd(std::string scriptOnEnd);
			std::string getScriptOnEnd();
			void setScriptOnReturnToGameplay(std::string scriptOnReturnToGameplay);
			std::string getScriptOnReturnToGameplay();
			void setTextOnEnd(std::string textOnEnd);
			std::string getTextOnEnd();
			void setTextOnDeath(std::string textOnDeath);
			std::string getTextOnDeath();
			void setRewardOnEnd(int rewardOnEnd);
			int getRewardOnEnd();
			void setSrc(std::string src);
			std::string getSrc();
			void setFadeOnReturn(bool fadeOnReturn);
			bool getFadeOnReturn();

		private:
			std::multimap<unsigned long long, BattleNote*> notes;
			std::multimap<unsigned long long, KeyMovement> keyMovements;
			std::map<unsigned long long, BattleDialogue> dialogue;
			std::map<unsigned long long, std::pair<std::string, std::string>> avatarChanges;
			std::map<unsigned long long, std::string> scriptRuns;
			std::map<unsigned long long, BackgroundChange> backgroundChanges;
			std::vector<unsigned long long> dialogueHideTimes;
			std::string avatarSrc, blinkingAvatarSrc;
			std::string scriptOnEnd = "";
			std::string scriptOnReturnToGameplay = "";
			std::string textOnEnd = "You win!";
			std::string textOnDeath = "You were arrested.";
			std::string src = "";
			std::string defaultBackground = DEFAULT_BACKGROUND;
			bool fadeOnReturn = true;
			int rewardOnEnd = 0;

			// Track refers to a musical track.
			std::string trackSrc;

			unsigned long long duration = 0;
	};
}
