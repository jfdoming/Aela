/*
* Class: Player
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent a player.
*/

#pragma once
#include "3D/Models/ModelEntity.h"
#include "../NPCs/NPC.h"

using namespace Aela;

class Player : public Character {
	public:
		Player() {

		}

		void setRotationSpeed(float rotationSpeed), setWalkingSpeed(float walkingSpeed);
		float getRotationSpeed(), getWalkingSpeed();
	private:
		float rotationSpeed = 0.00000001f, walkingSpeed = 0.00000001f;
};