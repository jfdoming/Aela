/*
* Class: Physics Manager
* Author: Robert Ciborowski
* Date: 10/09/2017
* Description: A class used to manage the physical elements of a world.
*              Note: I'm not 100% sure as to what to call this class, so I'm calling it Physics Manager for now.
*              In fact, this class probably won't exist once we figure out how to structure to physics system differently.
*/

#pragma once
#include "../Bounding Box/BoundingBox3D.h"
#include "../Maps/Map3D.h"

class PhysicsManager {
	public:
		PhysicsManager() {

		}

		// These functions check as to whether something is within the bounds of a BoundingBox or if that something is touching
		// a BoundingBox at all.
		bool isInside(glm::vec3 point, BoundingBox3D* box), isTouching(glm::vec3 point, BoundingBox3D* box),
			areColliding(BoundingBox3D* boxA, BoundingBox3D* boxB);

		// This checks a model in a map against every other model in the map. Yes, this is slow and will probably be replaced.
		bool collidingInMap(int whichModel, Map3D* map);
};