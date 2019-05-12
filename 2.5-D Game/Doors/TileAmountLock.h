/*
* Class: Tile Amount Lock
* Author: Robert Ciborowski
* Date: 30/07/2018
* Description: A special type of lock that is only unlocked once a certain area
*              contains a certain amount of tiles. Its name sounds silly but
*              idk what else to call it.
*/

#pragma once
#include "Lock.h"
#include "Door.h"
#include "../Location/Location.h"
#include "../Tiles/TileGroup.h"
#include "../../Project Aela/Utilities/Rect/Cuboid.h"
#include <list>
#include <unordered_map>

namespace Game {
	class TileAmountLock : public Lock {
		public:
			TileAmountLock();
			virtual ~TileAmountLock();

			void specifyRegionAsCuboid(const Location& location, size_t width, size_t height, size_t depth);
			void specifyRegion(std::list<Location>& region);
			void addToRegion(Location* addition);
			void addToRegion(std::list<Location>* addition);
			std::list<Location>* getRegion();

			void addTileTypeToCheckFor(size_t type, size_t amountOfTiles);

			void checkIfUnlocked();

			void setDoorByID(std::string doorID);
			void setIDInDoor(size_t id);

		private:
			// Don't use TileGroup* for this, use Location. That's because TileGroup* can change when you reload a save.
			std::list<Location> region;

			// This isn't a Door* because it seems to change when you reload a save (sometimes).
			std::string doorID = "";

			std::unordered_map<size_t, size_t> amounts;
			size_t id = 0;
	};
}