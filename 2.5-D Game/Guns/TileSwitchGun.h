/*
* Class: TileSwitchGun
* Author: Robert Ciborowski
* Date: 11/07/2018
* Description: A class used to represent the tile switch gun that the character uses.
*/

#pragma once
#include "../Game Mode/GameMode.h"
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Location/Location.h"

namespace Game {
	class TileSwitchGun {
		public:
			TileSwitchGun();

			void setup();

			void setActive(bool active);
			bool isActive();

			bool use(GameMode gameMode);

		private:
			Player* player;
			WorldManager* worldManager;
			TileInventoryDisplay* tileInventoryDisplay;
			TileBehaviourExecuter* tileBehaviourExecuter;
			TileAtlas* tileAtlas;
			Time* time;
			AelaGame* game;

			bool active = false;

			void addTileSwitchParticleEmitter(Location* location, GLTexture* texture);
	};
}