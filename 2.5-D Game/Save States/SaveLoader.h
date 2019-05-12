/*
* Class: SaveLoader
* Author: Robert Ciborowski
* Date: 24/12/2018
* Description: A class used to load Saves.
*/

#pragma once
#include <string>
#include "../Doors/Lock.h"

namespace Game {
	class SaveState;

	class SaveLoader {
		public:
			SaveLoader() {}

			bool loadSaveState(std::string path, SaveState& saveState);
			bool loadSaveStateAsTextFile(std::string path, SaveState& saveState);

		private:
			void generateLocks(std::stringstream& dataWithinTag, Door* door);
			bool interpretText(std::istream& in, SaveState& saveState);
	};
}
