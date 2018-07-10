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
#include "../Tiles/TileAtlas.h"
#include "../Character/CharacterProvider.h"

namespace Game {
	class MapRebuilder {
		public:
			MapRebuilder();

			void setup();

			void rebuildMap(World* world);

			void setAnimator(Animator* animator);
			void setTileAtlas(TileAtlas* tileAtlas);
			void setChunkRenderingDistances(glm::vec3 chunkRenderDistances);
			void setPlayerCharacter(Character* playerCharacter);

		private:
			Map3D* map;
			Animator* animator;
			TileAtlas* tileAtlas;
			CharacterProvider* characterProvider;
			Character* playerCharacter;
			WorldManager* worldManager;

			// This stores the render distances of the chunks (x = width, y = length, z = depth if looking down at the
			// ground from a bird's eye view).
			glm::ivec3 chunkRenderDistances;
	};
}