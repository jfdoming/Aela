/*
* Class: NPC
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent an NPC.
*/

#pragma once
#include "3D\Models\ModelEntity.h"

using namespace Aela;

class NPC {
	public:
		NPC() {

		}

		// These are getters and setters.
		void setEntity(ModelEntity* entity);

	private:
		ModelEntity entity;
};