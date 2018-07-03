/*
* Class: WorldLoader
* Author: Robert Ciborowski
* Date: 06/12/2017
* Description: A class used to load Worlds.
*/

#pragma once
#include <string>
#include "World.h"
#include "WorldManager.h"
#include "../../Project Aela/Aela_Engine.h"

namespace Game {
	class WorldLoader {
		public:
			WorldLoader() {}

			bool loadWorld(std::string path, World& world);

			void setWorldManager(WorldManager* worldManager);
			void setEngine(Engine* engine);

		private:
			WorldManager* worldManager;
			ResourceManager* resourceManager;
			GLRenderer* renderer;

			bool generateTileTypes(TileGroupMap& tiles, std::stringstream& data, int height);
	};
}