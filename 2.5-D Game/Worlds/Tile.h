/*
* Class: Tile
* Author: Robert Ciborowski
* Date: 03/12/2017
* Description: A class used to represent an in-game tile/space.
*/

#pragma once
#include <string>
#include "2D/Texture/Texture.h"
#include "3D/Models/ModelEntity.h"

using namespace Aela;

namespace Game {
	class Tile {
		public:
		Tile() {
			type = 0;
			entity = nullptr;
		}

		Tile(unsigned int type) {
			this->type = type;
			entity = nullptr;
		}

		size_t getType();
		void setType(size_t type);
		ModelEntity* getEntity();
		void setEntity(ModelEntity* entity);

		private:
		// The texture used to represent the tile, which comes from one of the images that contain
		// several textures.
		size_t type;

		ModelEntity* entity;
	};
}