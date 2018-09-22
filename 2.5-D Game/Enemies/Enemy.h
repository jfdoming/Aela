/*
* Class: Enemy
* Author: Robert Ciborowski
* Date: 06/04/2018
* Description: A class which represents an enemy.
*/

#pragma once
#include "../Character/Character.h"

namespace Game {
	enum class EnemyType {
		GENERIC, TURRET
	};

	class Enemy : public Character {
		friend class EnemyProvider;
		public:
			Enemy();
			Enemy(std::string name, const Location& location);
			Enemy(int strength, long long attackCooldown);
			Enemy(std::string name, const Location& location, int strength, long long attackCooldown);

			virtual ~Enemy();

			virtual EnemyType getType();

			void setStrength(int strength);
			int getStrength();
			void setAttackCooldown(long long attackCooldown);

			bool attackIfPossible(Character* character);

			bool isHostile();
			void setHostile(bool hostile);
			void toggleHostility();

			void addTarget(size_t characterID);
			void removeTarget(size_t characterID);
			void clearTargets();

		protected:
			virtual void update();
			virtual void attack(Character* character);
			bool isInLineOfSight(Location* enemyLocation, TileDirection direction, int detectionRange);
			
			int strength = 0;
			long long attackCooldown = 1;
			long long timeOnLastAttack = 0;

			bool hostile = true;

			std::vector<size_t> targets;
	};
}
