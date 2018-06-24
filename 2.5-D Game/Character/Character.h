/*
* Class: Character
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent a Character.
*/

#pragma once
#include "3D/Models/Model.h"
#include "../Location/Location.h"
#include "3D/Models/ModelEntity.h"
#include "CharacterStep.h"

using namespace Aela;

namespace Game {
	class Character {
		friend class CharacterManager;
		friend class WorldManager;
		public:
			Character() {
				// According to Platinum, it should be 0.00375.
				walkingSpeed = 0.00375f;
				// According to Platinum, it should be 0.0075, but that feels too fast.
				runningSpeed = 0.0075f;
				directionFacing = TileDirection::BACKWARD;
			}

			Character(std::string name) : Character() {
				this->name = name;
			}

			void setup(Location* location);

			// These are getters, setters and adders.
			void setLocation(Location* location);
			Location* getLocation();
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

			void animationHasEnded();
			bool animationHasJustEnded();
			void stopMoving();

		private:
			Location location;
			TileDirection directionFacing;

			// Each character should have a unique name.
			std::string name;

			// Unlike tiles, characters are unique and get their own models.
			Model* baseModel = nullptr;
			ModelEntity* entity = nullptr;

			std::string textureName;

			// This stores the translations that the animator should use next to animate the character.
			std::vector<std::pair<glm::vec3, std::string>> translations;

			// These represent several speeds, in units/millisecond.
			float walkingSpeed, runningSpeed;

			// These store the states of player movement.
			bool moving = false;
			bool running = false;

			// This stores the foot that the player is using to step. It can be a 0, 1 or 2.
			CharacterStep currentStep = CharacterStep::LEFT;

			// This stores at to whether the animation had ended during the current tick/frame. This is used
			// to smoothen out character animation.
			bool animationHadJustEnded = false;

			long long timePassedAfterAnimationEnd = 0;

			// To be accessed by this class's friends.
			void addTranslation(glm::vec3 translation, std::string scriptOnceComplete);
			void removeNextTranslation();
			std::pair<glm::vec3, std::string>* getNextTranslation();
			std::pair<glm::vec3, std::string>* getLastTranslation();

			void turnSimple(TileDirection direction);
			void moveSimple(TileDirection direction, std::string scriptOnCompletion);
	};
}