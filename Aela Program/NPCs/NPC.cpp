/*
* Class: NPC
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent an NPC.
*/

#include "stdafx.h"
#include "NPC.h"

void NPC::setEntity(ModelEntity* entity) {
	this->entity = entity;
}

ModelEntity* NPC::getEntity() {
	return entity;
}

void NPC::storeEntityIDInMap(int ID) {
	entityIDInMap = ID;
}

int NPC::getEntityIDInMap() {
	return entityIDInMap;
}
