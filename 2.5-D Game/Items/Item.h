/*
* Class: Item
* Author: Robert Ciborowski
* Date: 28/02/2018
* Description: A class which represents an in-game item.
*/

#pragma once
#include <string>

namespace Game {
	class Item {
		public:
			Item() = default;

			void setType(size_t type);
			void setName(std::string name);

		private:
			size_t type;
			std::string name;
	};
}