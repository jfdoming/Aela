/*
* Class: Character
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent an Character.
*/

#pragma once
#include "3D/Models/Model.h"
#include "../Location/Location.h"
#include "3D/Models/ModelEntity.h"

using namespace Aela;

namespace Game {
	class Character {
		friend class CharacterManager;
		public:
			Character() {
				walkingSpeed = 0.004f;
				runningSpeed = 0.008f;
				directionFacing = TileDirection::BACKWARD;
			}

			// These are getters, setters and adders.
			void setLocation(Location* location);
			Location* getLocation();
			void setModel(Model* entity);
			Model* getModel();
			void setName(std::string name);
			std::string getName();
			void setWalkingSpeed(float walkingSpeed);
			float getWalkingSpeed();
			void setTextureName(TileDirection direction, std::string name);
			void setTextureNames(std::unordered_map<TileDirection, std::string>* textureNames);
			void setTextureNames(std::string right, std::string forward, std::string left, std::string backward);
			std::string getTextureName(TileDirection direction);
			std::unordered_map<TileDirection, std::string>* getTextureNames();
			TileDirection getDirectionFacing();
			ModelEntity* getEntity();
			void setEntity(ModelEntity* entity);
			bool isMoving();
			void addTranslation(glm::vec3 translation);
			void removeNextTranslation();
			glm::vec3* getNextTranslation();

		private:
			Location location;
			TileDirection directionFacing;

			// Each character should have a unique name.
			std::string name;

			// Unlike tiles, characters are unique and get their own models.
			Model* model;
			ModelEntity* entity;

			// Tiles can have materialistic properties (eg luminance) and so they get their own material.
			// However, characters only need a texture, so this stores the name of that texture, which eventually
			// gets loaded.
			std::unordered_map<TileDirection, std::string> textureNames;

			// This stores the translations that the animator should use next to animate the character.
			std::vector<glm::vec3> translations;

			// These represent several speeds, in units/millisecond.
			float walkingSpeed, runningSpeed;

			// These store the states of player movement.
			bool moving = false;
	};
}