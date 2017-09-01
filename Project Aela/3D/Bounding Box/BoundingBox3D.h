/*
* Class: 3D Bounding Box
* Author: Robert Ciborowski
* Date: 29/08/2017
* Description: A class used for storing the properties of a 3D bounding box. This class is similar to the Cuboid class
*              but it also stores its vertices, making it possible to perform matrix transformations on the box.
*              Note: the position of the bounding box is the center of it.
*/

#pragma once
#include "../../Utilities/Rect/Cuboid.h"
#include "../Transformable/Transformable3D.h"

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class BoundingBox3D : public Cuboid<float>, public Transformable3D {
	public:
		BoundingBox3D() {

		}

		BoundingBox3D(float x, float y, float z, float width, float height, float depth) : Cuboid(x, y, z, width, height, depth) {
		}

		void generateVertices();
		void isInside(glm::vec3 point);

		// These are getters and setters.
		glm::vec3 getVertex(int id);
		std::string getVerticesAsString();

	private:
		std::vector<glm::vec3> vertices;
};