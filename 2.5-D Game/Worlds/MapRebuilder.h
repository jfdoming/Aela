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
#include "../Constants/MathConstants.h"

namespace Game {
	class MapRebuilder {
		public:
			MapRebuilder();

			void setup();
			void scenesWereSetup();

			void rebuildMap(World* world);

			void setAnimator(Animator* animator);
			void setTileAtlas(TileAtlas* tileAtlas);
			void setChunkRenderingDistances(glm::vec3 chunkRenderDistances);
			void setPlayerCharacter(Character* playerCharacter);

			glm::ivec3* getChunkRenderDistances();
			float getCharacterYOffsetInWorldspace();

		private:
			Map3D* map;
			Animator* animator;
			GLRenderer* renderer;
			TileAtlas* tileAtlas;
			CharacterProvider* characterProvider;
			Character* playerCharacter;
			WorldManager* worldManager;
			Scene* gameplayScene;

			// This stores the render distances of the chunks (x = width, y = length, z = depth if looking down at the
			// ground from a bird's eye view).
			glm::ivec3 chunkRenderDistances;

			const float characterYOffsetInWorldspace = 0.3f;

			const size_t CHARACTER_LAYER = 0;
			const size_t GLASS_LAYER = 1;
			const float CHARACTER_ANGLE = (float) ELEVEN_SIXTHS_PI;
	};
}
