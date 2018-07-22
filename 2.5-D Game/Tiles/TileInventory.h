/*
* Class: TileInventory
* Author: Robert Ciborowski
* Date: 24/02/2018
* Description: A class used to manage tiles owned by the player->
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "Tile.h"
#include "TileGroup.h"
#include "../Location/Location.h"

namespace Game {
	class TileInventory {
		public:
			TileInventory() {

			}

			Tile* switchTile(TileGroup* tileGroup, size_t whichTile);
			Tile* switchCurrentTile(TileGroup* tileGroup);
			void replaceTile(Tile* tile, size_t whichTile);
			size_t addTile(Tile* tile);
			size_t getNumberOfTiles();
			size_t getCurrentTileIndex();
			Tile* getCurrentTile();
			Tile* getTile(size_t whichTile);
			void setCurrentTile(size_t currentTile);
			void increaseCurrentTileIfPossible();
			void decreaseCurrentTileIfPossible();
			void placeTile(TileGroup* tileGroup);

		private:
			std::vector<Tile> tiles;
			size_t currentTile = 0;
		};
}