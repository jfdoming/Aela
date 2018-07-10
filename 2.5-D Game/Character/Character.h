/*
* Class: Character
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent a character.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "3D/Models/Model.h"
#include "../Location/Location.h"
#include "3D/Models/ModelEntity.h"
#include "CharacterStep.h"
#include "CharacterModelGenerator.h"
#include "../Movement/Movement.h"
#include <queue>

using namespace Aela;

namespace Game {
	class Character {
		friend class WorldManager;
		friend class CharacterTracker;
		public:
			Character();
			Character(std::string name);

			void turn(TileDirection direction);
			void move(Movement* movement, std::string scriptOnCompletion);
			void moveIfPossible(TileDirection direction);
			void moveIfPossible(std::list<TileDirection> directions);
			void teleport(Location* location, bool animate);
			void kill();
			void revive();

			void generateModel(ResourceManager* resourceManager);

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
			bool getRunning();
			void setTextureName(std::string name);
			std::string getTextureName();
			TileDirection getDirectionFacing();
			ModelEntity* getEntity();
			void setEntity(ModelEntity* entity);
			CharacterStep getCurrentStep();
			void switchStep();
			bool isMoving();
			bool isAlive();

			void animationHasEnded();
			bool animationHasJustEnded();
			void stopMoving();

			void setHealth(int health);
			int getHealth();
			void increaseHealth(int amount);
			void decreaseHealth(int amount);
			void setMaxHealth(int maxHealth);
			int getMaxHealth();
			void increaseMaxHealth(int amount);
			void decreaseMaxHealth(int amount);
			void setVisibility(bool visible);
			bool isVisible();
			void allowNewMovements(bool newMovementsAreAllowed);
			bool areNewMovementsAllowed();

		protected:
			Location location;
			Location locationBeforeAnimation;
			TileDirection directionFacing;

			// Each character should have a unique name.
			std::string name;

			int health, maxHealth;

			virtual void update();

		private:
			// Unlike tiles, characters are unique and get their own models.
			Model* baseModel = nullptr;
			ModelEntity* entity = nullptr;

			std::list<TileDirection> possibleMovementsToProcess;

			std::string textureName;

			// This stores the translations that the animator should use next to animate the character.
			std::vector<std::pair<Movement, std::string>> translations;

			// These represent several speeds, in units/millisecond.
			float walkingSpeed, runningSpeed;

			// These store the states of player movement.
			bool moving = false;
			bool running = false;

			bool alive = true;

			// This stores the foot that the player is using to step. It can be a 0, 1 or 2.
			CharacterStep currentStep = CharacterStep::LEFT;

			// This stores at to whether the animation had ended during the current tick/frame. This is used
			// to smoothen out character animation.
			bool animationHadJustEnded = false;

			long long timePassedAfterAnimationEnd = 0;

			bool visible = true;
			bool newMovementsAreAllowed = true;

			// To be accessed by this class's friends.
			void addTranslation(Movement* movement, std::string scriptOnceComplete);

			void setLocation(Location* location);

			void animateDeath();

			// This processes movements added using move().
			void processMovement(Movement* movement, std::string scriptOnCompletion);

			// This processes movements added using moveIfPossible(...).
			void processPossibleMovement(TileDirection direction);
	};
}