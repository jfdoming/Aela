/*
* Class: WorldManager
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A class used to manage worlds.
*/

#pragma once
#include <unordered_map>
#include "World.h"
#include "../Character/CharacterManager.h"
#include "3D\Maps\Map3D.h"
#include "TileAtlas.h"
#include "Resource Management/ResourceManager.h"
#include "../Scripts/ScriptManager.h"
#include "Renderer/GLRenderer.h"

#define PI 3.14159265359
#define HALF_PI 1.57079632679
#define THREE_HALVES_PI 4.71238898038
#define THREE_QUARTERS_PI 2.35619449019
#define QUARTER_PI 0.78539816339
#define SIXTH_PI 0.52359877559
#define ELEVEN_SIXTHS_PI 5.75958653158
#define ROOT_OF_TWO 1.41421356237

namespace Game {
	class WorldManager {
		public:
			WorldManager() {
				chunkRenderDistances = glm::vec3(1, 1, 1);
				currentWorld = 0;
			}

			bool setup(ResourceManager* resourceManager, GLRenderer* renderer, Animator* animator, ScriptManager* scriptManager, Character* player);
			void update();

			// This creates/recreates the Aela::Map3D that is used by the game.
			void rebuildMap();

			TileAtlas* getTileAtlas();
			CharacterManager* getCharacterManager();
			unsigned long long addWorld(World* world);
			World* getWorld(unsigned long long id);
			bool setCurrentWorld(unsigned long long id);
			unsigned long long getCurrentWorld();
			void setChunkRenderDistances(glm::vec3 chunkRenderDistances);
			bool moveCharacterIfPossible(Character* character, TileDirection direction);
			bool moveCharacterIfPossible(unsigned long long id, TileDirection direction);
			bool moveCharacterIfPossible(std::string name, TileDirection direction);
			void addWalkedOnScript(std::string script, Location* location);
			void addPromptedScript(std::string script, Location* location);

		private:
			// These are some Aela objects.
			ResourceManager* resourceManager;
			ScriptManager* scriptManager;
			GLRenderer* renderer;

			std::vector<World> worlds;
			unsigned long long currentWorld;

			// These are some handles to game-related objects.
			CharacterManager characterManager;
			Character* player;
			TileAtlas tileAtlas;

			// This stores the render distances of the chunks (x = width, y = length, z = depth if looking down at the
			// ground from a bird's eye view).
			glm::ivec3 chunkRenderDistances;

			// This is a handle to an Aela 3D map. The chunks of a world are loaded into this map. Because Aela's 3D
			// system revolves around maps, and because this game is part-3D-part-2D, there must be a Map3D somewhere!
			Map3D* map;

			// This is the path to the Aela 3D Map.
			const std::string mapFileLocation = "../../res/maps/map.txt";
	};
}