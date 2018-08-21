/*
* Class: Physics Manager
* Author: Robert Ciborowski
* Date: 10/09/2017
* Description: A class used to manage the physical elements of a world.
*/

#include "PhysicsManager.h"

using namespace Aela;

// Have fun: https://math.stackexchange.com/questions/1472049/check-if-a-point-is-inside-a-rectangular-shaped-area-3d
bool Physics::isInside(glm::vec3 point, BoundingBox3D* box) {
	/*glm::vec3 u = vertices[0] - vertices[1], v = vertices[0] - vertices[3], w = vertices[0] - vertices[4];
	// glm::vec3 u2 = glm::cross(v, w), v2 = glm::cross(u, w), w2 = glm::cross(u, v);
	glm::vec3 u2 = u, v2 = v, w2 = w;
	float dotUX = glm::dot(u2, x), dotVX = glm::dot(v2, x), dotWX = glm::dot(w2, x);

	std::cout << "-----\n";

	if (glm::dot(u2, vertices[0]) <= dotUX && dotUX <= glm::dot(u2, vertices[1])) {
	std::cout << "1: Yes.\n";
	}

	if (glm::dot(v2, vertices[0]) <= dotVX && dotVX <= glm::dot(v2, vertices[3])) {
	std::cout << "2: Yes.\n";
	}

	if (glm::dot(w2, vertices[0]) <= dotWX && dotWX <= glm::dot(w2, vertices[4])) {
	std::cout << "3: Yes.\n";
	}

	return glm::dot(u2, vertices[0]) < dotUX && dotUX < glm::dot(u2, vertices[1])
	&& glm::dot(v2, vertices[0]) < dotVX && dotVX < glm::dot(v2, vertices[3])
	&& glm::dot(w2, vertices[0]) < dotWX && dotWX < glm::dot(w2, vertices[4]);*/

	if (box->verticesWereGenerated()) {
		glm::vec3 i = box->getVertex(1) - box->getVertex(0), j = box->getVertex(3) - box->getVertex(0), k = box->getVertex(4) - box->getVertex(0), v = point - box->getVertex(0);
		float dotVI = glm::dot(v, i), dotVJ = glm::dot(v, j), dotVK = glm::dot(v, k);
		return 0 < dotVI && dotVI < glm::dot(i, i) && 0 < dotVJ && dotVJ < glm::dot(j, j) && 0 < dotVK && dotVK < glm::dot(k, k);
	} else {
		return false;
	}
}

bool Physics::isTouching(glm::vec3 point, BoundingBox3D* box) {
	if (box->verticesWereGenerated()) {
		glm::vec3 i = box->getVertex(1) - box->getVertex(0), j = box->getVertex(3) - box->getVertex(0), k = box->getVertex(4) - box->getVertex(0), v = point - box->getVertex(0);
		float dotVI = glm::dot(v, i), dotVJ = glm::dot(v, j), dotVK = glm::dot(v, k);
		return 0 <= dotVI && dotVI <= glm::dot(i, i) && 0 <= dotVJ && dotVJ <= glm::dot(j, j) && 0 <= dotVK && dotVK <= glm::dot(k, k);
	} else {
		return false;
	}
}

// This function works by checking each vertex of boxA against boxB, and then each vertex of boxB against boxA. This happens
// since during a collision, one of a box's vertices may be in the other but the other box's vertices may not be in the first box.
// However, during a collision, at least one vertex has to be inside of the other box.


// HOW ON EARTH COULD ONE BOX'S VERTICES BE INSIDE THE OTHER BOX, BUT NOT VICE VERSA???????
bool Physics::areColliding(BoundingBox3D* boxA, BoundingBox3D* boxB) {
	if (boxA->verticesWereGenerated() && boxB->verticesWereGenerated()) {
		// In order for the boxes to be colliding, at least one of one box's vertices must be touching the other box.
		for (unsigned int i = 0; i < 8; i++) {
			std::cout << boxA->getVertex(i).x << " is the x of A, ";
			std::cout << boxB->getVertex(i).x << " is the x of B, ";
			std::cout << "\n";
			if (isTouching(boxA->getVertex(i), boxB)) {
				return true;
			}
			if (isTouching(boxB->getVertex(i), boxA)) {
				return true;
			}
		}
		return false;
	} else {
		return false;
	}
}

bool Physics::collidingInMap(int whichModel, Map3D* map) {
	ModelEntity* model = map->getModel(whichModel);
	if (model->getBoundingBox()->verticesWereGenerated()) {
		/*for (auto pair : *map->getModels()) {
			if (whichModel != pair) {
				pair.generateBoundingBox();
			}
			if (whichModel != pair && pair.getBoundingBox()->verticesWereGenerated() && areColliding(model->getBoundingBox(), pair.getBoundingBox())) {
				std::cout << model->getPosition()->x << " " << pair.getBoundingBox()->getPosition()->x << " " << pair.getModel()->getSrc() << "TRUE!\n";
				return true;
			} else if (whichModel != pair && pair.getBoundingBox()->verticesWereGenerated()) {
			}
		}*/
		return false;
	} else {
		return false;
	}
}
