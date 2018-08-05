/*
* Class: Save State
* Author: Robert Ciborowski
* Date: 02/08/2018
* Description: A class used to store the state of the player and the world.
*/


#pragma once
#include "../Location/Location.h"
#include "../Player/Player.h"
#include "../Movement/Movement.h"

namespace Game {
	struct CharacterInformationBlock {
		Location location;
		TileDirection directionFacing;
		std::list<Movement> possibleMovementsToProcess;
		std::vector<std::pair<Movement, std::string>> translations;
		float walkingSpeed, runningSpeed;
		bool moving;
		bool running;
		bool alive;
		bool visible;
		bool newMovementsAreAllowed;
	};
}