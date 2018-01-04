/*
* Class: Character
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent an Character.
*/

#include "stdafx.h"
#include "NPC.h"

void Character::setEntity(ModelEntity* entity) {
	this->entity = entity;
}

ModelEntity* Character::getEntity() {
	return entity;
}

void Character::storeEntityIDInMap(int ID) {
	entityIDInMap = ID;
}

int Character::getEntityIDInMap() {
	return entityIDInMap;
}
