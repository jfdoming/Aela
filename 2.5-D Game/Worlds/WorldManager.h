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
#include "../Save States/Saveable.h"
#include <unordered_map>

namespace Game {
	class WorldManager : public Saveable {
		public:
			WorldManager();

			void setup();
			void scenesWereSetUp();
			void update();

			// This creates/recreates the Aela::Map3D that is used by the game.
			void rebuildMapNextUpdate();
			void rebuildMap();

			size_t addWorld(World* world);
			World* getWorld(size_t id);
			size_t getNumberOfWorlds();
			Map3D* getMap3D();
			Teleporter* getTeleporter(const Location& location);
			Teleporter* getTeleporter(const Location& location, size_t tileType);
			Chunk* getChunk(const Location& location);
			TileGroup* getTileGroup(const Location& location);
			glm::ivec3* getChunkRenderDistances();
			float getCharacterYOffsetInWorldspace();

			void setChunkRenderDistances(glm::vec3 chunkRenderDistances);
			void getCoordinateOfNeighbouringTile(glm::ivec3& tile, glm::ivec2& chunk, TileDirection direction);
			void createChunkInCurrentWorld(glm::ivec2 coordinate);
			void createLayerInCurrentWorld(glm::ivec2 chunkCoordinate, unsigned int layer);

			void addWalkedOnScript(std::string script, const Location& location);
			void addPromptedScript(std::string script, const Location& location);
			void addTileSwitchScript(std::string script, const Location& location);

			void removeWalkedOnScript(const Location& location);
			void removePromptedScript(const Location& location);
			void removeTileSwitchScript(const Location& location);

			bool addTeleporterToFloorTile(const Location& teleporterLocation, const Location& teleporterDestination);
			bool addTeleporterToSwitchableFloorTile(const Location& teleporterLocation, const Location& teleporterDestination);
			bool addTeleporterToTile(const Location& teleporterLocation, const Location& teleporterDestination, size_t tileType);

			void runPromptedScriptOfTile(const Location& location);
			void runTileSwitchScriptOfTileGroup(const Location& location);

			bool exportCurrentWorld();
			bool exportCurrentWorld(std::string path);
			bool autoExportCurrentWorld();

			void tileWasPlaced(const Location& location, size_t tileType);

			void saveDataToSaveState(SaveState* saveState) override;
			void loadDataFromSaveState(SaveState* saveState) override;

		private:
			// These are obtained from GameObjectProvider.
			ResourceManager* resourceManager;
			AnimationLooper* animationLooper;
			CharacterProvider* characterProvider;
			TileBehaviourExecutor* tileBehaviourExecutor;
			ScriptManager* scriptManager;
			TileAtlas* tileAtlas;
			Character* playerCharacter;
			AelaGame* game;
			WorldExporter* worldExporter;

			std::vector<World> worlds;
			MapRebuilder mapRebuilder;

			// This is a handle to an Aela 3D map. The chunks of a world are loaded into this map. Because Aela's 3D
			// system revolves around maps, and because this game is part-3D-part-2D, there must be a Map3D somewhere!
			Map3D* map;

			bool mapNeedsToBeRebuilt;

			// This is the path to the Aela 3D Map.
			const std::string DEFAULT_MAP_SRC = "res/maps/map.txt";

			// This is the path to the autosave location.
			const std::string WORLD_AUTO_EXPORT_PATH = "res/tiled maps/autosave.txt";

			void processCharacterMovement(Character* character, TileDirection& direction);
	};
}