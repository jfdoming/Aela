/*
* Name: ModelEntity
* Author: Robert Ciborowski
* Date: 06/08/2017
* Description: A class used by Aela's Renderer to store properties of a 3D entity.
*              Note: This class used to be known as "Model" but was changed.
*/

#include "ModelEntity.h"
#include "../Error Handler/ErrorHandler.h"

using namespace Aela;

Model* Aela::ModelEntity::getModel() {
	return model;
}

void Aela::ModelEntity::setModel(Model* model) {
	this->model = model;
}

Cuboid<double>* Aela::ModelEntity::getBoundingBox() {
	return &boundingBox;
}

EntityType ModelEntity::getEntityType() {
	return EntityType::MODEL;
}

void Aela::ModelEntity::generateBoundingBox() {
	if (model->getSubModels()->size() != 0) {
		SubModel* firstSubModel = &model->getSubModels()->at(0);
		float smallestX = firstSubModel->getVertices()->at(firstSubModel->getIndices()->at(0)).x, greatestX = smallestX,
			smallestY = firstSubModel->getVertices()->at(firstSubModel->getIndices()->at(0)).y, greatestY, smallestZ, greatestZ;

		for (SubModel subModel : *model->getSubModels()) {
			for (unsigned short index : *subModel.getIndices()) {
				glm::vec3* vertex = &subModel.getVertices()->at(index);
				if (vertex->x < smallestX) {
					smallestX = vertex->x;
				} else if (vertex->x > greatestX) {
					greatestX = vertex->x;
				}
				if (vertex->y < smallestY) {
					smallestY = vertex->y;
				} else if (vertex->y > greatestY) {
					greatestY = vertex->y;
				}
				if (vertex->z < smallestZ) {
					smallestZ = vertex->z;
				} else if (vertex->z > greatestZ) {
					greatestZ = vertex->z;
				}
			}
		}

		boundingBox = Cuboid<double>(smallestX, smallestY, smallestZ, greatestX - smallestX, greatestY - smallestY, greatestZ - smallestZ);
	} else {
		AelaErrorHandling::consoleWindowError("ModelEntity", "Something tried to generate an entity's bounding box without loading its model first!");
	}
}
