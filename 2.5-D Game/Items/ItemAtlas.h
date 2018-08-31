/*
* Class: ItemAtlas
* Author: Robert Ciborowski
* Date: 28/02/2018
* Description: A class which stores basic data on each type of item.
*/

#pragma once
#include <vector>
#include "ItemType.h"

namespace Game {
	class ItemAtlas {
		public:
			ItemAtlas();

			size_t addItemType(ItemType* type);
			ItemType* getItemType(size_t id);

		private:
			std::vector<ItemType> types;
	};
}