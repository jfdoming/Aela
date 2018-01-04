/*
* Class: ScriptManager
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used to manage scripts. This class might be moved to Project Aela.
*/

#pragma once
#include <string>
#include <unordered_map>
#include <functional>

namespace Game {
	class ScriptManager {
		public:
			ScriptManager() {}

			// Ideally, one would load scripts like this, using LUA:
			void loadScript(std::string path);

			// However, since Project Aela's LUA functionality was never completed...
			void addScript(std::string name, std::function<void()> script);
			bool addScript(std::string name, std::function<void()> script, bool replaceExistingScript);

			bool runScript(std::string name);

		private:
			std::unordered_map<std::string, std::function<void()>> scripts;
	};
}
