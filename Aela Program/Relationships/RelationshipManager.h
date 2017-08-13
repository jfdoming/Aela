/*
* Class: Relationship Manager
* Author: Robert Ciborowski
* Date: 07/08/2017
* Description: A class used to manage relationships.
*/

#pragma once
#include <string>
#include "Relationship.h"

class RelationshipManager {
	public:
		RelationshipManager() {

		}

		Relationship* getRelationship(std::string firstNPC, std::string secondNPC);
	private:


};