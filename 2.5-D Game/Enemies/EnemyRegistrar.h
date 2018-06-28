/*
* Class: EnemyRegistrar
* Author: Robert Ciborowski
* Date: 06/04/2018
* Description: A class which keeps track of enemies and tells them to updateRegisteredEnemies themselves.
*/

#pragma once
#include "Turret.h"
#include "../Player/Player.h"

namespace Game {
	class EnemyRegistrar {
		friend class Turret;
		public:
			EnemyRegistrar();

			void setAelaObjects(Engine* engine);
			void setPlayer(Player* player);
			void setGameplayScene(Scene* scene);

			void updateRegisteredEnemies();

			size_t registerTurret(Turret* turret);
			Turret* getTurret(size_t id);

		private:
			std::vector<Turret*> turrets;

			Player* player;
			Time* time;
			ResourceManager* resourceManager;
			Scene* scene;
	};
}