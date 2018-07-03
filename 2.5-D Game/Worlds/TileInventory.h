/*
* Class: TileInventory
* Author: Robert Ciborowski
* Date: 24/02/2018
* Description: A class used to manage tiles owned by the player.
*/

#pragma once
#include "TileAtlas.h"
#include "Tile.h"
#include "../Location/Location.h"

namespace Game {
	class TileInventory {
		public:
			TileInventory() {

			}

			Tile* switchTile(TileGroup* tileGroup, TileAtlas* atlas, size_t whichTile);
			Tile* switchCurrentTile(TileGroup* tileGroup, TileAtlas* atlas);
			void replaceTile(Tile* tile, size_t whichTile);
			int addTile(Tile* tile);
			size_t getNumberOfTiles();
			size_t getCurrentTileIndex();
			Tile* getCurrentTile();
			Tile* getTile(size_t whichTile);
			void setCurrentTile(size_t currentTile);
			void increaseCurrentTileIfPossible();
			void decreaseCurrentTileIfPossible();

		private:
			TileAtlas* tileAtlas;
			std::vector<Tile> tiles;
			size_t currentTile = 0;
		};
}