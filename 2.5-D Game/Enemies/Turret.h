/*
* Class: Turret
* Author: Robert Ciborowski
* Date: 06/04/2018
* Description: A class which represents an enemy that attacks if
*              the player is within its line of sight.
*/

#pragma once
#include "Enemy.h"
#include "../../Project Aela/Scenes/Scene.h"
#include "../../Project Aela/Resource Management/ResourceManager.h"

namespace Game {
	class Turret : public Enemy {
		friend class EnemyProvider;
		public:
			Turret();
			Turret(std::string name, const Location& location, unsigned int detectionRange);
			Turret(std::string name, const Location& location, unsigned int detectionRange, int strength, long long attackCooldown);
			Turret(std::string name, const Location& location, unsigned int detectionRange, bool detectRight, bool detectForward, bool detectLeft, bool detectBackward,
				int strength, long long attackCooldown);

			virtual void update(Character* playerCharacter, long long time);

			bool isInLineOfSight(Location* location);
			void setDetectionRange(unsigned int detectionRange);
			void setDetectionAngle(TileDirection direction, bool detect);
			void setDetectionAngles(bool detectRight, bool detectForward, bool detectLeft, bool detectBackward);
			bool hasRecentlyAttacked();

			virtual EnemyType getType();

		protected:
			virtual void update();

		private:
			unsigned int detectionRange = 0;
			std::unordered_map<TileDirection, bool> detectionAngles;
			bool recentlyAttacked = false;

			void addBulletEffects(Scene* gameplayScene, ResourceManager* resourceManager, Clock* time);
	};
}
