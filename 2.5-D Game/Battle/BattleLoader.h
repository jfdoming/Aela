/*
* Class: BattleLoader
* Author: Robert Ciborowski
* Date: 07/01/2018
* Description: A class used to load battles.
*/

#pragma once
#include <string>
#include "Battle.h"
#include "../Game Object Provider/GameObjectProvider.h"

namespace Game {
	class BattleLoader {
		public:
			BattleLoader() {}

			bool loadBattle(std::string path, Battle& battle);
			bool loadBattleAsTextFile(std::string path, Battle& battle);

		private:
			void generateNotes(std::stringstream& dataWithinTag, Battle& battle, NoteDirection direction, unsigned long long timing, unsigned long long currentTailLength);
			bool interpretText(std::istream& in, Battle& battle);
	};
}