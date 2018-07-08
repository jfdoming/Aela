/*
* Class: MapRebuilder
* Author: Robert Ciborowski
* Date: 10/04/2018
* Description: A class used to rebuild a Map3D that is used by a world.
*/

#pragma once
#include "../../Project Aela/3D/Maps/Map3D.h"
#include "../../Project Aela/Animation/Animator.h"
#include "World.h"
#include "TileAtlas.h"
#include "../Character/CharacterTracker.h"

namespace Game {
	class MapRebuilder {
		public:
			MapRebuilder();

			void setup();

			void rebuildMap(World* world, size_t currentWorldID, CharacterTracker* characterTracker);

			void bindMap(Map3D* map);
			void setAnimator(Animator* animator);
			void setTileAtlas(TileAtlas* tileAtlas);
			void setChunkRenderingDistances(glm::vec3 chunkRenderDistances);
			void setPlayerCharacter(Character* playerCharacter);

		private:
			Map3D* map = nullptr;
			Animator* animator;
			TileAtlas* tileAtlas;
			Character* playerCharacter;

			// This stores the render distances of the chunks (x = width, y = length, z = depth if looking down at the
			// ground from a bird's eye view).
			glm::ivec3 chunkRenderDistances;
	};
}