/*
* Class: NPC
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to represent an NPC.
*/

#include "stdafx.h"
#include "NPC.h"

void NPC::setEntity(ModelEntity* entity) {
	this->entity = *entity;
}
