/*
* Class: Character
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent an Character.
*/

#pragma once
#include "3D\Models\ModelEntity.h"

using namespace Aela;

class Character {
	public:
		Character() {

		}

		// These are getters and setters.
		void setEntity(ModelEntity* entity);
		ModelEntity* getEntity();
		void storeEntityIDInMap(int ID);
		int getEntityIDInMap();

	private:
		ModelEntity* entity;
		int entityIDInMap;
};