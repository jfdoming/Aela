/*
* Class: ItemInventory
* Author: Robert Ciborowski
* Date: 24/02/2018
* Description: A class used to manage items owned by a character.
*/

#pragma once
#include "ItemAtlas.h"
#include "Item.h"

namespace Game {
	class ItemInventory {
		public:
			ItemInventory();

			size_t addItem(Item* item);
			Item* getItem(size_t id);
			bool removeItem(size_t id);
			ItemAtlas* getItemAtlas();

		private:
			ItemAtlas atlas;
			std::vector<Item> items;
	};
}