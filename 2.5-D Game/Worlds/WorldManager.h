/*
* Class: WorldManager
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A class used to manage worlds.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "World.h"
#include "MapRebuilder.h"
#include "3D/Maps/Map3D.h"
#include "../Teleporter/Teleporter.h"
#include <unordered_map>

namespace Game {
	// The following represents a map of teleporters, in which the teleporters are organized by map, chunk and position in 
	// their chunk.
	typedef std::unordered_map<size_t, std::unordered_map<glm::ivec2, std::unordered_map<glm::ivec3, Teleporter,
		IVec3HashMapFunctions, IVec3HashMapFunctions>, IVec2HashMapFunctions, IVec2HashMapFunctions>> TeleporterMap;

	class WorldManager {
		public:
			WorldManager();

			bool setup();
			void update();

			void rebuildMapWhenPossible();

			/*TileAtlas* getTileAtlas();
			CharacterTracker* getCharacterTracker();*/
			size_t addWorld(World* world);
			World* getWorld(size_t id);
			Map3D* getMap3D();
			bool setCurrentWorld(size_t id);
			size_t getCurrentWorld();
			Teleporter* getTeleporter(Location* location);
			void setChunkRenderDistances(glm::vec3 chunkRenderDistances);
			void setGameplayScene(Scene* gameplayScene);
			void getCoordinateOfNeighbouringTile(glm::vec3& tile, glm::vec2& chunk, TileDirection direction);

			void moveCharacterIfPossible(size_t id, TileDirection direction);
			void moveCharacterIfPossible(std::string name, TileDirection direction);
			void moveCharacterIfPossible(size_t id, std::vector<TileDirection> directions);
			void moveCharacterIfPossible(std::string name, std::vector<TileDirection> directions);

			void addWalkedOnScript(std::string script, Location* location);
			void addPromptedScript(std::string script, Location* location);
			void addTileSwitchScript(std::string script, Location* location);
			void addTeleporter(Teleporter* teleporter, Location* location);

			void runPromptedScriptOfTile(Location* location);
			void runTileSwitchScriptOfTile(Location* location);

		private:
			// These are obtained from GameObjectProvider.
			ResourceManager* resourceManager;
			AnimationLooper* animationLooper;
			CharacterTracker* characterTracker;
			ScriptManager* scriptManager;
			TileAtlas* tileAtlas;
			Character* playerCharacter;

			std::vector<World> worlds;
			size_t currentWorld;
			MapRebuilder mapRebuilder;

			// This is a handle to an Aela 3D map. The chunks of a world are loaded into this map. Because Aela's 3D
			// system revolves around maps, and because this game is part-3D-part-2D, there must be a Map3D somewhere!
			Map3D* map;

			bool mapNeedsToBeRebuilt;

			TeleporterMap teleporters;

			// These can't actually be std::queues.
			std::vector<std::pair<size_t, TileDirection>> characterMovementQueueByID;
			std::vector<std::pair<std::string, TileDirection>> characterMovementQueueByName;

			// This is the path to the Aela 3D Map.
			const std::string DEFAULT_MAP_SRC = "res/maps/map.txt";

			// This creates/recreates the Aela::Map3D that is used by the game->
			void rebuildMap();

			void setupAnimationLoopingForTiles();

			void processCharacterMovement(Character* character, TileDirection& direction);
	};
}