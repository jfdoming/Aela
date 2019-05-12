/*
* Class: Character
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent a character.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "../../Project Aela/3D/Models/Model.h"
#include "../Location/Location.h"
#include "../../Project Aela/3D/Models/ModelEntity.h"
#include "CharacterStep.h"
#include "../Movement/TeleportationAnimation.h"
#include "../Movement/Movement.h"
#include "CharacterInformationBlock.h"
#include <queue>

using namespace Aela;

namespace Game {
	class Character {
		// I know friend classes may be bad, they allow certain functions to only be accessible by certain classes.
		friend class WorldManager;
		friend class CharacterProvider;
		public:
			Character();
			explicit Character(std::string name);
			Character(std::string name, const Location& location);
			Character(std::string name, const Location& location, float walkingSpeed, float runningSpeed);

			virtual ~Character();

			void changeLocation(Location& location);
			void turnFreely(TileDirection direction);
			void turn(TileDirection direction);
			void startMovingInDirection(TileDirection direction);
			void addTileMovementToQueue(TileDirection direction), addTileMovementToQueue(TileDirection direction, bool running);
			void addTileMovementsToQueue(std::queue<TileDirection>& directions), addTileMovementsToQueue(std::queue<TileDirection>& directions, bool running);
			void addMovementToQueue(TileDirection direction, float magnitude), addMovementToQueue(TileDirection direction, float magnitude, bool running);
			void addMovementsToQueue(std::queue<TileDirection>& directions, std::queue<float>& magnitudes),
				addMovementsToQueue(std::queue<TileDirection>& directions, std::queue<float>& magnitudes, bool running);
			void addNoCollisionTileMovementToQueue(TileDirection direction), addNoCollisionTileMovementToQueue(TileDirection direction, bool running);
			void addNoCollisionMovementToQueue(TileDirection direction, float magnitude), addNoCollisionMovementToQueue(TileDirection direction, float magnitude, bool running);
			void addTurnToQueue(TileDirection direction);
			void addTeleportToQueue(Location& location, TeleportationAnimation animation);
			void addTeleportToQueue(Location& location, TeleportationAnimation animation, std::string scriptOnHalfLife);
			void stopMoving();
			void clearMovementQueue();

			// This is generally used by Movements to move the character, but could techncially be used by anything else.
			void translate(glm::vec3 vector);

			void kill();
			void killWithoutAnimation();
			void revive();
			void setOnKill(std::function<void()> onKill);

			// These are getters, setters and adders.
			Location* getLocation();
			Location* getLocationBeforeAnimation();
			void setModel(Model* model);
			Model* getModel();
			void setName(std::string name);
			std::string getName();
			void setWalkingSpeed(float walkingSpeed);
			float getWalkingSpeed();
			void setRunningSpeed(float runningSpeed);
			float getRunningSpeed();
			float getCurrentSpeed();
			void setRunning(bool running);
			bool isRunning();
			void setTexture(std::string textureName);
			std::string getTextureName();
			TileDirection getDirectionFacing();
			TileDirection getOppositeDirectionFacing();
			TileDirection getMovementDirection();
			ModelEntity* getEntity();
			void setEntity(ModelEntity* entity);
			CharacterStep getCurrentStep();
			
			bool isMoving();
			bool isMovingFreely();
			bool hasMovementsInQueue();
			bool isAlive();
			void setPropertiesUsingCharacterInformationBlock(CharacterInformationBlock* block);
			CharacterInformationBlock getCharacterInformationBlock();

			void setHealth(int health);
			int getHealth();
			float getHealthLeftAsFraction();
			void increaseHealth(int amount);
			void decreaseHealth(int amount);
			void setMaxHealth(int maxHealth);
			int getMaxHealth();
			void increaseMaxHealth(int amount);
			void decreaseMaxHealth(int amount);

			void setMoney(int money);
			int getMoney();

			// These return whether the money went below zero.
			bool increaseMoney(int amount);
			bool decreaseMoney(int amount);

			// This returns whether the purchase was successful.
			bool makePurchase(int cost);

			void setVisibility(bool visible);
			bool isVisible();
			void allowNewMovements(bool newMovementsAreAllowed);
			bool areNewMovementsAllowed();
			void setCollidable(bool collidable);
			bool isCollidable();
			void toggleCollidability();
			bool hasAModel();

		protected:
			// I don't even know why there's a protected section because I don't think any classes extend this one
			// (at least not anymore).
			Location location;
			Location locationBeforeAnimation;
			TileDirection directionFacing;

			// Each character should have a unique name.
			std::string name;

			bool collidable = true;

			int health, maxHealth;
			int money = 0;

			virtual void update();

		private:
			// Unlike tiles, characters are unique and get their own models.
			Model* baseModel = nullptr;
			ModelEntity* entity = nullptr;

			std::queue<Movement*> movementQueue;

			std::string textureName = "";

			// These represent several speeds, in units/millisecond.
			float walkingSpeed, runningSpeed;

			// These store the states of player movement.
			bool moving = false;
			bool movingFreely = true;
			bool running = false;
			TileDirection movementDirection;
			unsigned long long timeSinceLastMovement = 0;
			unsigned long long timeSinceLastCharacterStep = 0;

			bool alive = true;

			// This stores the foot that the player is using to step.
			CharacterStep currentStep = CharacterStep::POST_RIGHT;

			const long long TIME_BETWEEN_CHARACTER_STEPS_WALKING = 175;
			const long long TIME_BETWEEN_CHARACTER_STEPS_RUNNING = 125;

			bool visible = true;
			bool newMovementsAreAllowed = true;

			std::function<void()> onKill = {};

			// This stores the min and max tile position remainder the player must be from the teleporter center
			// for them to activate it.
			const float TELEPORTER_ACTIVATION_MIN_X = 0.2f;
			const float TELEPORTER_ACTIVATION_MAX_X = 0.8f;
			const float TELEPORTER_ACTIVATION_MIN_Z = 0.4f;
			const float TELEPORTER_ACTIVATION_MAX_Z = 0.85f;

			// Instead of having some big, bulky collision system, we instead have 4 points to check for when
			// any character moves. This makes the game run a little faster. xCollisionPoints stores the i of
			// 2 of the collision points, which are defined as origin + (i, 0, 0). zCollision points stores the
			// i of the other 2, which are defined as origin + (0, 0, i).
			/*float xCollisionPoints[2] = {0.25f, -0.25f};
			float zCollisionPoints[2] = {0.36f, -0.3f};*/
			glm::vec3 collisionPoints[4] = {
				glm::vec3(0.26f, 0, 0), glm::vec3(-0.26f, 0, 0),
				glm::vec3(0, 0, 0.36f), glm::vec3(0, 0, -0.3f)
			};

			void startMovement(TileDirection direction);

			void setLocation(Location* location);

			void animateDeath();

			void generateModel();
			void generatePlayerModel();
			void changeModel();

			void switchStep();
			void bothFeetOnGround();
			void playFootstepSound();

			glm::vec3 getWorldSpaceTranslation(TileDirection direction);
			bool collisionCheck(Location& location);
	};
}