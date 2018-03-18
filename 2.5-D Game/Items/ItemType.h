/*
* Class: ItemType
* Author: Robert Ciborowski
* Date: 14/03/2018
* Description: A class which represents the basic properties of a type of item.
*/

#pragma once
#include "../../Project Aela/2D/Texture/Texture.h"

using namespace Aela;

namespace Game {
	class ItemType {
		public:
			ItemType() {}

			void setTexture(Texture* texture);
			Texture* getTexture();

		private:
			Texture* texture;
	};
}