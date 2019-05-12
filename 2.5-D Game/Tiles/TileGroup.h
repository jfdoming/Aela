/*
* Class: Chunk
* Author: Robert Ciborowski
* Date: 03/07/2018
* Description: A class used to store tiles.
*/

#pragma once
#include <unordered_map>
#include "Tile.h"
#include "TileAtlas.h"

namespace Game {
	typedef std::unordered_map<size_t, Tile> TileMap;

	class TileGroup {
		public:
			TileGroup();

			void addTile(Tile* tile);
			void addTile(size_t type);
			bool removeTile(size_t type);
			void clear();

			// This is probably bad and should be removed.
			TileMap* getTiles();
			Tile* getTile(size_t type);

			Tile* getFloorTile();
			Tile* getLiquidFloorTile();
			Tile* getTeleporterTile();
			Tile* getSwitchableFloorTile();
			bool containsCollidableTile();
			bool isCollidable();
			bool containsTile(size_t type);
			bool containsCollidableNonSwitchableTile();

			// Note: if the IDs of the scripts are stored as nothing (""), then these functions
			// return nullptr. Otherwise, they return a pointer to the name of the script, which
			// could be long (since there could be thousands of scripts).
			std::string* getWalkedOnScriptID(), *getPromptedScriptID(), *getSwitchScript();

			void setWalkedOnScript(std::string name);
			void setPromptedScript(std::string name);
			void setSwitchScript(std::string name);

		private:
			TileMap tiles;

			// These are the names of scripts that occur when the TileGroup was walked on and when the TileGroup
			// was in front of the player when the player pressed the "prompt/interact" button. The
			// ScriptManager holds these scripts (obviously).
			std::string walkedOnScriptID = "", promptedScriptID = "", switchScriptID = "";
	};
}