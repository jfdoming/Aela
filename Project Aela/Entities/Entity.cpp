/*
* Name: ModelEntity
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Aela's GLRenderer to store properties of a 3D entity.
*/

#include "Entity.h"

using namespace Aela;

EntityType Entity::getEntityType() {
	return EntityType::GENERIC;
}
