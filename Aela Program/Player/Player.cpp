/*
* Class: Player
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent a player.
*/

#include "stdafx.h"
#include "Player.h"

void Player::setRotationSpeed(float rotationSpeed) {
	this->rotationSpeed = rotationSpeed;
}

void Player::setWalkingSpeed(float walkingSpeed) {
	this->walkingSpeed = walkingSpeed;
}

float Player::getRotationSpeed() {
	return rotationSpeed;
}

float Player::getWalkingSpeed() {
	return walkingSpeed;
}
