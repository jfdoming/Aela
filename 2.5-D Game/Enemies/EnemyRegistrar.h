/*
* Class: EnemyRegistrar
* Author: Robert Ciborowski
* Date: 06/04/2018
* Description: A class which keeps track of enemies and tells them to updateRegisteredEnemies themselves.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "Turret.h"

namespace Game {
	class EnemyRegistrar {
		friend class Turret;
		public:
			EnemyRegistrar();

			void setup();
			void scenesWereSetUp();

			void updateRegisteredEnemies();

			size_t registerTurret(Turret* turret);
			Turret* getTurret(size_t id);

		private:
			// These are obtained from GameObjectProvider.
			Player* player;
			Time* time;
			Scene* gameplayScene;
			ResourceManager* resourceManager;

			std::vector<Turret*> turrets;
	};
}