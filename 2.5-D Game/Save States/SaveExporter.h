/*
* Class: World Exporter
* Author: Robert Ciborowski
* Date: 05/05/2018
* Description: A class used to export worlds.
*/

#pragma once
#include <string>

namespace Game {
	class SaveState;

	class SaveExporter {
		public:
			SaveExporter();

			bool exportSaveState(std::string path, SaveState* saveState);
			bool exportSaveStateAsText(std::string path, SaveState* saveState);

		private:
			void abbreviate(std::string& src, std::string defaultPath);
			bool exportSaveToStream(std::ostream& stream, SaveState* saveState);
	};
}
