#pragma once
#include <vector>
#include "Mod.h"
#include "LuaManager.h"

namespace Aela {
	class ModManager {
		private:
			Aela::LuaManager lua;
			std::vector<Mod> mods;

		public:
			int loadMod();
			Aela::LuaManager getLuaManager();
	};
}