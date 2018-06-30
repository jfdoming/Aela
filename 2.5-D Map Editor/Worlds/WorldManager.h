/*
* Class: WorldManager
* Author: Robert Ciborowski
* Date: 16/12/2017
* Description: A class used to manage worlds.
*/

#pragma once
#include <unordered_map>
#include "World.h"
#include "MapRebuilder.h"
#include "../Character/CharacterTracker.h"
#include "../Dialogue/DialogueHandler.h"
#include "../../Project Aela/3D/Maps/Map3D.h"
#include "TileAtlas.h"
#include "Resource Management/ResourceManager.h"
#include "../Scripts/ScriptManager.h"
#include "Renderer/GLRenderer.h"
#include "../Teleporter/Teleporter.h"
#include "WorldExporter.h"


namespace Game {
	// The following represents a map of teleporters, in which the teleporters are organized by map, chunk and position in 
	// their chunk.
	typedef std::unordered_map<size_t, std::unordered_map<glm::ivec2, std::unordered_map<glm::ivec3, Teleporter,
		IVec3HashMapFunctions, IVec3HashMapFunctions>, IVec2HashMapFunctions, IVec2HashMapFunctions>> TeleporterWorld;

	class WorldManager {
		public:
			WorldManager() {
				currentWorld = 0;
			}

			bool setup(Engine* engine, ScriptManager* scriptManager, DialogueHandler* dialogueHandler, Character* player);
			void update();

			void rebuildMapWhenPossible();

			TileAtlas* getTileAtlas();
			CharacterTracker* getCharacterTracker();
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

			void setCoordinateLabel(std::shared_ptr<Label> coordinateLabel);

			void createChunkInCurrentWorld(glm::ivec2 coordinate);
			void createLayerInCurrentWorld(glm::ivec2 chunkCoordinate, unsigned int layer);

			// This shoudn't actually be in this class, but I'm lazy.
			bool exportCurrentWorld();

		private:
			// These are some Aela objects.
			ResourceManager* resourceManager;
			GLRenderer* renderer;
			Animator* animator;
			AnimationLooper* animationLooper;

			std::vector<World> worlds;
			size_t currentWorld;

			// These are some handles to game-related objects.
			CharacterTracker characterTracker;
			ScriptManager* scriptManager;
			DialogueHandler* dialogueHandler;
			Character* player;
			TileAtlas tileAtlas;
			MapRebuilder mapRebuilder;
			WorldExporter worldExporter;

			std::shared_ptr<Label> coordinateLabel;

			// This is a handle to an Aela 3D map. The chunks of a world are loaded into this map. Because Aela's 3D
			// system revolves around maps, and because this game is part-3D-part-2D, there must be a Map3D somewhere!
			Map3D* map;

			bool mapNeedsToBeRebuilt;

			TeleporterWorld teleporters;

			// These can't actually be std::queues.
			std::vector<std::pair<size_t, TileDirection>> characterMovementQueueByID;
			std::vector<std::pair<std::string, TileDirection>> characterMovementQueueByName;

			// This is the path to the Aela 3D World.
			const std::string mapFileLocation = "res/maps/map.txt";

			// This creates/recreates the Aela::Map3D that is used by the game.
			void rebuildMap();

			void setupAnimationLoopingForTiles();

			void processCharacterMovement(Character* character, TileDirection& direction);
	};
}